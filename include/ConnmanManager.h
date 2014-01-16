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
	gint verdict_ho(const gchar *object_path);
	gint start();
	void stop();

private:
	GHashTable *m_services;
	gboolean m_do_logging;
	guint m_interval;
	GThread *m_thread;

	static GStaticMutex services_mutex;

	gint request_services_sync();
	ConnmanService* lookup_service(const gchar *object_path);
	ConnmanService* create_service(const gchar *a_object_path);
	gint add_service(ConnmanService *a_service);
	gint delete_service(const gchar *object_path);
	void delete_services();
	void run();
	static void* helper(gpointer a_user_data);
	GList* delete_invalid_services();

};

#endif /* CONNMANMANAGER_H_ */
