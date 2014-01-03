

//state machine that checks what to do with a scanned tag
typedef enum {
  rfid_state_locked,
  rfid_state_unlocked,
  rfid_state_add
}  rfid_states;

