#include "../../src/alloc.h"
#include <gtest/gtest.h>

namespace tests {

static void panic(const char *message)
{
    throw std::runtime_error(message);
}

TEST(AllocTestSuite, testStoreAndRetrieve)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 10, 10, panic);

    int *mem = (int *) dumb_malloc(&alloc, sizeof(int));
    *mem = 10;
    ASSERT_EQ(*mem, 10);
}

TEST(AllocTestSuite, testStoreAndRetrieveAfterFree)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 10, 10, panic);

    int *mem0 = (int *) dumb_malloc(&alloc, sizeof(int));
    int *mem1 = (int *) dumb_malloc(&alloc, sizeof(int));
    dumb_free(&alloc, mem0);
    *mem1 = 10;
    ASSERT_EQ(*mem1, 10);
}

TEST(AllocTestSuite, testStoreAndDeleteManyTimes)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 10, 10000, panic);
    for (int i = 0; i < 10000; i++) {
        int *mem = (int *) dumb_malloc(&alloc, sizeof(int));
        dumb_free(&alloc, (char *) mem);
    }
}

TEST(AllocTestSuite, testFreeFunction)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 1024 * 1024, 10, panic);

    for (int i = 0; i < 1000000; i++) {
        int *mem = (int *) dumb_malloc(&alloc, sizeof(int));
        dumb_free(&alloc, (char *) mem);
    }
}

TEST(AllocTestSuite, testManyRetrieves)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 1024 * 1024, 10000, panic);

    int arr[10000];
    int **arr2 = new int *[10000];
    int tmp;
    for (int i = 0; i < 10000; i++) {
        tmp = rand() % 5000;
        arr[i] = tmp;
        arr2[i] = (int *) dumb_malloc(&alloc, sizeof(int));
        *(arr2[i]) = tmp;
    }

    for (int i = 0; i < 10000; i++) {
        ASSERT_EQ(*(arr2[i]), arr[i]);
    }
}

TEST(AllocTestSuite, testMyMallocSpeed)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 1024 * 1024, 10, panic);

    for (int i = 0; i < 1000000; i++) {
        char *arr0 = (char *) dumb_malloc(&alloc, rand() % 500);
        dumb_free(&alloc, arr0);
        char *arr1 = (char *) dumb_malloc(&alloc, rand() % 10);
        dumb_free(&alloc, arr1);
        char *arr2 = (char *) dumb_malloc(&alloc, 500 + rand() % 500);
        dumb_free(&alloc, arr2);
    }
    char *arr3 = (char *) dumb_malloc(&alloc, 500000);
    dumb_free(&alloc, arr3);
}

TEST(AllocTestSuite, testMany)
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 1024 * 1024 * 3, 10002, panic);
    int **arr = new int *[3000000];
    for (int i = 0; i < 10000; i++) {
        arr[i] = (int *) dumb_malloc(&alloc, sizeof(int));
    }
    for (int i = 0; i < 10000; i++) {
        dumb_free(&alloc, (char *) arr[i]);
    }

    dumb_malloc(&alloc, 50000);
}

} // namespace tests
