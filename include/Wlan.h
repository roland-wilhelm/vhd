/*
 * Wlan.h
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#ifndef WLAN_H_
#define WLAN_H_

#include "INetwork.h"

class Wlan : public INetwork {
public:
	Wlan(const gchar *a_object_path);
	virtual ~Wlan();

private:

};

#endif /* WLAN_H_ */
