#include "machine.h"

bool StateMachine_run(State_t* currentState, TransitionEvent_t event, Transition_t* transitions, size_t numTransitions) {
  for (int i = 0; i < numTransitions; i++) {
    if (transitions[i].state == *currentState && transitions[i].event == event) {
      return transitions[i].transitionLogic(currentState) >= 0;
    }
  }

  return false;
}
