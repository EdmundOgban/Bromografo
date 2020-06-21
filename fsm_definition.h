#ifndef _FSM_DEFINITION_H
#define _FSM_DEFINITION_H

#define ROM_QUALIFIER

/*** Function prototypes declaration ***/
/***** RUN functions *****/
extern void s_vacuum_lost__run(void);
extern void s_navigation__run(void);
extern void s_holdpcb_door__run(void);
extern void s_holdpcb__run(void);
extern void s_countdown_done__run(void);
extern void s_lamp_preheat__run(void);
extern void s_door_opened__run(void);
extern void s_countdown__run(void);
extern void s_testmenu__run(void);
extern void s_set_counter__run(void);

/***** ARC functions *****/
/***** State S_VACUUM_LOST *****/
extern void btn_enter_hold__arc_common(fsm_signal_t trigger);

/***** State S_NAVIGATION *****/
extern void s_navigation__arc_to__s_set_counter(fsm_signal_t trigger);

/***** State S_HOLDPCB_DOOR *****/
extern void s_holdpcb_door__arc_to__s_door_opened(fsm_signal_t trigger);
extern void btn_prev_hold__arc_common(fsm_signal_t trigger);

/***** State S_HOLDPCB *****/
extern void s_holdpcb__arc_to__s_countdown(fsm_signal_t trigger);

/***** State S_COUNTDOWN_DONE *****/
extern void s_countdown_done__arc_to__s_navigation(fsm_signal_t trigger);

/***** State S_LAMP_PREHEAT *****/
extern void s_lamp_preheat__arc_to__s_set_counter(fsm_signal_t trigger);
extern void s_lamp_preheat__arc_to__s_holdpcb(fsm_signal_t trigger);

/***** State S_DOOR_OPENED *****/
extern void s_door_opened__arc_to__s_countdown(fsm_signal_t trigger);
extern void vacuum_nok__arc_common(fsm_signal_t trigger);

/***** State S_COUNTDOWN *****/
extern void s_countdown__arc_to__s_door_opened(fsm_signal_t trigger);
extern void s_countdown__arc_to__s_countdown_done(fsm_signal_t trigger);

/***** State S_TESTMENU *****/

/***** State S_SET_COUNTER *****/
extern void s_set_counter__arc_to__s_lamp_preheat(fsm_signal_t trigger);

/*** States extern declaration ***/
extern ROM_QUALIFIER const struct fsm_state_s S_VACUUM_LOST;
extern ROM_QUALIFIER const struct fsm_state_s S_NAVIGATION;
extern ROM_QUALIFIER const struct fsm_state_s S_HOLDPCB_DOOR;
extern ROM_QUALIFIER const struct fsm_state_s S_HOLDPCB;
extern ROM_QUALIFIER const struct fsm_state_s S_COUNTDOWN_DONE;
extern ROM_QUALIFIER const struct fsm_state_s S_LAMP_PREHEAT;
extern ROM_QUALIFIER const struct fsm_state_s S_DOOR_OPENED;
extern ROM_QUALIFIER const struct fsm_state_s S_COUNTDOWN;
extern ROM_QUALIFIER const struct fsm_state_s S_TESTMENU;
extern ROM_QUALIFIER const struct fsm_state_s S_SET_COUNTER;

