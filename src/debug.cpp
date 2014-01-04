/*
 * debug.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: roland
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>

#include "debug.h"


#define DEFAULT_OUTPUT stdout
static FILE *debug_fp = DEFAULT_OUTPUT;


static const char* get_time_string() {

	static time_t now;
	static struct tm *local_time;
	static char time_str[9];

	now = time((time_t *) NULL);
	local_time = localtime(&now);
	strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);

	return time_str;
}


void debug_print(const char *name, const char *fmt, ...) {

	char s[1024];
	va_list args;

	va_start(args, fmt);
	vsnprintf(s, sizeof(s), fmt, args);
	va_end(args);

	fprintf(debug_fp, "[%s] ", get_time_string());

	if(name)
		fprintf(debug_fp, "%s ", name);

	fprintf(debug_fp, "%s\n", s);
	fflush(debug_fp);

}

int debug_open(char *path) {

	FILE *fp = NULL;
	time_t now;
	struct tm *local_time;
	char time_str[11], filepath[100];

	if(!path)
		return -EINVAL;

	if(debug_fp && (debug_fp != DEFAULT_OUTPUT))
		return -EALREADY;

	/* create file path with filename and time stamp to store debug information */
	now = time((time_t *) NULL);
	local_time = localtime(&now);
	strftime(time_str, sizeof(time_str), "%Y%m%d", local_time);
	snprintf(filepath, sizeof(filepath), "%s-%s.log", path, time_str);
	DBG("opening debug file '%s'", filepath);
	fp = fopen(filepath, "a");
	if(!fp) {

		debug_fp = DEFAULT_OUTPUT;
		ERR("cannot open file '%s', error '%d'", filepath, errno);
		return -errno;
	}
	else {

		debug_fp = fp;
	}

	return 0;
}

void debug_init() {


}

void debug_close(void) {

	if(debug_fp && (debug_fp != DEFAULT_OUTPUT)) {

		fflush(debug_fp);
		fclose(debug_fp);
		debug_fp = DEFAULT_OUTPUT;
	}

}


