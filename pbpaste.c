#include <stdio.h>

#include <windows.h>

int main(int argc, char **argv) {
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
    return GetLastError();
}
