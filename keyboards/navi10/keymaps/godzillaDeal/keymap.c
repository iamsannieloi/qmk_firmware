/* Copyright 2019 Ethan Durrant (emdarcher)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

//create the tap type
typedef struct {
    bool is_press_action;
    int state;
} tap;

//tap dance states
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
};

//tap dance keys
enum {
    TAPPY_KEY = 0
};

//function to handle all the tap dances
int cur_dance(qk_tap_dance_state_t *state);

//functions for each tap dance
void tk_finished(qk_tap_dance_state_t *state, void *user_data);
void tk_reset(qk_tap_dance_state_t *state, void *user_data);

#define INDICATOR_LED   B5

#define _FN0    1
#define _ML1    2

enum custom_keycodes {
    SHRUG = SAFE_RANGE,
    GLASSES_OFF,
    GLASSES_ON,
    TABLE_FLIP,
    TABLE_PUT,
    UWU,
    JUDGE,
    WUT,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SHRUG:
        if (record->event.pressed) {
            send_unicode_string("¯\\_(ツ)_/¯");
        } else {
            //when keycode SHRUG is released
        }
        break;

    case GLASSES_OFF:
        if (record->event.pressed) {
            send_unicode_string("( •_•)>⌐■-■");
        } else {
            //when keycode is released
        }
        break;

    case GLASSES_ON:
        if (record->event.pressed) {
            send_unicode_string("(⌐■_■)");
        } else {
            //when keycode is released
        }
        break;

    case TABLE_FLIP:
        if (record->event.pressed) {
            send_unicode_string("(╯°□°）╯︵ ┻━┻");
        } else {
            //when keycode is released
        }
        break;

    case TABLE_PUT:
        if (record->event.pressed) {
            send_unicode_string("(ヘ･_･)ヘ┳━┳");
        } else {
            //when keycode is released
        }
        break;

    case UWU:
        if (record->event.pressed) {
            send_unicode_string("✪ ω ✪");
        } else {
            //when keycode is released
        }
        break;

    case JUDGE:
        if (record->event.pressed) {
            send_unicode_string("ಠ_ಠ");
        } else {
            //when keycode is released
        }
        break;

    case WUT:
        if (record->event.pressed) {
            send_unicode_string("(⊙_⊙)？");
        } else {
            //when keycode is released
        }
        break;
    }
    return true;
};

void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                 TD(TAPPY_KEY), KC_HOME,    KC_PGUP,
                 KC_DEL,    KC_END,     KC_PGDN,
                 
                            KC_UP,
                 KC_LEFT,   KC_DOWN,    KC_RIGHT),
    [_FN0] = LAYOUT(/* Base */
                 KC_TRNS,   KC_HOME,    KC_PGUP,
                 KC_DEL,    KC_END,     KC_PGDN,
                 
                            KC_UP,
                 KC_LEFT,   KC_DOWN,    KC_RIGHT),
    [_ML1] = LAYOUT(/* meme layer, toggled on a single tap */
                 KC_TRNS,   GLASSES_OFF,    TABLE_FLIP, 
                 SHRUG,     GLASSES_ON,     TABLE_PUT,
                 
                            UWU,
                 JUDGE,     WUT,    SHRUG),
};

//determine the current tap dance state
int cur_dance (qk_tap_dance_state_t *state){
    if(state->count == 1){
        //if a tap was registered
        if(!state->pressed){
            //if not still pressed, then was a single tap
            return SINGLE_TAP;
        } else {
            //if still pressed/held down, then it's a single hold
            return SINGLE_HOLD;
        }
    } else {
        return 8;
    }
}

//initialize the tap structure for the tap key
static tap tk_tap_state = {
    .is_press_action = true,
    .state = 0
};

//functions that control what our tap dance key does
void tk_finished(qk_tap_dance_state_t *state, void *user_data){
    tk_tap_state.state = cur_dance(state);
    switch(tk_tap_state.state){
        case SINGLE_TAP:
            //send desired key when tapped:
            //setting to the media layer
            if(layer_state_is(_ML1)){
                //if already active, toggle it to off
                layer_off(_ML1);
                //turn off the indicator LED
                //set LED HI to turn it off
                writePinHigh(INDICATOR_LED);
            } else {
                //turn on the media layer
                layer_on(_ML1);
                //turn on the indicator LED
                //set LED pin to LOW to turn it on
                writePinLow(INDICATOR_LED);
            }
            break;
        case SINGLE_HOLD:
            //set to desired layer when held:
            //setting to the function layer
            layer_on(_FN0);
            break;
    }
}

void tk_reset(qk_tap_dance_state_t *state, void *user_data){
    //if held and released, leave the layer
    if(tk_tap_state.state == SINGLE_HOLD){
        layer_off(_FN0);
    }
    //reset the state
    tk_tap_state.state = 0; 
}

//associate the tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TAPPY_KEY] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, tk_finished, tk_reset, 275)
};
