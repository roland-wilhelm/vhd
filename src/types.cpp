/*
 * types.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#include "types.h"

#include <glib.h>
#include "debug.h"



eState string2State(const gchar *a_state) {

	eState state = StateUnknown;

	if(a_state == NULL) {

		ERR("no string state given.");
		return StateUnknown;
	}

	if(g_strcmp0(a_state, "idle") == 0) {

		state = StateIdle;
	}
	else if(g_strcmp0(a_state, "failure") == 0) {

		state = StateFailure;
	}
	else if(g_strcmp0(a_state, "association") == 0) {

		state = StateAssociation;
	}
	else if(g_strcmp0(a_state, "configuration") == 0) {

		state = StateConfiguration;
	}
	else if(g_strcmp0(a_state, "ready") == 0) {

		state = StateReady;
	}
	else if(g_strcmp0(a_state, "disconnect") == 0) {

		state = StateDisconnect;
	}
	else if(g_strcmp0(a_state, "online") == 0) {

		state = StateOnline;
	}

	//DBG2("string state '%s' --> state '%u'.", a_state, state);

	return state;
}

const gchar* state2String(eState a_state) {

	//DBG3("state '%u'", a_state);

	switch(a_state) {

		case StateIdle:
			return "idle";
			break;

		case StateFailure:
			return "failure";
			break;

		case StateAssociation:
			return "association";
			break;

		case StateConfiguration:
			return "configuration";
			break;

		case StateReady:
			return "ready";
			break;

		case StateDisconnect:
			return "disconnect";
			break;

		case StateOnline:
			return "online";
			break;

		default:
			return "unknown";
			break;
	}

	return NULL;
}

eRat string2Rat(const gchar *a_rat) {

	eRat rat = RatUnknown;

	//DBG3();

	if(a_rat == NULL) {

		ERR("no string state given.");
		return RatUnknown;
	}

	if(g_strcmp0(a_rat, "ethernet") == 0) {

		return RatEthernet;
	}
	else if(g_strcmp0(a_rat, "wifi") == 0) {

		return RatWifi;
	}
	else if(g_strcmp0(a_rat, "qmi") == 0) {

		return RatQmi;
	}

	//DBG2("string rat '%s' --> state '%u'.", a_rat, rat);

	return rat;
}

const gchar* rat2String(eRat a_rat) {


	//DBG3("rat type '%u'", a_rat);

	switch(a_rat) {

		case RatEthernet:
			return "ethernet";
			break;

		case RatWifi:
			return "wifi";
			break;

		case RatQmi:
			return "qmi";
			break;

		default:
			return "unknown";
			break;
	}

	return NULL;
}


