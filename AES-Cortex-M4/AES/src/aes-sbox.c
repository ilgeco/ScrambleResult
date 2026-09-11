#include "aes-sbox.h"
#include "aes-defs.h"
#include <stdint.h>

__attribute__((always_inline)) static inline uint8_t rj_xtime(uint8_t x) {
  uint8_t y = (uint8_t)(x << 1);
  int8_t mask = ((int8_t)(x & 0x80)) >> 7;
  return ((y ^ 0x1b) & mask) | (y & ~mask);
}

__attribute__((always_inline)) static inline uint8_t if_val(uint8_t val) {
  if (val & 0x80) {
    return 0x1b;
  }
  return 0x00;
}

__attribute__((always_inline)) static inline void MixColumns(uint8_t *state) {
  uint8_t a[4] = {0}, b[4] = {0};

  a[0] = state[0 * 4 + 0];
  b[0] = (a[0] << 1) ^ if_val(a[0]);
  a[1] = state[0 * 4 + 1];
  b[1] = (a[1] << 1) ^ if_val(a[1]);
  a[2] = state[0 * 4 + 2];
  b[2] = (a[2] << 1) ^ if_val(a[2]);
  a[3] = state[0 * 4 + 3];
  b[3] = (a[3] << 1) ^ if_val(a[3]);
  state[0 * 4] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
  state[0 * 4 + 1] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
  state[0 * 4 + 2] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
  state[0 * 4 + 3] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];

  a[0] = state[1 * 4 + 0];
  b[0] = (a[0] << 1) ^ if_val(a[0]);
  a[1] = state[1 * 4 + 1];
  b[1] = (a[1] << 1) ^ if_val(a[1]);
  a[2] = state[1 * 4 + 2];
  b[2] = (a[2] << 1) ^ if_val(a[2]);
  a[3] = state[1 * 4 + 3];
  b[3] = (a[3] << 1) ^ if_val(a[3]);
  state[1 * 4] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
  state[1 * 4 + 1] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
  state[1 * 4 + 2] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
  state[1 * 4 + 3] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];

  a[0] = state[2 * 4 + 0];
  b[0] = (a[0] << 1) ^ if_val(a[0]);
  a[1] = state[2 * 4 + 1];
  b[1] = (a[1] << 1) ^ if_val(a[1]);
  a[2] = state[2 * 4 + 2];
  b[2] = (a[2] << 1) ^ if_val(a[2]);
  a[3] = state[2 * 4 + 3];
  b[3] = (a[3] << 1) ^ if_val(a[3]);
  state[2 * 4] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
  state[2 * 4 + 1] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
  state[2 * 4 + 2] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
  state[2 * 4 + 3] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];

  a[0] = state[3 * 4 + 0];
  b[0] = (a[0] << 1) ^ if_val(a[0]);
  a[1] = state[3 * 4 + 1];
  b[1] = (a[1] << 1) ^ if_val(a[1]);
  a[2] = state[3 * 4 + 2];
  b[2] = (a[2] << 1) ^ if_val(a[2]);
  a[3] = state[3 * 4 + 3];
  b[3] = (a[3] << 1) ^ if_val(a[3]);
  state[3 * 4] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
  state[3 * 4 + 1] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
  state[3 * 4 + 2] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
  state[3 * 4 + 3] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
}

__attribute__((always_inline)) static inline void InvMixColumns(uint8_t *buf) {
  uint8_t i, a, b, c, d, e, x, y, z;

#pragma unroll(16)
  for (i = 0; i < 16; i += 4) {
    a = buf[i];
    b = buf[i + 1];
    c = buf[i + 2];
    d = buf[i + 3];
    e = a ^ b ^ c ^ d;
    z = rj_xtime(e);
    x = e ^ rj_xtime(rj_xtime(z ^ a ^ c));
    y = e ^ rj_xtime(rj_xtime(z ^ b ^ d));
    buf[i] ^= x ^ rj_xtime(a ^ b);
    buf[i + 1] ^= y ^ rj_xtime(b ^ c);
    buf[i + 2] ^= x ^ rj_xtime(c ^ d);
    buf[i + 3] ^= y ^ rj_xtime(d ^ a);
  }
}

