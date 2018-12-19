// EVENTS
#define TIMER_EXPIRED 0
#define SWITCH_TRIGGERED 1

// STATES
#define READY_STATE 0
#define NOT_READY_STATE 1
#define SUCCESS_STATE 2
#define FAIL_STATE 3

struct State {
  int stateDuration;
  int* melody;
  int noteCount;
};

struct Machine {
  int currentState;
  int timeInCurrentState;
  struct State* states;
  int stateCount;
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

int main() {
  struct State* state;
  state->stateDuration = 12;
  int melody[] = { 400, 4, 800, 8 };
  state->melody = melody;
  state->noteCount = 4;
  
  return 0;
}
