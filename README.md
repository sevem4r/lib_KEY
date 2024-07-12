# Usage example

```c
#include <avr/io.h>
#include <util/delay.h>

#include "CL_KEY/CL_key.h"


enum KEY_ID{
  KEY_ID_BRAKES,
  KEY_ID_LIGHT,
  KEY_ID_MAX
};

static CL_Key_t keys[KEY_ID_MAX];

static CL_KeyState_t on_key_get_state(void){
  return ((PIND & (1<<PD7)) ? CL_KEY_STATE_IDLE : CL_KEY_STATE_ACTIVE);
}

static CL_KeyState_t on_key_light_get_state(void){
  return ((PIND & (1<<PD6)) ? CL_KEY_STATE_IDLE : CL_KEY_STATE_ACTIVE);
}

static void on_key_action(uint8_t ID, CL_KeyAction_t action){
  if(action & CL_KEY_ACTION_LONG_PRESS){
    PORTD |= (1<<PD5);
  }

  if(action & CL_KEY_ACTION_LONG_RELEASE){
    PORTD &= ~(1<<PD5);
  }
}

static void on_key_light_action(uint8_t ID, CL_KeyAction_t action){
  if(action & CL_KEY_ACTION_PRESS){
    PORTD |= (1<<PD4);
  }
  
  if(action & CL_KEY_ACTION_RELEASE){
    PORTD &= ~(1<<PD4);
  }
}

int main(void){
  DDRD |= (1<<PD4) | (1<<PD5);
  PORTD |= (1<<PD6) | (1<<PD7);
  
  CL_Key_t* key;
  key = &keys[KEY_ID_BRAKES];
  key->init.ID = KEY_ID_BRAKES;
  key->init.ticks = 5;
  key->init.ticks_wait = 50;
  key->init.ticks_repeat = 10;
  key->init.callbakck_get_state = on_key_get_state;
  key->init.callback_action = on_key_action;
  CL_key_init(key);

  key = &keys[KEY_ID_LIGHT];
  key->init.ID = KEY_ID_LIGHT;
  key->init.ticks = 5;
  key->init.ticks_wait = 50;
  key->init.ticks_repeat = 10;
  key->init.callbakck_get_state = on_key_light_get_state;
  key->init.callback_action = on_key_light_action;
  CL_key_init(key);

  while(1){
    CL_key_poll(keys, KEY_ID_MAX);

    _delay_ms(10);
  }
}
```
