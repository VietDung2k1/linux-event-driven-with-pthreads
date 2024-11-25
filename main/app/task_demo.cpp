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
#include "task_demo.h"
#include "task_list.h"
#include "string.h"

#define TAG "DEMO"

void *task_demo(void *) {
	pos_wait_all_task_started();
	POS_LOG_INFO(TAG, "STARTED");

	uint16_t msg_id	 = 0;
	void *msg_data	 = NULL;
	uint32_t msg_len = 0;

	/* create task post demo */
	pos_task_post_msg(TASK_DEMO_ID, AC_DEMO_TASK_POST, NULL, 0);

	const char *data = "Hello World!";
	pos_timer_set(TASK_DEMO_ID, AC_DEMO_TASK_POST_AND_DATA, (void *)data, strlen(data), AC_DEMO_TASK_TIMER_INTERVAL, true);

	while (pos_task_receive_msg(TASK_DEMO_ID, &msg_id, &msg_data, &msg_len)) {
		switch (msg_id) {
		case AC_DEMO_TASK_POST: {
			POS_LOG_INFO(TAG, "AC_DEMO_TASK_POST");
		} break;

		case AC_DEMO_TASK_POST_AND_DATA: {
			POS_LOG_INFO(TAG, "AC_DEMO_TASK_POST_AND_DATA: %s", (char *)msg_data);
		} break;

		default:
			break;
		}
		pos_free_msg(msg_data);
	}

	return (void *)0;
}