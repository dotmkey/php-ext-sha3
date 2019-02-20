#include <stdlib.h>
#include "keccak.h"

// Структура сущности криптографической губки
typedef struct {
	unsigned long long state[5][5]; // матрицв состояния из 64-битных слов
	unsigned short rate; // rate
	unsigned short capacity;
	unsigned char delimiter; //разделитель в паддинге ("единичный" байт)
	unsigned short digest_len;
} Sponge_Instance;

void sponge_initialize(Sponge_Instance *sponge_instance, unsigned short capacity, unsigned short rate, unsigned char delimiter, unsigned short digest_len);
void sponge_absorb(Sponge_Instance *sponge_instance, unsigned char *message);
void sponge_pad(Sponge_Instance *sponge_instance, unsigned char *padding_message, unsigned long long padding_message_len, unsigned char *message);
void sponge_squeeze(Sponge_Instance *sponge_instance, unsigned char *digest);