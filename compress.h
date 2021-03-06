/*

compress.h

Author: Tatu Ylonen <ylo@ssh.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@ssh.fi>, Espoo, Finland
Copyright (c) 1995-1999 SSH Communications Security Oy, Espoo, Finland
                        All rights reserved

Created: Wed Oct 25 22:12:46 1995 ylo

Interface to packet compression for ssh.

*/

/*
 * $Id: compress.h,v 1.3 1999/11/17 17:04:42 tri Exp $
 * $Log: compress.h,v $
 * Revision 1.3  1999/11/17 17:04:42  tri
 * 	Fixed copyright notices.
 *
 * Revision 1.2  1997/03/26 07:11:32  kivinen
 *      Fixed prototypes.
 *
 * Revision 1.1.1.1  1996/02/18 21:38:11  ylo
 *      Imported ssh-1.2.13.
 *
 * $EndLog$
 */

#ifndef COMPRESS_H
#define COMPRESS_H

/* Initializes compression; level is compression level from 1 to 9 (as in
   gzip). */
void buffer_compress_init(int level);

/* Frees any data structures allocated by buffer_compress_init. */
void buffer_compress_uninit(void);

/* Compresses the contents of input_buffer into output_buffer.  All
   packets compressed using this function will form a single
   compressed data stream; however, data will be flushed at the end of
   every call so that each output_buffer can be decompressed
   independently (but in the appropriate order since they together
   form a single compression stream) by the receiver.  This appends
   the compressed data to the output buffer. */
void buffer_compress(Buffer *input_buffer, Buffer *output_buffer);

/* Uncompresses the contents of input_buffer into output_buffer.  All
   packets uncompressed using this function will form a single
   compressed data stream; however, data will be flushed at the end of
   every call so that each output_buffer.  This must be called for the
   same size units that the buffer_compress was called, and in the
   same order that buffers compressed with that.  This appends the
   uncompressed data to the output buffer. */
void buffer_uncompress(Buffer *input_buffer, Buffer *output_buffer);

#endif /* COMPRESS_H */
