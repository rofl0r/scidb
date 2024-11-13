// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifdef __unix__

# include <stdio.h>

# if defined(IO_NOT_MTSAFE)

#  define ferror	ferror_unlocked
#  define fflush	fflush_unlocked
#  define fgetc		fgetc_unlocked
#  define fgets		fgets_unlocked
#  define fileno	fileno_unlocked
#  define fflush	fflush_unlocked
#  define fputc		fputc_unlocked
#  define fputs		fputs_unlocked
#  define fread		fread_unlocked
#  define fwrite	fwrite_unlocked
#  define feof		feof_unlocked

# endif

#else

# include "m_stdio_internal.h"

#endif // __unix__

// vi:set ts=8 sw=8:
