/****************************************************************************/
/*                                                                          */
/* Debug.h		Definition of the debug output                              */
/*                                                                          */
/*              Written for BMW Forschung und Technik GmbH, ZT4             */
/*              by smartup solution GmbH                                    */
/*                                                                          */
/*                                                                          */
/* Version History                                                          */
/*                                                                          */
/* <JJJJ-MM-DD>	<1.0>	<Marco Anzinger>	Initial Version					*/
/*                                                                          */
/****************************************************************************/


// Debugging level definition
#define DEBUGGING_HIGH			3			// Debug output enabled for: dbg_l() dbg_m() dbg_h()
#define DEBUGGING_MID			2			// Debug output enabled for: dbg_l() dbg_m()
#define DEBUGGING_LOW			1			// Debug output enabled for: dbg_l()
#define DEBUGGING_DISABLED		0			// Debug output disabled


// Debugging level
#define DEBUG_LEVEL		DEBUGGING_DISABLED

// Debug output
#define DEBUG_OUTPUT	stdout				// Standard output


// Debugging low
#if (DEBUG_LEVEL > DEBUGGING_DISABLED) 
#define dbg_l(...)  { (void) fprintf(DEBUG_OUTPUT, __VA_ARGS__); }
#else
#define dbg_l(...)  ((void) 0)
#endif

// Debugging mid
#if (DEBUG_LEVEL > DEBUGGING_LOW) 
#define dbg_m(...)  { (void) fprintf(DEBUG_OUTPUT, __VA_ARGS__); }
#else
#define dbg_m(...)  ((void) 0)
#endif

// Debugging high
#if (DEBUG_LEVEL > DEBUGGING_MID) 
#define dbg_h(...)  { (void) fprintf(DEBUG_OUTPUT, __VA_ARGS__); }
#else
#define dbg_h(...)  ((void) 0)
#endif