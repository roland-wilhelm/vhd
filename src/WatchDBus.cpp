/*
 * WatchDBus.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */

#include "WatchDBus.h"

#include <gio/gio.h>
#include <glib.h>
#include "debug.h"


WatchDBus::WatchDBus() {
}

WatchDBus::WatchDBus(const gchar *a_service_name,
		GBusNameAppearedCallback a_on_name_appeared_cb,
		GBusNameVanishedCallback a_on_name_vanished_cb,
		WatchDBus *a_derivation) : m_watch_id(0) {

	m_watch_id = g_bus_watch_name(G_BUS_TYPE_SYSTEM,
									a_service_name,
									G_BUS_NAME_WATCHER_FLAGS_NONE,
									a_on_name_appeared_cb,
									a_on_name_vanished_cb,
									(gpointer)a_derivation,
									NULL);

	DBG("Watching service '%s', id '%u'", a_service_name, m_watch_id);

}

WatchDBus::~WatchDBus() {

	g_bus_unwatch_name(m_watch_id);

}
