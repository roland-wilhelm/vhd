/*
 * Qmi.h
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#ifndef QMI_H_
#define QMI_H_

#include "INetwork.h"

class Qmi : public INetwork{
public:
	Qmi(const gchar *a_object_path);
	virtual ~Qmi();
};

#endif /* QMI_H_ */
