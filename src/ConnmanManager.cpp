/*
 * ConnmanManager.cpp
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#include "ConnmanManager.h"

#include "debug.h"
#include "ConnmanService.h"
#include <glib.h>

ConnmanManager::ConnmanManager(const gchar *a_service_name,
		const gchar *a_object_path,
		const gchar *a_interface) : ProxyDBus(a_service_name,
										a_object_path,
										a_interface) {


}

ConnmanManager::~ConnmanManager() {

}

gint ConnmanManager::create_manager_sync() {

	gint ret = 0;

	DBG3();

	DBG("Creating proxy ...");
	ret = create_proxy_sync();
	if(ret != 0) {

		ERR("creating proxy.");
		return -1;
	}

	return 0;
}

void ConnmanManager::clean_manager() {

	DBG3();

	delete_proxy();
}

gint ConnmanManager::request_services_sync() {

	GVariant *ret = NULL;

	DBG3();

	ret = call_sync("GetServices", NULL);
	DBG("Requested services, reply '%p'", ret);

	return 0;
}
