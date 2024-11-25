/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#ifndef __POS_LOG_H__
#define __POS_LOG_H__

#include "sys_log.h"

#ifdef POS_LOG_ENABLE
#define POS_LOG_ERROR	SYS_LOGE
#define POS_LOG_WARNING SYS_LOGW
#define POS_LOG_INFO	SYS_LOGI
#define POS_LOG_DEBUG	SYS_LOGD
#else
#define POS_LOG_ERROR(...)	 (void)0
#define POS_LOG_WARNING(...) (void)0
#define POS_LOG_INFO(...)	 (void)0
#define POS_LOG_DEBUG(...)	 (void)0
#endif

#endif