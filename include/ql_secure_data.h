/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2016
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_secure_data.h 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   Secure user data storage APIs definition.
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

#ifndef __SECURE_DATA_H__
#define __SECURE_DATA_H__

/*****************************************************************
* Function:     Ql_SecureData_Store 
* 
* Description:
*              This function can be used to store some critical user data 
*              to prevent them from losing.
*
*              Note:
*              1.
*              OpenCPU has designed 63 blocks of system storage space to 
*              backup critical user data. Developer may specify the first
*              parameter index [1-63] to specify different storage block. 
 *              Each block can store 2048 bytes.
*
*              2.
*              User should not call this API function frequently, which is not
*              good for life cycle of flash.
*
* Parameters:
*              index:
*               [in]  the index of the secure data block. The range is: 1~63.
*              
*               pData: 
*                   [in] The data to be stored. 
*
*               len:
*                   [in] The length of the data.
* Return:        
*               QL_RET_OK, this function succeeds.
*               QL_RET_ERR_PARAM, invalid paramter.
*               QL_RET_ERR_GET_MEM, the heap memory is no enough.
*               ......
*****************************************************************/
s32 Ql_SecureData_Store(u8 index , u8* pData, u32 len);

/*****************************************************************
* Function:     Ql_SecureData_Read 
* 
* Description:
*              This functin reads secure data which is previously 
*              stored by Ql_SecureData_Store.
* Parameters:
*               index:
*                   [in] The index of the secure data block. The range is: 1~63.
*
*               len:
*                   [in] The length of the data.
* Return:        
*               If this function succeeds, the real read length is returned.
*               QL_RET_ERR_PARAM, invalid paramter.
*               QL_RET_ERR_GET_MEM, the heap memory is no enough.
*               Ql_RET_ERR_UNKOWN, unknown error.
*****************************************************************/
s32 Ql_SecureData_Read(u8 index, u8* pBuffer, u32 len);

#endif  //__SECURE_DATA_H__

