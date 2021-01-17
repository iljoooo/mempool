#include <iostream>
#include <sys/time.h>

#include "mempool.h"

class Test {
  public:
    Test(long id, Test* next) {
        m_id = id;
        m_next = next;
    }

    Test* getNext() {
        return m_next;
    }

  private:
    long m_id;
    Test* m_next;
    long dummy[6];
};

double get_current_time() {
    timeval tv;
    gettimeofday(&tv, 0);
    return (double)tv.tv_sec * 1000000 + tv.tv_usec;
}

int main() {
    static const int num_objects = 100000000;  // 100 million class objects
    Test* ptr_new = NULL;
    Test* ptr_pool = NULL;
    double start, elapsed;
    MemoryPool mem_pool;

    printf("size of objects: %ld bytes\n", sizeof(Test));
    printf("the number of objects: %d\n", num_objects);
    printf("=================================================\n");

    /**
     * memory allocation test
     */
    printf("Memory Allocation Test\n");
    // new opreator
    start = get_current_time();
    for (int i = 0; i < num_objects; i++) {
        Test* t = new Test(i, ptr_new);
        ptr_new = t;
    }
    elapsed = get_current_time() - start;
    printf("  [RESULT] allocate memory using new operator: %.3lf sec\n", elapsed/1000000);

    // memory pool
    start = get_current_time();
    for (int i = 0; i < num_objects; i++) {
        Test* t = new (mem_pool.get<Test>()) Test(i, ptr_pool);
        ptr_pool = t;
    }
    elapsed = get_current_time() - start;
    printf("  [RESULT] allocate memory using memory pool: %.3lf sec\n", elapsed/1000000);
    printf("-------------------------------------------------\n");

    /**
     * memory traverse test
     */
    printf("Memory Traverse Test\n");
    // new opreator
    start = get_current_time();
    while (ptr_new != NULL) {
        ptr_new = ptr_new->getNext();
    }
    elapsed = get_current_time() - start;
    printf("  [RESULT] traverse objects those are allocated using new operator: %.3lf sec\n", elapsed/1000000);

    // memory pool
    start = get_current_time();
    while (ptr_pool != NULL) {
        ptr_pool = ptr_pool->getNext();
    }
    elapsed = get_current_time() - start;
    printf("  [RESULT] traverse objects those are allocated using memory pool: %.3lf sec\n", elapsed/1000000);
    printf("-------------------------------------------------\n");

    return 0;
}
