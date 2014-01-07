/*
 * ProxyDBus.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */

#include "ProxyDBus.h"

#include "debug.h"

ProxyDBus::ProxyDBus() : m_proxy(NULL) {


}

ProxyDBus::~ProxyDBus() {

	delete_proxy();
}

gint ProxyDBus::create_sync(const gchar *a_service_name,
		const gchar *a_manager_path,
		const gchar *a_manager_interface) {

	GError *error = NULL;

	DBG3();

	if(m_proxy) {

		DBG("DBus proxy exists already, delete it.");
		delete_proxy();
	}

	if((a_service_name == NULL) || (a_manager_path == NULL) || (a_manager_interface == NULL)) {

		ERR("service name '%p', manager path '%p', manager interface '%s'.", a_service_name, a_manager_path, a_manager_interface);
		return -1;
	}

	DBG("Creating proxy, service name '%s'.", a_service_name);
	m_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										G_DBUS_PROXY_FLAGS_NONE,
										NULL,	/* GDBusInterfaceInfo */
										a_service_name,
										a_manager_path,
										a_manager_interface,
										NULL,
										&error);

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

const GDBusProxy* ProxyDBus::get_proxy() const {

	return m_proxy;
}

GVariant* ProxyDBus::call_sync(const char* a_method, const char* a_parameter) const {

	GError *error = NULL;
	GVariant *result = NULL, *parameter = NULL;

	DBG3();

	if(m_proxy == NULL) {

		ERR("No proxy available.");
		return NULL;
	}

	if((a_method == NULL) || (a_parameter == NULL)) {

		ERR("method '%p', parameter '%p'", a_method, a_parameter);
		return NULL;
	}

	parameter = g_variant_new("(s)", a_parameter);

	if(!parameter) {

		ERR("Conversion cstring '%s' --> gvariant.", parameter);
		return NULL;
	}

	DBG("Invoke DBus call method '%s' parameter '%s'", a_method, a_parameter);
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


