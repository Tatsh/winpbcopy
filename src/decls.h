#ifndef WINPBCOPY_DECLS_H
#define WINPBCOPY_DECLS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef wchar_t WCHAR;
typedef WCHAR *LPTSTR;

typedef bool BOOL;
typedef uint32_t DWORD;
typedef void *HGLOBAL;

void *GlobalAlloc(unsigned int uFlags, size_t dwBytes);
void *GlobalLock(void *hMem);
void GlobalFree(void *hMem);
void GlobalUnlock(void *hMem);

int EmptyClipboard(void);
int OpenClipboard(void *hWndNewOwner);
void *GetClipboardData(unsigned int uFormat);
void *SetClipboardData(unsigned int uFormat, void *hMem);
void CloseClipboard(void);

int GetLastError(void);
void Sleep(unsigned int ms);

#endif // WINPBCOPY_DECLS_H
