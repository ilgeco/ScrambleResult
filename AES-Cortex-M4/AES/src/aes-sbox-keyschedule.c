#include <stdint.h>
#include "aes-sbox.h"
#include "aes-defs.h"

void aes_sbox_set_enc_key(uint8_t key[AES_SBOX_KEY_SIZE],
                          uint8_t rk[AES_SBOX_RK_SIZE])
{
  for (unsigned i = 0; i != AES_SBOX_KEY_SIZE; ++i)
    rk[i] = key[i];

  for (unsigned i = 4; i != 44; ++i) {
    uint8_t t1 = rk[(i - 1) * 4];
    uint8_t t2 = rk[(i - 1) * 4 + 1];
    uint8_t t3 = rk[(i - 1) * 4 + 2];
    uint8_t t4 = rk[(i - 1) * 4 + 3];

    if (i % 4 == 0) {
      uint8_t tt = t1;
      t1 = SBox[t2] ^ RCON[(i - 1) / 4];
      t2 = SBox[t3];
      t3 = SBox[t4];
      t4 = SBox[tt];
    }
    rk[i * 4] = rk[(i - 4) * 4] ^ t1;
    rk[i * 4 + 1] = rk[(i - 4) * 4 + 1] ^ t2;
    rk[i * 4 + 2] = rk[(i - 4) * 4 + 2] ^ t3;
    rk[i * 4 + 3] = rk[(i - 4) * 4 + 3] ^ t4;
  }
}
