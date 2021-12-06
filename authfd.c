/*

authfd.c

Author: Tatu Ylonen <ylo@cs.hut.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
                   All rights reserved

Created: Wed Mar 29 01:30:28 1995 ylo

Functions for connecting the local authentication agent.

*/

/*
 * $Id: authfd.c,v 1.10 1996/10/29 22:34:52 kivinen Exp $
 * $Log: authfd.c,v $
 * Revision 1.10  1996/10/29 22:34:52  kivinen
 * 	log -> log_msg. Removed userfile.h.
 *
 * Revision 1.9  1996/10/24 14:05:44  ttsalo
 *       Cleaning up old fd-auth trash
 *
 * Revision 1.8  1996/10/21 16:15:40  ttsalo
 *       Fixed auth socket name handling
 *
 * Revision 1.7  1996/10/20 16:26:17  ttsalo
 * 	Modified the routines to use agent socket directly
 *
 * Revision 1.6  1996/10/03 18:46:13  ylo
 * 	Fixed a bug that caused "Received signal 14" errors.
 *
 * Revision 1.5  1996/09/27 13:56:35  ttsalo
 * 	Fixed a memory deallocation bug
 *
 * Revision 1.4  1996/09/11 17:54:22  kivinen
 * 	Added check for bind errors in
 * 	ssh_get_authentication_connection_fd.
 * 	Fixed bug in old alarm / timeout restoration.
 * 	Changed limit of messages from 256 kB to 30 kB.
 *
 * Revision 1.3  1996/09/08 17:21:04  ttsalo
 * 	A lot of changes in agent-socket handling
 *
 * Revision 1.2  1996/09/04 12:41:50  ttsalo
 * 	Minor fixes
 *
 * Revision 1.1.1.1  1996/02/18 21:38:11  ylo
 * 	Imported ssh-1.2.13.
 *
 * Revision 1.7  1995/09/21  17:08:11  ylo
 * 	Support AF_UNIX_SIZE.
 *
 * Revision 1.6  1995/09/09  21:26:38  ylo
 * /m/shadows/u2/users/ylo/ssh/README
 *
 * Revision 1.5  1995/08/29  22:18:58  ylo
 * 	Added remove_all_identities.
 *
 * Revision 1.4  1995/08/21  23:21:04  ylo
 * 	Deleted ssh_authenticate().
 * 	Pass session key and response_type in agent request.
 *
 * Revision 1.3  1995/07/13  01:14:40  ylo
 * 	Removed the "Last modified" header.
 *
 * Revision 1.2  1995/07/13  01:11:31  ylo
 * 	Added cvs log.
 *
 * $Endlog$
 */

#include "includes.h"
#include "ssh.h"
#include "rsa.h"
#include "authfd.h"
#include "buffer.h"
#include "bufaux.h"
#include "xmalloc.h"
#include "getput.h"

/* Returns the authentication fd, or -1 if there is none.
   Socket directory privileges are checked to prevent anyone
   from connecting to other users' sockets with suid root ssh. */

int ssh_get_authentication_fd()
{
  char *authsocket, *authsocketdir;
  int sock, namelen;
  struct sockaddr_un sunaddr;
  struct stat st;
  struct passwd *pw;
  
  authsocketdir = getenv(SSH_AUTHSOCKET_ENV_NAME);
  if (!authsocketdir)
    return -1;
  else
    authsocketdir = xstrdup(authsocketdir);

  /* Point to the end of the name */
  authsocket = authsocketdir + strlen(authsocketdir);

  /* Cut the authsocketdir and make authsocket point to
     the bare socket name */
  while (authsocket != authsocketdir && *authsocket != '/')
    authsocket--;

  *authsocket = '\0';
  authsocket++;

  pw = getpwuid(original_real_uid);
  
  /* Change to the socket directory so it's privileges can be
     reliably checked */
  chdir(authsocketdir);

  if (stat(".", &st) == -1)
    {
      perror("stat:");
      return -1;
    }

  if (st.st_uid != pw->pw_uid)
    {
      error("Invalid owner of authentication socket directory %s\n",
	    authsocketdir);
      return -1;
    }

  if ((st.st_mode & 077) != 0)
    {
      error("Invalid modes for authentication socket directory %s\n",
	    authsocketdir);
      return -1;
    }

  sunaddr.sun_family = AF_UNIX;
  strncpy(sunaddr.sun_path, authsocket, sizeof(sunaddr.sun_path));

  xfree(authsocketdir);
  
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0)
    return -1;

  if (connect(sock, (struct sockaddr *)&sunaddr,
	      AF_UNIX_SIZE(sunaddr)) < 0)
    {
      close(sock);
      return -1;
    }
  else
    {
      fcntl(sock, F_SETFL, 0);  /* Set the socket to blocking mode */
      return sock;
    }
}

