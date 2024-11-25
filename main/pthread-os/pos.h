/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#ifndef __POS_H__
#define __POS_H__

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <vector>

#define POS_USER_DEFINE_SIG (10)
#define POS_TIMER_UNIT		(100) /* 100 ms */

typedef struct POS_MSG {
	uint16_t id;
	uint32_t len;
	void *data;
} pos_msg_t;

typedef void *(*pf_task)(void *);

typedef struct POS_TASK {
	uint32_t id;
	pf_task task;
	const char *name;
	std::vector<pos_msg_t> *msg_handle;
	pthread_t pthread;
	pthread_cond_t mailbox_cond;
	pthread_mutex_t mt_mailbox_cond;
} pos_task_t;

typedef enum {
	TIMER_ONE_SHOT,
	TIMER_PERIODIC
} timer_type_t;

typedef struct POS_TIMER_MSG {
	uint16_t task_id;
	uint16_t msg_id;
	uint32_t msg_len;
	void *msg_data;
	int32_t timer_period;
	int32_t timer_counter;
	timer_type_t timer_auto_reload;
} pos_timer_msg_t;

extern uint32_t table_task_size;

/* function using to init task */
extern void pos_init(void);
extern void pos_start(void);
extern void pos_create_task(uint16_t index);
extern void pos_create_vector(uint16_t index);

/* function using to make sure that all task is initialed */
extern void pos_wait_all_task_started(void);

/* function exchange messages */
extern bool pos_task_post_msg(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len);
extern bool pos_task_receive_msg(uint16_t task_id, uint16_t *msg_id, void **msg_data, uint32_t *msg_len);
extern void pos_free_msg(void *msg_data);

/* function timer */
extern void pos_timer_set(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len, uint32_t timer_period,
						  bool timer_auto_reload);

#endif