#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cmocka.h>

extern int pbcopy_main();
extern int pbpaste_main();

int __wrap_isatty(int fd) {
    (void)fd;
    return mock_type(int);
}

void *__wrap_GlobalAlloc(unsigned int uFlags, size_t dwBytes) {
    check_expected(uFlags);
    check_expected(dwBytes);
    return mock_type(void *);
}

static const size_t BUF_SIZE = 10 * 1024 * 1024;
wchar_t *_shared_mem = nullptr;

wchar_t *__wrap_GlobalLock(void *hMem) {
    check_expected_ptr(hMem);
    return mock_type(wchar_t *);
}

void __wrap_GlobalUnlock(void *hMem) {
    check_expected_ptr(hMem);
}

void __wrap_GlobalFree(void *hMem) {
    check_expected_ptr(hMem);
}

int __wrap_OpenClipboard(void *hWndNewOwner) {
    (void)hWndNewOwner;
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

void __wrap_Sleep(unsigned int ms) {
    (void)ms;
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

static void test_pbcopy_success(void **state) {
    (void)state;

    // Mock isatty to return 0 (not a tty)
    will_return(__wrap_isatty, 0);

    // Mock read to simulate reading "hello\n" from stdin
    _shared_mem = malloc(BUF_SIZE * sizeof(wchar_t));
    memset(_shared_mem, 0, BUF_SIZE * sizeof(wchar_t));
    _shared_mem[0] = L'h';
    _shared_mem[1] = L'e';
    _shared_mem[2] = L'l';
    _shared_mem[3] = L'l';
    _shared_mem[4] = L'o';
    _shared_mem[5] = L'\n';
    expect_value(__wrap_read, fd, 0);
    expect_any(__wrap_read, buf);
    expect_value(__wrap_read, count, 0x800);
    will_return(__wrap_read, 0x2800000);

    // Mock GlobalAlloc, GlobalLock, and GlobalUnlock
    expect_value(__wrap_GlobalAlloc, uFlags, 0x42);
    expect_value(__wrap_GlobalAlloc, dwBytes, 0x2800000);
    will_return(__wrap_GlobalAlloc, (void *)0x1234);

    expect_any(__wrap_GlobalLock, hMem);
    will_return(__wrap_GlobalLock, _shared_mem);

    expect_any(__wrap_GlobalUnlock, hMem);

    will_return(__wrap_OpenClipboard, 1);

    will_return(__wrap_EmptyClipboard, 1);

    expect_value(__wrap_SetClipboardData, uFormat, 1);
    expect_any(__wrap_SetClipboardData, hMem);
    will_return(__wrap_SetClipboardData, (void *)0x1234);

    will_return(__wrap_CloseClipboard, 1);

    expect_any(__wrap_GlobalFree, hMem);

    int ret = pbcopy_main();
    assert_int_equal(ret, 0);

    free(_shared_mem);
    _shared_mem = nullptr;
}

static void test_pbcopy_clipboard_fail(void **state) {
    (void)state;

    will_return(__wrap_isatty, 0);

    _shared_mem = malloc(BUF_SIZE * sizeof(wchar_t));
    memset(_shared_mem, 0, BUF_SIZE * sizeof(wchar_t));
    _shared_mem[0] = L'f';
    _shared_mem[1] = L'a';
    _shared_mem[2] = L'i';
    _shared_mem[3] = L'l';
    _shared_mem[5] = L'\n';
    expect_value(__wrap_read, fd, 0);
    expect_any(__wrap_read, buf);
    expect_value(__wrap_read, count, 0x800);
    will_return(__wrap_read, 0x2800000);

    expect_value(__wrap_GlobalAlloc, uFlags, 0x42);
    expect_value(__wrap_GlobalAlloc, dwBytes, 0x2800000);
    will_return(__wrap_GlobalAlloc, (void *)0x1234);

    expect_any(__wrap_GlobalLock, hMem);
    will_return(__wrap_GlobalLock, _shared_mem);

    expect_any(__wrap_GlobalUnlock, hMem);

    will_return_always(__wrap_OpenClipboard, 0);

    will_return_always(__wrap_GetLastError, 1234);

    expect_any(__wrap_GlobalFree, hMem);

    int ret = pbcopy_main();
    assert_int_not_equal(ret, 0);

    free(_shared_mem);
    _shared_mem = nullptr;
}

static void test_pbcopy_set_clipboard_data_fail(void **state) {
    (void)state;

    // Mock isatty to return 0 (not a tty)
    will_return(__wrap_isatty, 0);

    // Mock read to simulate reading "hello\n" from stdin
    _shared_mem = malloc(BUF_SIZE * sizeof(wchar_t));
    memset(_shared_mem, 0, BUF_SIZE * sizeof(wchar_t));
    _shared_mem[0] = L'h';
    _shared_mem[1] = L'e';
    _shared_mem[2] = L'l';
    _shared_mem[3] = L'l';
    _shared_mem[4] = L'o';
    _shared_mem[5] = L'\n';
    expect_value(__wrap_read, fd, 0);
    expect_any(__wrap_read, buf);
    expect_value(__wrap_read, count, 0x800);
    will_return(__wrap_read, 0x2800000);

    // Mock GlobalAlloc, GlobalLock, and GlobalUnlock
    expect_value(__wrap_GlobalAlloc, uFlags, 0x42);
    expect_value(__wrap_GlobalAlloc, dwBytes, 0x2800000);
    will_return(__wrap_GlobalAlloc, (void *)0x1234);

    expect_any(__wrap_GlobalLock, hMem);
    will_return(__wrap_GlobalLock, _shared_mem);

    expect_any(__wrap_GlobalUnlock, hMem);

    will_return(__wrap_OpenClipboard, 1);

    will_return(__wrap_EmptyClipboard, 1);

    expect_value(__wrap_SetClipboardData, uFormat, 1);
    expect_any(__wrap_SetClipboardData, hMem);
    will_return(__wrap_SetClipboardData, (void *)0);

    will_return_always(__wrap_GetLastError, 1234);

    expect_any(__wrap_GlobalFree, hMem);

    int ret = pbcopy_main();
    assert_int_not_equal(ret, 0);

    free(_shared_mem);
    _shared_mem = nullptr;
}

static char _test_clipboard_data[32];

static void test_pbpaste_success(void **state) {
    (void)state;

    will_return(__wrap_OpenClipboard, 1);

    _test_clipboard_data[0] = 'H';
    _test_clipboard_data[1] = 'i';
    _test_clipboard_data[3] = '\0';

    expect_value(__wrap_GetClipboardData, uFormat, 1);
    will_return(__wrap_GetClipboardData, _test_clipboard_data);

    will_return(__wrap_CloseClipboard, 1);

    int ret = pbpaste_main();
    assert_int_equal(ret, 0);
}

static void test_pbpaste_clipboard_fail(void **state) {
    (void)state;

    will_return(__wrap_OpenClipboard, 0);

    will_return(__wrap_GetLastError, 5678);

    int ret = pbpaste_main();
    assert_int_not_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pbcopy_success),
        cmocka_unit_test(test_pbcopy_clipboard_fail),
        cmocka_unit_test(test_pbcopy_set_clipboard_data_fail),
        cmocka_unit_test(test_pbpaste_success),
        cmocka_unit_test(test_pbpaste_clipboard_fail),
    };
    return cmocka_run_group_tests(tests, nullptr, nullptr);
}
