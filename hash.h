#include "sponge.h"

#define HASH_SUCCESS 1
#define HASH_FAILURE 0
#define KECCAK_WIDTH 1600

int check_sha3_security_level(unsigned short security_level);
int check_shake_security_level(unsigned short security_level);
void hash(unsigned short security_level, unsigned short digest_len, unsigned char *message, unsigned char *digest, unsigned char delimiter);
void hash_run(Sponge_Instance *sponge_instance, unsigned char *message, unsigned char *digest);