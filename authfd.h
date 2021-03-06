/*

authfd.h

Author: Tatu Ylonen <ylo@ssh.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@ssh.fi>, Espoo, Finland
Copyright (c) 1995-1999 SSH Communications Security Oy, Espoo, Finland
                        All rights reserved

Created: Wed Mar 29 01:17:41 1995 ylo

Functions to interface with the SSH_AUTHENTICATION_FD socket.

*/

/*
 * $Id: authfd.h,v 1.6 1999/11/17 17:04:39 tri Exp $
 * $Log: authfd.h,v $
 * Revision 1.6  1999/11/17 17:04:39  tri
 * 	Fixed copyright notices.
 *
 * Revision 1.5  1997/03/26 07:01:19  kivinen
 *      Removed ssh_close_authentication function.
 *      Fixed prototypes.
 *
 * Revision 1.4  1996/10/20 16:24:39  ttsalo
 *      Removed ssh_close_authentication_socket
 *
 * Revision 1.3  1996/10/20 16:23:53  ttsalo
 *      Removed ssh_close_authentication_socket
 *
 * Revision 1.2  1996/09/08 17:21:05  ttsalo
 *      A lot of changes in agent-socket handling
 *
 * Revision 1.1.1.1  1996/02/18 21:38:11  ylo
 *      Imported ssh-1.2.13.
 *
 * Revision 1.5  1995/08/29  22:19:30  ylo
 *      Removed Last Modified line.
 *
 * Revision 1.4  1995/08/29  22:19:12  ylo
 *      Added remove_all_identities.
 *
 * Revision 1.3  1995/08/21  23:21:33  ylo
 *      Added session_id and reponse_type arguments to
 *      ssh_decrypt_challenge.
 *
 *      Deleted ssh_authenticate.
 *
 * Revision 1.2  1995/07/13  01:11:42  ylo
 *      Added cvs log.
 *
 * $Endlog$
 */

#ifndef AUTHFD_H
#define AUTHFD_H

#include "buffer.h"
#include "userfile.h"

/* Message types for SSH_AUTHENTICATION_FD socket. */
#define SSH_AUTHFD_CONNECT      0xf0

/* Messages for the authentication agent connection. */
#define SSH_AGENTC_REQUEST_RSA_IDENTITIES       1
#define SSH_AGENT_RSA_IDENTITIES_ANSWER         2
#define SSH_AGENTC_RSA_CHALLENGE                3
#define SSH_AGENT_RSA_RESPONSE                  4
#define SSH_AGENT_FAILURE                       5
#define SSH_AGENT_SUCCESS                       6
#define SSH_AGENTC_ADD_RSA_IDENTITY             7
#define SSH_AGENTC_REMOVE_RSA_IDENTITY          8
#define SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES    9

typedef struct
{
  int fd;
  Buffer packet;
  Buffer identities;
  int num_identities;
} AuthenticationConnection;

/* Returns the authentication fd or -1 if there is none. */
int ssh_get_authentication_fd(void);

/* Opens a socket to the authentication server.  Returns the number of
   that socket, or -1 if no connection could be made. */
int ssh_get_authentication_connection_fd(void);

/* Opens and connects a private socket for communication with the
   authentication agent.  Returns NULL if an error occurred and the 
   connection could not be opened.  The connection should be closed by
   the caller by calling ssh_close_authentication_connection(). */
AuthenticationConnection *ssh_get_authentication_connection(void);

/* Closes the connection to the authentication agent and frees any associated
   memory. */
void ssh_close_authentication_connection(AuthenticationConnection *ac);

/* Returns the first authentication identity held by the agent.
   Returns true if an identity is available, 0 otherwise.
   The caller must initialize the integers before the call, and free the
   comment after a successful call (before calling ssh_get_next_identity). */
int ssh_get_first_identity(AuthenticationConnection *connection,
                           int *bitsp, MP_INT *e, MP_INT *n, char **comment);

/* Returns the next authentication identity for the agent.  Other functions
   can be called between this and ssh_get_first_identity or two calls of this
   function.  This returns 0 if there are no more identities.  The caller
   must free comment after a successful return. */
int ssh_get_next_identity(AuthenticationConnection *connection,
                          int *bitsp, MP_INT *e, MP_INT *n, char **comment);

/* Requests the agent to decrypt the given challenge.  Returns true if
   the agent claims it was able to decrypt it. */
int ssh_decrypt_challenge(AuthenticationConnection *auth,
                          int bits, MP_INT *e, MP_INT *n, MP_INT *challenge,
                          unsigned char session_id[16], 
                          unsigned int response_type,
                          unsigned char response[16]);

/* Adds an identity to the authentication server.  This call is not meant to
   be used by normal applications.  This returns true if the identity
   was successfully added. */
int ssh_add_identity(AuthenticationConnection *connection,
                     RSAPrivateKey *key, const char *comment);

/* Removes the identity from the authentication server.  This call is
   not meant to be used by normal applications.  This returns true if the
   identity was successfully added. */
int ssh_remove_identity(AuthenticationConnection *connection,
                        RSAPublicKey *key);

/* Removes all identities from the authentication agent.  This call is not
   meant to be used by normal applications.  This returns true if the
   operation was successful. */
int ssh_remove_all_identities(AuthenticationConnection *connection);

#endif /* AUTHFD_H */
