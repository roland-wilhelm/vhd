/*
 * ConnmanService.cpp
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#include "ConnmanService.h"

#include "debug.h"
#include <glib.h>

ConnmanService::ConnmanService(const gchar* a_service_name,
		const gchar* a_object_path,
		const gchar* a_interface) :
		m_state(StateUnknown),
		m_rat(RatUnknown),
		m_strength(0),
		m_visited(FALSE),
		m_updated(FALSE),
		m_name(NULL),
		m_strength_average(0),
		m_active_service(NULL), ProxyDBus(a_service_name, a_object_path, a_interface) {


}

ConnmanService::~ConnmanService() {

	if(m_name != NULL) {

		g_free(m_name);
		m_name = NULL;
	}
}

gint ConnmanService::create_service_sync() {

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

gint ConnmanService::connect_sync() {

	GVariant *ret = NULL;

	DBG3();

	DBG("Connecting service");
	ret = call_sync("Connect", NULL);
	if(ret == NULL) {

		ERR("cannot call 'Connect'.");
		return -1;
	}

	return 0;
}

gint ConnmanService::disconnect_sync() {

	GVariant *ret = NULL;

	DBG3();

	DBG("Disconnecting services");
	ret = call_sync("Disconnect", NULL);
	if(ret == NULL) {

		ERR("cannot call 'Disconnect'.");
		return -1;
	}

	return 0;
}

gint ConnmanService::move_after(const gchar *a_object_path) {

	GVariant *ret = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object path given.");
		return -1;
	}

	DBG("Moving after service");
	ret = call_sync("MoveAfter", a_object_path);
	if(ret == NULL) {

		ERR("cannot call 'MoveAfter'.");
		return -1;
	}

	return 0;
}

gint ConnmanService::move_before(const gchar *a_object_path) {

	GVariant *ret = NULL;

	DBG3();

	if(a_object_path == NULL) {

		ERR("no object path given.");
		return -1;
	}

	DBG("Moving before '%s'", a_object_path);
	ret = call_sync("MoveBefore", a_object_path);
	if(ret == NULL) {

		ERR("cannot call 'MoveBefore'.");
		return -1;
	}

	return 0;
}

void ConnmanService::set_strength(guint8 a_strength) {

	guint8 old = 0.0;
	DBG3();

	old = m_strength_average;
	m_strength_average = (m_strength_average + a_strength) / 2;

	if(old != m_strength_average)
		m_updated = TRUE;

	if(m_strength != a_strength) {

		m_strength = a_strength;
		m_updated = TRUE;
		DBG("update new strength '%u'", m_strength);
	}

}

void ConnmanService::set_state(eState a_state) {

	DBG3();

	if(m_state != a_state) {

		m_state = a_state;
		m_updated = TRUE;
		DBG("update new state '%s'.", state2String(m_state));
	}

}

void ConnmanService::set_name(const gchar *a_name) {

	DBG3();

	if(a_name == NULL) {

		ERR("no name.");
		return;
	}

	if(m_name != NULL) {

		if(g_strcmp0(m_name, a_name) == 0) {

			return;
		}
		else {

			g_free(m_name);
		}
	}

	m_name = g_strdup(a_name);
	DBG("set name '%s'", m_name);
}

void ConnmanService::set_active_service(const ConnmanService *a_active_service) {

	if(m_active_service != a_active_service)
		m_active_service = a_active_service;

}