__attribute__((always_inline)) static inline void ShiftRows(uint8_t *state) {
  uint8_t tmp;

  // Second row [x10 x11 x12 x13] to [x11 x12 x13 x10]
  tmp = state[1];
  state[1] = state[1 + 4];      // x10' <- x11
  state[1 + 4] = state[1 + 8];  // x11' <- x12
  state[1 + 8] = state[1 + 12]; // x12' <- x13
  state[1 + 12] = tmp;          // x13' <- x10

  // Third row [x20 x21 x22 x23] to [x22 x23 x20 x21]
  tmp = state[2];
  state[2] = state[2 + 8]; // x20' <- x22
  state[2 + 8] = tmp;      // x22' <- x20
  tmp = state[2 + 4];
  state[2 + 4] = state[2 + 12]; // x21' <- x23
  state[2 + 12] = tmp;          // x23' <- x21

  // Fourth row [x30 x31 x32 x33] to [x33 x30 x31 x32]
  tmp = state[3];
  state[3] = state[3 + 12];     // x30' <- x33
  state[3 + 12] = state[3 + 8]; // x33' <- x32
  state[3 + 8] = state[3 + 4];  // x32' <- x31
  state[3 + 4] = tmp;           // x31' <- x30
}

__attribute__((always_inline)) static inline void InvShiftRows(uint8_t *state) {
  uint8_t tmp;

  tmp = state[1];
  state[1] = state[1 + 12];
  state[1 + 12] = state[1 + 8];
  state[1 + 8] = state[1 + 4];
  state[1 + 4] = tmp;

  tmp = state[2];
  state[2] = state[2 + 8];
  state[2 + 8] = tmp;
  tmp = state[2 + 4];
  state[2 + 4] = state[2 + 12];
  state[2 + 12] = tmp;

  tmp = state[3];
  state[3] = state[3 + 4];
  state[3 + 4] = state[3 + 8];
  state[3 + 8] = state[3 + 12];
  state[3 + 12] = tmp;
}

__attribute__((always_inline)) static inline void
AddRoundKeys(uint8_t *state, uint8_t *rk, int round) {
  state[0] ^= rk[AES_SBOX_BLOCK_SIZE * round + 0];
  state[1] ^= rk[AES_SBOX_BLOCK_SIZE * round + 1];
  state[2] ^= rk[AES_SBOX_BLOCK_SIZE * round + 2];
  state[3] ^= rk[AES_SBOX_BLOCK_SIZE * round + 3];
  state[4] ^= rk[AES_SBOX_BLOCK_SIZE * round + 4];
  state[5] ^= rk[AES_SBOX_BLOCK_SIZE * round + 5];
  state[6] ^= rk[AES_SBOX_BLOCK_SIZE * round + 6];
  state[7] ^= rk[AES_SBOX_BLOCK_SIZE * round + 7];
  state[8] ^= rk[AES_SBOX_BLOCK_SIZE * round + 8];
  state[9] ^= rk[AES_SBOX_BLOCK_SIZE * round + 9];
  state[10] ^= rk[AES_SBOX_BLOCK_SIZE * round + 10];
  state[11] ^= rk[AES_SBOX_BLOCK_SIZE * round + 11];
  state[12] ^= rk[AES_SBOX_BLOCK_SIZE * round + 12];
  state[13] ^= rk[AES_SBOX_BLOCK_SIZE * round + 13];
  state[14] ^= rk[AES_SBOX_BLOCK_SIZE * round + 14];
  state[15] ^= rk[AES_SBOX_BLOCK_SIZE * round + 15];
}

__attribute__((always_inline)) static inline void SubBytes(uint8_t *state) {
  state[0] = SBox[state[0]];
  state[1] = SBox[state[1]];
  state[2] = SBox[state[2]];
  state[3] = SBox[state[3]];
  state[4] = SBox[state[4]];
  state[5] = SBox[state[5]];
  state[6] = SBox[state[6]];
  state[7] = SBox[state[7]];
  state[8] = SBox[state[8]];
  state[9] = SBox[state[9]];
  state[10] = SBox[state[10]];
  state[11] = SBox[state[11]];
  state[12] = SBox[state[12]];
  state[13] = SBox[state[13]];
  state[14] = SBox[state[14]];
  state[15] = SBox[state[15]];
}

