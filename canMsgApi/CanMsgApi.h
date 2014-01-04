/****************************************************************************/
/*                                                                          */
/* CanMsgApi.h     Definition of the CAN message API                        */
/*                                                                          */
/*                 Written for BMW Forschung und Technik GmbH, ZT4          */
/*                 by smartup solution GmbH                                 */
/*                 Ported to Linux by Dennis Lenz, SOLIN AG                 */
/*                                                                          */
/* Version History                                                          */
/*                                                                          */
/* <JJJJ-MM-DD>	<1.0>	<Marco Anzinger>	Initial Version					*/
/* <2010-11-02> <1.0L>  <Dennis Lenz>       Linux Port                      */
/****************************************************************************/
#ifndef CAN_MSG_API_H
#define CAN_MSG_API_H


#ifdef __cplusplus
extern "C"
{
#endif


// Predefinitions
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef BOOL
typedef int BOOL;
#define TRUE   (1)
#define FALSE  (0)
#endif


#ifndef HANDLE
typedef unsigned int HANDLE;
#endif
#ifndef SOCKET
typedef int SOCKET;
#define INVALID_SOCKET				-1
#endif
#ifndef SOCKADDR_IN
typedef struct sockaddr_in SOCKADDR_IN;
#endif




// Default CAN message server address
#define CAN_MSG_SRV_DEFAULT_IP		"127.0.0.1"
#define CAN_MSG_SRV_DEFAULT_PORT	"7272"

// CAN bus ids
#ifndef CAN_BUS_ID
#define CAN_BUS_ID
#define CAN_BUS_ID_PT_CAN		1 /* Powertrain-can */
#define CAN_BUS_ID_K_CAN		2 /* Karosserie-can Bordnetz */
#define CAN_BUS_ID_ERGO_CAN		3
#define CAN_BUS_ID_MT_CAN		4
#define CAN_BUS_ID_FA_CAN		5
#define CAN_BUS_ID_BODY_CAN		6
#define CAN_BUS_ID_A_CAN		7
#define CAN_BUS_ID_D_CAN		8
#define CAN_BUS_ID_S_CAN		9 /* Sensor can */
#endif


// Message types
#define MSGTYPE_STANDARD		0x00
#define MSGTYPE_RTR				0x01
#define MSGTYPE_EXTENDED		0x02
#define MSGTYPE_ERRFRAME		0x40
#define MSGTYPE_STATUS			0x80


// CAN bus baudrates
#define CAN_BAUD_1M				0x0014    //   1 MBit/s
#define CAN_BAUD_500K			0x001C    // 500 kBit/s
#define CAN_BAUD_250K			0x011C    // 250 kBit/s
#define CAN_BAUD_125K			0x031C    // 125 kBit/s
#define CAN_BAUD_100K			0x432F    // 100 kBit/s
#define CAN_BAUD_50K			0x472F    //  50 kBit/s
#define CAN_BAUD_20K			0x532F    //  20 kBit/s
#define CAN_BAUD_10K			0x672F    //  10 kBit/s
#define CAN_BAUD_5K				0x7F7F    //   5 kBit/s

// Special CAN messages
#define ID_ALL_CAN_MESSAGES		0xFFFF;


typedef struct {

	BYTE CanBusId;
	
	struct {
		WORD Id;
		BYTE MsgType;
		BYTE Length;
		BYTE Data[8];
	}CanMsg;

} TxCanMsg;


typedef struct {

	BYTE CanBusId;
	
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

} RxCanMsg;


// Function prototype for callback 
typedef void (*CanMsgReceivedCbFunc)(RxCanMsg * pgrRxCanMsg);


// Function prototypes

/**
 * Open connection to CAN message server
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	szIp		IP address as string
 *			wPort		IP port
 *			dwTimeout	Timeout in microseconds until CAN message server has to respond
 *
 * @return	0	OK
 *			-1	Error at WSA startup
 *			-2	Error at creating socket
 *          -3	Error at sending "ping" (sendto() error)
 *          -5  Answer on ping (-> echo) timed out
 */
int cma_open_connection(char * szIp, char * wPort, DWORD dwTimeout);

/**
 * Close connection to CAN message server
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	-
 *
 * @return	0	OK
 *			-1	Error at closesocket()	
 *			-2	Error at WSAcleanup()
 */
int cma_close_connection(void);

/**
 * Send CAN message into physical CAN
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	pgrTxCanMsg		CAN Message to send
 *
 * @return	0	OK
 *			-1	ERROR at sendto()
 */
int cma_send_msg(TxCanMsg * pgrTxCanMsg);

/**
 * Subscribe CAN messages for a CAN message receiver function
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	ubCanBusId			Subscribe CAN bus ID		
 * @param	uwListSize			Subscribe CAN message ID list length
 * @param	uwaList				Subscribe CAN message ID list
 * @param	fpCanMsgReceiver	Function pointer to receiver function
 *
 * @return	Number of CAN message ID's added to IM_SUBSCRIBE_CAN_MESSAGES
 *			-1 if a socket error occured
 */
int cma_subscribe_msg(BYTE ubCanBus, WORD uwListSize, WORD uwaList[], CanMsgReceivedCbFunc fpCanMsgReceiver);

/**
 * Subscribe all CAN messages from one Bus ID for a CAN message receiver function
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	ubCanBusId			Subscribe CAN bus ID		
 * @param	fpCanMsgReceiver	Function pointer to receiver function
 *
 * @return	Number of CAN message ID's added to IM_SUBSCRIBE_CAN_MESSAGES
 *			-1 if a socket error occured
 */
int cma_subscribe_all_msg(BYTE ubCanBusId, CanMsgReceivedCbFunc fpCanMsgReceiver);

/**
 * Unsubscribe CAN messages of a CAN message receiver function
 *
 * @author	Marco Anzinger
 * @version	1.0
 * 
 * @param	fpCanMsgReceiver	Function pointer to receiver function
 *
 * @return	Number of removed CAN message ID's
 */
int cma_unsubscribe_msg(CanMsgReceivedCbFunc fpCanMsgReceiver);


#ifdef __cplusplus
}
#endif

#endif
