/*
 * RssBased.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: roland
 */

#include "RssBased.h"
#include "DataStorage.h"
#include "INetwork.h"
#include "debug.h"
#include "types.h"

RssBased::RssBased(gboolean a_active, ConnmanManager &a_connman) :
								m_connman(a_connman),
								IVhda("RssBased", a_active) {


}

RssBased::~RssBased() {

}

gint RssBased::find_active_network(gconstpointer a_net, gconstpointer a_user_data) {

	INetwork *network = (INetwork *)a_net;

	if(network->get_state() == StateOnline) {

		return 0;
	}

	return 1;
}

void RssBased::do_calculation() {

	GList *networks = NULL, *item_active = NULL;
	INetwork *network = NULL, *net_active = NULL;
	guint length = 0, i;

	DBG3();

	DataStorage::get_instance()->set_networks_reader_lock();

	networks = DataStorage::get_instance()->get_networks();
	if(networks == NULL) {

		ERR("no networks available");
		goto end;
	}

	item_active = g_list_find_custom(networks, NULL, find_active_network);
	if(item_active != NULL) {

		net_active = (INetwork *)item_active->data;
		DBG("active rat '%s', network '%s'", rat2String(net_active->get_rat()), net_active->get_name());
	}

	length = g_list_length(networks);
	DBG("'%u' networks available", length);
	for(i = 0; i < length; i++) {

		network = (INetwork *)g_list_nth_data(networks, i);

		if(g_strcmp0("dasistmeininternet", network->get_name()) == 0) {

			break;
		}
	}

end:

	DataStorage::get_instance()->set_networks_reader_unlock();

	if(network != NULL) {

		DBG("HO verdict, name '%s', service '%s'", network->get_name(), network->get_object_path());
		m_connman.verdict_ho(network->get_object_path());
	}

}
