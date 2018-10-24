/*==========================================================================
 |               Quectel OpenCPU --  User API
 |
 |              Copyright (c) 2010 Quectel Ltd.
 |
 |--------------------------------------------------------------------------
 |
 | File Description
 | ----------------
 |      Definitions for audio parameters & APIs
 |
 |--------------------------------------------------------------------------
 |
 |  Designed by    :    Jay XIN
 |  Coded by       :    Jay XIN
 |  Tested by      :    Stanley YONG
 |
 \=========================================================================*/

#ifndef __QL_AUDIO_H__
#define __QL_AUDIO_H__

#include "Ql_type.h"

#if 0
/****************************************************************************
 *  Built-in Audio Resource
 ***************************************************************************/
typedef enum QlAudioNameTag
{
    QL_AUDIO_EMS_CHIMES_HI = 1,
    QL_AUDIO_EMS_CHIMES_LO,
    QL_AUDIO_EMS_DING,
    QL_AUDIO_EMS_TADA,
    QL_AUDIO_EMS_NOTIFY,
    QL_AUDIO_EMS_DRUM,
    QL_AUDIO_EMS_CLAPS,
    QL_AUDIO_EMS_FANFARE,
    QL_AUDIO_EMS_CHORD_HI,
    QL_AUDIO_EMS_CHORD_LO,
    QL_AUDIO_1,
    QL_AUDIO_2,
    QL_AUDIO_3,
    QL_AUDIO_4,
    QL_AUDIO_5,
    QL_AUDIO_6,
    QL_AUDIO_7,
    QL_AUDIO_8,
    QL_AUDIO_9,
    QL_AUDIO_10,
    QL_AUDIO_11,
    QL_AUDIO_12,
    QL_AUDIO_13,
    QL_AUDIO_14,
    QL_AUDIO_15,
    QL_AUDIO_16,
    QL_AUDIO_17,
    QL_AUDIO_18,
    QL_AUDIO_19,
    QL_AUDIO_END
}
QlAudioName;


/****************************************************************************
 *  Audio Volume Level Definition
 ***************************************************************************/
typedef enum QlAudioVolumeLeveltag
{
    QL_AUDIO_VOLUME_LEVEL1 = 0,
    QL_AUDIO_VOLUME_LEVEL2,
    QL_AUDIO_VOLUME_LEVEL3,
    QL_AUDIO_VOLUME_LEVEL4,
    QL_AUDIO_VOLUME_LEVEL5,
    QL_AUDIO_VOLUME_LEVEL6,
    QL_AUDIO_VOLUME_LEVEL7,
    QL_AUDIO_VOLUME_MUTE,
    QL_AUDIO_VOLUME_LEVEL_END
}QlAudioVolumeLevel;


/****************************************************************************
 *  Audio Output Source
 ***************************************************************************/
typedef enum QlAudioPlayPathTag
{
    QL_AUDIO_PATH_NORMAL = 0,       /* speak1 */
    QL_AUDIO_PATH_HEADSET = 1,      /* earphone */
    QL_AUDIO_PATH_LOUDSPEAKER = 2,  /* speaker2, loudspeaker for free sound */
    QL_AUDIO_PATH_END
}QlAudioPlayPath;
#endif

/****************************************************************************
 *  Audio Format
 ***************************************************************************/
typedef enum QlAudioStreamFormattag
{
    QL_AUDIO_STREAMFORMAT_MP3 = 1,
    QL_AUDIO_STREAMFORMAT_AMR = 2,
    QL_AUDIO_STREAMFORMAT_WAV = 3,
    QL_AUDIO_STREAMFORMAT_EVRC_VAR_FULL = 4,    
	QL_AUDIO_STREAMFORMAT_EVRC_FIX_FULL = 5,
    QL_AUDIO_STREAMFORMAT_END
}QlAudioStreamFormat;

/****************************************************************************
 *  Audio Direct
 ***************************************************************************/
typedef enum QlAudioStreamDirecttag
{
    QL_AUDIO_REC_LINK_REVERSE,
    QL_AUDIO_REC_LINK_FORWARD,
    QL_AUDIO_REC_LINK_INVALID
}QlAudioStreamDirect;

