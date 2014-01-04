/****************************************************************************/
/*                                                                          */
/* Error.h		Definition of the error output                              */
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

#define ERR_OUTPUT	stderr
#define WARN_OUTPUT	stderr

#define err(...)  { (void) fprintf(ERR_OUTPUT, "ERROR: "); (void) fprintf(ERR_OUTPUT, __VA_ARGS__); }
#define warn(...)  { (void) fprintf(WARN_OUTPUT, "WARNING: "); (void) fprintf(WARN_OUTPUT, __VA_ARGS__); }
