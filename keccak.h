void keccak_f1600(unsigned long long A[5][5]);
void keccak_round(unsigned long long A[5][5], unsigned short round);
void rot(unsigned long long *word, unsigned short offset);
void keccak_theta(unsigned long long A[5][5]);
void keccak_rho(unsigned long long A[5][5]);
void keccak_pi(unsigned long long A[5][5]);
void keccak_chi(unsigned long long A[5][5]);
void keccak_iota(unsigned long long A[5][5], unsigned short round);