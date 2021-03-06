/*

match.c

Author: Tatu Ylonen <ylo@ssh.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@ssh.fi>, Espoo, Finland
Copyright (c) 1995-1999 SSH Communications Security Oy, Espoo, Finland
                        All rights reserved

Created: Thu Jun 22 01:17:50 1995 ylo

Simple pattern matching, with '*' and '?' as wildcards.

*/

/*
 * $Id: match.c,v 1.8 1999/11/17 17:04:47 tri Exp $
 * $Log: match.c,v $
 * Revision 1.8  1999/11/17 17:04:47  tri
 * 	Fixed copyright notices.
 *
 * Revision 1.7  1998/07/08 01:08:02  kivinen
 *      Added more conts, fixed typo.
 *
 * Revision 1.6  1998/07/08 01:05:11  kivinen
 *      Added consts.
 *
 * Revision 1.5  1998/07/08 00:44:44  kivinen
 *      Added match_host. Changed match_user to use it.
 *
 * Revision 1.4  1998/06/11 00:07:36  kivinen
 *      Added match_user function.
 *
 * Revision 1.3  1997/04/27  22:19:35  kivinen
 *      Fixed typo.
 *
 * Revision 1.2  1997/04/27 21:52:05  kivinen
 *      Added F-SECURE stuff. Added match_port function.
 *
 * Revision 1.1.1.1  1996/02/18 21:38:12  ylo
 *      Imported ssh-1.2.13.
 *
 * Revision 1.2  1995/07/13  01:27:07  ylo
 *      Removed "Last modified" header.
 *      Added cvs log.
 *
 * $Endlog$
 */

#include "includes.h"
#include "ssh.h"
#include "xmalloc.h"

/* Returns true if the given string matches the pattern (which may contain
   ? and * as wildcards), and zero if it does not match. */
          
int match_pattern(const char *s, const char *pattern)
{
  while (1)
    {
      /* If at end of pattern, accept if also at end of string. */
      if (!*pattern)
        return !*s;

      /* Process '*'. */
      if (*pattern == '*')
        {
          /* Skip the asterisk. */
          pattern++;

          /* If at end of pattern, accept immediately. */
          if (!*pattern)
            return 1;

          /* If next character in pattern is known, optimize. */
          if (*pattern != '?' && *pattern != '*')
            {
              /* Look instances of the next character in pattern, and try
                 to match starting from those. */
              for (; *s; s++)
                if (*s == *pattern &&
                    match_pattern(s + 1, pattern + 1))
                  return 1;
              /* Failed. */
              return 0;
            }

          /* Move ahead one character at a time and try to match at each
             position. */
          for (; *s; s++)
            if (match_pattern(s, pattern))
              return 1;
          /* Failed. */
          return 0;
        }

      /* There must be at least one more character in the string.  If we are
         at the end, fail. */
      if (!*s)
        return 0;

      /* Check if the next character of the string is acceptable. */
      if (*pattern != '?' && *pattern != *s)
        return 0;
      
      /* Move to the next character, both in string and in pattern. */
      s++;
      pattern++;
    }
  /*NOTREACHED*/
}

/* Check that host name matches the pattern. If the pattern only contains
   numbers and periods, and wildcards compare it against the ip address
   otherwise assume it is host name */
int match_host(const char *host, const char *ip, const char *pattern)
{
  int is_ip_pattern;
  const char *p;

  /* if the pattern does not contain any alpha characters then
     assume that it is a IP address (with possible wildcards),
     otherwise assume it is a hostname */
  if (ip)
    is_ip_pattern = 1;
  else
    is_ip_pattern = 0;

  for(p = pattern; *p; p++)
    if (!(isdigit(*p) || *p == '.' || *p == '?' || *p == '*'))
      {
        is_ip_pattern = 0;
        break;
      }
  if (is_ip_pattern)
    {
      return match_pattern(ip, pattern);
    } 
  return match_pattern(host, pattern);
}

/* this combines the effect of match_pattern on a username, hostname
   and IP address. If the pattern contains a @ then the part preceding
   the @ is checked against the username. The part after the @ is
   checked against the hostname and IP address. If no @ is found then
   a normal match_pattern is done against the username 

   This is more useful than just a match_pattern as it allows you to
   specify exactly what users are alowed to login from what hosts
   (tridge, May 1998)
*/
int match_user(const char *user, const char *host, const char *ip,
               const char *pattern)
{
  int ret;
  char *p2;
  char *p;

  p = strchr(pattern,'@');
  
  if (!p)
    return match_pattern(user, pattern);

  p2 = xstrdup(pattern);
  p = strchr(p2, '@');
  
  *p = 0;

  ret = match_pattern(user,p2) && match_host(host, ip, p + 1);
  
  xfree(p2);
  return ret;
}

#ifdef F_SECURE_COMMERCIAL








































































#endif /* F_SECURE_COMMERCIAL */
