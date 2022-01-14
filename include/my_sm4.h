#ifndef MY_SM4_INCLUDED
#define MY_SM4_INCLUDED

/**
  @file include/my_sm4.h
  Wrapper to give simple interface for MySQL to SM4 standard encryption.
*/

#include "my_inttypes.h"
#include "my_macros.h"

/** SM4 IV size is 16 bytes for all supported ciphers except ECB */
#define MY_SM4_IV_SIZE 16

/** SM4 block size is fixed to be 128 bits for CBC and ECB */
#define MY_SM4_BLOCK_SIZE 16

/** Supported SM4 cipher/block mode combos */
enum my_sm4_opmode {
    my_sm4_ecb,
    my_sm4_cbc,
    my_sm4_cfb,
    my_sm4_ctr,
    my_sm4_cfb128,
    my_sm4_ofb
};

#define MY_SM4_BEGIN my_sm4_ecb
#define MY_SM4_END my_sm4_ofb

/* If bad data discovered during decoding */
#define MY_SM4_BAD_DATA  -1

/** String representations of the supported SM4 modes. Keep in sync with my_sm4_opmode */
extern const char *my_sm4_opmode_names[];

/**
  Encrypt a buffer using SM4

  @param source         [in]  Pointer to data for encryption
  @param source_length  [in]  Size of encryption data
  @param dest           [out] Buffer to place encrypted data (must be large enough)
  @param key            [in]  Key to be used for encryption
  @param key_length     [in]  Length of the key. Will handle keys of any length
  @param mode           [in]  encryption mode
  @param iv             [in]  16 bytes initialization vector if needed. Otherwise NULL
  @param padding        [in]  if padding needed.
  @return              size of encrypted data, or negative in case of error
*/

int my_sm4_encrypt(const unsigned char *source, uint32 source_length,
                   unsigned char *dest,
                   const unsigned char *key, uint32 key_length,
                   enum my_sm4_opmode mode, const unsigned char *iv,
                   bool padding = true);

/**
  Decrypt an SM4 encrypted buffer

  @param source         Pointer to data for decryption
  @param source_length  size of encrypted data
  @param dest           buffer to place decrypted data (must be large enough)
  @param key            Key to be used for decryption
  @param key_length     Length of the key. Will handle keys of any length
  @param mode           encryption mode
  @param iv             16 bytes initialization vector if needed. Otherwise NULL
  @param padding        if padding needed.
  @return size of original data.
*/


int my_sm4_decrypt(const unsigned char *source, uint32 source_length,
                   unsigned char *dest,
                   const unsigned char *key, uint32 key_length,
                   enum my_sm4_opmode mode, const unsigned char *iv,
                   bool padding = true);

/**
  Calculate the size of a buffer large enough for encrypted data

  @param source_length  length of data to be encrypted
  @param mode           encryption mode
  @return               size of buffer required to store encrypted data
*/

int my_sm4_get_size(uint32 source_length, enum my_sm4_opmode mode);

/**
  Return true if the SM4 cipher and block mode requires an IV

  SYNOPSIS
  my_sm4_needs_iv()
  @param mode           encryption mode

  @retval true   IV needed
  @retval false  IV not needed
*/

bool my_sm4_needs_iv(my_sm4_opmode opmode);

#endif /* MY_SM4_INCLUDED */
