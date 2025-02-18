#include "minunit.h"
#include <lcthw/ring_buffer.h>
#include <assert.h>

static RingBuffer* buffer = NULL;
char *tests[] = {"test1 data", "test2 data", "test3 data"};
#define NUM_TESTS 3

char *test_create()
{
    buffer = RingBuffer_create(100);
    mu_assert(buffer != NULL, "Failed to create queue.");

    return NULL;
}

char *test_destroy()
{
    mu_assert(buffer != NULL, "Failed to make queue #2");
    RingBuffer_destroy(buffer);

    return NULL;
}

char *test_read_write()
{
    int i = 0;
    for (i = 0; i < NUM_TESTS; i++)
    {
        RingBuffer_write(buffer, tests[i],10);
        bstring temp = bfromcstr(tests[i]);
        bstring temp2 = RingBuffer_gets(buffer, 10);
        mu_assert(bstricmp(temp2, temp) == BSTR_OK, "Wrong next value.");
        bdestroy(temp);
        bdestroy(temp2);
    }
    for (i = 0; i < NUM_TESTS; i++)
    {
        RingBuffer_write(buffer, tests[i],10);
        char data_in_buffer[11] = {0};
        RingBuffer_read(buffer, data_in_buffer, 10);
        bstring temp = bfromcstr(tests[i]);
        bstring temp2 = bfromcstr(data_in_buffer);
        mu_assert(bstricmp(temp2, temp) == BSTR_OK, "Wrong next value.");
        bdestroy(temp);
        bdestroy(temp2);
    }
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_read_write);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);