/*
 * key.c
 *
 *  Created on: 11 lip 2023
 *      Author: mariusz
 */

#include "key.h"

#include <stddef.h>
#include <stdbool.h>

void CL_key_init(
	CL_Key_t* key
){
	if(key == NULL){
		return;
	}

	key->state = CL_KEY_STATE_NONE;
	key->state_prev = key->state;

	key->filter_counter = 0;
	key->state_counter = 0;
	key->repeat_counter = 0;
}

void CL_key_poll(
	CL_Key_t* keys,
	uint8_t num
){
	for(uint8_t i = 0; i < num; i++){
		CL_Key_t* key = &keys[i];

		if(key == NULL){
			continue;
		}

		if(key->init.callback_action && key->init.callbakck_get_state){
			CL_KeyState_t delta;
			uint8_t action = CL_KEY_ACTION_NONE;

			if(key->init.callbakck_get_state() == CL_KEY_STATE_ACTIVE){
				if(key->filter_counter < key->init.ticks){
					key->filter_counter++;

					if(key->filter_counter == key->init.ticks){
						key->state = CL_KEY_STATE_ACTIVE;
					}
				}
			}
			else{
				if(key->filter_counter > -key->init.ticks){
					key->filter_counter--;

					if(key->filter_counter == -key->init.ticks){
						key->state = CL_KEY_STATE_IDLE;
					}
				}
			}

			delta = key->state - key->state_prev;

			if(delta > CL_KEY_STATE_NONE){
				action |= CL_KEY_ACTION_PRESS;

				key->state_counter = 0;
				key->repeat_counter = 0;
			}

			if(key->state == CL_KEY_STATE_ACTIVE){
				action |= CL_KEY_ACTION_PRESSED;

				if(key->state_counter < key->init.ticks_wait){
					key->state_counter++;
				}
				else{
					if(key->state_counter == key->init.ticks_wait){
						action |= CL_KEY_ACTION_LONG_PRESS;

						key->state_counter++;
					}

					key->repeat_counter++;

					if(key->repeat_counter > key->init.ticks_repeat){
						action |= CL_KEY_ACTION_LONG_REPEAT;

						key->repeat_counter = 0;
					}
				}
			}

			if(delta < CL_KEY_STATE_NONE){
				if(key->state_counter > key->init.ticks_wait){
					action |= CL_KEY_ACTION_LONG_RELEASE;
				}

				action |= CL_KEY_ACTION_RELEASE;

				key->state_counter = 0;
				key->repeat_counter = 0;
			}

			if(key->state == CL_KEY_STATE_IDLE){
				action |= CL_KEY_ACTION_RELEASED;
			}

			key->state_prev = key->state;

			key->init.callback_action(key->init.ID, action);
		}
	}
}
