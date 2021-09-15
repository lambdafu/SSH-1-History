/*

rsa.h

Author: Tatu Ylonen <ylo@cs.hut.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
                   All rights reserved

Created: Fri Mar  3 22:01:06 1995 ylo

RSA key generation, encryption and decryption.

*/

/*
 * $Id: rsa.h,v 1.3 1995/07/13 01:33:11 ylo Exp $
 * $Log: rsa.h,v $
 * Revision 1.3  1995/07/13  01:33:11  ylo
 * 	Fixed comments and label used to protect again multiple inclusion.
 *
 * Revision 1.2  1995/07/13  01:31:43  ylo
 * 	Removed "Last modified" header.
 * 	Added cvs log.
 *
 * $Endlog$
 */

#ifndef RSA_H
#define RSA_H

#include "gmp.h"
#include "randoms.h"

typedef struct
{
  unsigned int bits;		/* Modulus size in bits. */
  MP_INT e;			/* Public exponent. */
  MP_INT n;			/* Modulus. */
} RSAPublicKey;

typedef struct
{
  unsigned int bits;		/* Modulus size in bits. */
  MP_INT n;			/* Modulus. */
  MP_INT e;			/* Public exponent. */
  MP_INT d;			/* Private exponent. */
  MP_INT u;			/* Multiplicative inverse of p mod q. */
  MP_INT p;			/* Prime number p. */
  MP_INT q;			/* Prime number q. */
} RSAPrivateKey;

/* Generates a random integer of the desired number of bits. */
void rsa_random_integer(MP_INT *ret, RandomState *state, unsigned int bits);

/* Makes and returns a random prime of the desired number of bits.
   Note that the random number generator must be initialized properly
   before using this.

   The generated prime will have the highest bit set, and will have
   the two lowest bits set. */
void rsa_random_prime(MP_INT *ret, RandomState *state, unsigned int bits);

/* Generates RSA public and private keys.  This initializes the data
   structures; they should be freed with rsa_clear_private_key and
   rsa_clear_public_key. */
void rsa_generate_key(RSAPrivateKey *prv, RSAPublicKey *pub, 
		      RandomState *state, unsigned int bits);

/* Frees any memory associated with the private key. */
void rsa_clear_private_key(RSAPrivateKey *prv);

/* Frees any memory associated with the public key. */
void rsa_clear_public_key(RSAPublicKey *pub);

/* Performs a private-key RSA operation (encrypt/decrypt). */
void rsa_private(MP_INT *output, MP_INT *input, RSAPrivateKey *prv);

/* Performs a public-key RSA operation (encrypt/decrypt). */
void rsa_public(MP_INT *output, MP_INT *input, RSAPublicKey *pub);

/* Sets MP_INT memory allocation routines to ones that clear any memory
   when freed. */
void rsa_set_mp_memory_allocation();

/* Indicates whether the rsa module is permitted to show messages on
   the terminal. */
void rsa_set_verbose(int verbose);

/************* Kludge functions for RSAREF compatibility *******************/

/* These functions are a kludge but can be implemented using rsaref. */

/* Encrypt input using the public key.  The 24 least significant bits of
   input must contain the value 0x000200. */
void rsa_public_encrypt(MP_INT *output, MP_INT *input, RSAPublicKey *key,
			RandomState *state);

/* Decrypt input using the private key.  The 24 least significant bits of
   the result must contain the value 0x000200. */
void rsa_private_decrypt(MP_INT *output, MP_INT *input, RSAPrivateKey *key);

#endif /* RSA_H */
