#ifndef WINPBCOPY_MACROS_H
#define WINPBCOPY_MACROS_H

#if !HAVE_NULLPTR_T
//! `nullptr` definition if not defined by `stddef.h`.
#define nullptr (void *)0
#endif

#if !HAVE_STDBOOL
#define bool BOOL
#define true TRUE
#define false FALSE
#else
#include <stdbool.h>
#endif

#endif // WINPBCOPY_MACROS_H
