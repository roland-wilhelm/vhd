/****************************************************************************/
/*                                                                          */
/* IpMsgDef.c      Definition of the CAN message API over IP                */
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
// Defines
// CAN bus ids
#ifndef CAN_BUS_ID
#define CAN_BUS_ID
#define CAN_BUS_ID_PT_CAN		1
#define CAN_BUS_ID_K_CAN		2
#define CAN_BUS_ID_ERGO_CAN		3
#define CAN_BUS_ID_MT_CAN		4
#define CAN_BUS_ID_FA_CAN		5
#define CAN_BUS_ID_BODY_CAN		6
#define CAN_BUS_ID_A_CAN		7
#define CAN_BUS_ID_D_CAN		8
#define CAN_BUS_ID_S_CAN		9
#define CAN_BUS_ID_I_CAN		10
#define CAN_BUS_ID_DIAGNOSE_CAN 11
#endif


// IP message id
#define IM_TX_CAN_MESSAGE				1
#define IM_SUBSCRIBE_CAN_MESSAGES		2
#define IM_RX_CAN_MESSAGE				3
#define IM_UNSUBSCRIBE_CAN_MESSAGES		4
#define IM_SET_CAN_BAUD_RATE			5
#define IM_API_KEEPALIVE				65		// 65 = ASCII 'A' (for Alive)
#define IM_API_BYE						66		// 66 = ASCII 'B' (for Bye)
#define IM_API_PING						80		// 80 = ASCII 'P' (for Ping)
#define IM_API_ECHO						69		// 112= ASCII 'E' (for Echo)


// Structures

#pragma pack(1)		// Enable byte wise alignment for all IP message structures

// Structure for all ip message headers
typedef struct {

	BYTE MsgType;
	//WORD PktLength;

} im_Header;


// Structure for IM_TX_CAN_MESSAGE
typedef struct {

	BYTE MsgType;
//	WORD PktLength;

	BYTE CanBusId;
	
	struct {
		WORD Id;
		BYTE MsgType;
		BYTE Length;
		BYTE Data[8];
	} CanMsg;

} im_TxCanMsg;


// Structure for IM_SET_CAN_BAUD_RATE
typedef struct {

	BYTE MsgType;
//	WORD PktLength;

	BYTE CanBusId;
	WORD CanBaud;

} im_SetCanBaud;


// Structure for IM_SUBSCRIBE_CAN_MESSAGES
typedef struct {

	BYTE MsgType;
//	WORD PktLength;

	BYTE CanBusId;
	WORD ListLength;
	WORD List[2048];

} im_SubsCanMsg;


// Structure for IM_SUBSCRIBE_CAN_MESSAGES
typedef struct {

	BYTE MsgType;
//	WORD PktLength;

	BYTE CanBusId;
	WORD ListLength;
	WORD List[2048];

} im_UnSubsCanMsg;


// Structure for IM_RX_CAN_MESSAGE
typedef struct {

	BYTE MsgType;
//	WORD PktLength;

	BYTE CanBusId;

	WORD CanDispId;
	
	struct {
		WORD Id;
		BYTE MsgType;
		BYTE Length;
		BYTE Data[8];
	}CanMsg;

	struct {
		DWORD millis;
		WORD millis_overflow;
		WORD micros;     
	}Timestamp;

} im_RxCanMsg;

#pragma pack()		// Switch back to default alignment