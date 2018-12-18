// EVENTS
#define TIMER_EXPIRED 0
#define SWITCH_TRIGGERED 1

// STATES
#define READY_STATE 0
#define NOT_READY_STATE 1
#define SUCCESS_STATE 2
#define FAIL_STATE 3

#include <iostream>
using namespace std;

class State {
    int type;
    int stateDuration;
    int* melody;
    int noteCount;

  public:
    State(int type, int stateDuration, int* melody, int melodyLength) {
      this->stateDuration = stateDuration;
      this->type = type;
      this->melody = (int*) malloc(melodyLength * sizeof(int));
      for (int i = 0; i < melodyLength; ++i) {
        this->melody[i] = melody[i];
      }
      this->noteCount = melodyLength / 2;
    }

    State(State state) {
      this(state.type, state.stateDuration, state.melody, state.noteCount * 2);
    }

    int getDuration() {
      return stateDuration;
    }

    int getCurrentNote(int currentTime) {
      int currentMelodyTimePoint = currentTime % melody[2 * noteCount - 1];
      int currentNote = 0;
      int endTime = melody[2 * currentNote + 1];
      while (currentNote < noteCount && currentMelodyTimePoint >= endTime) {
        ++currentNote;
        endTime = melody[2 * currentNote + 1];
      }
      if (currentNote >= noteCount) {
        return 0;
      }
      return melody[2 * currentNote];
    }
};

class Machine {
  int currentState;
  int timeInCurrentState = 0;
  State* states;
  int stateCount;
  public:
    Machine(State* states, int stateCount, int startState) {
      this->stateCount = stateCount;
      currentState = startState;
    }
};

int main() {
  int melody[] = {400, 5, 800, 10};
  State s(READY_STATE, 12, melody, 4);
  for (int i = 0; i < 12; ++i) {
    int note = s.getCurrentNote(i);
    cout << "time = " << i << "; note = " << note << endl;
  }
  return 0;
}
