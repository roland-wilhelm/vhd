/*
 * INetwork.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#include "INetwork.h"

#include "debug.h"
#include <glib.h>

INetwork::INetwork(const gchar* a_object_path, eRat a_rat) :
								m_object_path(g_strdup(a_object_path)),
								m_strength(0),
								m_strength_average(0),
								m_rat(a_rat),
								m_state(StateUnknown),
								m_name(NULL) {


}

INetwork::~INetwork() {

	if(m_object_path != NULL) {

		g_free(m_object_path);
		m_object_path = NULL;
	}

	if(m_name != NULL) {

		g_free(m_name);
		m_name = NULL;
	}
}

void INetwork::update(INetwork *a_network) {

	DBG3();

	if(a_network == NULL) {

		ERR("no network '%p'", a_network);
		return;
	}

	DBG("update network '%p'.", a_network);

	set_strength(a_network->get_strength());
	set_name(a_network->get_name());
	set_strength_average(a_network->get_strength_average());
	set_state(a_network->get_state());
}

void INetwork::set_name(const gchar * a_name) {

	DBG3();

	if(a_name == NULL) {

		DBG("no network name.");
		return;
	}

	if(m_name != NULL) {

		if(g_strcmp0(m_name, a_name) == 0)
			return;

		g_free(m_name);

	}

	m_name = g_strdup(a_name);
	DBG("set network name '%s'.", m_name);
}

void INetwork::print_network() {

	DBG("rat '%s', state '%s', name '%s', strength '%u', average '%u'", rat2String(m_rat), state2String(m_state), m_name, m_strength, m_strength_average);
}
