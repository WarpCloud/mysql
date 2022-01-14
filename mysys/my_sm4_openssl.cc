/**
  @file mysys/my_sm4_openssl.cc
*/

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "m_string.h"
#include "my_sm4.h"
#include "my_sm4_impl.h"
#include "my_dbug.h"

/* keep in sync with enum my_aes_opmode in my_sm4.h */
const char *my_sm4_opmode_names[] = {
    "my_sm4_ecb",       "my_sm4_cbc",   "my_sm4_cfb",   "my_sm4_ctr",
    "my_sm4_cfb128",    "my_sm4_ofb",   nullptr /* needed for the type enumeration */
};


/* keep in sync with enum my_aes_opmode in my_sm4.h */
static uint my_sm4_opmode_key_sizes_impl[] = {
    128 /* sm4_ecb */,      128 /* sm4_cbc */,
    128 /* sm4_cfb */,      128 /* sm4_ctr */,
    128 /* sm4_cfb128 */,   128 /* sm4_ofb */
};

uint *my_sm4_opmode_key_sizes= my_sm4_opmode_key_sizes_impl;

static const EVP_CIPHER *sm4_evp_type(const my_sm4_opmode mode) {
    switch (mode) {
        case my_sm4_ecb:      return EVP_sm4_ecb();
        case my_sm4_cbc:      return EVP_sm4_cbc();
        case my_sm4_cfb:      return EVP_sm4_cfb();
        case my_sm4_ctr:      return EVP_sm4_ctr();
        case my_sm4_cfb128:   return EVP_sm4_cfb128();
        case my_sm4_ofb:      return EVP_sm4_ofb();
        default:              return nullptr;
    }
}

int my_sm4_encrypt(const unsigned char *source, uint32 source_length,
                   unsigned char *dest,
                   const unsigned char *key, uint32 key_length,
                   enum my_sm4_opmode mode, const unsigned char *iv,
                   bool padding) {
    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();
    const EVP_CIPHER *cipher= sm4_evp_type(mode);
    int u_len, f_len;
    /* The real key to be used for encryption */
    unsigned char rkey[MAX_SM4_KEY_LENGTH / 8];

    my_sm4_create_key(key, key_length, rkey, mode);
    if (!ctx || !cipher || (EVP_CIPHER_iv_length(cipher) > 0 && !iv))
        return MY_SM4_BAD_DATA;

    if (!EVP_EncryptInit(ctx, cipher, rkey, iv)) goto sm4_error;    /* Error */
    if (!EVP_CIPHER_CTX_set_padding(ctx, padding)) goto sm4_error;  /* Error */
    if (!EVP_EncryptUpdate(ctx, dest, &u_len, source, source_length))
        goto sm4_error; /* Error */

    if (!EVP_EncryptFinal(ctx, dest + u_len, &f_len)) goto sm4_error;   /* Error */

    EVP_CIPHER_CTX_free(ctx);

    return u_len + f_len;

sm4_error:
    /* need to explicitly clean up the error if we want to ignore it */
    ERR_clear_error();
    EVP_CIPHER_CTX_free(ctx);
    return MY_SM4_BAD_DATA;
}

int my_sm4_decrypt(const unsigned char *source, uint32 source_length,
                   unsigned char *dest,
                   const unsigned char *key, uint32 key_length,
                   enum my_sm4_opmode mode, const unsigned char *iv,
                   bool padding) {
    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();
    const EVP_CIPHER *cipher= sm4_evp_type(mode);
    int u_len, f_len;

    /* The real key to be used for decryption */
    unsigned char rkey[MAX_SM4_KEY_LENGTH / 8];

    my_sm4_create_key(key, key_length, rkey, mode);
    if (!ctx || !cipher || (EVP_CIPHER_iv_length(cipher) > 0 && !iv))
        return MY_SM4_BAD_DATA;

    if (!EVP_DecryptInit(ctx, cipher, rkey, iv))
        goto sm4_error;                                             /* Error */
    if (!EVP_CIPHER_CTX_set_padding(ctx, padding)) goto sm4_error;  /* Error */
    if (!EVP_DecryptUpdate(ctx, dest, &u_len, source, source_length))
        goto sm4_error; /* Error */
    if (!EVP_DecryptFinal_ex(ctx, dest + u_len, &f_len))
        goto sm4_error; /* Error */

    EVP_CIPHER_CTX_free(ctx);

    return u_len + f_len;

sm4_error:
    /* need to explicitly clean up the error if we want to ignore it */
    ERR_clear_error();
    EVP_CIPHER_CTX_free(ctx);
    return MY_SM4_BAD_DATA;
}

int my_sm4_get_size(uint32 source_length, my_sm4_opmode opmode) {
    const EVP_CIPHER *cipher= sm4_evp_type(opmode);
    size_t block_size;

    block_size= EVP_CIPHER_block_size(cipher);

    return block_size > 1 ? block_size * (source_length / block_size) + block_size : source_length;
}

/**
  Return true if the SM4 cipher and block mode requires an IV

  SYNOPSIS
  my_sm4_needs_iv()
  @param mode           encryption mode

  @retval true   IV needed
  @retval false  IV not needed
*/

bool my_sm4_needs_iv(my_sm4_opmode opmode) {
    const EVP_CIPHER *cipher= sm4_evp_type(opmode);
    int iv_length;

    iv_length = EVP_CIPHER_iv_length(cipher);
    assert(iv_length == 0 || iv_length == MY_SM4_IV_SIZE);
    return iv_length != 0 ? true : false;
}
