/*
 * IVhda.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: roland
 */

#include "IVhda.h"

#include "debug.h"
#include <glib.h>
#include <unistd.h>

IVhda::IVhda(const gchar *a_name, gboolean a_active) :
					m_name(g_strdup(a_name)),
					m_active(a_active),
					m_thread(NULL),
					m_do_logging(FALSE),
					m_interval(1000) {


}

IVhda::~IVhda() {

}

gint IVhda::start() {

	DBG3();

	DBG("start thread.");
	m_thread = g_thread_create(helper, this, TRUE, NULL);
	if(m_thread == NULL) {

		ERR("thread cannot start.");
		return -1;
	}

	return 0;
}

void IVhda::stop() {

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

void IVhda::run() {

	m_do_logging = TRUE;

	DBG3();

	while(m_do_logging) {

		DBG("*** VHDA '%s' ***", m_name);
		do_calculation();

		usleep(m_interval*1000);
	}
}

void* IVhda::helper(gpointer a_user_data) {

	IVhda *vhda = reinterpret_cast<IVhda *>(a_user_data);

	DBG3("manager '%p'", vhda);

	vhda->run();

	return NULL;
}