/* Opens a socket to the authentication server.  Returns the number of
   that socket, or -1 if no connection could be made. */

int ssh_get_authentication_connection_fd()
{
  int authfd;

  /* Get the the socket number from the environment. */
  authfd = ssh_get_authentication_fd();

  if (authfd >= 0)
    debug("Connection to authentication agent opened.");
  else
    debug("No agent.");
  
  return authfd;
}  

/* Opens and connects a private socket for communication with the
   authentication agent.  Returns the file descriptor (which must be
   shut down and closed by the caller when no longer needed).
   Returns NULL if an error occurred and the connection could not be
   opened. */

AuthenticationConnection *
ssh_get_authentication_connection()
{
  AuthenticationConnection *auth;
  int sock;
  
  /* Get a connection to the authentication agent. */
  sock = ssh_get_authentication_connection_fd();

  /* Fail if we couldn't obtain a connection.  This happens if we exited
     due to a timeout. */
  if (sock < 0)
    return NULL;

  /* Applocate the connection structure and initialize it. */
  auth = xmalloc(sizeof(*auth));
  auth->fd = sock;
  buffer_init(&auth->packet);
  buffer_init(&auth->identities);
  auth->num_identities = 0;

  return auth;
}

/* Closes the connection to the authentication agent and frees any associated
   memory. */

void ssh_close_authentication_connection(AuthenticationConnection *ac)
{
  buffer_free(&ac->packet);
  buffer_free(&ac->identities);
  close(ac->fd);
}

/* Returns the first authentication identity held by the agent.
   Returns true if an identity is available, 0 otherwise.
   The caller must initialize the integers before the call, and free the
   comment after a successful call (before calling ssh_get_next_identity). */

int ssh_get_first_identity(AuthenticationConnection *auth,
			   int *bitsp, MP_INT *e, MP_INT *n, char **comment)
{
  unsigned char msg[8192];
  int len, l;

  /* Send a message to the agent requesting for a list of the identities
     it can represent. */
  msg[0] = 0;
  msg[1] = 0;
  msg[2] = 0;
  msg[3] = 1;
  msg[4] = SSH_AGENTC_REQUEST_RSA_IDENTITIES;
  if (write(auth->fd, msg, 5) != 5)
    {
      error("write auth->fd: %.100s", strerror(errno));
      return 0;
    }

  /* Read the length of the response.  XXX implement timeouts here. */
  len = 4;
  while (len > 0)
    {
      l = read(auth->fd, msg + 4 - len, len);
      if (l <= 0)
	{
	  error("read auth->fd: %.100s", strerror(errno));
	  return 0;
	}
      len -= l;
    }

  /* Extract the length, and check it for sanity.  (We cannot trust
     authentication agents). */
  len = GET_32BIT(msg);
  if (len < 1 || len > 30*1024)
    fatal("Authentication reply message too long: %d\n", len);

  /* Read the packet itself. */
  buffer_clear(&auth->identities);
  while (len > 0)
    {
      l = len;
      if (l > sizeof(msg))
	l = sizeof(msg);
      l = read(auth->fd, msg, l);
      if (l <= 0)
	fatal("Incomplete authentication reply.");
      buffer_append(&auth->identities, (char *)msg, l);
      len -= l;
    }
  
  /* Get message type, and verify that we got a proper answer. */
  buffer_get(&auth->identities, (char *)msg, 1);
  if (msg[0] != SSH_AGENT_RSA_IDENTITIES_ANSWER)
    fatal("Bad authentication reply message type: %d", msg[0]);
  
  /* Get the number of entries in the response and check it for sanity. */
  auth->num_identities = buffer_get_int(&auth->identities);
  if (auth->num_identities > 1024)
    fatal("Too many identities in authentication reply: %d\n", 
	  auth->num_identities);

  /* Return the first entry (if any). */
  return ssh_get_next_identity(auth, bitsp, e, n, comment);
}

