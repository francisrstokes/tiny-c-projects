#ifndef INC_MACHINE_IMPL_H
#define INC_MACHINE_IMPL_H

typedef enum {
  NoEvent = -1,
  EventA,
  EventB,
  EventC
} TransitionEvent;

typedef enum {
  StateA,
  StateB,
  StateC,
} MachineState;

#endif
