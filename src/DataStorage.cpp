/*
 * DataStorage.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: roland
 */

#include "DataStorage.h"

#include "debug.h"
#include "INetwork.h"

#include <glib.h>

DataStorage *DataStorage::instance = NULL;
GStaticRWLock DataStorage::networks_lock = G_STATIC_RW_LOCK_INIT;

DataStorage::DataStorage() : m_list_networks(NULL) {


}

DataStorage::~DataStorage() {

	/*
	 * TODO delete  all allocated memory
	 */
}

DataStorage* DataStorage::get_instance() {

	if(instance == NULL) {

		instance = new DataStorage;
	}

	return instance;
}

void DataStorage::set_networks_reader_lock() {

	g_static_rw_lock_reader_lock(&networks_lock);
}

void DataStorage::set_networks_reader_unlock() {

	g_static_rw_lock_reader_unlock(&networks_lock);
}

gint DataStorage::sort_networks(gconstpointer a_net1, gconstpointer a_net2) {

	INetwork *net1 = (INetwork *)a_net1;
	INetwork *net2 = (INetwork *)a_net2;

	if(net1->get_strength() > net2->get_strength()) {

		/* net1 comes before net2 */
		return -1;
	}
	else if(net1->get_strength() < net2->get_strength()) {

		/* net1 comes after net2 */
		return 1;
	}
	else {

		/* net1 and net 2 are equal */
		return 0;
	}

}

void DataStorage::print_networks(gpointer a_net, gpointer a_user_data) {

	INetwork *net = (INetwork *)a_net;
	net->print_network();
}

void DataStorage::update_networks(GList *a_list_networks_update, GList *a_list_object_paths_invalid) {

	guint length = 0, i;
	GList *item = NULL;
	INetwork *network = NULL;
	gchar *help;

	DBG3();

	g_static_rw_lock_writer_lock(&networks_lock);

	/* delete all invalid networks from the list */
	if(a_list_object_paths_invalid != NULL) {

		length = g_list_length(a_list_object_paths_invalid);
		DBG("INVALID NETWORKS '%u'", length);
		for(i = 0; i < length; i++) {

			help = (gchar *)g_list_nth_data(a_list_object_paths_invalid, i);
			DBG2("delete invalid network '%s'.", help);
			delete_network(help);

			g_free(help);
		}

		m_list_networks = g_list_sort(m_list_networks, sort_networks);
	}

	/* update networks and sort them */
	if(a_list_networks_update != NULL) {

		length = g_list_length(a_list_networks_update);
		DBG("UPDATE NETWORKS '%u'", length);
		for(i = 0; i < length; i++) {

			network = (INetwork *)g_list_nth_data(a_list_networks_update, i);
			DBG2("rat '%s', strength '%u', average '%u', name'%s'.", rat2String(network->get_rat()),
					network->get_strength(),
					network->get_strength_average(),
					network->get_name());

			item = lookup_network(network->get_object_path());
			if(item != NULL) {

				/* network already available, just update the old one and delete the new */
				INetwork *old = (INetwork *)item->data;
				old->update(network);
				delete network;
			}
			else {

				/* network not yet available, add new one */
				m_list_networks = g_list_insert_sorted(m_list_networks, network, sort_networks);
			}
		}

		m_list_networks = g_list_sort(m_list_networks, sort_networks);
	}

	g_static_rw_lock_writer_unlock(&networks_lock);

	g_list_foreach(m_list_networks, (GFunc)print_networks, NULL);
}

void DataStorage::delete_network(const gchar *a_object_path) {

	GList *item = NULL;
	INetwork *network = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object_path.");
		return;
	}

	item = lookup_network(a_object_path);
	if(item == NULL) {

		ERR("network '%s' not found", a_object_path);
		return;
	}

	DBG("delete network '%s'", a_object_path);
	network = (INetwork *)item->data;
	delete network;
	m_list_networks = g_list_remove_link(m_list_networks, item);

	g_list_free_1(item);
}

void DataStorage::delete_network_all() {

	guint length = 0, i;
	INetwork *network = NULL;

	DBG3();

	/* delete all networks */
	if(m_list_networks != NULL) {

		length = g_list_length(m_list_networks);
		DBG("delete all networks '%u'", length);
		for(i = 0; i < length; i++) {

			network = (INetwork *)g_list_nth_data(m_list_networks, i);

			delete network;
		}

		g_list_free(m_list_networks);
		m_list_networks = NULL;
	}

}

gint DataStorage::compare_object_path(gconstpointer a_net, gconstpointer a_object_path) {

	gchar *object_path = (gchar *)a_object_path;
	INetwork *network = (INetwork *)a_net;

	if(g_strcmp0(object_path, network->get_object_path()) == 0) {

		return 0;
	}

	return 1;
}

GList* DataStorage::lookup_network(const gchar *a_object_path) {

	GList *item = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object_path.");
		return NULL;
	}

	if(m_list_networks == NULL) {

		DBG2("network not found, list is empty.");
		return NULL;
	}

	item = g_list_find_custom(m_list_networks, a_object_path, compare_object_path);
	if(item != NULL) {

		DBG2("network found");
		return item;
	}
	else {

		DBG2("network not found.");
	}

	return NULL;
}
