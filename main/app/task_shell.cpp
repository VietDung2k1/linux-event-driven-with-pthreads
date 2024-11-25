/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include <unistd.h>

#include "pos.h"
#include "pos_log.h"
#include "app.h"
#include "task_shell.h"
#include "task_list.h"

#define TAG "SHELL"

void *task_shell(void *) {
	pos_wait_all_task_started();
	POS_LOG_INFO(TAG, "STARTED");

	shell_buffer_t shell_buffer = {0};
	char c						= 0xff;

	while (1) {
		c = getchar();
		if (shell_buffer.index < SHELL_BUFFER_SIZE - 1) {
			if (c == '\r' || c == '\n' || c == '.') {
				shell_buffer.data[shell_buffer.index]	  = c;
				shell_buffer.data[shell_buffer.index + 1] = 0;
				{
					/* parser with table */
					switch (cmd_line_parser(lgn_cmd_table, shell_buffer.data)) {
					case CMD_SUCCESS:
						break;

					case CMD_NOT_FOUND:
						if (shell_buffer.data[0] != '\r' && shell_buffer.data[0] != '\n') {
							SYS_PRINTF("cmd unknown\n");
						}
						break;

					case CMD_TOO_LONG:
						SYS_PRINTF("cmd too long\n");
						break;

					case CMD_TBL_NOT_FOUND:
						SYS_PRINTF("cmd table not found\n");
						break;

					default:
						SYS_PRINTF("cmd error\n");
						break;
					}
				}
				SYS_PRINTF("#\n");

				shell_buffer.index = 0;
			}
			else if (c == 8) {
				if (shell_buffer.index) {
					shell_buffer.index--;
				}
			}
			else {
				shell_buffer.data[shell_buffer.index++] = c;
			}
		}
		else {
			SYS_PRINTF("\nerror: cmd too long, cmd size: %d, try again !\n", SHELL_BUFFER_SIZE);
			shell_buffer.index = 0;
		}
	}

	return (void *)0;
}