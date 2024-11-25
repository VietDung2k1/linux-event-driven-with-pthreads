/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include "app.h"
#include "task_list.h"

#define TAG "APP"

int main() {
	/*********************
	 * software configure *
	 **********************/
	pos_init();

	/* shell: debug and config */
	pos_create_task(TASK_SHELL_ID);

	/* demo */
	pos_create_vector(TASK_DEMO_ID);
	pos_create_task(TASK_DEMO_ID);

	pos_start();
	return 0;
}