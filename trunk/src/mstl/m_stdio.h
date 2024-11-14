// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifdef __unix__
# include <stdio.h>
#else
# include "m_stdio_internal.h"
#endif // __unix__


#if defined(IO_NO_THREADS)

# ifdef getc
#  undef getc
# endif
# ifdef getchar
#  undef getchar
# endif
# ifdef putc
#  undef putc
# endif
# ifdef putchar
#  undef putchar
# endif

# define feof		feof_unlocked
# define ferror	ferror_unlocked
# define fflush	fflush_unlocked
# define fgetc		fgetc_unlocked
# define fgets		fgets_unlocked
# define fileno	fileno_unlocked
# define fputc		fputc_unlocked
# define fputs		fputs_unlocked
# define fread		fread_unlocked
# define fwrite	fwrite_unlocked
# define getc		getc_unlocked
# define getchar	getchar_unlocked
# define putc		putc_unlocked
# define putchar	putchar_unlocked

#endif

// vi:set ts=3 sw=3:
