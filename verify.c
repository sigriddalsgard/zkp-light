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
  gmp_randstate_t rand;
  mpz_t y, p, g, x, r, c, s, cy, v, t, q;
  mpz_inits(y, x, r, p, s, g, c, cy, v, t, q, NULL);

  printf("\n-->YOU ARE BOB!<--\n\n");

  printf("p and g is public knowledge:\n\n");

  gmp_randinit_default(rand);
  gmp_randseed_ui(rand, 34685443);

  mpz_urandomb(p, rand, 1024); 
  mpz_nextprime(p, p);

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

  printf("👉🏻 p = ");
  mpz_out_str(stdout, 10, p); 
  printf("\n");

  printf("👉🏻 g = ");
  mpz_out_str(stdout, 10, g);
  printf("\n\n");

  printf("The first thing Alice does is to give you the public key:\n");
  printf("👉🏻 y = g^x mod p = ");
  gmp_scanf("%Zd", y);
  printf("Then Alice gives you her commit, and you will enter it here: \n"
         "👉🏻 C = g^r mod p = ");
  gmp_scanf("%Zd", c);

  printf("------------------------------------------------\n");

  printf("If we multiply 'y * C mod p', we get:\n"
         "y * C mod p = g^x mod p * g^r mod p = g^x * g^r mod p\n"
         "= g^(x + r) mod p\n\n"
         "Bob knows both y, C and p, so it is easy for him to get the result.\n"
         "But what if Alice lies about r or doesn't know x?\n");

  printf("------------------------------------------------\n");

  printf("Fermat's Little Theorem says:\n"
         "g^(p-1) ≡ 1 mod p\n"
         "As a concequence, we get:\n"
         "g^a ≡ g^b mod p iff a ≡ b mod (p-1)\n"
         "Which means, when we calculate y * C, we can also say:\n"
         "g^(x + r) ≡ g^((x + r) mod (p - 1)) mod p\n"
         "where we say that:\n"
         "s = (x + r) mod (p - 1)\n");

  printf("------------------------------------------------\n");

  printf("Bob needs to verify that Alice's commit works every time, no matter\n"
         "which r she picks. So Bob challenges Alice by wanting her to tell\n"
         "him either of these two:\n"
         "1) r\n"
         "2) s\n\n");

  printf("If Bob chooses the first, then Alice only has to tell him r:\n"
         "g^r mod p = C\n"
         "which is quite easy to verify. But what if Alice lies?\n"
         "If Bob chooses the second, Alice has to know both x and use the true "
         "r.\n\n"
         "So what does Bob choose? 1 or 2?\n");

  int choice;

  scanf("%d", &choice);

  if (choice == 1) {
    printf("Insert r here:\n");
    gmp_scanf("%Zd", r);

    mpz_t computed_c;
    mpz_init(computed_c);

    modular_pow(computed_c, g, r, p);
    if (mpz_cmp(c, computed_c) == 0) {
      printf("Verification successful: C matches g^r mod p\n");
    } else {
      printf("Verification failed: C does NOT match g^r mod p, hence Alice is "
             "a liar\n");
    }

  } else if (choice == 2) {
    printf("Insert s here:\n");
    gmp_scanf("%Zd", s);

    mpz_t computed_s;
    mpz_init(computed_s);

    printf("g^s mod p = ");
    modular_pow(computed_s, g, s, p);

    printf("Now you will calculate 'C * y mod p' to verify:\n");
    mpz_mul(cy, c, y);
    mpz_mod(v, cy, p);
    gmp_printf("y * C mod p = %Zd * %Zd mod %Zd = %Zd\n", c, y, p, v);

    if (mpz_cmp(computed_s, v) == 0) {
      printf("🍾 🥳 Verification successful: 'C' matches 'g^r mod p' 🥳🍾\n");
    } else {
      printf("❌ Verification failed ❌: 'y * C' does NOT match 'g^s mod p', "
             "hence Alice is a liar\n");
    }

    printf("\nWas the verification successful? If so, do you still believe "
           "that Alice is telling the truth?\n"
           "She could just be lucky with her choice of r... if I were you, I "
           "would try again.\n"
           "If you try again, Alice has to use a new random number r.\n");
  } else {
    printf("Invalid choice. Please choose 1 or 2.\n");
  }

  return 0;
}
