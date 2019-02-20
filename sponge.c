#include "sponge.h"

// Функция инициализации параметров сущности криптографической губки
void sponge_initialize(Sponge_Instance *sponge_instance, unsigned short capacity, unsigned short rate, unsigned char delimiter, unsigned short digest_len)
{
	unsigned short x, y;

	// Инициализируем массив состояния с нулевыми значениями
	for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			sponge_instance->state[x][y] = 0;
		}
	}

	sponge_instance->capacity = capacity;
	sponge_instance->rate = rate; // присваиваем rate сущности криптографической губки
	sponge_instance->delimiter = delimiter;
	sponge_instance->digest_len = digest_len;
}

// Функция фазы "впитывания"
void sponge_absorb(Sponge_Instance *sponge_instance, unsigned char *message)
{
	unsigned long long message_len = strlen(message); // количество байт сообщения
	unsigned long long padding_message_len = message_len + (sponge_instance->rate / 8 - (message_len % (sponge_instance->rate / 8)));  // количество байт в сообщении после его доведения до длины, необходимой для вычислений
	unsigned long long N = padding_message_len / (sponge_instance->rate / 8); // количество блоков сообщения
	unsigned char *padding_message = (unsigned char*) malloc(sizeof(unsigned char) * padding_message_len); // сообщение после доведения его до необходимой длины
	unsigned long long i, j;
	
	sponge_pad(sponge_instance, padding_message, padding_message_len, message); // вызов функции доведения сообщения до необходимой длины

	/*
	 * В цикле мы заполняем двумерный массив (матрица состояния) 64-битных размером 5*5 значениями из одномерного массива байт (довденного сообщения) размеров 72 по найденной зависимости.
	 */
	for (i = 0; i < N; i++) {
		for (j = i * sponge_instance->rate / 8; j < (i + 1) * sponge_instance->rate / 8; j++) {
			sponge_instance->state[((j % (sponge_instance->rate / 8)) / 8) % 5][((j % (sponge_instance->rate / 8)) / 40) % 5] ^= ((unsigned long long)padding_message[j] << (j % 8) * 8);
		}

		keccak_f1600(sponge_instance->state); // вызов функции для запуска раундов алгоритма Keccak
	}
}

// Функция доведения сообщения до необходимого размера
void sponge_pad(Sponge_Instance *sponge_instance, unsigned char *padding_message, unsigned long long padding_message_len, unsigned char *message)
{
	unsigned long long message_len = strlen(message); // длина сообщения

	// дополняем сообщение определенными байтами до необходимой длины в зависимости от длины исходного сообщения
	for (unsigned long long i = 0; i < padding_message_len; i++) {
		padding_message[i] = (i < message_len) ? message[i] : ((i == (padding_message_len - 1)) ? 0x80 : 0x00);
	}
	padding_message[message_len] = ((padding_message_len - message_len) == 1) ? 0x81 : sponge_instance->delimiter;
}

// Функция фазы "выжимания"
void sponge_squeeze(Sponge_Instance *sponge_instance, unsigned char *digest)
{
	//unsigned short digest_len = sponge_instance->digest_len; // длина дайджеста в битах
	unsigned short x, y, k;
	long long cur_digest_len = 0;
	unsigned char temp;
	static const unsigned char hexits[16] = "0123456789abcdef"; // массив содержащий все символы 16-ричной системы исчисления
    
	do {
		for (y = 0; y < 5; y++) {
			for (x = 0; x < 5; x++) {
				if ((5 * y + x) < (sponge_instance->rate / 64)) {
					for (k = 8; k > 0; k--) {
						if (cur_digest_len >= sponge_instance->digest_len / 4) {
							x = y = 5;
							k = 0;
							break;
						}

						temp = ((sponge_instance->state[x][y] << (k - 1) * 8) >> 56);
						digest[cur_digest_len] = hexits[temp >> 4];
						digest[cur_digest_len + 1] = hexits[temp & 0x0f];
						cur_digest_len += 2;
					}
				}
			}
		}

		keccak_f1600(sponge_instance->state);
	} while (cur_digest_len < sponge_instance->digest_len / 4);
}