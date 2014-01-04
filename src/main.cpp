/*
 * main.cpp
 *
 *  Created on: Jan 3, 2014
 *      Author: roland
 */

#include "debug.h"


#define LOG_FILE_PATH "/var/log/"
#define LOG_FILE_NAME LOG_FILE_PATH "vhd"



int main(int argc, char **argv) {


	debug_open((char *)LOG_FILE_NAME);




	debug_close();

	return 0;
}
