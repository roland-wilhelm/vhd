/*
 * WatchConnman.h
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#ifndef WATCHCONNMAN_H_
#define WATCHCONNMAN_H_

#include "WatchDBus.h"
#include "RssBased.h"

class ConnmanManager;

class WatchConnman : public WatchDBus {
public:
	WatchConnman();
	virtual ~WatchConnman();

private:

	ConnmanManager *m_manager;
	RssBased *m_rss;

	static void on_name_vanished_cb(GDBusConnection *connection,
			const gchar *name,
			gpointer user_data);

	static void on_name_appeared_cb(GDBusConnection *connection,
			const gchar *name,
			const gchar *name_owner,
			gpointer user_data);
};

#endif /* WATCHCONNMAN_H_ */
