/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <vector>

#include "pos.h"
#include "pos_log.h"
#include "timer.h"
#include "sys_log.h"
#include "task_list.h"

#define TAG "TIMER"

/* internal define */
#define CLOCKID			   CLOCK_REALTIME
#define TIMER_INTERVAL_SIG SIGUSR1

static timer_t timer_id;
std::vector<pos_timer_msg_t> vector_timer;
pthread_mutex_t mt_timer_service;

/* timer stick counter */
static volatile uint32_t timer_stick_counter;

void timer_handler(sigval_t) {
	pthread_mutex_lock(&mt_timer_service);

	timer_stick_counter += POS_TIMER_UNIT;

	for (auto it = vector_timer.begin(); it != vector_timer.end();) {
		it->timer_counter -= POS_TIMER_UNIT;
		if (it->timer_counter <= 0) {
			POS_LOG_DEBUG(TAG, " task_id:%d msg_id:%d", it->task_id, it->msg_id);
			pos_task_post_msg(it->task_id, it->msg_id, it->msg_data, it->msg_len);

			if (it->timer_auto_reload != TIMER_ONE_SHOT) {
				it->timer_counter = it->timer_period;
				/* next timer */
				++it;
			}
			else {
				/* remove timer */
				if (it->msg_data != NULL)
					free(it->msg_data);
				it = vector_timer.erase(it);
			}
		}
		else {
			/* next timer */
			++it;
		}
	}

	pthread_mutex_unlock(&mt_timer_service);
}

void timer_init(void) {
	/* configure timer */
	struct sigevent sev;
	struct itimerspec its;

	/* create the timer */
	sev.sigev_notify			= SIGEV_THREAD;
	sev.sigev_signo				= TIMER_INTERVAL_SIG;
	sev.sigev_value.sival_ptr	= &timer_id;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_notify_function	= timer_handler;
	timer_create(CLOCKID, &sev, &timer_id);

	/* start the timer */
	its.it_value.tv_sec		= 0;
	its.it_value.tv_nsec	= POS_TIMER_UNIT * 1000000;
	its.it_interval.tv_sec	= its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	timer_settime(timer_id, 0, &its, NULL);
}

uint32_t timer_stick_get(void) {
	return timer_stick_counter;
}
