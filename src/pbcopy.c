#if defined(_WIN32) && !defined(__WINE__)
#include <io.h>
#define isatty _isatty
#define read _read
#endif
#include <stdio.h>
#include <stdlib.h>
#if defined(__WINE__) || defined(__GNUC__)
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include "decls.h"
#endif
#include "macros.h"

static const size_t BUF_SIZE = 10 * 1024 * 1024;
static const size_t READ_SIZE = 512;

/**
 * @brief Copies data from standard input to the Windows clipboard.
 *
 * This function reads data from standard input, stores it in a global buffer, and sets the content
 * of the Windows clipboard to this data.
 *
 * This function only reads 10 MiB of data.
 *
 * @return int Returns 0 on success, or the last error code on failure.
 */
int pbcopy_main() {
    if (!isatty(fileno(stdin))) {
        // Read stdin to a global buffer
        size_t len = 0;
        int offset = 0;
        int r;
        HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, BUF_SIZE);
        char *g_buf = GlobalLock(h);
        while ((r = read(fileno(stdin), g_buf + offset, READ_SIZE)) > 0) {
            offset += r;
            len += r;
            if (len >= BUF_SIZE) {
                len = BUF_SIZE - 1;
                break;
            }
        }
        g_buf[len] = 0;
        GlobalUnlock(h);
        // Set the clipboard data
        int attempts = 0;
        const int max = 5;
        const DWORD sleep_time_ms = 100;
        bool ok = false;
        while (attempts < max) {
            if (OpenClipboard(nullptr)) {
                EmptyClipboard();
                if (!SetClipboardData(CF_TEXT, h)) {
                    break;
                }
                ok = true;
                break;
            }
            fprintf(stderr,
                    "OpenClipboard() failed, attempt %d of %d: 0x%lx.\n",
                    attempts + 1,
                    max,
                    GetLastError());
            attempts++;
            Sleep(sleep_time_ms);
        }
        if (ok) {
            CloseClipboard();
            GlobalFree(h);
            return 0;
        }
        GlobalFree(h);
    }
    DWORD last_error = GetLastError();
    fprintf(stderr, "Error occurred (or not a pipe): 0x%lx.\n", last_error);
    return last_error;
}