/* Returns the next authentication identity for the agent.  Other functions
   can be called between this and ssh_get_first_identity or two calls of this
   function.  This returns 0 if there are no more identities.  The caller
   must free comment after a successful return. */

int ssh_get_next_identity(AuthenticationConnection *auth,
			  int *bitsp, MP_INT *e, MP_INT *n, char **comment)
{
  /* Return failure if no more entries. */
  if (auth->num_identities <= 0)
    return 0;

  /* Get the next entry from the packet.  These will abort with a fatal
     error if the packet is too short or contains corrupt data. */
  *bitsp = buffer_get_int(&auth->identities);
  buffer_get_mp_int(&auth->identities, e);
  buffer_get_mp_int(&auth->identities, n);
  *comment = buffer_get_string(&auth->identities, NULL);

  /* Decrement the number of remaining entries. */
  auth->num_identities--;

  return 1;
}

/* Generates a random challenge, sends it to the agent, and waits for response
   from the agent.  Returns true (non-zero) if the agent gave the correct
   answer, zero otherwise.  Response type selects the style of response
   desired, with 0 corresponding to protocol version 1.0 (no longer supported)
   and 1 corresponding to protocol version 1.1. */

int ssh_decrypt_challenge(AuthenticationConnection *auth,
			  int bits, MP_INT *e, MP_INT *n, MP_INT *challenge,
			  unsigned char session_id[16],
			  unsigned int response_type,
			  unsigned char response[16])
{
  Buffer buffer;
  unsigned char buf[8192];
  int len, l, i;

  /* Response type 0 is no longer supported. */
  if (response_type == 0)
    fatal("Compatibility with ssh protocol version 1.0 no longer supported.");

  /* Format a message to the agent. */
  buf[0] = SSH_AGENTC_RSA_CHALLENGE;
  buffer_init(&buffer);
  buffer_append(&buffer, (char *)buf, 1);
  buffer_put_int(&buffer, bits);
  buffer_put_mp_int(&buffer, e);
  buffer_put_mp_int(&buffer, n);
  buffer_put_mp_int(&buffer, challenge);
  buffer_append(&buffer, (char *)session_id, 16);
  buffer_put_int(&buffer, response_type);

  /* Get the length of the message, and format it in the buffer. */
  len = buffer_len(&buffer);
  PUT_32BIT(buf, len);

  /* Send the length and then the packet to the agent. */
  if (write(auth->fd, buf, 4) != 4 ||
      write(auth->fd, buffer_ptr(&buffer), buffer_len(&buffer)) !=
        buffer_len(&buffer))
    {
      error("Error writing to authentication socket.");
    error_cleanup:
      buffer_free(&buffer);
      return 0;
    }

  /* Wait for response from the agent.  First read the length of the
     response packet. */
  len = 4;
  while (len > 0)
    {
      l = read(auth->fd, buf + 4 - len, len);
      if (l <= 0)
	{
	  error("Error reading response length from authentication socket.");
	  goto error_cleanup;
	}
      len -= l;
    }

  /* Extract the length, and check it for sanity. */
  len = GET_32BIT(buf);
  if (len > 30*1024)
    fatal("Authentication response too long: %d", len);

  /* Read the rest of the response in to the buffer. */
  buffer_clear(&buffer);
  while (len > 0)
    {
      l = len;
      if (l > sizeof(buf))
	l = sizeof(buf);
      l = read(auth->fd, buf, l);
      if (l <= 0)
	{
	  error("Error reading response from authentication socket.");
	  goto error_cleanup;
	}
      buffer_append(&buffer, (char *)buf, l);
      len -= l;
    }

  /* Get the type of the packet. */
  buffer_get(&buffer, (char *)buf, 1);

  /* Check for agent failure message. */
  if (buf[0] == SSH_AGENT_FAILURE)
    {
      log_msg("Agent admitted failure to authenticate using the key.");
      goto error_cleanup;
    }
      
  /* Now it must be an authentication response packet. */
  if (buf[0] != SSH_AGENT_RSA_RESPONSE)
    fatal("Bad authentication response: %d", buf[0]);

  /* Get the response from the packet.  This will abort with a fatal error
     if the packet is corrupt. */
  for (i = 0; i < 16; i++)
    response[i] = buffer_get_char(&buffer);

  /* The buffer containing the packet is no longer needed. */
  buffer_free(&buffer);

  /* Correct answer. */
  return 1;
}  

