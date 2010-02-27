// Specific definitions for AIX  -*- C++ -*-

// Copyright (C) 2000, 2002 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#ifndef _GLIBCXX_OS_DEFINES
#define _GLIBCXX_OS_DEFINES

// System-specific #define, typedefs, corrections, etc, go here.  This
// file will come before all others.

#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

// off64_t
#ifndef _LARGE_FILE_API
#define _LARGE_FILE_API
#endif

// atomic types
#ifndef _ALL_SOURCE
#define _ALL_SOURCE
#endif

#define __off_t off_t
#define __off64_t off64_t
#define __ssize_t ssize_t

#undef _G_USING_THUNKS
#define _G_USING_THUNKS 0

#endif
