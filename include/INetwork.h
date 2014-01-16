/*
 * INetwork.h
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#ifndef INETWORK_H_
#define INETWORK_H_

#include "types.h"

class INetwork {
public:
	virtual ~INetwork();

	virtual void update(INetwork *a_network);
	virtual void set_strength(guint a_strength) { m_strength = a_strength; }
	virtual void set_state(eState a_state) { m_state = a_state; }
	virtual const gchar* get_name() { return m_name; }
	virtual void set_name(const gchar * a_name);
	virtual eRat get_rat() const { return m_rat; }
	virtual eState get_state() const { return m_state; }
	virtual const gchar* get_object_path() const { return m_object_path; }
	virtual guint get_strength() const { return m_strength; }
	virtual guint get_strength_average() const { return m_strength_average; }
	virtual void set_strength_average(guint a_strength_average) { m_strength_average = a_strength_average; }
	virtual void print_network();

protected:
	INetwork(const gchar* a_object_path, eRat a_rat);

private:
	GList *list_parameters;
	gchar *m_object_path;
	guint m_strength;
	guint m_strength_average;
	eRat m_rat;
	eState m_state;
	gchar *m_name;

};

#endif /* INETWORK_H_ */
