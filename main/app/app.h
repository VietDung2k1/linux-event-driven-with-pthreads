/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#ifndef __APP_H__
#define __APP_H__

#include "pos.h"
#include "pos_log.h"

/*****************************************************************************/
/*  global define variable
 */
/*****************************************************************************/
#define APP_FW_MODEL   "POS V1"
#define APP_FW_VERSION "1.0.0"
#ifdef DEBUG
#define APP_FW_TITLE APP_FW_MODEL "-DEBUG"
#else
#define APP_FW_TITLE APP_FW_MODEL "-PROD"
#endif

/*****************************************************************************/
/*  demo task define
 */
/****************************************************************************/
/* define timer */
#define AC_DEMO_TASK_TIMER_INTERVAL (1000)
/* define signal */
enum {
	AC_DEMO_TASK_POST = POS_USER_DEFINE_SIG,
	AC_DEMO_TASK_POST_AND_DATA,
};

#endif