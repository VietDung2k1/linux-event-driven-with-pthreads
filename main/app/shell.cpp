/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include "app.h"
#include "cmd_line.h"
#include "task_shell.h"
#include "pos_log.h"
#include "task_list.h"
#include "timer.h"

#define TAG "SHELL"

/*****************************************************************************/
/*  command function declare
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t *argv);
int32_t shell_ver(uint8_t *argv);
int32_t shell_help(uint8_t *argv);

/*****************************************************************************/
/*  command table
 */
/*****************************************************************************/
cmd_line_t lgn_cmd_table[] = {

	/*************************************************************************/
	/* system command */
	/*************************************************************************/
	{(const int8_t *)"reset", 	shell_reset,	(const int8_t *)"reset terminal"},
	{(const int8_t *)"ver",		shell_ver,	   	(const int8_t *)"version info"	},
	{(const int8_t *)"help",	shell_help,	 	(const int8_t *)"help info"		},

 /* end of table */
	{(const int8_t *)0,			(pf_cmd_func)0, (const int8_t *)0				}
};

/*****************************************************************************/
/*  command function definaion
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t *argv) {
	(void)argv;
	POS_LOG_INFO(TAG, "\033[2J\r");
	return 0;
}

int32_t shell_ver(uint8_t *argv) {
	(void)argv;
	POS_LOG_INFO(TAG, "-Firmware model:   %s", APP_FW_MODEL);
	POS_LOG_INFO(TAG, "-Firmware title    %s", APP_FW_TITLE);
	POS_LOG_INFO(TAG, "-Firmware version: %s", APP_FW_VERSION);
#ifdef DEBUG
	POS_LOG_INFO(TAG, "-Build type:       DEBUG");
#else
	POS_LOG_INFO(TAG, "-Build type:       PROD");
#endif
	POS_LOG_INFO(TAG, "-Uptime:           %u", timer_stick_get());
	return 0;
}

int32_t shell_help(uint8_t *argv) {
	uint32_t idx = 0;
	switch (*(argv + 5)) {
	default:
		POS_LOG_INFO(TAG, "\nCOMMANDS INFORMATION:");
		while (lgn_cmd_table[idx].cmd != (const int8_t *)0) {
			POS_LOG_INFO(TAG, "%s\n\t%s", lgn_cmd_table[idx].cmd, lgn_cmd_table[idx].info);
			idx++;
		}
		break;
	}
	return 0;
}