#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <gmp.h>

void modular_pow(mpz_t result, const mpz_t base, const mpz_t exp,
                 const mpz_t mod) {
  mpz_powm(result, base, exp, mod);

  gmp_printf("(%Zd^%Zd) mod %Zd = %Zd\n", base, exp, mod, result);
}

void calc_s(mpz_t s, mpz_t x, mpz_t r, mpz_t p) {
  mpz_t p_minus_1;
  mpz_init(p_minus_1);
  mpz_sub_ui(p_minus_1, p, 1); // temp = p-1
  mpz_add(s, x, r);            // s = x + r
  mpz_mod(s, s, p_minus_1);    // s = s mod (p-1)
  gmp_printf("%Zd + %Zd mod (%Zd-1) = %Zd\n", x, r, p, s);

  mpz_clear(p_minus_1);
}

int main() {
  if (sodium_init() < 0)
    return 1;

  uint32_t num = randombytes_uniform(10000);

  gmp_randstate_t rand;
  mpz_t c, g, p, q, r, s, t, x, y;
  mpz_inits(c, g, p, q, r, s, t, x, y, NULL);
  mpz_set_ui(r, num);

  printf("\n-->YOU ARE ALICE!<--\n\n");

  printf("We have this equality: y = g^x mod p \n"
         "where g and p are public knowledge, y is a 'public key', and x is my "
         "secret. \n"
         "I will try to convince the verifier, that I know x without telling "
         "what it is.\n \n"
         "For simplicity we have: \n");

  gmp_randinit_default(rand);
  gmp_randseed_ui(rand, 34685443);

  mpz_urandomb(p, rand, 1024);
  mpz_nextprime(p, p);

  printf("👉🏻 p = ");
  mpz_out_str(stdout, 10, p); 
  printf("\n");

  do {
    mpz_urandomb(q, rand, 1023);
    mpz_nextprime(q, q);
    mpz_mul_ui(p, q, 2);
    mpz_add_ui(p, p, 1);
  } while (!mpz_probab_prime_p(p, 25));

  for (mpz_set_ui(g, 2);; mpz_add_ui(g, g, 1)) {
    mpz_powm(t, g, q, p);
    if (!mpz_cmp_ui(t, 1))
      continue;
    mpz_powm_ui(t, g, 2, p);
    if (mpz_cmp_ui(t, 1))
      break;
  }

  printf("👉🏻 g = ");
  mpz_out_str(stdout, 10, g);
  printf("\n\n");

  printf("Choose a secret number x: \n👉🏻 ");
  gmp_scanf("%Zd", x);

  printf("\nNow you use this x in the equation, and then you give Bob the "
         "result of 'y':\n");

  printf("👉🏻 y = ");
  modular_pow(y, g, x, p);

  printf("\n-----------------------------------------------------\n\n");

  gmp_printf("You need a random number 'r' for your commit. Instead of you "
             "choosing one, \n"
             "the computer will generate one for you.\n"
             "👉🏻 r = %Zd",
             r);

  printf("\n\n-----------------------------------------------------\n\n");

  printf("After you have given Bob 'y', you have to give him your commit:\n");

  printf("C = g^r mod p\n");

  printf("👉🏻 C = ");
  modular_pow(c, g, r, p);

  printf("\nGive C to Bob.");

  printf("\n\n-----------------------------------------------------\n\n");

  printf("If Bob asks you to tell him 's', you give him this number:\n");
  printf("s = x + r mod (p-1)\n");

  printf("👉🏻 s = ");
  calc_s(s, x, r, p);

  printf("\nNow you need to wait for Bob to believe you.\n");

  return 0;
}