/* Adds an identity to the authentication server.  This call is not meant to
   be used by normal applications. */

int ssh_add_identity(AuthenticationConnection *auth,
		     RSAPrivateKey *key, const char *comment)
{
  Buffer buffer;
  unsigned char buf[8192];
  int len, l, type;

  /* Format a message to the agent. */
  buffer_init(&buffer);
  buffer_put_char(&buffer, SSH_AGENTC_ADD_RSA_IDENTITY);
  buffer_put_int(&buffer, key->bits);
  buffer_put_mp_int(&buffer, &key->n);
  buffer_put_mp_int(&buffer, &key->e);
  buffer_put_mp_int(&buffer, &key->d);
  buffer_put_mp_int(&buffer, &key->u);
  buffer_put_mp_int(&buffer, &key->p);
  buffer_put_mp_int(&buffer, &key->q);
  buffer_put_string(&buffer, comment, strlen(comment));

  /* Get the length of the message, and format it in the buffer. */
  len = buffer_len(&buffer);
  PUT_32BIT(buf, len);

  /* Send the length and then the packet to the agent. */
  if (write(auth->fd, buf, 4) != 4 ||
      write(auth->fd, buffer_ptr(&buffer), buffer_len(&buffer)) !=
        buffer_len(&buffer))
    {
      error("Error writing to authentication socket.");
    error_cleanup:
      buffer_free(&buffer);
      return 0;
    }

  /* Wait for response from the agent.  First read the length of the
     response packet. */
  len = 4;
  while (len > 0)
    {
      l = read(auth->fd, buf + 4 - len, len);
      if (l <= 0)
	{
	  error("Error reading response length from authentication socket.");
	  goto error_cleanup;
	}
      len -= l;
    }

  /* Extract the length, and check it for sanity. */
  len = GET_32BIT(buf);
  if (len > 30*1024)
    fatal("Add identity response too long: %d", len);

  /* Read the rest of the response in tothe buffer. */
  buffer_clear(&buffer);
  while (len > 0)
    {
      l = len;
      if (l > sizeof(buf))
	l = sizeof(buf);
      l = read(auth->fd, buf, l);
      if (l <= 0)
	{
	  error("Error reading response from authentication socket.");
	  goto error_cleanup;
	}
      buffer_append(&buffer, (char *)buf, l);
      len -= l;
    }

  /* Get the type of the packet. */
  type = buffer_get_char(&buffer);
  switch (type)
    {
    case SSH_AGENT_FAILURE:
      buffer_free(&buffer);
      return 0;
    case SSH_AGENT_SUCCESS:
      buffer_free(&buffer);
      return 1;
    default:
      fatal("Bad response to add identity from authentication agent: %d", 
	    type);
    }
  /*NOTREACHED*/
  return 0;
}  

/* Removes an identity from the authentication server.  This call is not meant 
   to be used by normal applications. */

