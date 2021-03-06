/*

ttymodes.c

Author: Tatu Ylonen <ylo@ssh.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@ssh.fi>, Espoo, Finland
Copyright (c) 1995-1999 SSH Communications Security Oy, Espoo, Finland
                        All rights reserved

Created: Tue Mar 21 15:59:15 1995 ylo

Encoding and decoding of terminal modes in a portable way.
Much of the format is defined in ttymodes.h; it is included multiple times
into this file with the appropriate macro definitions to generate the
suitable code.

*/

/*
 * $Id: ttymodes.c,v 1.5 1999/11/17 17:05:00 tri Exp $
 * $Log: ttymodes.c,v $
 * Revision 1.5  1999/11/17 17:05:00  tri
 * 	Fixed copyright notices.
 *
 * Revision 1.4  1998/04/17 00:42:50  kivinen
 *      Fixed previous fix.
 *
 * Revision 1.3  1998/03/27 17:05:47  kivinen
 *      Combine ECHOE and LCRTBS/LCRTERA flags.
 *
 * Revision 1.2  1996/10/29 22:47:53  kivinen
 *      log -> log_msg.
 *
 * Revision 1.1.1.1  1996/02/18 21:38:12  ylo
 *      Imported ssh-1.2.13.
 *
 * Revision 1.6  1995/10/02  01:29:44  ylo
 *      Changed warning message to make it less likely people will
 *      report it as a bug because it is not.
 *
 * Revision 1.5  1995/09/27  02:19:11  ylo
 *      Use SPEED_T_IN_STDTYPES_H.
 *
 * Revision 1.4  1995/09/09  21:26:48  ylo
 * /m/shadows/u2/users/ylo/ssh/README
 *
 * Revision 1.3  1995/08/18  22:58:34  ylo
 *      Added support for NextStep.
 *      Fixed typos with EXTB.
 *
 * Revision 1.2  1995/07/13  01:41:15  ylo
 *      Removed "Last modified" header.
 *      Added cvs log.
 *
 * $Endlog$
 */

#include "includes.h"
#include "packet.h"
#include "ssh.h"

#define TTY_OP_END      0
#define TTY_OP_ISPEED   192 /* int follows */
#define TTY_OP_OSPEED   193 /* int follows */

/* Speed extraction & setting macros for sgtty. */

#ifdef USING_SGTTY
#define cfgetospeed(tio)        ((tio)->sg_ospeed)
#define cfgetispeed(tio)        ((tio)->sg_ispeed)
#define cfsetospeed(tio, spd)   ((tio)->sg_ospeed = (spd), 0)
#define cfsetispeed(tio, spd)   ((tio)->sg_ispeed = (spd), 0)
#ifndef SPEED_T_IN_STDTYPES_H
typedef char speed_t;
#endif
#endif

/* Converts POSIX speed_t to a baud rate.  The values of the constants
   for speed_t are not themselves portable. */

static int speed_to_baud(speed_t speed)
{
  switch (speed)
    {
    case B0:
      return 0;
    case B50:
      return 50;
    case B75:
      return 75;
    case B110:
      return 110;
    case B134:
      return 134;
    case B150:
      return 150;
    case B200:
      return 200;
    case B300:
      return 300;
    case B600:
      return 600;
    case B1200:
      return 1200;
    case B1800:
      return 1800;
    case B2400:
      return 2400;
    case B4800:
      return 4800;
    case B9600:
      return 9600;

#ifdef B19200
    case B19200:
      return 19200;
#else /* B19200 */
#ifdef EXTA
    case EXTA:
      return 19200;
#endif /* EXTA */
#endif /* B19200 */

#ifdef B38400
    case B38400:
      return 38400;
#else /* B38400 */
#ifdef EXTB
    case EXTB:
      return 38400;
#endif /* EXTB */
#endif /* B38400 */

#ifdef B7200
    case B7200:
      return 7200;
#endif /* B7200 */
#ifdef B14400
    case B14400:
      return 14400;
#endif /* B14400 */
#ifdef B28800
    case B28800:
      return 28800;
#endif /* B28800 */
#ifdef B57600
    case B57600:
      return 57600;
#endif /* B57600 */
#ifdef B76800
    case B76800:
      return 76800;
#endif /* B76800 */
#ifdef B115200
    case B115200:
      return 115200;
#endif /* B115200 */
#ifdef B230400
    case B230400:
      return 230400;
#endif /* B230400 */
    default:
      return 9600;
    }
}

