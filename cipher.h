/*

cipher.h

Author: Tatu Ylonen <ylo@cs.hut.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
                   All rights reserved

Created: Wed Apr 19 16:50:42 1995 ylo

*/

/*
 * $Id: cipher.h,v 1.2 1996/02/18 21:52:35 ylo Exp $
 * $Log: cipher.h,v $
 * Revision 1.2  1996/02/18 21:52:35  ylo
 * 	Added comments that len must be multiple of 8.
 *
 * Revision 1.1.1.1  1996/02/18 21:38:11  ylo
 * 	Imported ssh-1.2.13.
 *
 * Revision 1.3  1995/08/18  22:48:27  ylo
 * 	Made IDEA optional.
 *
 * Revision 1.2  1995/07/13  01:19:52  ylo
 * 	Removed "Last modified" header.
 * 	Added cvs log.
 *
 * $Endlog$
 */

#ifndef CIPHER_H
#define CIPHER_H

#ifndef WITHOUT_IDEA
#include "idea.h"
#endif /* WITHOUT_IDEA */
#include "des.h"
#include "tss.h"
#include "arcfour.h"

/* Cipher types.  New types can be added, but old types should not be removed
   for compatibility.  The maximum allowed value is 31. */
#define SSH_CIPHER_NOT_SET	-1 /* None selected (invalid number). */
#define SSH_CIPHER_NONE		0 /* no encryption */
#define SSH_CIPHER_IDEA		1 /* IDEA CFB */
#define SSH_CIPHER_DES		2 /* DES CBC */
#define SSH_CIPHER_3DES		3 /* 3DES CBC */
#define SSH_CIPHER_TSS		4 /* TRI's Simple Stream encryption CBC */
#define SSH_CIPHER_ARCFOUR	5 /* Arcfour */

typedef struct {
  unsigned int type;
  union {
#ifndef WITHOUT_IDEA
    struct {
      IDEAContext key;
      unsigned char iv[8];
    } idea;
#endif /* WITHOUT_IDEA */
    struct {
      DESContext key;
      unsigned char iv[8];
    } des;
    struct {
      DESContext key1;
      unsigned char iv1[8];
      DESContext key2;
      unsigned char iv2[8];
      DESContext key3;
      unsigned char iv3[8];
    } des3;
    struct tss_context tss;
    ArcfourContext arcfour;
  } u;
} CipherContext;

/* Returns a bit mask indicating which ciphers are supported by this
   implementation.  The bit mask has the corresponding bit set of each
   supported cipher. */
unsigned int cipher_mask();

/* Returns the name of the cipher. */
const char *cipher_name(int cipher);

/* Parses the name of the cipher.  Returns the number of the corresponding
   cipher, or -1 on error. */
int cipher_number(const char *name);

/* Selects the cipher to use and sets the key.  If for_encryption is true,
   the key is setup for encryption; otherwise it is setup for decryption. */
void cipher_set_key(CipherContext *context, int cipher,
		    const unsigned char *key, int keylen, int for_encryption);

/* Sets key for the cipher by computing the MD5 checksum of the passphrase,
   and using the resulting 16 bytes as the key. */
void cipher_set_key_string(CipherContext *context, int cipher,
			   const char *passphrase, int for_encryption);

/* Encrypts data using the cipher.  For most ciphers, len should be a
   multiple of 8. */
void cipher_encrypt(CipherContext *context, unsigned char *dest,
		    const unsigned char *src, unsigned int len);

/* Decrypts data using the cipher.  For most ciphers, len should be a
   multiple of 8. */
void cipher_decrypt(CipherContext *context, unsigned char *dest,
		    const unsigned char *src, unsigned int len);

#endif /* CIPHER_H */
