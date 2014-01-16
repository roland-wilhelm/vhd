/*
 * IParameter.h
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#ifndef IPARAMETER_H_
#define IPARAMETER_H_

#include "types.h"

class ICooperative;

class IParameter {
public:
	virtual ~IParameter();

	virtual void update(IParameter *a_parameter) = 0;
	virtual eParameter get_parameter_type() const { return m_parameter_type; }

protected:
	IParameter(eParameter a_parameter);

private:
	eParameter m_parameter_type;

};

#endif /* IPARAMETER_H_ */