/* Converts a numeric baud rate to a POSIX speed_t. */

static speed_t baud_to_speed(int baud)
{
  switch (baud)
    {
    case 0:
      return B0;
    case 50:
      return B50;
    case 75:
      return B75;
    case 110:
      return B110;
    case 134:
      return B134;
    case 150:
      return B150;
    case 200:
      return B200;
    case 300:
      return B300;
    case 600:
      return B600;
    case 1200:
      return B1200;
    case 1800:
      return B1800;
    case 2400:
      return B2400;
    case 4800:
      return B4800;
    case 9600:
      return B9600;

#ifdef B19200
    case 19200:
      return B19200;
#else /* B19200 */
#ifdef EXTA
    case 19200:
      return EXTA;
#endif /* EXTA */
#endif /* B19200 */

#ifdef B38400
    case 38400:
      return B38400;
#else /* B38400 */
#ifdef EXTB
    case 38400:
      return EXTB;
#endif /* EXTB */
#endif /* B38400 */

#ifdef B7200
    case 7200:
      return B7200;
#endif /* B7200 */
#ifdef B14400
    case 14400:
      return B14400;
#endif /* B14400 */
#ifdef B28800
    case 28800:
      return B28800;
#endif /* B28800 */
#ifdef B57600
    case 57600:
      return B57600;
#endif /* B57600 */
#ifdef B76800
    case 76800:
      return B76800;
#endif /* B76800 */
#ifdef B115200
    case 115200:
      return B115200;
#endif /* B115200 */
#ifdef B230400
    case 230400:
      return B230400;
#endif /* B230400 */
    default:
      return B9600;
    }
}

/* Encodes terminal modes for the terminal referenced by fd in a portable
   manner, and appends the modes to a packet being constructed. */

