#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include "decls.h"
#endif
#include "macros.h"

/**
 * @brief Prints the content of the Windows clipboard to standard output.
 *
 * This function retrieves the text data from the clipboard and prints it to standard output. If the
 * clipboard is empty or an error occurs, it returns the last error code.
 *
 * @return int Returns 0 on success, or the last error code on failure.
 */
int pbpaste_main() {
    if (OpenClipboard(nullptr)) {
        char *data = (char *)GetClipboardData(CF_TEXT);
        if (data) {
            printf("%s", data);
            if (data[strlen(data) - 1] != '\n') {
                printf("\n");
            }
            CloseClipboard();
            return 0;
        }
    }
    DWORD last_error = GetLastError();
    fprintf(stderr, "Error occurred: 0x%x.\n", (unsigned int)last_error);
    return last_error;
}
