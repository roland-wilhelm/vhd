/*
 * main.cpp
 *
 *  Created on: Jan 3, 2014
 *      Author: roland
 */

#include "debug.h"
#include "WatchConnman.h"

#include <glib.h>
#include <gio/gio.h>

#define LOG_FILE_PATH "/var/log/"
#define LOG_FILE_NAME LOG_FILE_PATH "vhd"

static GMainLoop *loop = NULL;

static void
signals_handler(int signum)
{

	g_main_loop_quit(loop);
}

int main(int argc, char **argv) {

	WatchConnman *manager = NULL;
	GError *error = NULL;
	gint sig_id = 0;
	//g_type_init();

	/* Setup signals */
	signal(SIGINT, signals_handler);
	signal(SIGHUP, signals_handler);
	signal(SIGTERM, signals_handler);


	manager = new WatchConnman();

	//debug_open((char *)LOG_FILE_NAME);

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	//debug_close();

	g_main_loop_unref(loop);

	return 0;
}
