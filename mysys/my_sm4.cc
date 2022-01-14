/**
  @file mysys/my_sm4.cc
*/

#include <sys/types.h>

#include "m_string.h"
#include "my_sm4.h"
#include "my_inttypes.h"
#include "mysys/my_sm4_impl.h"

/**
  Transforms an arbitrary long key into a fixed length SM4 key

  SM4 keys are of fixed length. This routine takes an arbitrary long key
  iterates over it in SM4 key length increment and XORs the bytes with the
  SM4 key buffer being prepared.
  The bytes from the last incomplete iteration are XORed to the start
  of the key until their depletion.
  Needed since crypto function routines expect a fixed length key.

  @param key        [in]       Key to use for real key creation
  @param key_length [in]       Length of the key
  @param rkey       [out]      Real key (used by OpenSSL)
  @param opmode     [out]      encryption mode
*/

void my_sm4_create_key(const unsigned char *key, uint key_length,
                       uint8 *rkey, enum my_sm4_opmode opmode) {
  const uint key_size = my_sm4_opmode_key_sizes[opmode] / 8;
  uint8 *rkey_end;                                    /* Real key boundary */
  uint8 *ptr;                                         /* Start of the real key*/
  const uint8 *sptr;                                  /* Start of the working key */
  const uint8 *key_end = ((const uint8 *)key) + key_length; /* Working key boundary*/

  rkey_end = rkey + key_size;

  memset(rkey, 0, key_size); /* Set initial key  */

  for (ptr = rkey, sptr = key; sptr < key_end; ptr++, sptr++) {
//  for (ptr = rkey, sptr = (const uint8 *)key; sptr < key_end; ptr++, sptr++) {
//>>>>>>> 10e0235db52... WARP-00000 fix mysql debug compile
    if (ptr == rkey_end) /*  Just loop over tmp_key until we used all key */
      ptr = rkey;
    *ptr ^= *sptr;
  }
}
