/*
 * DataStorage.h
 *
 *  Created on: Jan 14, 2014
 *      Author: roland
 */

#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_

#include <glib.h>

class DataStorage {
public:
	virtual ~DataStorage();

	static DataStorage* get_instance();

	GList* get_networks() { return m_list_networks; }
	void update_networks(GList *a_list_networks_update, GList *a_list_object_paths_invalid);
	void set_networks_reader_lock();
	void set_networks_reader_unlock();

private:
	DataStorage();
	DataStorage(const DataStorage&);
	static DataStorage *instance;
	static GStaticRWLock networks_lock;

	GList *m_list_networks;

	void delete_network(const gchar *a_object_path);
	GList* lookup_network(const gchar *a_object_path);
	void delete_network_all();
	void cleanup();

	static gint compare_object_path(gconstpointer a_data, gconstpointer a_user_data);
	static void print_networks(gpointer a_data, gpointer a_user_data);
	static gint sort_networks(gconstpointer a_net1, gconstpointer a_net2);
};

#endif /* DATASTORAGE_H_ */
