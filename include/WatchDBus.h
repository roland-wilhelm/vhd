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
	WatchDBus(const gchar *a_service_name,
			GBusNameAppearedCallback a_on_name_appeared_cb,
			GBusNameVanishedCallback a_on_name_vanished_cb,
			WatchDBus *a_derivation);
	virtual ~WatchDBus();

private:
	WatchDBus();

	guint m_watch_id;

};

#endif /* WATCHDBUS_H_ */
