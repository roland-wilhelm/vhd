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

class ProxyDBus {
public:
	ProxyDBus();
	virtual ~ProxyDBus();

	gint create_sync(const gchar *a_service_name,
					const gchar *a_manager_path,
					const gchar *a_manager_interface);
	const GDBusProxy* get_proxy() const;
	void delete_proxy();
	GVariant* call_sync(const char* a_method, const char* a_parameter) const;

private:
	GDBusProxy *m_proxy;
};

#endif /* PROXYDBUS_H_ */
