/****************************************************************************/
/*                                                                          */
/* CanMsgDisp.h    Definition of the CAN message dispatcher                 */
/*                                                                          */
/*                 Written for BMW Forschung und Technik GmbH, ZT4          */
/*                 by smartup solution GmbH                                 */
/*                                                                          */
/*                                                                          */
/* Version History                                                          */
/*                                                                          */
/* <JJJJ-MM-DD>	<1.0>	<Marco Anzinger>	Initial Version					*/
/*                                                                          */
/****************************************************************************/
#include <stdlib.h>
#include "CanMsgApi.h"

#define ID_ALL_CAN_MESSAGES		0xFFFF;


// CAN dispatcher element
typedef struct CanDspElmt  
{
	BYTE CanBusId;
	WORD CanId;
	CanMsgReceivedCbFunc Receiver;
	struct CanDspElmt * Next;

} CanDspElmt;	


// Function prototypes
void cmd_init(void);
void cmd_add_element(CanDspElmt * pListElement);
CanDspElmt * cmd_remove_element(CanDspElmt * pListElement);
int cmd_remove_all_elements(CanMsgReceivedCbFunc fpCanMsgReceiver);
int cmd_dispatch_can_msg(BYTE bCanBusId, WORD wCanDispId, RxCanMsg * pgrRxCanMsg);
CanDspElmt * cmd_is_in_busId_canId(BYTE bCanBusId, WORD wCanId);
CanDspElmt * cmd_is_in_Receiver(CanMsgReceivedCbFunc fpCanMsgReceiver);
//void cmd_print_list(void);



