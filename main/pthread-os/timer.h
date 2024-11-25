/**
 ******************************************************************************
 * @author: VietDung
 * @date:   25/11/2024
 ******************************************************************************
 **/
#ifndef __TIMER_H__
#define __TIMER_H__

extern std::vector<pos_timer_msg_t> vector_timer;
extern pthread_mutex_t mt_timer_service;

extern void timer_init(void);
extern uint32_t timer_stick_get(void);

#endif	  //__TIMER_H__
