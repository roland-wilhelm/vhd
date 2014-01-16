/*
 * Wlan.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#include "Wlan.h"

#include "debug.h"
#include "types.h"

#include <glib.h>

Wlan::Wlan(const gchar *a_object_path) : INetwork(a_object_path, RatWifi) {


}

Wlan::~Wlan() {


}


