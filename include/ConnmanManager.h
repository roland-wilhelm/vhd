/*
 * ConnmanManager.h
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#ifndef CONNMANMANAGER_H_
#define CONNMANMANAGER_H_

#include "ProxyDBus.h"

class ConnmanService;

class ConnmanManager : public ProxyDBus {
public:
	ConnmanManager(const gchar *a_service_name,
			const gchar *a_object_path,
			const gchar *a_interface);
	virtual ~ConnmanManager();

	gint create_manager_sync();
	void clean_manager();
	gint request_services_sync();

private:

};

#endif /* CONNMANMANAGER_H_ */
