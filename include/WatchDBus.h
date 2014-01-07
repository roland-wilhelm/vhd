/*
 * WatchDBus.h
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */

#ifndef WATCHDBUS_H_
#define WATCHDBUS_H_

#include <gio/gio.h>
#include <glib.h>


class WatchDBus {
public:
	WatchDBus(const gchar *a_service_name);
	virtual ~WatchDBus();

private:
	WatchDBus();

	void on_name_vanished(GDBusConnection *connection,
			const gchar *name,
			gpointer user_data);

	void on_name_appeared(GDBusConnection *connection,
			const gchar *name,
			const gchar *name_owner,
			gpointer user_data);


	guint m_watch_id;
};

#endif /* WATCHDBUS_H_ */
