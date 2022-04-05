# State Machine

The idea with this little project is to build a minimal example of a reusable state machine interface, flexible enough to cater to event driven systems (in the interrupt sense) or simply logic running in a loop.

## The "Library" Side

`machine.h` defines an interface a simple interface, where states and events are aliases for `int32_t`, as well as a typedef for a state transition struct (`Transition_t`), and a single function `StateMachine_run` that will be in charge of orchestrating transitions.

A `Transition_t` holds a relevant state, an event that it is sensitive to, and a function pointer that can be called when those conditions are met which can optionally change the machines state.

`machine.c` holds the implementation for `StateMachine_run`, whose signature is:

```C
bool StateMachine_run(State_t* currentState, TransitionEvent_t event, Transition_t* transitions, size_t numTransitions);
```

It's not much more complex than looping over the transition table, checking if the conditions match for the current state and the event, then running transition logic function. A little note here is that the transition logic function returns an `int32_t` (in most cases probably just the state it transitioned to if a transition occurred), but a negative value can signify that no transition happened. `StateMachine_run` itself returns `bool`, which is basically an indication of "was there a transition at all?" This is purely convention and can safely be ignored if you happened to implement states with different semantics, but seems useful enough to leave in.

## The "User" Side

A user can include those two files in their project, and then the only thing they would need to do is to define some kind of schema for the states and events, and build a transition table. There is a `machine-impl.h` file, which I've set up some enums for states and events, but it's really just illustrative. `main.c` shows the rest - some functions that handle state transition logic, a transition table, a function to generate events, and some code to exercise the state machine through some transitions.