/****************************************************************************
 *  Audio record stream state
 ***************************************************************************/
typedef enum QlAudioStreamStatetag
{
    QL_AUDIO_STATE_SUCCESS,			/* Requested command was accepted			*/
	QL_AUDIO_STATE_DONE,			/* Command has been carried out 			*/
	QL_AUDIO_STATE_ERROR,			/* Error occured with requested command 		*/
	QL_AUDIO_STATE_PRO_DATA,		/* there are record data to process 			*/	
	QL_AUDIO_STATE_GET_DATA,		/* there are record data to process 			*/
	QL_AUDIO_STATE_END
}QlAudioStreamState;

typedef enum
{
    QL_AUD_REC_FMT_BY_EXTENSION = -1,
    QL_AUD_REC_FMT_AMR = 0,
    QL_AUD_REC_FMT_WAV,
    QL_AUD_REC_FMT_WAV_ALAW,
    QL_AUD_REC_FMT_WAV_ULAW,
    QL_AUD_REC_FMT_WAV_DVI_ADPCM
}QlAudioRecordFormat;
#if 0
typedef enum
{
    QL_AUD_REC_QUALITY_LOW = 0,
    QL_AUD_REC_QUALITY_HIGH
}QlAudioRecordQuality;

typedef enum
{
    QL_AUD_REC_MIC1 = 0,    /*A1*/
    QL_AUD_REC_MIC2         /*A2,Headset*/
}QlAudioRecordDevice;

typedef enum
{
    QL_RING_TONE_ENABLE = 0,  /*normal mode*/
    QL_RING_TONE_DISABLE       /*quiet mode*/
}QlRingToneEnable;

typedef void (*OCPU_CB_GET_VOLUME)(s8 vol_level);

/* Define volume type.*/
typedef enum {
    VOL_TYPE_CTN  = 0 ,   /* MMI can apply to associate volume; call tone attribute */
    VOL_TYPE_KEY      ,   /* MMI can apply to associate volume; keypad tone attribute */
    VOL_TYPE_MIC      ,   /* microphone attribute */
    VOL_TYPE_GMI      ,   /* MMI can apply to associate volume; melody, imelody, midi attribute */
    VOL_TYPE_SPH      ,   /* MMI can apply to associate volume; speech sound attribute */
    VOL_TYPE_SID	  ,   /* side tone attribute */
    VOL_TYPE_MEDIA    ,   /* MMI can apply to associate volume; As MP3, Wave,... attribute */
    MAX_VOL_TYPE           
}volume_type_enum;


typedef enum
{
    QL_CONFIG_100MS_DETECT_THRESHOLD= 0, /*config 1400Hz and 2300Hz 100ms detect threshold*/
    QL_CONFIG_400MS_DETECT_THRESHOLD, /*config 1400Hz and 2300Hz 400ms detect threshold*/
    QL_CONFIG_DTMF_DETECT_THRESHOLD,  /*config DTMF detect threshold,excluding 1400Hz and 2300Hz */
}QlDTMFMODE;

 typedef struct
{
    QlDTMFMODE mode;//  mode config
    u32 prefixpause; //prefix pause number
    u32 lowthreshold; //low threshold value
    u32 highthreshold; //high threshold value
}QlDTMFCONFIG;

 typedef struct
{
    s16 key;   //<dtmfcode> DTMF tone code corresponding ASSCII 
                    //48  DTMF 0 
                    //49  DTMF 1 
                    //50  DTMF 2 
                    //51  DTMF 3 
                    //52  DTMF 4 
                    //53  DTMF 5 
                    //54  DTMF 6 
                    //55  DTMF 7 
                    //56  DTMF 8 
                    //57  DTMF 9 
                    //65  DTMF A 
                    //66  DTMF B
                    //67  DTMF C 
                    //68  DTMF D 
                    //42  DTMF * 
                    //35  DTMF # 
                    //69  1400Hz frequency 
                    //70  2300Hz frequency 
    s16 persisttime; //< persistence time> 
                              //100  100ms of the tone detected, only 1400Hz and 2300 Hz 
                              //400  400ms of the tone detected, only 1400Hz and 2300 Hz 
                              // 0    other tone.
}QlDTMFCode;

