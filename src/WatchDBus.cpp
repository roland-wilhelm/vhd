/*
 * WatchDBus.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */

#include "WatchDBus.h"

#include "debug.h"


WatchDBus::WatchDBus() {


}

WatchDBus::WatchDBus(const gchar *a_service_name) : m_watch_id(0) {

	m_watch_id = g_bus_watch_name(G_BUS_TYPE_SYSTEM,
									a_service_name,
									G_BUS_NAME_WATCHER_FLAGS_NONE,
									on_name_appeared,
									on_name_vanished,
									(gpointer)this,
									NULL);

	DBG("Watching service '%s', id '%u'", a_service_name, m_watch_id);
}

WatchDBus::~WatchDBus() {

	g_bus_unwatch_name(m_watch_id);

}

void WatchDBus::on_name_vanished(GDBusConnection *connection,
						const gchar *name,
						gpointer user_data) {

	DBG("Service '%s' disappeared.", name);

	remove_manager();
}

void WatchDBus::on_name_appeared(GDBusConnection *connection,
					  const gchar *name,
					  const gchar *name_owner,
					  gpointer user_data) {

	DBG("Service '%s' appeared, owned by '%s'.", name, name_owner);

	create_manager();
}

gint WatchDBus::create_manager(const gchar *a_manager_path, const gchar *a_manager_interface) {


}

void WatchDBus::remove_manager() {

	DBG3();

	if(m_manager) {

		g_object_unref(m_manager);
		m_manager = NULL;
	}
}
