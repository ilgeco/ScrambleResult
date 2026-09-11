#pragma once

#include <stdint.h>

#define AES_SBOX_KEY_SIZE 16
#define AES_SBOX_REG_ROUNDS 9
#define AES_SBOX_RK_SIZE (44 * 4)
#define AES_SBOX_BLOCK_SIZE 16

void aes_sbox_set_enc_key(uint8_t key[AES_SBOX_KEY_SIZE],
                          uint8_t rk[AES_SBOX_RK_SIZE]);

void aes_sbox_encrypt_normal(uint8_t rk[AES_SBOX_RK_SIZE],
                             uint8_t rk1[AES_SBOX_RK_SIZE],
                             uint8_t output[AES_SBOX_BLOCK_SIZE],
                             uint8_t output1[AES_SBOX_BLOCK_SIZE],
                             uint8_t input[AES_SBOX_BLOCK_SIZE]);

void aes_sbox_encrypt_no_first_half(uint8_t rk[AES_SBOX_RK_SIZE],
                                    uint8_t rk1[AES_SBOX_RK_SIZE],
                                    uint8_t output[AES_SBOX_BLOCK_SIZE],
                                    uint8_t output1[AES_SBOX_BLOCK_SIZE],
                                    uint8_t input[AES_SBOX_BLOCK_SIZE]);

void aes_sbox_decrypt(uint8_t rk[AES_SBOX_RK_SIZE],
                      uint8_t input[AES_SBOX_BLOCK_SIZE],
                      uint8_t output[AES_SBOX_BLOCK_SIZE]);

void aes_sbox_bulk_encrypt(uint8_t *in, uint8_t *out, int block_count,
                           uint8_t rk[AES_SBOX_RK_SIZE]);

void aes_sbox_bulk_decrypt(uint8_t *in, uint8_t *out, int block_count,
                           uint8_t rk[AES_SBOX_RK_SIZE]);

void aes_sbox_encrypt_in_place(uint8_t rk[AES_SBOX_RK_SIZE],
                               uint8_t state[AES_SBOX_BLOCK_SIZE]);
