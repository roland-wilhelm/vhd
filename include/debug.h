/*
 * debug.h
 *
 *  Created on: Jan 2, 2014
 *      Author: roland
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#define VHD_DEBUG_LEVEL 3

#if VHD_DEBUG_LEVEL >= 1
#define DBG _DBG
#else
#define DBG(...)
#endif /* DBG */

#if VHD_DEBUG_LEVEL >= 2
#define DBG2 _DBG
#else
#define DBG2(...)
#endif /* DBG2 */

#if VHD_DEBUG_LEVEL >= 3
#define DBG3 _DBG
#else
#define DBG3(...)
#endif /* DBG3 */


#define ERR(fmt, args...) \
		debug_print("ERROR **: ", "%s: %s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#define _DBG(fmt, args...) \
		debug_print("[DEBUG] : ", "%s: %s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##args)


void debug_print(const char *name, const char *fmt, ...);
int debug_open(char *path);
void debug_close(void);



#endif /* DEBUG_H_ */
