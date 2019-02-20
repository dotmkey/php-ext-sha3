#include "keccak.h"

const unsigned long long RC[24] = {
	0x0000000000000001,
	0x0000000000008082,
	0x800000000000808A,
	0x8000000080008000,
	0x000000000000808B,
	0x0000000080000001,
	0x8000000080008081,
	0x8000000000008009,
	0x000000000000008A,
	0x0000000000000088,
	0x0000000080008009,
	0x000000008000000A,
	0x000000008000808B,
	0x800000000000008B,
	0x8000000000008089,
	0x8000000000008003,
	0x8000000000008002,
	0x8000000000000080,
	0x000000000000800A,
	0x800000008000000A,
	0x8000000080008081,
	0x8000000000008080,
	0x0000000080000001,
	0x8000000080008008
};

const unsigned short r[5][5] = {
	{0, 36, 3, 41, 18},
	{1, 44, 10, 45, 2},
	{62, 6, 43, 15, 61},
	{28, 55, 25, 21, 56},
	{27, 20, 39, 8, 14}
};

void keccak_f1600(unsigned long long A[5][5])
{
	unsigned short i;

	for (i = 0; i < 24; i++) {
		keccak_round(A, i);
	}
}

void keccak_round(unsigned long long A[5][5], unsigned short round)
{
	keccak_theta(A);
	keccak_rho(A);
	keccak_pi(A);
	keccak_chi(A);
	keccak_iota(A, round);
}

void rot(unsigned long long *word, unsigned short offset)
{
	*word = offset ? (*word << offset) ^ (*word >> (64 - offset)) : *word;
}

void keccak_theta(unsigned long long A[5][5])
{
	unsigned long long C[5], D[5];
	unsigned short x, y;

	for (x = 0; x < 5; x++) {
		C[x] = A[x][0] ^ A[x][1] ^ A[x][2] ^ A[x][3] ^ A[x][4];
	}

	for (x = 0; x < 5; x++) {
		unsigned long long temp = (C[(x + 1) % 5] << 1) ^ (C[(x + 1) % 5] >> 63);
		D[x] = C[(x + 4) % 5] ^ temp;
	}

	for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			A[x][y] ^= D[x];
		}
	}
}

void keccak_rho(unsigned long long A[5][5])
{
	unsigned short x, y;

	for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			A[x][y] = (A[x][y] << r[x][y]) ^ (A[x][y] >> (64 - r[x][y]));
		}
	}	
}

void keccak_pi(unsigned long long A[5][5])
{
	unsigned long long A_temp[5][5];
	unsigned short x, y;

	for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			A_temp[x][y] = A[x][y];
		}
	}

	for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			A[y][(2 * x + 3 * y) % 5] = A_temp[x][y];
		}
	}
}

void keccak_chi(unsigned long long A[5][5])
{
	unsigned long long C[5];
	unsigned short x, y;

	for (y = 0; y < 5; y++) {
		for (x = 0; x < 5; x++) {
			C[x] = A[x][y] ^ ((~A[(x + 1) % 5][y]) & A[(x + 2) % 5][y]);
		}

		for (x = 0; x < 5; x++) {
			A[x][y] = C[x];
		}
	}
}

void keccak_iota(unsigned long long A[5][5], unsigned short round)
{
	A[0][0] ^= RC[round];
}