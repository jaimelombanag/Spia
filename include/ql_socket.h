/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2013
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_socket.h 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   socket APIs defines.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/
 

#ifndef __QL_SOCKET_H__
#define __QL_SOCKET_H__

/* Socket Type */ 
typedef enum {
    SOC_TYPE_TCP = 0,  /* TCP */ 
    SOC_TYPE_UDP       /* datagram socket, UDP */ 
} Enum_SocketType;

/****************************************************************************
 * Return Codes Definition for TCP/IP
 ***************************************************************************/
typedef enum {
    SOC_SUCCESS             = 0,
    SOC_ERROR               = -1,
    SOC_WOULDBLOCK          = -2,
    SOC_LIMIT_RESOURCE      = -3,    /* limited resource */
    SOC_INVALID_SOCKET      = -4,    /* invalid socket */
    SOC_INVALID_ACCOUNT     = -5,    /* invalid account id */
    SOC_NAMETOOLONG         = -6,    /* address too long */
    SOC_ALREADY             = -7,    /* operation already in progress */
    SOC_OPNOTSUPP           = -8,    /* operation not support */
    SOC_CONNABORTED         = -9,    /* Software caused connection abort */
    SOC_INVAL               = -10,   /* invalid argument */
    SOC_PIPE                = -11,   /* broken pipe */
    SOC_NOTCONN             = -12,   /* socket is not connected */
    SOC_MSGSIZE             = -13,   /* msg is too long */
    SOC_BEARER_FAIL         = -14,   /* bearer is broken */
    SOC_CONNRESET           = -15,   /* TCP half-write close, i.e., FINED */
    SOC_DHCP_ERROR          = -16,
    SOC_IP_CHANGED          = -17,
    SOC_ADDRINUSE           = -18,
    SOC_CANCEL_ACT_BEARER   = -19    /* cancel the activation of bearer */
} Enum_SocErrCode;

typedef struct {
    void(*callback_socket_connect)(s32 socketId, s32 errCode, void* customParam );
    void(*callback_socket_close)(s32 socketId, s32 errCode, void* customParam );
    void(*callback_socket_accept)(s32 listenSocketId, s32 errCode, void* customParam );/*this function is reserved not used */
    void(*callback_socket_read)(s32 socketId, s32 errCode, void* customParam );
    void(*callback_socket_write)(s32 socketId, s32 errCode, void* customParam );
}ST_SOC_Callback;

typedef struct {
    s32 optionType;
    void* pOptionParam;
} ST_SocketOption;

typedef enum
{
	SOC_INIT = 0,
	SOC_CREATE,
	SOC_CONNECTING,
	SOC_LISTEN,
	SOC_CONNECTED,
	SOC_CLOSEING,
	SOC_CLOSED = SOC_INIT
}ST_SOC_STATE;

typedef void (*Callback_IpHelper_GetIpByName)(u8 contexId, u8 requestId, s32 errCode,  u32 ipAddrCnt);


typedef struct
{
	u8                         pdp_ctxid;
	u8                         soc_id;
	u8                         soc_type;
	u8                         soc_state;
	u8                         remote_ip[128];
	u16                       remote_port;
	u16                       local_port;
	u64                       tcp_ack_no;
	ST_SOC_Callback  soc_cb_fcn;
	void *                    user_param;
}ST_Socket;

typedef struct
{
	u8  soc_id;
	u8  *data_ptr;
	u32 offset;
	u32  data_len;
}ST_SOC_RW_Struct;

typedef struct
{
	s32   listen_soc_id;
	s32   accept_soc_id;
	u8     accept_ip[128];
	u16   accept_port;
}ST_TCP_InCome_tag;

/*****************************************************************
* Function:     Ql_SOC_Register
*
* Description:
*               This function registers callback functions for the specified socket.
*
* Parameters:
*               cb:
*                   [In] The pointer of the socket-related callback function.
*
*               customParam:
*                   [In] One customized parameter that can be passed into the callback functions.
*
* Return:
*               The socket ID, or other Error Codes. To get extended
*               information, please see Enum_SocErrCode.
*****************************************************************/
s32 Ql_SOC_Register(ST_SOC_Callback *cb, void* customParam);

