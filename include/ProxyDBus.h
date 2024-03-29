/*
 * ProxyDBus.h
 *
 *  Created on: Jan 6, 2014
 *      Author: roland
 */

#ifndef PROXYDBUS_H_
#define PROXYDBUS_H_

#include <gio/gio.h>
#include <glib.h>

typedef struct {
	gchar *sig_name;
	gint sig_handler_id;
}_Data;

class ProxyDBus {
public:
	virtual ~ProxyDBus();

	const gchar* get_service_name() const;
	const GDBusProxy* get_proxy() const;
	const gchar* get_object_path() const;
	gint register_signal(const gchar *a_signal_name, GCallback a_cb, gpointer a_user_data);
	gint unregister_signal(const gchar *a_signal_name);

protected:
	ProxyDBus(const gchar *a_service_name,
				const gchar *a_object_path,
				const gchar *a_interface);

	gint create_proxy_sync();
	GVariant *call_sync(const char* a_method, const char* a_parameter) const;


private:
	void add_signal(const gchar *a_signal_name, gint a_handler_id);
	void remove_signal(_Data *a_data);
	_Data* get_data(const gchar *a_signal_name);
	void delete_proxy();
	void remove_signal_all();

	gchar *m_service_name;
	gchar *m_object_path;
	gchar *m_interface;
	GDBusProxy *m_proxy;
	GHashTable *m_table;
};

#endif /* PROXYDBUS_H_ */
