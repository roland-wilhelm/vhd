/*
 * WatchConnman.cpp
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#include "WatchConnman.h"

#include "debug.h"
#include "ConnmanManager.h"
#include <glib.h>
#include <gio/gio.h>

#define CONNMAN_OBJECT_PATH				"/"
#define CONNMAN_SERVICE					"net.connman"
#define CONNMAN_MANAGER_INTERFACE		CONNMAN_SERVICE ".Manager"
#define CONNMAN_SERVICE_INTERFACE		CONNMAN_SERVICE ".Service"

WatchConnman::WatchConnman() : WatchDBus(CONNMAN_SERVICE,
								on_name_appeared_cb,
								on_name_vanished_cb,
								this) {


}

WatchConnman::~WatchConnman() {

}

void WatchConnman::on_name_appeared_cb(GDBusConnection *connection,
					  const gchar *name,
					  const gchar *name_owner,
					  gpointer user_data) {

	WatchConnman *manager = reinterpret_cast<WatchConnman *>(user_data);

	DBG("Service '%s' appeared, owned by '%s'.", name, name_owner);

}

void WatchConnman::on_name_vanished_cb(GDBusConnection *connection,
						const gchar *name,
						gpointer user_data) {

	WatchConnman *manager = reinterpret_cast<WatchConnman *>(user_data);

	DBG("Service '%s' disappeared.", name);

}
