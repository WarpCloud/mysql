/**
  @file mysys/my_sm4_impl.h
*/


/** Maximum supported key kength */
#define MAX_SM4_KEY_LENGTH 128

extern uint *my_sm4_opmode_key_sizes;

void my_sm4_create_key(const unsigned char *key, uint key_length,
                       uint8 *rkey, enum my_sm4_opmode opmode);