/*****************************************************************
* Function:     Ql_SOC_Create
*
* Description:
*               This function creates a socket. The maximum number of socket is 6.
*
* Parameters:
*               contxtId:
*                   OpenCPU supports two PDP-contexts to the destination
*                   host at a time. This parameter can be 0 or 1.
*
*               socketType:
*                   A value of socket_type_enum.
*
* Return:
*               The socket ID, or other Error Codes. To get extended
*               information, please see Enum_SocErrCode.
*****************************************************************/
s32     Ql_SOC_Create(u8 contextId, u8 socketType);
/*****************************************************************
* Function:     Ql_SOC_Connect
*
* Description:
*               This function connects to TCP server. The server
*               is specified by an IP address and a port number.
*
* Parameters:
*               socketId:
*                   A socket Id.
*
*               address:
*                   Peer IPv4  address or an name . for example is "192.168.1.1" or "www.baidu.com"
*
*               port:
*                   Socket port.
* Return:
*               SOC_SUCCESS: the function succeeds.
*               SOC_WOULDBLOCK: this return value is for none-blocking mode. 
*                   It mean the operation is in progressing, needs to wait callback_socket_connect() for the result.
*               Other values: error code, please refer to Enum_SocErrCode.
*****************************************************************/
s32   Ql_SOC_Connect(s32 socketId, u8 *remoteIP, u16 remotePort);

/*****************************************************************
* Function:     Ql_SOC_Close
*
* Description:
*               This function closes a socket.
*
* Parameters:
*               socketId:
*                   A socket Id.
*
* Return:
*               This return value will be SOC_SUCCESS (0) if
*               the function succeeds; or a negative number
*               (Error Code) will be returned.
*****************************************************************/
s32   Ql_SOC_Close(s32 socketId);

/*****************************************************************
* Function:     Ql_SOC_Send
*
* Description:
*               The function sends data to a connected TCP socket.
*
* Parameters:
*               socketId:
*                   Socket Id.
*
*               pData:
*                   Pointer to the data to send.
*
*               dataLen:
*                   Number of bytes to send.
* Return:
*               If no error occurs, Ql_SOC_Send returns the total
*               number of bytes sent, which can be less than the number
*               requested to be sent in the dataLen parameter.
*               Otherwise, a value of Enum_SocErrCode is returned.
*****************************************************************/
s32   Ql_SOC_Send(s32 socketId, u8* pData, s32 dataLen);

/*****************************************************************
* Function:     Ql_SOC_SendTo
*
* Description:
*               The function sends data to the specified UCP socket.
*
* Parameters:
*               socketId:
*                   Socket Id.
*
*               pData:
*                   Pointer to the data to send.
*
*               dataLen:
*                   Number of bytes to send.
*
*               remoteIP:
*                   [out] An optional pointer to a buffer that
*                   receives the address of the connecting entity.
*
*               remotePort:
*                   [out] An optional pointer to an integer that
*                   contains the port number of the connecting entity.
* Return:
*               If no error occurs, Ql_SOC_Send returns the total
*               number of bytes sent, which can be less than the number
*               requested to be sent in the dataLen parameter.
*               Otherwise, a value of Enum_SocErrCode is returned.
*****************************************************************/
s32   Ql_SOC_SendTo(s32 socketId, u8* pData, s32 dataLen, char* remoteIP, u16 remotePort);

/*****************************************************************
* Function:     Ql_SOC_Recv
*
* Description:
*               The function receives data from a bound socket.
*
* Parameters:
*               socketId:
*                   A socket Id.
*
*               pBuffer:
*                   Point to a buffer that is the storage space
*                   for the received data.
*
*               bufferLen:
*                   Length of pData, in bytes.
* Return:
*               If no error occurs, Ql_SOC_Recv returns the total
*               number of bytes received. Otherwise, a value of
*               Enum_SocErrCode is returned.
*****************************************************************/
s32   Ql_SOC_Recv(s32 socketId, u8* pBuffer, s32 bufferLen);


