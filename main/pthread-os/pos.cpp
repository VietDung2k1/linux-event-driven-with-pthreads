/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include "pos.h"
#include "pos_log.h"
#include "timer.h"
#include "task_list.h"
#include "sys_log.h"

#define TAG "POS"

uint32_t table_task_size = POS_TASK_END_ID;
static pthread_mutex_t mt_pos_thread_started;
static uint32_t pos_thread_started = 0;

void pos_init(void) {
	printf("\r\n"
		   "     _/_/_/      _/_/      _/_/_/   \r\n"
		   "    _/    _/  _/    _/  _/          \r\n"
		   "   _/_/_/    _/    _/    _/_/       \r\n"
		   "  _/        _/    _/        _/      \r\n"
		   " _/          _/_/    _/_/_/         \r\n"
		   "Author: VietDung\r\n"
		   "Build: %s\r\n",
		   __DATE__);

	POS_LOG_INFO(TAG, "table task size: %u", table_task_size);
	/* configure timer */
	timer_init();
}

void pos_start(void) {
	POS_LOG_INFO(TAG, "POS STARTED");
	for (uint32_t index = 0; index < table_task_size; index++) {
		pthread_join(table_task[index].pthread, NULL);
	}
}

void pos_create_task(uint16_t task_id) {
	SYS_ASSERT(task_id < table_task_size);

	/* create task */
	pthread_create(&(table_task[task_id].pthread), NULL, table_task[task_id].task, NULL);

	POS_LOG_INFO(TAG, "create task name: %s, id: %u, task: %p", table_task[task_id].name, table_task[task_id].id,
				 table_task[task_id].task);

	/* create mailbox trigger */
	pthread_cond_init(&table_task[task_id].mailbox_cond, NULL);
}

void pos_create_vector(uint16_t task_id) {
	SYS_ASSERT(task_id < table_task_size);

	/* create mailbox */
	std::vector<pos_msg_t> *msg_vector_buf = new std::vector<pos_msg_t>();

	table_task[task_id].msg_handle = msg_vector_buf;

	POS_LOG_INFO(TAG, "create msg handle : %s, id: %u, task: %p", table_task[task_id].name, table_task[task_id].id,
				 table_task[task_id].msg_handle);
}

bool pos_task_post_msg(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len) {
	POS_LOG_DEBUG(TAG, "task post id: %u, sig: %u, msg: %p, len: %u", task_id, msg_id, msg_data, msg_len);
	SYS_ASSERT(task_id < table_task_size);

	pthread_mutex_lock(&(table_task[task_id].mt_mailbox_cond));

	/* create message */
	pos_msg_t pos_msg = {msg_id, msg_len, NULL};
	if (msg_len) {
		pos_msg.data = malloc(msg_len);
		SYS_ASSERT(pos_msg.data != NULL);

		memcpy(pos_msg.data, msg_data, msg_len);
		POS_LOG_DEBUG(TAG, "task post copy msg %u -> %p, len: %u", pos_msg.id, pos_msg.data, pos_msg.len);
	}

	/* send message */
	table_task[task_id].msg_handle->push_back(pos_msg);

	pthread_cond_signal(&(table_task[task_id].mailbox_cond));
	pthread_mutex_unlock(&(table_task[task_id].mt_mailbox_cond));
	return true;
};

bool pos_task_receive_msg(uint16_t task_id, uint16_t *msg_id, void **msg_data, uint32_t *msg_len) {
	SYS_ASSERT(task_id < table_task_size);
	pthread_mutex_lock(&(table_task[task_id].mt_mailbox_cond));
	/* get message form mailbox */
	pos_msg_t pos_msg = {0, 0, NULL};

	if (table_task[task_id].msg_handle->empty() == true) {
		pthread_cond_wait(&(table_task[task_id].mailbox_cond), &(table_task[task_id].mt_mailbox_cond));
	}

	if (table_task[task_id].msg_handle->empty() == false) {
		pos_msg = table_task[task_id].msg_handle->front();
		table_task[task_id].msg_handle->erase(table_task[task_id].msg_handle->begin());
	}

	/* copy message details */
	*msg_id	  = pos_msg.id;
	*msg_data = pos_msg.data;
	*msg_len  = pos_msg.len;

	POS_LOG_DEBUG(TAG, "task receive copy msg %u -> %p, len: %u", *msg_id, *msg_data, *msg_len);

	pthread_mutex_unlock(&(table_task[task_id].mt_mailbox_cond));
	return true;
}

void pos_timer_set(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len, uint32_t timer_period,
				   bool timer_auto_reload) {
	POS_LOG_DEBUG(TAG, "timer set id: %u, sig: %u, msg: %p, len: %u", task_id, msg_id, msg_data, msg_len);
	SYS_ASSERT(task_id < table_task_size);

	pthread_mutex_lock(&mt_timer_service);
	/* create timer message */
	pos_timer_msg_t *pos_timer_msg = (pos_timer_msg_t *)malloc(sizeof(pos_timer_msg_t));
	SYS_ASSERT(pos_timer_msg != NULL);

	pos_timer_msg->task_id		 = task_id;
	pos_timer_msg->msg_id		 = msg_id;
	pos_timer_msg->msg_len		 = msg_len;
	pos_timer_msg->msg_data		 = NULL;
	pos_timer_msg->timer_period	 = timer_period;
	pos_timer_msg->timer_counter = timer_period;

	if (timer_auto_reload == true) {
		pos_timer_msg->timer_auto_reload = TIMER_PERIODIC;
	}
	else {
		pos_timer_msg->timer_auto_reload = TIMER_ONE_SHOT;
	}

	if (msg_len > 0) {
		pos_timer_msg->msg_data = malloc(msg_len);

		SYS_ASSERT(pos_timer_msg->msg_data != NULL);

		memcpy(pos_timer_msg->msg_data, msg_data, msg_len);
		POS_LOG_DEBUG(TAG, "timer set copy msg %u -> %p, len: %u", pos_timer_msg->msg_id, pos_timer_msg->msg_data,
					  pos_timer_msg->msg_len);
	}

	/* create timer */
	vector_timer.push_back(*pos_timer_msg);

	free(pos_timer_msg);
	pthread_mutex_unlock(&mt_timer_service);
}

void pos_wait_all_task_started(void) {
	POS_LOG_DEBUG(TAG, "wait ...");
	bool check_ret = true;

	pthread_mutex_lock(&mt_pos_thread_started);
	pos_thread_started++;
	pthread_mutex_unlock(&mt_pos_thread_started);

	while (check_ret) {
		pthread_mutex_lock(&mt_pos_thread_started);

		if (pos_thread_started < table_task_size) {
			check_ret = true;
		}
		else {
			check_ret = false;
		}

		pthread_mutex_unlock(&mt_pos_thread_started);

		usleep(100);
	}
}

void pos_free_msg(void *msg_data) {
	if (msg_data != NULL) {
		POS_LOG_DEBUG(TAG, "delete msg %p", msg_data);
		free(msg_data);
	}
}