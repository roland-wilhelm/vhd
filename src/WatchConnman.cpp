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

WatchConnman::WatchConnman() : m_manager(NULL),
								m_rss(NULL),
								WatchDBus(CONNMAN_SERVICE,
									on_name_appeared_cb,
									on_name_vanished_cb,
									this) {

	m_manager = new ConnmanManager(CONNMAN_SERVICE,
									CONNMAN_OBJECT_PATH,
									CONNMAN_MANAGER_INTERFACE);

	m_rss = new RssBased(TRUE, *m_manager);

}

WatchConnman::~WatchConnman() {

	if(m_manager != NULL) {

		m_manager->stop();
		delete m_manager;
		m_manager = NULL;
	}

	if(m_rss != NULL) {

		m_rss->stop();
		delete m_rss;
		m_rss = NULL;
	}
}

void WatchConnman::on_name_appeared_cb(GDBusConnection *connection,
					  const gchar *name,
					  const gchar *name_owner,
					  gpointer user_data) {

	WatchConnman *connman = reinterpret_cast<WatchConnman *>(user_data);
	DBG("Service '%s' appeared, owned by '%s'.", name, name_owner);

	if(connman->m_manager == NULL) {

		ERR("Connman manager not created.");
		return;
	}

	connman->m_manager->create_manager_sync();
	connman->m_manager->start();
	connman->m_rss->start();

}

void WatchConnman::on_name_vanished_cb(GDBusConnection *connection,
						const gchar *name,
						gpointer user_data) {

	WatchConnman *connman = reinterpret_cast<WatchConnman *>(user_data);
	DBG("Service '%s' disappeared.", name);

	if(connman->m_manager != NULL) {

		connman->m_manager->stop();
	}

	connman->m_rss->stop();
}
