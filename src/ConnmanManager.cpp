/*
 * ConnmanManager.cpp
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#include "ConnmanManager.h"

#include "debug.h"
#include "types.h"
#include "ConnmanService.h"
#include "INetwork.h"
#include "Wlan.h"
#include "Qmi.h"
#include "DataStorage.h"

#include <glib.h>

#define CONNMAN_SERVICE					"net.connman"
#define CONNMAN_SERVICE_INTERFACE		CONNMAN_SERVICE ".Service"

GStaticMutex ConnmanManager::services_mutex = G_STATIC_MUTEX_INIT;

ConnmanManager::ConnmanManager(const gchar *a_service_name,
		const gchar *a_object_path,
		const gchar *a_interface) : m_services(NULL),
									m_do_logging(FALSE),
									m_interval(1000),
									m_thread(NULL),
									ProxyDBus(a_service_name,
														a_object_path,
														a_interface) {


}

ConnmanManager::~ConnmanManager() {

	if(m_services) {

		delete_services();
		g_hash_table_destroy(m_services);
		m_services = NULL;
	}

}

gint ConnmanManager::create_manager_sync() {

	gint ret = 0;

	DBG3();

	DBG("Creating proxy of object path '%s'", get_object_path());
	ret = create_proxy_sync();
	if(ret != 0) {

		ERR("creating proxy.");
		return -1;
	}

	return 0;
}

ConnmanService* ConnmanManager::lookup_service(const gchar *a_object_path) {

	ConnmanService *service = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object path.");
		return NULL;
	}
	DBG("object path '%s'.", a_object_path);

	if(m_services == NULL) {

		ERR("no services available.");
		return NULL;
	}

	service = (ConnmanService *)g_hash_table_lookup(m_services, a_object_path);

	return service;
}

ConnmanService* ConnmanManager::create_service(const gchar *a_object_path) {

	ConnmanService *service = NULL;
	gint ret = 0;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object path.");
		return NULL;
	}

	DBG("object path '%s'.", a_object_path);

	if(m_services == NULL) {

		m_services = g_hash_table_new(g_str_hash, g_str_equal);
		if(m_services == NULL) {

			ERR("cannot create hash table for services.");
			return NULL;
		}
	}

	service = lookup_service(a_object_path);
	if(service != NULL) {

		DBG("service '%s' already exists.", a_object_path);
		return service;
	}

	DBG("create new service '%s'.", a_object_path);
	service = new ConnmanService(get_service_name(), a_object_path, CONNMAN_SERVICE_INTERFACE);
	if(service == NULL) {

		ERR("cannot allocate new connman service.");
		return NULL;
	}

	ret = service->create_service_sync();
	if(ret < 0) {

		ERR("cannot create new connman service.");
		delete service;
		return NULL;
	}

	return service;
}

gint ConnmanManager::add_service(ConnmanService *a_service) {

	ConnmanService *service = NULL;
	gint ret = 0;

	DBG3();

	if(a_service == NULL) {

		ERR("no connman service '%p'", a_service);
		return -1;
	}

	if(m_services == NULL) {

		ERR("no services available.");
		return -1;
	}

	service = lookup_service(a_service->get_object_path());
	if(service != NULL) {

		DBG("service '%s' already exists.", service->get_object_path());
		return 0;
	}

	DBG("add new service '%s'.", a_service->get_object_path());
	g_hash_table_insert(m_services, (gchar *)a_service->get_object_path(), a_service);

	return 0;
}

gint ConnmanManager::delete_service(const gchar *a_object_path) {

	ConnmanService *service = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object path.");
		return -1;
	}

	if(m_services == NULL) {

		ERR("no services available.");
		return -1;
	}

	service = lookup_service(a_object_path);
	if(service == NULL) {

		DBG("service does not exists.", a_object_path);
		return 0;
	}

	DBG("delete service '%s'.", a_object_path);
	g_hash_table_remove(m_services, a_object_path);
	delete service;

	return 0;
}

void ConnmanManager::delete_services() {

	GHashTableIter iter;
	gpointer key, value;

	DBG3();

	if(m_services == NULL)
		return;

	g_hash_table_iter_init(&iter, m_services);
	while(g_hash_table_iter_next(&iter, &key, &value)) {

		ConnmanService *service = (ConnmanService *)value;

		delete service;
	}

	g_hash_table_remove_all(m_services);
}

gint ConnmanManager::verdict_ho(const gchar *a_object_path) {

	ConnmanService *service = NULL;
	gint err = 0;
	DBG3();

	g_static_mutex_lock(&services_mutex);

	service = lookup_service(a_object_path);
	if(service == NULL) {

		ERR("service does not exists.", a_object_path);
		err = -1;
		goto end;
	}

	DBG("object path '%s'.", a_object_path);
	if(service->get_active_service() == NULL) {

		ERR("no active service.");
		err = -1;
		goto end;
	}
	else if(service == service->get_active_service()) {

		DBG("service already connected");
		err = 0;
		goto end;
	}

	if((service->get_state() == StateIdle) || (service->get_state() == StateAssociation)) {

		err = service->connect_sync();
		if(err < 0) {

			ERR("connecting");
			goto end;
		}
	}

	if(service->get_state() == StateReady) {

		DBG("active service '%s'", service->get_active_service()->get_object_path());
		err = service->move_before(service->get_active_service()->get_object_path());
		if(err < 0) {

			ERR("handover");
			goto end;
		}
	}

	end:

	g_static_mutex_unlock(&services_mutex);

	return err;
}

gint ConnmanManager::start() {

	DBG3();

	DBG("start thread.");
	m_thread = g_thread_create(helper, this, TRUE, NULL);
	if(m_thread == NULL) {

		ERR("thread cannot start.");
		return -1;
	}

	return 0;
}

void ConnmanManager::stop() {

	DBG3();

	DBG("stop thread");

	if(m_thread == NULL) {

		ERR("thread not started.");
		return;
	}

	m_do_logging = FALSE;
	g_thread_join(m_thread);
	m_thread = NULL;
}

void ConnmanManager::run() {

	m_do_logging = TRUE;

	DBG3();

	while(m_do_logging) {

		request_services_sync();

		usleep(m_interval*1000);
	}
}

void* ConnmanManager::helper(gpointer a_user_data) {

	ConnmanManager *manager = reinterpret_cast<ConnmanManager *>(a_user_data);

	DBG3("manager '%p'", manager);

	manager->run();

	return NULL;
}

gint ConnmanManager::request_services_sync() {

	GVariant *ret = NULL;
	GVariantIter *iter, *iter2;
	gchar *object_path;
	GVariant *dict = NULL;
	gsize length = 0;
	ConnmanService *service = NULL, *active_service = NULL;
	GList *list_networks_update = NULL;
	GList *list_networks_invalid = NULL;
	INetwork *network = NULL;
	gint i = 0;
	gboolean first_service = TRUE;

	DBG3();

	ret = call_sync("GetServices", NULL);
	if(ret == NULL) {

		ERR("call error.");
		return -1;
	}

	g_static_mutex_lock(&services_mutex);

	DBG("reply '%p'", ret);

	g_variant_get(ret, "(a(oa{sv}))", &iter);
	while(g_variant_iter_loop(iter, "(oa{sv})", &object_path, &iter2)) {

		gchar *key;
		GVariant *value;

		DBG("service '%s'", object_path);
		service = lookup_service(object_path);
		if(service == NULL) {

			DBG("service '%s' does not yet exist, create one.", object_path);
			service = create_service(object_path);
			if(service != NULL) {

				add_service(service);
				service->set_updated(TRUE);
			}
			else {
				ERR("creating service.");
				continue;
			}
		}
		else {

			DBG("service '%s' exist, update values.", object_path);
		}

		if(first_service == TRUE) {

			first_service = FALSE;
			active_service = service;
		}

		service->set_active_service(active_service);

		while(g_variant_iter_loop(iter2, "{sv}", &key, &value)) {

			if(g_strcmp0(key, "Type") == 0) {

				if(value && g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {

					const gchar *help = g_variant_get_string(value, &length);
					eRat rat = string2Rat(help);
					DBG3("Type string '%s', enum '%u'", help, rat);
					service->set_rat(rat);

				}
				else {

					ERR("parsing 'Type'");
				}
			}
			else if(g_strcmp0(key, "Name") == 0) {

				if(value && g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {

					const gchar *help = g_variant_get_string(value, &length);
					DBG3("Name '%s'", help);
					service->set_name(help);
				}
				else {

					ERR("parsing 'Name'");
				}
			}
			else if(g_strcmp0(key, "State") == 0) {

				if(value && g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {

					const gchar *help = g_variant_get_string(value, &length);
					DBG3("State string '%s', enum '%u'", help, string2State(help));
					service->set_state(string2State(help));
				}
				else {

					ERR("parsing 'State'");
				}
			}
			else if(g_strcmp0(key, "Strength") == 0) {

				if(value && g_variant_is_of_type(value, G_VARIANT_TYPE_BYTE)) {

					guint8 help = g_variant_get_byte(value);
					DBG3("Strength '%u'", help);
					service->set_strength(help);
				}
				else {

					ERR("parsing 'Strength'");
				}
			}

		} // End dictionary/ one service

		i++;
		/* set service as visited, to delete invalid services afterwards. */
		service->set_visited(TRUE);

		/* create network */
		if(service->get_updated() == FALSE)
			continue;

		if((service->get_rat() == RatWifi)) {

			DBG2("Network wifi");

			network = new Wlan(service->get_object_path());
			if(network == NULL) {

				ERR("network '%s' with object path '%s'", rat2String(RatWifi), service->get_object_path());
				delete network;
				continue;
			}

		}
		else if(service->get_rat() == RatQmi) {

			DBG2("Network qmi");

			network = new Qmi(service->get_object_path());
			if(network == NULL) {

				ERR("network '%s' with object path '%s'", rat2String(RatQmi), service->get_object_path());
				delete network;
				continue;
			}

		}
		else {

			continue;
		}

		service->set_updated(FALSE);
		network->set_strength(service->get_strength());
		network->set_strength_average((service->get_strength_average()));
		network->set_state(service->get_state());
		network->set_name((service->get_name()));
		list_networks_update = g_list_prepend(list_networks_update, network);


	} // End all services

	DBG2("NETWORKS '%d'", i);
	/* delete not visited (invalid) services and set visited to FALSE */
	list_networks_invalid = delete_invalid_services();
	DataStorage::get_instance()->update_networks(list_networks_update, list_networks_invalid);

	g_static_mutex_unlock(&services_mutex);

	g_list_free(list_networks_update);
	g_list_free(list_networks_invalid);
	g_variant_iter_free(iter);
	g_variant_unref(ret);

	return 0;
}

GList* ConnmanManager::delete_invalid_services() {

	GHashTableIter iter;
	gpointer key, value;
	GList *list = NULL;
	gint valid = 0, invalid = 0;

	DBG3();

	if(m_services == NULL)
		return NULL;

	g_hash_table_iter_init(&iter, m_services);
	while(g_hash_table_iter_next(&iter, &key, &value)) {

		ConnmanService *service = (ConnmanService *)value;

		if(service->get_visited() == TRUE) {

			valid++;
			/* service is valid, set visited flag to FALSE */
			service->set_visited(FALSE);
		}
		else {

			invalid++;
			/* service is not valid, remove it from the table and delete it. */
			DBG("delete invalid service '%s'", service->get_object_path());
			list = g_list_prepend(list, g_strdup(service->get_object_path()));
			delete service;
			g_hash_table_iter_remove(&iter);
		}

	}

	DBG2("NETWORKS VALID '%d', INVALID '%d'", valid, invalid);

	return list;

}

