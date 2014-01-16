/*
 * types.h
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <glib.h>

typedef enum {

	StateUnknown,
	StateIdle,
	StateFailure,
	StateAssociation,
	StateConfiguration,
	StateReady,
	StateDisconnect,
	StateOnline

}eState;

typedef enum {

	RatUnknown,
	RatEthernet,
	RatWifi,
	RatQmi

}eRat;

typedef enum {

	ParaUnknown,
	ParaVelocity,
	ParaGps,
	ParaCoverage,
	ParaCost,
	ParaLoad,
	ParaThroughput

}eParameter;

eState string2State(const gchar *a_state);
const gchar* state2String(eState a_state);

eRat string2Rat(const gchar *a_rat);
const gchar* rat2String(eRat a_rat);


#endif /* TYPES_H_ */
