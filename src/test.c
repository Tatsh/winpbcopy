#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include <cmocka.h>

int __wrap_isatty(int fd) {
    return mock_type(int);
}

int __wrap_fileno(FILE *stream) {
    check_expected_ptr(stream);
    return mock_type(int);
}

void *__wrap_GlobalAlloc(unsigned int uFlags, size_t dwBytes) {
    check_expected(uFlags);
    check_expected(dwBytes);
    return mock_type(void *);
}

void *__wrap_GlobalLock(void *hMem) {
    check_expected_ptr(hMem);
    return mock_type(void *);
}

void __wrap_GlobalUnlock(void *hMem) {
    check_expected_ptr(hMem);
}

void __wrap_GlobalFree(void *hMem) {
    check_expected_ptr(hMem);
}

int __wrap_OpenClipboard(void *hWndNewOwner) {
    check_expected_ptr(hWndNewOwner);
    return mock_type(int);
}

ssize_t __wrap_read(int fd, void *buf, size_t count) {
    check_expected(fd);
    check_expected_ptr(buf);
    check_expected(count);
    return mock_type(ssize_t);
}

int __wrap_EmptyClipboard(void) {
    return mock_type(int);
}

void *__wrap_SetClipboardData(unsigned int uFormat, void *hMem) {
    check_expected(uFormat);
    check_expected_ptr(hMem);
    return mock_type(void *);
}

int __wrap_fprintf(FILE *stream, const char *format, ...) {
    check_expected_ptr(stream);
    check_expected_ptr(format);

    va_list args;
    va_start(args, format);
    int ret = mock_type(int);
    va_end(args);

    return ret;
}

void __wrap_Sleep(unsigned int ms) {
    check_expected(ms);
}

unsigned long __wrap_GetLastError(void) {
    return mock_type(unsigned long);
}

int __wrap_CloseClipboard(void) {
    return mock_type(int);
}

void *__wrap_GetClipboardData(unsigned int uFormat) {
    check_expected(uFormat);
    return mock_type(void *);
}

int __wrap_printf(const char *format, ...) {
    check_expected_ptr(format);

    va_list args;
    va_start(args, format);
    int ret = mock_type(int);
    va_end(args);

    return ret;
}

static void test_main_success(void **state) {
    (void)state;
    char *argv[] = {"pbcopy", NULL};
    int argc = 1;

    // Mock isatty to return 0 (not a tty)
    will_return(__wrap_isatty, 0);

    // Mock fileno to return 0 (stdin)
    expect_any(__wrap_fileno, stream);
    will_return(__wrap_fileno, 0);

    // Mock read to simulate reading "hello\n" from stdin
    char input[] = "hello\n";
    expect_value(__wrap_read, fd, 0);
    expect_any(__wrap_read, buf);
    expect_value(__wrap_read, count, sizeof(input) - 1);
    will_return(__wrap_read, sizeof(input) - 1);

    // Mock GlobalAlloc, GlobalLock, and GlobalUnlock
    expect_value(__wrap_GlobalAlloc, uFlags, 0);
    expect_value(__wrap_GlobalAlloc, dwBytes, sizeof(input));
    will_return(__wrap_GlobalAlloc, (void *)0x1234);

    expect_any(__wrap_GlobalLock, hMem);
    will_return(__wrap_GlobalLock, input);

    expect_any(__wrap_GlobalUnlock, hMem);

    // Mock OpenClipboard, EmptyClipboard, SetClipboardData, CloseClipboard
    expect_any(__wrap_OpenClipboard, hWndNewOwner);
    will_return(__wrap_OpenClipboard, 1);

    will_return(__wrap_EmptyClipboard, 1);

    expect_value(__wrap_SetClipboardData, uFormat, 1);
    expect_any(__wrap_SetClipboardData, hMem);
    will_return(__wrap_SetClipboardData, (void *)0x1234);

    will_return(__wrap_CloseClipboard, 1);

    // Mock GlobalFree
    expect_any(__wrap_GlobalFree, hMem);

    // Mock printf for success message
    expect_any(__wrap_printf, format);
    will_return(__wrap_printf, 0);

    int ret = main(argc, argv);
    assert_int_equal(ret, 0);
}

static void test_main_clipboard_fail(void **state) {
    (void)state;
    char *argv[] = {"pbcopy", NULL};
    int argc = 1;

    will_return(__wrap_isatty, 0);
    expect_any(__wrap_fileno, stream);
    will_return(__wrap_fileno, 0);

    char input[] = "fail\n";
    expect_value(__wrap_read, fd, 0);
    expect_any(__wrap_read, buf);
    expect_value(__wrap_read, count, sizeof(input) - 1);
    will_return(__wrap_read, sizeof(input) - 1);

    expect_value(__wrap_GlobalAlloc, uFlags, 0);
    expect_value(__wrap_GlobalAlloc, dwBytes, sizeof(input));
    will_return(__wrap_GlobalAlloc, (void *)0x1234);

    expect_any(__wrap_GlobalLock, hMem);
    will_return(__wrap_GlobalLock, input);

    expect_any(__wrap_GlobalUnlock, hMem);

    expect_any(__wrap_OpenClipboard, hWndNewOwner);
    will_return(__wrap_OpenClipboard, 0); // Simulate failure

    // Mock fprintf for error message
    expect_any(__wrap_fprintf, stream);
    expect_any(__wrap_fprintf, format);
    will_return(__wrap_fprintf, 0);

    // Mock GlobalFree
    expect_any(__wrap_GlobalFree, hMem);

    int ret = main(argc, argv);
    assert_int_not_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_main_success),
        cmocka_unit_test(test_main_clipboard_fail),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