void tty_make_modes(int fd)
{
#ifdef USING_TERMIOS
  struct termios tio;
#endif
#ifdef USING_SGTTY
  struct sgttyb tio;
  struct tchars tiotc;
  struct ltchars tioltc;
  int tiolm;
#ifdef TIOCGSTAT
  struct tstatus tiots;
#endif /* TIOCGSTAT */
#endif /* USING_SGTTY */
  int baud;

  /* Get the modes. */
#ifdef USING_TERMIOS
  if (tcgetattr(fd, &tio) < 0)
    {
      packet_put_char(TTY_OP_END);
      log_msg("tcgetattr: %.100s", strerror(errno));
      return;
    }
#endif /* USING_TERMIOS */
#ifdef USING_SGTTY
  if (ioctl(fd, TIOCGETP, &tio) < 0)
    {
      packet_put_char(TTY_OP_END);
      log_msg("ioctl(fd, TIOCGETP, ...): %.100s", strerror(errno));
      return;
    }
  if (ioctl(fd, TIOCGETC, &tiotc) < 0)
    {
      packet_put_char(TTY_OP_END);
      log_msg("ioctl(fd, TIOCGETC, ...): %.100s", strerror(errno));
      return;
    }
  if (ioctl(fd, TIOCLGET, &tiolm) < 0)
    {
      packet_put_char(TTY_OP_END);
      log_msg("ioctl(fd, TIOCLGET, ...): %.100s", strerror(errno));
      return;
    }
  if (ioctl(fd, TIOCGLTC, &tioltc) < 0)
    {
      packet_put_char(TTY_OP_END);
      log_msg("ioctl(fd, TIOCGLTC, ...): %.100s", strerror(errno));
      return;
    }
#ifdef TIOCGSTAT
  if (ioctl(fd, TIOCGSTAT, &tiots) < 0) 
    {
      packet_put_char(TTY_OP_END);
      log_msg("ioctl(fd, TIOCGSTAT, ...): %.100s", strerror(errno));
      return;
    }
#endif /* TIOCGSTAT */
  /* termio's ECHOE is really both LCRTBS and LCRTERA - so wire them
     together */
  if (tiolm & LCRTBS)
    tiolm |= LCRTERA;
#endif /* USING_SGTTY */

  /* Store input and output baud rates. */
  baud = speed_to_baud(cfgetospeed(&tio));
  packet_put_char(TTY_OP_OSPEED);
  packet_put_int(baud);
  baud = speed_to_baud(cfgetispeed(&tio));
  packet_put_char(TTY_OP_ISPEED);
  packet_put_int(baud);

  /* Store values of mode flags. */
#ifdef USING_TERMIOS
#define TTYCHAR(NAME, OP) \
  packet_put_char(OP); packet_put_char(tio.c_cc[NAME]);
#define TTYMODE(NAME, FIELD, OP) \
  packet_put_char(OP); packet_put_char((tio.FIELD & NAME) != 0);
#define SGTTYCHAR(NAME, OP)
#define SGTTYMODE(NAME, FIELD, OP)
#define SGTTYMODEN(NAME, FIELD, OP)
#endif /* USING_TERMIOS */

#ifdef USING_SGTTY
#define TTYCHAR(NAME, OP)
#define TTYMODE(NAME, FIELD, OP)
#define SGTTYCHAR(NAME, OP) \
  packet_put_char(OP); packet_put_char(NAME);
#define SGTTYMODE(NAME, FIELD, OP) \
  packet_put_char(OP); packet_put_char((FIELD & NAME) != 0);
#define SGTTYMODEN(NAME, FIELD, OP) \
  packet_put_char(OP); packet_put_char((FIELD & NAME) == 0);
#endif /* USING_SGTTY */

#include "ttymodes.h"

#undef TTYCHAR
#undef TTYMODE
#undef SGTTYCHAR
#undef SGTTYMODE
#undef SGTTYMODEN

  /* Mark end of mode data. */
  packet_put_char(TTY_OP_END);
}

/* Decodes terminal modes for the terminal referenced by fd in a portable
   manner from a packet being read. */