/*** State S_VACUUM_LOST - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_VACUUM_LOST__BTN_ENTER_HOLD = { BTN_ENTER_HOLD, &S_NAVIGATION, btn_enter_hold__arc_common };
/*** State S_VACUUM_LOST - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_VACUUM_LOST_transitions[1] = { &S_VACUUM_LOST__BTN_ENTER_HOLD };
/*** State S_VACUUM_LOST - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_VACUUM_LOST = { 0, s_vacuum_lost__run, 1, S_VACUUM_LOST_transitions };

/*** State S_NAVIGATION - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_NAVIGATION__BTN_ENTER_HOLD = { BTN_ENTER_HOLD, &S_SET_COUNTER, s_navigation__arc_to__s_set_counter };
ROM_QUALIFIER const struct fsm_transition_s S_NAVIGATION__ENTER_TEST_MENU = { ENTER_TEST_MENU, &S_TESTMENU, NULL };
/*** State S_NAVIGATION - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_NAVIGATION_transitions[2] = { &S_NAVIGATION__BTN_ENTER_HOLD, &S_NAVIGATION__ENTER_TEST_MENU };
/*** State S_NAVIGATION - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_NAVIGATION = { 1, s_navigation__run, 2, S_NAVIGATION_transitions };

/*** State S_HOLDPCB_DOOR - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB_DOOR__VACUUM_OK = { VACUUM_OK, &S_DOOR_OPENED, s_holdpcb_door__arc_to__s_door_opened };
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB_DOOR__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB_DOOR__DOOR_CLOSE = { DOOR_CLOSE, &S_HOLDPCB, NULL };
/*** State S_HOLDPCB_DOOR - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_HOLDPCB_DOOR_transitions[3] = { &S_HOLDPCB_DOOR__VACUUM_OK, &S_HOLDPCB_DOOR__BTN_PREV_HOLD, &S_HOLDPCB_DOOR__DOOR_CLOSE };
/*** State S_HOLDPCB_DOOR - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_HOLDPCB_DOOR = { 2, s_holdpcb_door__run, 3, S_HOLDPCB_DOOR_transitions };

/*** State S_HOLDPCB - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB__VACUUM_OK = { VACUUM_OK, &S_COUNTDOWN, s_holdpcb__arc_to__s_countdown };
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
ROM_QUALIFIER const struct fsm_transition_s S_HOLDPCB__DOOR_OPEN = { DOOR_OPEN, &S_HOLDPCB_DOOR, NULL };
/*** State S_HOLDPCB - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_HOLDPCB_transitions[3] = { &S_HOLDPCB__VACUUM_OK, &S_HOLDPCB__BTN_PREV_HOLD, &S_HOLDPCB__DOOR_OPEN };
/*** State S_HOLDPCB - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_HOLDPCB = { 3, s_holdpcb__run, 3, S_HOLDPCB_transitions };

/*** State S_COUNTDOWN_DONE - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN_DONE__DOOR_OPEN = { DOOR_OPEN, &S_NAVIGATION, s_countdown_done__arc_to__s_navigation };
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN_DONE__BTN_ENTER_HOLD = { BTN_ENTER_HOLD, &S_NAVIGATION, btn_enter_hold__arc_common };
/*** State S_COUNTDOWN_DONE - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_COUNTDOWN_DONE_transitions[2] = { &S_COUNTDOWN_DONE__DOOR_OPEN, &S_COUNTDOWN_DONE__BTN_ENTER_HOLD };
/*** State S_COUNTDOWN_DONE - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_COUNTDOWN_DONE = { 4, s_countdown_done__run, 2, S_COUNTDOWN_DONE_transitions };

/*** State S_LAMP_PREHEAT - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_LAMP_PREHEAT__DOOR_OPEN = { DOOR_OPEN, &S_SET_COUNTER, s_lamp_preheat__arc_to__s_set_counter };
ROM_QUALIFIER const struct fsm_transition_s S_LAMP_PREHEAT__RTC_CNT_END = { RTC_CNT_END, &S_HOLDPCB, s_lamp_preheat__arc_to__s_holdpcb };
ROM_QUALIFIER const struct fsm_transition_s S_LAMP_PREHEAT__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
/*** State S_LAMP_PREHEAT - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_LAMP_PREHEAT_transitions[3] = { &S_LAMP_PREHEAT__DOOR_OPEN, &S_LAMP_PREHEAT__RTC_CNT_END, &S_LAMP_PREHEAT__BTN_PREV_HOLD };
/*** State S_LAMP_PREHEAT - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_LAMP_PREHEAT = { 5, s_lamp_preheat__run, 3, S_LAMP_PREHEAT_transitions };

/*** State S_DOOR_OPENED - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_DOOR_OPENED__DOOR_CLOSE = { DOOR_CLOSE, &S_COUNTDOWN, s_door_opened__arc_to__s_countdown };
ROM_QUALIFIER const struct fsm_transition_s S_DOOR_OPENED__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
ROM_QUALIFIER const struct fsm_transition_s S_DOOR_OPENED__VACUUM_NOK = { VACUUM_NOK, &S_VACUUM_LOST, vacuum_nok__arc_common };
/*** State S_DOOR_OPENED - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_DOOR_OPENED_transitions[3] = { &S_DOOR_OPENED__DOOR_CLOSE, &S_DOOR_OPENED__BTN_PREV_HOLD, &S_DOOR_OPENED__VACUUM_NOK };
/*** State S_DOOR_OPENED - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_DOOR_OPENED = { 6, s_door_opened__run, 3, S_DOOR_OPENED_transitions };

/*** State S_COUNTDOWN - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN__DOOR_OPEN = { DOOR_OPEN, &S_DOOR_OPENED, s_countdown__arc_to__s_door_opened };
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN__RTC_CNT_END = { RTC_CNT_END, &S_COUNTDOWN_DONE, s_countdown__arc_to__s_countdown_done };
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
ROM_QUALIFIER const struct fsm_transition_s S_COUNTDOWN__VACUUM_NOK = { VACUUM_NOK, &S_VACUUM_LOST, vacuum_nok__arc_common };
/*** State S_COUNTDOWN - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_COUNTDOWN_transitions[4] = { &S_COUNTDOWN__DOOR_OPEN, &S_COUNTDOWN__RTC_CNT_END, &S_COUNTDOWN__BTN_PREV_HOLD, &S_COUNTDOWN__VACUUM_NOK };
/*** State S_COUNTDOWN - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_COUNTDOWN = { 7, s_countdown__run, 4, S_COUNTDOWN_transitions };

/*** State S_TESTMENU - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_TESTMENU__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, NULL };
/*** State S_TESTMENU - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_TESTMENU_transitions[1] = { &S_TESTMENU__BTN_PREV_HOLD };
/*** State S_TESTMENU - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_TESTMENU = { 8, s_testmenu__run, 1, S_TESTMENU_transitions };

/*** State S_SET_COUNTER - Single transition definitions ***/
ROM_QUALIFIER const struct fsm_transition_s S_SET_COUNTER__BTN_ENTER_HOLD = { BTN_ENTER_HOLD, &S_LAMP_PREHEAT, s_set_counter__arc_to__s_lamp_preheat };
ROM_QUALIFIER const struct fsm_transition_s S_SET_COUNTER__BTN_PREV_HOLD = { BTN_PREV_HOLD, &S_NAVIGATION, btn_prev_hold__arc_common };
/*** State S_SET_COUNTER - Transitions array ***/
ROM_QUALIFIER const struct fsm_transition_s * S_SET_COUNTER_transitions[2] = { &S_SET_COUNTER__BTN_ENTER_HOLD, &S_SET_COUNTER__BTN_PREV_HOLD };
/*** State S_SET_COUNTER - definition ***/
ROM_QUALIFIER const struct fsm_state_s S_SET_COUNTER = { 9, s_set_counter__run, 2, S_SET_COUNTER_transitions };

#endif
