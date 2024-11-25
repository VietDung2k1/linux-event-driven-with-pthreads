/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "pos.h"

enum {
	/* shell task */
	TASK_SHELL_ID,

	/* demo task */
	TASK_DEMO_ID,

	/* end task */
	POS_TASK_END_ID,
};

extern pos_task_t table_task[];

#endif