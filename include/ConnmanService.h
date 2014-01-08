/*
 * ConnmanService.h
 *
 *  Created on: 08.01.2014
 *      Author: Roland
 */

#ifndef CONNMANSERVICE_H_
#define CONNMANSERVICE_H_

#include "ProxyDBus.h"


class ConnmanService : public ProxyDBus {
public:
	ConnmanService();
	virtual ~ConnmanService();
};

#endif /* CONNMANSERVICE_H_ */