int ssh_remove_identity(AuthenticationConnection *auth, RSAPublicKey *key)
{
  Buffer buffer;
  unsigned char buf[8192];
  int len, l, type;

  /* Format a message to the agent. */
  buffer_init(&buffer);
  buffer_put_char(&buffer, SSH_AGENTC_REMOVE_RSA_IDENTITY);
  buffer_put_int(&buffer, key->bits);
  buffer_put_mp_int(&buffer, &key->e);
  buffer_put_mp_int(&buffer, &key->n);

  /* Get the length of the message, and format it in the buffer. */
  len = buffer_len(&buffer);
  PUT_32BIT(buf, len);

  /* Send the length and then the packet to the agent. */
  if (write(auth->fd, buf, 4) != 4 ||
      write(auth->fd, buffer_ptr(&buffer), buffer_len(&buffer)) !=
        buffer_len(&buffer))
    {
      error("Error writing to authentication socket.");
    error_cleanup:
      buffer_free(&buffer);
      return 0;
    }

  /* Wait for response from the agent.  First read the length of the
     response packet. */
  len = 4;
  while (len > 0)
    {
      l = read(auth->fd, buf + 4 - len, len);
      if (l <= 0)
	{
	  error("Error reading response length from authentication socket.");
	  goto error_cleanup;
	}
      len -= l;
    }

  /* Extract the length, and check it for sanity. */
  len = GET_32BIT(buf);
  if (len > 30*1024)
    fatal("Remove identity response too long: %d", len);

  /* Read the rest of the response in tothe buffer. */
  buffer_clear(&buffer);
  while (len > 0)
    {
      l = len;
      if (l > sizeof(buf))
	l = sizeof(buf);
      l = read(auth->fd, buf, l);
      if (l <= 0)
	{
	  error("Error reading response from authentication socket.");
	  goto error_cleanup;
	}
      buffer_append(&buffer, (char *)buf, l);
      len -= l;
    }

  /* Get the type of the packet. */
  type = buffer_get_char(&buffer);
  switch (type)
    {
    case SSH_AGENT_FAILURE:
      buffer_free(&buffer);
      return 0;
    case SSH_AGENT_SUCCESS:
      buffer_free(&buffer);
      return 1;
    default:
      fatal("Bad response to remove identity from authentication agent: %d", 
	    type);
    }
  /*NOTREACHED*/
  return 0;
}  

/* Removes all identities from the agent.  This call is not meant 
   to be used by normal applications. */

int ssh_remove_all_identities(AuthenticationConnection *auth)
{
  Buffer buffer;
  unsigned char buf[8192];
  int len, l, type;

  /* Get the length of the message, and format it in the buffer. */
  PUT_32BIT(buf, 1);
  buf[4] = SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES;

  /* Send the length and then the packet to the agent. */
  if (write(auth->fd, buf, 5) != 5)
    {
      error("Error writing to authentication socket.");
      return 0;
    }

  /* Wait for response from the agent.  First read the length of the
     response packet. */
  len = 4;
  while (len > 0)
    {
      l = read(auth->fd, buf + 4 - len, len);
      if (l <= 0)
	{
	  error("Error reading response length from authentication socket.");
	  return 0;
	}
      len -= l;
    }

  /* Extract the length, and check it for sanity. */
  len = GET_32BIT(buf);
  if (len > 30*1024)
    fatal("Remove identity response too long: %d", len);

  /* Read the rest of the response into the buffer. */
  buffer_init(&buffer);
  while (len > 0)
    {
      l = len;
      if (l > sizeof(buf))
	l = sizeof(buf);
      l = read(auth->fd, buf, l);
      if (l <= 0)
	{
	  error("Error reading response from authentication socket.");
	  buffer_free(&buffer);
	  return 0;
	}
      buffer_append(&buffer, (char *)buf, l);
      len -= l;
    }

  /* Get the type of the packet. */
  type = buffer_get_char(&buffer);
  switch (type)
    {
    case SSH_AGENT_FAILURE:
      buffer_free(&buffer);
      return 0;
    case SSH_AGENT_SUCCESS:
      buffer_free(&buffer);
      return 1;
    default:
      fatal("Bad response to remove identity from authentication agent: %d", 
	    type);
    }
  /*NOTREACHED*/
  return 0;
}  

/* Closes the connection to the authentication agent. */

void ssh_close_authentication(AuthenticationConnection *auth)
{
  /* Close the connection. */
  shutdown(auth->fd, 2);
  close(auth->fd);

  /* Free the buffers. */
  buffer_free(&auth->packet);
  buffer_free(&auth->identities);

  /* Free the connection data structure. */
  xfree(auth);
}
