/*
 * ProxyDBus.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */
#include "ProxyDBus.h"

#include <gio/gio.h>
#include <glib.h>
#include "debug.h"



ProxyDBus::ProxyDBus(const gchar *a_service_name,
		const gchar *a_object_path,
		const gchar *a_interface) : m_service_name(g_strdup(a_service_name)),
									m_object_path(g_strdup(a_object_path)),
									m_interface(g_strdup(a_interface)),
									m_proxy(NULL),
									m_table(NULL) {

	m_table = g_hash_table_new(g_str_hash, g_str_equal);
}

ProxyDBus::~ProxyDBus() {

	remove_signal_all();
	delete_proxy();

	g_hash_table_destroy(m_table);
	g_free(m_service_name);
	g_free(m_object_path);
	g_free(m_interface);

}

gint ProxyDBus::create_proxy_sync() {

	GError *error = NULL;

	DBG3();

	if(m_proxy) {

		DBG("DBus proxy exists already, delete it.");
		return 0;
	}

	if((m_service_name == NULL) || (m_object_path == NULL) || (m_interface == NULL)) {

		ERR("service name '%p', object path '%p', interface '%p'.", m_service_name, m_object_path, m_interface);
		return -1;
	}

	DBG("Creating proxy, service name '%s'.", m_service_name);
	m_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										G_DBUS_PROXY_FLAGS_NONE,
										NULL,	/* GDBusInterfaceInfo */
										m_service_name,
										m_object_path,
										m_interface,
										NULL,
										&error);

	DBG("proxy '%p'", m_proxy);
	if(error) {

		ERR("Proxy '%s'.", error->message);
		g_error_free(error);
		error = NULL;
		return -1;
	}

	return 0;
}

void ProxyDBus::delete_proxy() {

	DBG3();

	if(m_proxy) {

		g_object_unref(m_proxy);
		m_proxy = NULL;
	}
}

const gchar* ProxyDBus::get_service_name() const {

	return m_service_name;
}

const GDBusProxy* ProxyDBus::get_proxy() const {

	return m_proxy;
}

const gchar* ProxyDBus::get_object_path() const {

	return m_object_path;
}

GVariant *ProxyDBus::call_sync(const char* a_method, const char* a_parameter) const {

	GError *error = NULL;
	GVariant *result = NULL, *parameter = NULL;

	DBG3();

	if(m_proxy == NULL) {

		ERR("No proxy available.");
		return NULL;
	}

	if((a_method == NULL)) {

		ERR("method '%p'.", a_method);
		return NULL;
	}

	if(a_parameter != NULL) {

		parameter = g_variant_new("(o)", a_parameter);

		if(!parameter) {

			ERR("Conversion cstring '%s' --> gvariant.", parameter);
			return NULL;
		}
	}

	DBG("Invoke DBus call method '%s' parameter '%s'", a_method, a_parameter ? a_parameter : "none");
	result = g_dbus_proxy_call_sync(m_proxy,
									a_method,
									parameter,	/* Parameter */
									G_DBUS_CALL_FLAGS_NONE,
									2000,	/* Timeout in ms */
									NULL,
									&error);

	if(error) {

		ERR("Could not invoke d-bus method '%s' '%s'.", a_method, error->message);
		g_error_free(error);
		error = NULL;
		return NULL;
	}

	return result;
}

gint ProxyDBus::register_signal(const gchar *a_signal_name, GCallback a_cb, gpointer a_user_data) {

	gint handler_id = 0, ret = 0;

	DBG3();

	if((m_proxy == NULL) || (a_signal_name == NULL) || (a_cb == NULL)) {

		ERR("proxy '%p', signal name '%p', callback '%p'", m_proxy, a_signal_name, a_cb);
		return -1;
	}

	if(get_data(a_signal_name) != NULL) {

		DBG("Signal already registered.");
		return 0;
	}

	DBG("Register signal '%s', proxy '%p'", a_signal_name, m_proxy);
	handler_id = g_signal_connect(m_proxy,
					a_signal_name,
					a_cb,
					a_user_data);

	if(handler_id > 0) {

		add_signal(a_signal_name, handler_id);
	}
	else {

		ERR("proxy '%p', Signal '%s' not registered.", m_proxy, a_signal_name);
		ret = -1;
	}

	return ret;
}

gint ProxyDBus::unregister_signal(const gchar *a_signal_name) {

	_Data *data = NULL;

	DBG3();

	if(a_signal_name == NULL) {

		ERR("Signal name '%p'", a_signal_name);
		return -1;
	}

	data = get_data(a_signal_name);
	if(data == NULL) {

		DBG("Signal name '%s' not registered", a_signal_name);
		return -1;
	}

	g_signal_handler_disconnect(m_proxy, data->sig_handler_id);
	remove_signal(data);

	return 0;
}

_Data* ProxyDBus::get_data(const gchar *a_signal_name) {

	DBG3();

	return (_Data *)g_hash_table_lookup(m_table, a_signal_name);
}

void ProxyDBus::add_signal(const gchar *a_signal_name, gint a_handler_id) {

	_Data *data = NULL;

	DBG3();

	data = g_new0(_Data, 1);
	if(data == NULL) {

		ERR("creating data.");
		return;
	}

	data->sig_handler_id = a_handler_id;
	data->sig_name = g_strdup(a_signal_name);
	g_hash_table_insert(m_table, data->sig_name, data);
}

void ProxyDBus::remove_signal(_Data *a_data) {

	DBG3("data '%p'", a_data);
	if(a_data != NULL) {

		g_hash_table_remove(m_table, a_data->sig_name);
		g_free(a_data->sig_name);
		g_free(a_data);
	}

}

void ProxyDBus::remove_signal_all() {

	GHashTableIter iter;
	gpointer key, value;

	DBG3();

	g_hash_table_iter_init(&iter, m_table);
	while(g_hash_table_iter_next(&iter, &key, &value)) {

		_Data *data = (_Data *)value;

		g_signal_handler_disconnect(m_proxy, data->sig_handler_id);
		g_free(data->sig_name);
		g_free(data);
	}

	g_hash_table_remove_all(m_table);
}