typedef void (*OCPU_CB_DTMF_INFO)(QlDTMFCode dtmfcode); //callback dtmf code
 #endif

/*****************************************************************
* Function:     _Ocpu_PlayAudioStreCb_t 
* 
* Description:
*               call back for  play audio stream. if the state is QL_AUDIO_STATE_GET_DATA,  you should copy data to the buffer immediately, otherwise we will set default value for the buffer.
* Parameters:
*               state: audio play  state
*		   buff: If the return state is QL_AUDIO_STATE_GET_DATA,  this buff is used to store playing data. For other state,  this para is null. 
*		   byte_nums: size of the buffer.
*              client_data: client data.
* Return:        
*               void
*****************************************************************/
typedef void (*_Ocpu_PlaydAudioStreCb_t)(QlAudioStreamState state, void* buff, u32 byte_nums, void* client_data);

/*****************************************************************
* Function:     Ql_StartPlayAudioStream 
* 
* Description:
*               Plays an audio stream with nd stream format,
*               and sets the items as below:
*                   - Repeatedly or just Once
*                   - Volume level
*               When audio plays ended,
*               a QL_AUDIO_STATE_DONE event will be received by embedded application.
* Parameters:
*               streamformat:
*                       Audio stream format
*
*               repeat:
*                       TRUE or FALSE, indicates that Core System
*                       play the audio repeatedly or just once.
*
*               volumelevel:
*                       Volume level reference "QlAudioVolumeLevel"
*
*		   fun:
*			    call back function for audio playing,  if fun=NULL, it will return QL_RET_ERR_PARAM.
*		   client_data:
*			    client data.
* Return:        
*               QL_RET_OK, suceess
*               or other error codes, please see 'Error Code Definition'
*****************************************************************/
s32 Ql_StartPlayAudioStream(s32 streamformat, u8 volume, _Ocpu_PlaydAudioStreCb_t fun, void* client_data);


/*****************************************************************
* Function:     Ql_StopPlayAudioStream 
* 
* Description:
*               Stops playing the audio stream.
* Parameters:
*               None
* Return:        
*****************************************************************/
void Ql_StopPlayAudioStream(void);

/*****************************************************************
* Function:     _Ocpu_RecordAudioStreCb_t 
* 
* Description:
*               call back for record the audio stream. if the state is QL_AUDIO_STATE_PRO_DATA,  you should copy data from the buffer immediately, otherwise the data will lose.
* Parameters:
*               state: recording  state
*		   buff: If the return state is QL_AUDIO_STATE_PRO_DATA,  the recording data is stored in this buffer. For other state,  this para is null. 
*		   byte_nums: size of the buffer.
*              client_data: client data.
* Return:        
*****************************************************************/
typedef void (*_Ocpu_RecordAudioStreCb_t)(QlAudioStreamState state, void* buff, u32 byte_nums, void* client_data);

/*****************************************************************
* Function:     Ql_StartRecordAudioStream 
* 
* Description:
*               Start Record the audio stream.
* Parameters:
*               streamformat:
*                       Audio stream format
*
*		   direction:
*			     record local or far-end
*
*		   fun: 
*			    call back function for audio record,  if fun = NULL, it will return QL_RET_ERR_PARAM.   
*
*		   client_data:
*			    client data.
* Return:        
*               0, indicates the function suceeds
*               or other error codes, please see 'Error Code Definition'
*****************************************************************/

s32 Ql_StartRecordAudioStream(QlAudioStreamFormat fomart,  QlAudioStreamDirect direction, _Ocpu_RecordAudioStreCb_t fun, void* client_data);

/*****************************************************************
* Function:     Ql_StoptRecordAudioStream 
* 
* Description:
*               Stop Record the audio stream.
* Parameters:
*               None
* Return:        
*****************************************************************/
void Ql_StopRecordAudioStream(void);


#endif // End-of __QL_AUDIO_H__

