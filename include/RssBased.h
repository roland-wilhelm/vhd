/*
 * RssBased.h
 *
 *  Created on: Jan 15, 2014
 *      Author: roland
 */

#ifndef RSSBASED_H_
#define RSSBASED_H_

#include "IVhda.h"
#include "ConnmanManager.h"

class RssBased : public IVhda {
public:
	RssBased(gboolean a_active, ConnmanManager &a_connman);
	virtual ~RssBased();

	virtual void do_calculation();

private:
	ConnmanManager &m_connman;

	static gint find_active_network(gconstpointer a_net, gconstpointer a_user_data);
};

#endif /* RSSBASED_H_ */
