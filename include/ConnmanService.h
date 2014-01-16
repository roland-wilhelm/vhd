/*
 * ConnmanService.h
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#ifndef CONNMANSERVICE_H_
#define CONNMANSERVICE_H_

#include "ProxyDBus.h"

#include "types.h"


class ConnmanService : public ProxyDBus {
public:
	ConnmanService(const gchar* a_service_name,
			const gchar* a_object_path,
			const gchar* a_interface);
	virtual ~ConnmanService();

	eState get_state() const { return m_state; }
	void set_state(eState a_state);
	eRat get_rat() const { return m_rat; }
	void set_rat(eRat a_rat) { m_rat = a_rat; }
	guint8 get_strength() const { return m_strength; }
	void set_strength(guint8 a_strength);
	gboolean get_visited() const { return m_visited; }
	void set_visited(gboolean a_visited) { m_visited = a_visited; }
	gboolean get_updated() const { return m_updated; }
	void set_updated(gboolean a_updated) { m_updated = a_updated; }
	const gchar* get_name() const { return m_name; }
	void set_name(const gchar *a_name);
	double get_strength_average() const { return m_strength_average; }
	void set_active_service(const ConnmanService *a_active_service);
	const ConnmanService* get_active_service() const { return m_active_service; }

	gint create_service_sync();
	gint connect_sync();
	gint disconnect_sync();
	gint move_after(const gchar *a_object_path);
	gint move_before(const gchar *a_object_path);

private:
	eState m_state;
	eRat m_rat;
	guint8 m_strength;
	gboolean m_visited;
	gboolean m_updated;
	gchar *m_name;
	guint8 m_strength_average;
	const ConnmanService *m_active_service;
};

#endif /* CONNMANSERVICE_H_ */
