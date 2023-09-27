#if defined(WIN32) && !defined(__WINE__)
#include <io.h>
#define isatty _isatty
#endif
#include <stdio.h>
#ifdef __WINE__
#include <unistd.h>
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
#include <windows.h>

static const int BUF_SIZE = 10 * 1024 * 1024;
static const int READ_SIZE = 512;

int main(int argc, char **argv) {
    if (!isatty(0)) {
        // Read stdin to a global buffer
        ssize_t len = 0;
        ssize_t offset = 0;
        ssize_t r;
        HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, BUF_SIZE * sizeof(WCHAR));
        LPTSTR g_buf = GlobalLock(h);
        while ((r = read(0, g_buf + offset, READ_SIZE * sizeof(WCHAR))) > 0) {
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
        size_t attempts = 0;
        const size_t max = 5;
        const size_t sleep_time_ms = 100;
        BOOL ok = FALSE;
        while (attempts < max) {
            if (OpenClipboard(nullptr)) {
                EmptyClipboard();
                if (!SetClipboardData(CF_TEXT, h)) {
                    break;
                }
                ok = TRUE;
                break;
            }
            fprintf(stderr,
                    "OpenClipboard() failed, attempt %lu of %lu: %d\n",
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
    fprintf(stderr, "Error occurred (or not a pipe): %d\n", last_error);
    return last_error;
}
