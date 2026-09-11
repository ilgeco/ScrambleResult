#include <aes-sbox.h>
#include <stdint.h>

void usart_send_str(char *str, int bytes);
void usart_recv_str(char *str, int bytes);

#define BATCH_SIZE 1000
#define DELAY 5000
int entry(void) {

  /* receive AES key. If shorter than 128b, the host will zero-pad*/
  uint8_t key[AES_SBOX_KEY_SIZE * 2], keyschedule[AES_SBOX_RK_SIZE],
      keyschedule1[AES_SBOX_RK_SIZE] = {0};

  usart_send_str("Waiting Key\n", 13);
  usart_recv_str((char *)key, AES_SBOX_KEY_SIZE * 2);
  usart_send_str("Key Recived\n", 13);

  uint8_t mode;
  usart_recv_str((char *)&mode, 1);

  uint32_t is_mode_encryption = mode & 1;
  uint32_t use_256_bit_key = !!(mode & 2);

  if (is_mode_encryption) {
    usart_send_str("Setting Keyschedule\n", 21);
    aes_sbox_set_enc_key(key, keyschedule);
    // Per ottenere ke1 cifrare key usando chiave statica

    aes_sbox_set_enc_key(key, keyschedule1);
    usart_send_str("Setted Keyschedule\n", 20);
  } else {
    usart_send_str("Unsupported decryption\n", 24);
    return 0;
  }
  if (use_256_bit_key) {
    usart_send_str("Unsupported key\n", 17);
    return 0;
  }

  uint8_t ptx[AES_SBOX_BLOCK_SIZE] = {0}, ctx[AES_SBOX_BLOCK_SIZE] = {0},
          ctx1[AES_SBOX_BLOCK_SIZE] = {0};
  while (1) {
    usart_send_str("Gimmy ptx\n", 11);
    usart_recv_str((char *)ptx, AES_SBOX_BLOCK_SIZE);
    usart_send_str("Thy for ptx\n", 13);

    for (int i = 0; i < BATCH_SIZE; i++) {
#ifdef HALF
      aes_sbox_encrypt_no_first_half(keyschedule, keyschedule1, ctx, ctx1, ptx);
#else
      aes_sbox_encrypt_normal(keyschedule, keyschedule1, ctx, ctx1, ptx);
#endif
      /* Add a delay to allow trigger rearming, update input*/
      for (int j = 0; j < DELAY; j++)
        __asm__ volatile("nop;nop;nop;nop;nop;" ::: "memory");
      for (int j = 0; j < AES_SBOX_BLOCK_SIZE; j++) {
        ptx[j] = ptx[j];
      }
      for (int j = 0; j < DELAY; j++)
        __asm__ volatile("nop;nop;nop;nop;nop;" ::: "memory");
    }
    usart_send_str("Sending ctx\n", 13);
    usart_send_str((char *)ctx, AES_SBOX_BLOCK_SIZE);
  }
  return 0;
}
