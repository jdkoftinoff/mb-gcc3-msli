/*********************************************************************************-*-C-*-*
 * 
 * va-microblaze.h
 * 
 * Variable argument macros, used by MicroBlaze GNU Compiler
 * 
 * [03/29/2004] : Changed the comments from C++ style to C-style. C++ sytle comments cause issues with -ansi switch
 * 
 * $Header: /devl/xcs/repo/env/Jobs/MDT/sw/ThirdParty/gnu/src/newlib-3.4/libgloss/include/va-microblaze.h,v 1.1 2005/03/18 20:00:31 vasanth Exp $
 * 
 *********************************************************************************/

/*
#ifndef __VA_MICROBLAZE_H__
#define __VA_MICROBLAZE_H__
*/

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef void *__gnuc_va_list;
#endif

/* If this is for internal libc use, don't define anything but
   __gnuc_va_list.  */
#if defined (_STDARG_H) || defined (_VARARGS_H)

#ifdef _STDARG_H
#define va_start(AP, LASTARG)                                           \
  (AP = ((__gnuc_va_list) __builtin_next_arg (LASTARG)))
#else
#define __va_ellipsis ...
#define va_alist __builtin_va_alist
#define va_dcl int __builtin_va_alist; __va_ellipsis
#define va_start(AP) AP=(char *) &__builtin_va_alist
#endif

/* Now stuff common to both varargs & stdarg implementations.  */

/* Amount of space required in an argument list for an arg of type TYPE.
   TYPE may alternatively be an expression whose type is used.  */
#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#undef va_end
void va_end (__gnuc_va_list);		/* Defined in libgcc.a */ /* Need to check this out */
#define va_end(AP)	((void)0)

/* MicroBlaze definition for alignment for long long and doubles */
#define __va_align_big(AP,TYPE) (((__alignof__(TYPE) > 4) && (!((((__PTRDIFF_TYPE__)AP) & 7)))) ? 4 : 0)

/* We cast to void * and then to TYPE * because this avoids
   a warning about increasing the alignment requirement.  */

/* this definition is only for big endian. For small endian definition, take a look at stdarg.h */

#define va_arg(AP, TYPE)						\
 (AP = (__gnuc_va_list) ((char *) (AP) + __va_rounded_size (TYPE) + __va_align_big(AP,TYPE)), \
  *((TYPE *) (void *) ((char *) (AP)					\
		       - ((sizeof (TYPE) < __va_rounded_size (char)	\
			   ? sizeof (TYPE) : __va_rounded_size (TYPE))))))

/* #define __va_copy(dest, src) (dest) = (src) */

#endif /* defined(_STADARG_H) || defined (_VARARGS_H) */
/* #endif *//* #define __VA_MICROBLAZE_H__ */
