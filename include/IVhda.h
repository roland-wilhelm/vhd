/*
 * IVhda.h
 *
 *  Created on: Jan 15, 2014
 *      Author: roland
 */

#ifndef IVHDA_H_
#define IVHDA_H_

#include "glib.h"

class IVhda {
public:
	virtual ~IVhda();

	gint start();
	void stop();
	const gchar* get_name() const { return m_name; }
	gboolean get_active() const { return m_active; }
	void set_active(gboolean a_active) { m_active = a_active; }
	virtual void do_calculation() = 0;

protected:
	IVhda(const gchar *a_name, gboolean a_active);

private:
	gchar *m_name;
	gboolean m_active;
	GThread *m_thread;
	gboolean m_do_logging;
	guint m_interval;

	void run();
	static void* helper(gpointer a_user_data);

};

#endif /* IVHDA_H_ */
