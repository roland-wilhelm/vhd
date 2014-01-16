/*
 * Qmi.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: roland
 */

#include "Qmi.h"

#include "debug.h"
#include "types.h"

#include <glib.h>


Qmi::Qmi(const gchar *a_object_path) : INetwork(a_object_path, RatQmi) {


}

Qmi::~Qmi() {

}

