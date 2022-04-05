#include <stdio.h>
#include "machine.h"
#include "machine-impl.h"

static int32_t gotoStateA(State_t* state) {
  *state = StateA;
  return StateA;
}

static int32_t gotoStateB(State_t* state) {
  *state = StateB;
  return StateB;
}

static int32_t gotoStateC(State_t* state) {
  *state = StateC;
  return StateC;
}

Transition_t transitions[] = {
  { StateA, EventA, &gotoStateB },
  { StateB, EventB, &gotoStateC },
  { StateC, EventC, &gotoStateA },
};
#define NUM_TRANSITIONS 3

int counter = 0;

TransitionEvent_t getNextEvent(const State_t currentState) {
  if (counter < 2) {
    ++counter;
    return NoEvent;
  }

  if (counter == 2) {
    ++counter;
    return EventA;
  }

  if (counter < 4) {
    ++counter;
    return NoEvent;
  }

  if (counter == 4) {
    ++counter;
    return EventB;
  }

  if (counter == 5) {
    ++counter;
    return EventC;
  }

  counter = 0;
  return NoEvent;
}

State_t currentState = StateA;

int main() {
  TransitionEvent_t event;
  for (int i = 0; i < 10; i++) {
    event = getNextEvent(currentState);
    printf("State (%d) Event (%d) -> ", currentState, event);

    if (StateMachine_run(&currentState, event, transitions, NUM_TRANSITIONS)) {
      printf("State (%d)\n", currentState);
    } else {
      printf("No transition\n");
    }
  }

  return 0;
}
