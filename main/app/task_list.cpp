/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include "task_list.h"
#include "task_shell.h"
#include "task_demo.h"

pos_task_t table_task[] = {
	{TASK_SHELL_ID, 	task_shell, 	"_shell", 	NULL},
	{TASK_DEMO_ID,  	task_demo,  	"_demo",  	NULL},
};