enum EGlobal_states{
  stop,
  moveL, //hindernisse
  bug,
  moth,
  lineFollow,
  robotFollow,
  mapping,
  autonomic,
} global_states;

int global_state = global_states.stop;

void stateMachine() {
  switch (global_state) {
  case global_states.stop:

    break;

  case global_states.moveL:

    break;

  case global_states.bug:

    break;

  case global_states.lineFollow:

    break;

  case global_states.robotFollow:

    break;

  case global_states.mapping:

    break;

  case global_states.autonomic:

    break;

  default:
    global_state = global_states.stop;

  }
}
