/*
 * key.h
 *
 *  Created on: 2 maj 2017
 *      Author: Mariusz
 */

#ifndef CL_KEY_H_
#define CL_KEY_H_

#include <stdint.h>

typedef enum CL_KEY_ACTION{
	CL_KEY_ACTION_NONE			= 0,
	CL_KEY_ACTION_PRESSED		= (1<<0),
	CL_KEY_ACTION_RELEASED		= (1<<1),
	CL_KEY_ACTION_PRESS 		= (1<<2),
	CL_KEY_ACTION_RELEASE		= (1<<3),
	CL_KEY_ACTION_LONG_PRESS	= (1<<4),
	CL_KEY_ACTION_LONG_REPEAT	= (1<<5),
	CL_KEY_ACTION_LONG_RELEASE	= (1<<6)
}CL_KeyAction_t;

typedef enum CL_KEY_STATE{
	CL_KEY_STATE_IDLE			= -1,
	CL_KEY_STATE_NONE			= 0,
	CL_KEY_STATE_ACTIVE			= 1
}CL_KeyState_t;

// return ((PIND & (1<<PD7)) ? CL_KEY_STATE_IDLE : CL_KEY_STATE_ACTIVE);
typedef CL_KeyState_t (*CL_key_get_state_callback)(void);

// action & CL_KEY_ACTION_PRESSED
// action & CL_KEY_ACTION_RELEASED
// action & CL_KEY_ACTION_PRESS
// action & CL_KEY_ACTION_RELEASE
// action & CL_KEY_ACTION_LONG_PRESS
// action & CL_KEY_ACTION_LONG_REPEAT
// action & CL_KEY_ACTION_LONG_RELEASE
typedef void (*CL_key_action_callback)(uint8_t ID, CL_KeyAction_t action);

typedef struct{
	struct{
		uint8_t ID;

		uint8_t ticks;
		uint8_t ticks_wait;
		uint8_t ticks_repeat;

		CL_key_get_state_callback callbakck_get_state;
		CL_key_action_callback callback_action;
	}init;

	CL_KeyState_t state;
	CL_KeyState_t state_prev;

	int8_t filter_counter;
	uint8_t state_counter;
	uint8_t repeat_counter;
}CL_Key_t;

void CL_key_init(CL_Key_t* key);
void CL_key_poll(CL_Key_t* keys, uint8_t num);

#endif /* CL_KEY_H_ */