/*****************************************************************
* Function:     Ql_SOC_GetAckNumber
*
* Description:
*               The function gets the TCP socket ACK number.
*
* Parameters:
*               socket:
*                   [in] Socket Id.
*
*               ackNum:
*                   [out] Point to a u64 type  variable that is the storage
*                   space for the TCP ACK number
* Return:
*               If no error occurs, this return value will be SOC_SUCCESS (0). 
*               Otherwise, a value of "Enum_SocErrCode" is returned.
*****************************************************************/
s32   Ql_SOC_GetAckNumber(s32 socketId, u64* ackNum);
/*****************************************************************
* Function:     Ql_SOC_Listen
*
* Description:
*               The function places a socket in a state in which
*               it is listening for an incoming connection.
*
* Parameters:
*               listenSocketId:
*                   The listened socket id.
*
*
*               maxClientNum:
*                   Maximum connection number.
* Return:
*               If no error occurs, this function returns QL_SOC_SUCCESS (0).
*               Otherwise, a value of ql_soc_error_enum is returned.
*****************************************************************/
s32   Ql_SOC_Listen(s32 listenSocketId, s32 maxClientNum);


/*****************************************************************
* Function:     Ql_SOC_Bind
*
* Description:
*               This function associates a local address with a socket.
*
* Parameters:
*               socketId:
*                   Descriptor identifying an unbound socket.
*

*               LocalPort:
*                   Socket port number.
*
* Return:
*               If no error occurs, this function returns QL_SOC_SUCCESS (0).
*               Otherwise, a value of ql_soc_error_enum is returned.
*****************************************************************/
s32   Ql_SOC_Bind(s32 socketId, u16 LocalPort);


/*****************************************************************
* Function:     Ql_SOC_Accept
*
* Description:
*               The function permits a connection attempt on a socket.
*
* Parameters:
*               listenSocketId:
*                   [in]  The listened socket id.
*
*               remoteIP:
*                   [out] An optional pointer to a buffer that
*                   receives the address of the connecting entity.
*
*               remotePort:
*                   [out] An optional pointer to an integer that
*                   contains hte port number of the connecting entity.
*
* Return:
*               If no error occurs, this function returns a socket Id,
*               which is greater than or equal to zero.
*               Otherwise, a value of ql_soc_error_enum is returned.
*****************************************************************/
s32   Ql_SOC_Accept(s32 listenSocketId, u32 * remoteIP, u16* remotePort);


/*****************************************************************
* Function:     Ql_IpHelper _GetIPByHostName
*
* Description:
*               The function retrieves host IP corresponding to a host name.
*
* Parameters:
*               contxtId:
*                   [in] OpenCPU supports two PDP-contexts to the destination
*                   host at a time. This parameter can be 0 or 1.
*
*               hostName:
*                   [in] The host name.
*
*               requestId:
*	             [Out] Embedded in response message.
*
*               errCode:	
*                   [Out] Error code if fail
*
*               ipAddrCnt: 	
*                   [Out] Get address number.
*
*               ipAddr:
*                   [Out] The host IPv4 address.
*
*               callback_GetIpByName:
*                   [in] This callback is called by Core System to notify
*                   whether this function retrieves host IP successfully or not.
*
* Return:
*               If no error occurs, this function returns SOC_SUCCESS (0).
*               Otherwise, a value of Enum_SocErrCode is returned.
*****************************************************************/
s32 Ql_IpHelper_GetIPByHostName(u8 contextId, 
                                u8 requestId,
                                u8 *hostName, 
                                Callback_IpHelper_GetIpByName  callback_GetIpByName);

s32 Ql_IpHelper_Read_IpAddr(u8 contextId,
	                                                      u8 requestId,
	                                                      u8 ip_index,
	                                                      u32 *ip_addr);
#endif //__QL_TCPIP_H__