void tty_parse_modes(int fd)
{
#ifdef USING_TERMIOS
  struct termios tio;
#endif /* USING_TERMIOS */
#ifdef USING_SGTTY
  struct sgttyb tio;
  struct tchars tiotc;
  struct ltchars tioltc;
  int tiolm;
#ifdef TIOCGSTAT
  struct tstatus tiots;
#endif /* TIOCGSTAT */
#endif
  int opcode, baud;

  /* Get old attributes for the terminal.  We will modify these flags. 
     I am hoping that if there are any machine-specific modes, they will
     initially have reasonable values. */
#ifdef USING_TERMIOS
  if (tcgetattr(fd, &tio) < 0)
    return;
#endif /* USING_TERMIOS */
#ifdef USING_SGTTY
  if (ioctl(fd, TIOCGETP, &tio) < 0)
    return;
  if (ioctl(fd, TIOCGETC, &tiotc) < 0)
    return;
  if (ioctl(fd, TIOCLGET, &tiolm) < 0)
    return;
  if (ioctl(fd, TIOCGLTC, &tioltc) < 0)
    return;
#ifdef TIOCGSTAT
  if (ioctl(fd, TIOCGSTAT, &tiots) < 0)
    return;
#endif /* TIOCGSTAT */
#endif /* USING_SGTTY */

  for (;;)
    {
      switch (opcode = packet_get_char())
        {
        case TTY_OP_END:
          goto set;

        case TTY_OP_ISPEED:
          baud = packet_get_int();
          if (cfsetispeed(&tio, baud_to_speed(baud)) < 0)
            error("cfsetispeed failed for %d", baud);
          break;

        case TTY_OP_OSPEED:
          baud = packet_get_int();
          if (cfsetospeed(&tio, baud_to_speed(baud)) < 0)
            error("cfsetospeed failed for %d", baud);
          break;

#ifdef USING_TERMIOS
#define TTYCHAR(NAME, OP)                               \
        case OP:                                        \
          tio.c_cc[NAME] = packet_get_char();           \
          break;
#define TTYMODE(NAME, FIELD, OP)                        \
        case OP:                                        \
          if (packet_get_char())                        \
            tio.FIELD |= NAME;                          \
          else                                          \
            tio.FIELD &= ~NAME;                         \
          break;
#define SGTTYCHAR(NAME, OP)
#define SGTTYMODE(NAME, FIELD, OP)
#define SGTTYMODEN(NAME, FIELD, OP)
#endif /* USING_TERMIOS */

#ifdef USING_SGTTY
#define TTYCHAR(NAME, OP)
#define TTYMODE(NAME, FIELD, OP)
#define SGTTYCHAR(NAME, OP)                             \
        case OP:                                        \
          NAME = packet_get_char();                     \
          break;
#define SGTTYMODE(NAME, FIELD, OP)                      \
        case OP:                                        \
          if (packet_get_char())                        \
            FIELD |= NAME;                              \
          else                                          \
            FIELD &= ~NAME;                             \
          break;
#define SGTTYMODEN(NAME, FIELD, OP)                     \
        case OP:                                        \
          if (packet_get_char())                        \
            FIELD &= ~NAME;                             \
          else                                          \
            FIELD |= NAME;                              \
          break;
#endif /* USING_SGTTY */

#include "ttymodes.h"

#undef TTYCHAR
#undef TTYMODE
#undef SGTTYCHAR
#undef SGTTYMODE
#undef SGTTYMODEN

        default:
          debug("Ignoring unsupported tty mode opcode %d (0x%x)",
                opcode, opcode);
          /* Opcodes 0 to 127 are defined to have a one-byte argument. */
          if (opcode >= 0 && opcode < 128)
            {
              (void)packet_get_char();
              break;
            }
          else
            {
              /* Opcodes 128 to 159 are defined to have an integer argument. */
              if (opcode >= 128 && opcode < 160)
                {
                  (void)packet_get_int();
                  break;
                }
            }
          /* It is a truly undefined opcode (160 to 255).  We have no idea
             about its arguments.  So we must stop parsing.  Note that some
             data may be left in the packet; hopefully there is nothing more
             coming after the mode data. */
          log_msg("parse_tty_modes: unknown opcode %d", opcode);
          goto set;
        }
    }

 set:
  /* Set the new modes for the terminal. */
#ifdef USING_TERMIOS
  if (tcsetattr(fd, TCSANOW, &tio) < 0)
    log_msg("Setting tty modes failed: %.100s", strerror(errno));
#endif /* USING_TERMIOS */
#ifdef USING_SGTTY
  /* termio's ECHOE is really both LCRTBS and LCRTERA -
     so wire them together */
  if (tiolm & LCRTERA)
    tiolm |= LCRTBS;
  if (ioctl(fd, TIOCSETP, &tio) < 0
      || ioctl(fd, TIOCSETC, &tiotc) < 0
      || ioctl(fd, TIOCLSET, &tiolm) < 0
      || ioctl(fd, TIOCSLTC, &tioltc) < 0
#ifdef TIOCSSTAT
      || ioctl(fd, TIOCSSTAT, &tiots) < 0
#endif /* TIOCSSTAT */
     ) 
    log_msg("Setting tty modes failed: %.100s", strerror(errno));
#endif /* USING_SGTTY */
}