__attribute__((always_inline)) static inline void InvSubBytes(uint8_t *state) {
  state[0] = InvSBox[state[0]];
  state[1] = InvSBox[state[1]];
  state[2] = InvSBox[state[2]];
  state[3] = InvSBox[state[3]];
  state[4] = InvSBox[state[4]];
  state[5] = InvSBox[state[5]];
  state[6] = InvSBox[state[6]];
  state[7] = InvSBox[state[7]];
  state[8] = InvSBox[state[8]];
  state[9] = InvSBox[state[9]];
  state[10] = InvSBox[state[10]];
  state[11] = InvSBox[state[11]];
  state[12] = InvSBox[state[12]];
  state[13] = InvSBox[state[13]];
  state[14] = InvSBox[state[14]];
  state[15] = InvSBox[state[15]];
}

#define BODY                                                                   \
  for (int i = 0; i < AES_SBOX_BLOCK_SIZE; ++i) {                              \
    state[i] = input[i];                                                       \
  }                                                                            \
  AddRoundKeys(state, rk, 0);                                                  \
  for (int i = 0; i < 9; i++) {                                                \
    SubBytes(state);                                                           \
    ShiftRows(state);                                                          \
    MixColumns(state);                                                         \
    AddRoundKeys(state, rk, i + 1);                                            \
  }                                                                            \
  SubBytes(state);                                                             \
  ShiftRows(state);                                                            \
  AddRoundKeys(state, rk, 10);                                                 \
  for (int i = 0; i < AES_SBOX_BLOCK_SIZE; ++i)                                \
    output[i] = state[i];

void aes_sbox_encrypt_normal(uint8_t rk[AES_SBOX_RK_SIZE],
                             uint8_t rk1[AES_SBOX_RK_SIZE],
                             uint8_t output[AES_SBOX_BLOCK_SIZE],
                             uint8_t output1[AES_SBOX_BLOCK_SIZE],
                             uint8_t input[AES_SBOX_BLOCK_SIZE]) {
  uint8_t state[AES_SBOX_BLOCK_SIZE];

  // goto init;

init:
  BODY
}

void aes_sbox_encrypt_no_first_half(uint8_t rk[AES_SBOX_RK_SIZE],
                                    uint8_t rk1[AES_SBOX_RK_SIZE],
                                    uint8_t output[AES_SBOX_BLOCK_SIZE],
                                    uint8_t output1[AES_SBOX_BLOCK_SIZE],
                                    uint8_t input[AES_SBOX_BLOCK_SIZE]) {
  uint8_t state[AES_SBOX_BLOCK_SIZE];

  // goto init;

init:
  BODY
}

void aes_sbox_decrypt(uint8_t rk[AES_SBOX_RK_SIZE],
                      uint8_t input[AES_SBOX_BLOCK_SIZE],
                      uint8_t output[AES_SBOX_BLOCK_SIZE]) {
  uint8_t state[AES_SBOX_BLOCK_SIZE];

  for (int i = 0; i < AES_SBOX_BLOCK_SIZE; ++i) {
    state[i] = input[i];
  }
  AddRoundKeys(state, rk, AES_SBOX_REG_ROUNDS + 1);
  InvShiftRows(state);
  InvSubBytes(state);

  for (int i = 0; i < AES_SBOX_REG_ROUNDS; i++) {
    AddRoundKeys(state, rk, AES_SBOX_REG_ROUNDS - i);
    InvMixColumns(state);
    InvShiftRows(state);
    InvSubBytes(state);
  }

  AddRoundKeys(state, rk, 0);

  for (int i = 0; i < AES_SBOX_BLOCK_SIZE; ++i)
    output[i] = state[i];
}

void aes_sbox_bulk_decrypt(uint8_t *in, uint8_t *out, int block_count,
                           uint8_t rk[AES_SBOX_RK_SIZE]) {
  for (int i = 0; i < block_count * AES_SBOX_BLOCK_SIZE;
       i += AES_SBOX_BLOCK_SIZE) {
    aes_sbox_decrypt(rk, &in[i], &out[i]);
  }
}
