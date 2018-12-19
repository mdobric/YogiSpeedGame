#include <stdlib.h>
// EVENTS
#define TIMER_EXPIRED 0
#define SWITCH_TRIGGERED 1

// STATES
#define READY_STATE 0
#define NOT_READY_STATE 1
#define SUCCESS_STATE 2
#define FAIL_STATE 3

struct State {
  unsigned int stateDuration;
  unsigned int* melody;
  unsigned int noteCount;
};

struct Machine {
  int currentState;
  unsigned int timeInCurrentState;
  struct State** states;
  unsigned int stateCount;
};

int getCurrentNote(struct State* state, int currentTime) {
  int currentMelodyTimePoint = currentTime % state->melody[2 * state->noteCount - 1];
  int currentNote = 0;
  int endTime = state->melody[2 * currentNote + 1];
  while (currentNote < state->noteCount && currentMelodyTimePoint >= endTime) {
    ++currentNote;
    endTime = state->melody[2 * currentNote + 1];
  }
  if (currentNote >= state->noteCount) {
    return 0;
  }
  return state->melody[2 * currentNote];
}

struct Machine* stateMachine;

void setup() {
  stateMachine = (struct Machine*) malloc(sizeof(struct Machine));
  struct State* readyState = (struct State*) malloc(sizeof(struct State));
  readyState->stateDuration = 12;
  readyState->noteCount = 4;
  readyState->melody = (unsigned int*) malloc(sizeof(unsigned int) * readyState->noteCount);
  readyState->melody[0] = 400;
  readyState->melody[1] = 4;
  readyState->melody[2] = 800;
  readyState->melody[3] = 8;
  stateMachine->states = (struct State**) malloc(sizeof(struct State*)*1);
  stateMachine->states[0] = readyState;
}

int main() {
  setup();
  return 0;
}
