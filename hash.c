#include "hash.h"

int check_sha3_security_level(unsigned short security_level)
{
	if (security_level == 224 || security_level == 256 || security_level == 384 || security_level == 512) {
		return HASH_SUCCESS;
	} else {
		return HASH_FAILURE;
	}
}

int check_shake_security_level(unsigned short security_level)
{
	if (security_level == 128 || security_level == 256) {
		return HASH_SUCCESS;
	} else {
		return HASH_FAILURE;
	}
}

// Функция запуска алгоритма с исходными данными
void hash(unsigned short security_level, unsigned short digest_len, unsigned char *message, unsigned char *digest, unsigned char delimiter)
{
	Sponge_Instance sponge_instance;
	unsigned short capacity = 2 * security_level;
	unsigned short rate = KECCAK_WIDTH - capacity;

	sponge_initialize(&sponge_instance, capacity, rate, delimiter, digest_len);
	hash_run(&sponge_instance, message, digest); // вызов функции для запуска криптографической губки
}

// Функция для запуска криптографической губки
void hash_run(Sponge_Instance *sponge_instance, unsigned char *message, unsigned char *digest)
{
	sponge_absorb(sponge_instance, message); // Фаза "впитывания"
	sponge_squeeze(sponge_instance, digest); // Фаза "выжимания"
}