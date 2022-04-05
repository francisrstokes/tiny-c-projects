#ifndef INC_STATE_MACHINE_H
#define INC_STATE_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef int32_t TransitionEvent_t;
typedef int32_t State_t;

typedef TransitionEvent_t (*NextEventFn)(const State_t);
typedef int32_t (*TransitionFn)(State_t*);

typedef struct {
  State_t state;
  TransitionEvent_t event;
  TransitionFn transitionLogic;
} Transition_t;

bool StateMachine_run(State_t* currentState, TransitionEvent_t event, Transition_t* transitions, size_t numTransitions);

#endif
