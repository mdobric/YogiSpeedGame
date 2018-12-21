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
  unsigned int duration;
  unsigned int* melody;
  unsigned int noteCount;
};

struct Machine {
  unsigned int currentState;
  int timeInCurrentState;
  int timeInPreviousState;
  struct State** states;
  unsigned int stateCount;
};

volatile unsigned int currentNote = 0;
volatile char switchTriggered = 0;

void switchInterrupt() {
  switchTriggered = 1;
}

void playNote(unsigned int note) {
  if (currentNote == note) return;
  currentNote = note;
  if (currentNote == 0) {
    // noTone(TONE_PIN);
  } else {
    // tone(TONE_PIN, currentNote);
  }
}

unsigned int getCurrentNote(struct State* state, int currentTime) {
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

void handleEvent(struct Machine* machine, unsigned int event) {
  switch(machine->currentState) {
  case READY_STATE:
    if (event == TIMER_EXPIRED) {
      machine->currentState = NOT_READY_STATE;
      machine->timeInCurrentState = 0;
    } else if (event == SWITCH_TRIGGERED) {
      machine->currentState = SUCCESS_STATE;
      machine->timeInCurrentState = 0;
    }
    break;
  case NOT_READY_STATE:
    if (event == TIMER_EXPIRED) {
      machine->currentState = READY_STATE;
      machine->timeInCurrentState = 0;
    } else if (event == SWITCH_TRIGGERED) {
      machine->currentState = FAIL_STATE;
      machine->timeInPreviousState = machine->timeInCurrentState;
      machine->timeInCurrentState = 0;
    }
    break;
  case SUCCESS_STATE:
    if (event == TIMER_EXPIRED) {
      machine->currentState = READY_STATE;
      machine->timeInCurrentState = 0;
    } else if (event == SWITCH_TRIGGERED) {
      // do nothing
    }
    break;
  case FAIL_STATE:
    if (event == TIMER_EXPIRED) {
      machine->currentState = NOT_READY_STATE;
      machine->timeInCurrentState = machine->timeInPreviousState;
      machine->timeInPreviousState = 0;
    } else if (event == SWITCH_TRIGGERED) {
      // do nothing
    }
  }
}

void tick(struct Machine* machine) {
  machine->timeInCurrentState++;
  if (switchTriggered == 1) {
    handleEvent(machine, SWITCH_TRIGGERED);
    switchTriggered = 0;
  } else if (machine->timeInCurrentState >= machine->states[machine->currentState]->duration) {
    handleEvent(machine, TIMER_EXPIRED);
  } else {
    // nothing left to do; we already increased the time and don't need to transition state
  }
  // play tone for timepoint in current state
  unsigned int currentNote = getCurrentNote(machine->states[machine->currentState], machine->timeInCurrentState);
  playNote(currentNote);
}

struct Machine* stateMachine;

void setup() {
  stateMachine = (struct Machine*) malloc(sizeof(struct Machine));
  struct State* readyState = (struct State*) malloc(sizeof(struct State));
  readyState->duration = 12;
  readyState->noteCount = 4;
  readyState->melody = (unsigned int*) malloc(sizeof(unsigned int) * readyState->noteCount);
  readyState->melody[0] = 400;
  readyState->melody[1] = 4;
  readyState->melody[2] = 800;
  readyState->melody[3] = 8;
  stateMachine->stateCount = 4;
  stateMachine->states = (struct State**) malloc(sizeof(struct State*) * stateMachine->stateCount);
  stateMachine->states[READY_STATE] = readyState;

  struct State* notReadyState = (struct State*) malloc(sizeof(struct State));
  notReadyState->duration = 12;
  notReadyState->noteCount = 0;
  notReadyState->melody = NULL;
  stateMachine->states[NOT_READY_STATE] = notReadyState;

  struct State* successState = (struct State*) malloc(sizeof(struct State));
  successState->duration = 12;
  successState->noteCount = 0;
  successState->melody = NULL;
  stateMachine->states[SUCCESS_STATE] = successState;

  struct State* failState = (struct State*) malloc(sizeof(struct State));
  failState->duration = 12;
  failState->noteCount = 0;
  failState->melody = NULL;
  stateMachine->states[FAIL_STATE] = failState;

  stateMachine->currentState = READY_STATE;
  stateMachine->timeInCurrentState = -1;
}

int main() {
  setup();
  return 0;
}
