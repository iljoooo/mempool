#include <iostream>
#include <sys/time.h>

#include "mempool.h"

class Test {
  public:
    Test(int id) : m_id(id) {}
  private:
    int m_id;
    int data[15];
};

int main() {
    timeval start_tv, end_tv;

    // allocate 100 million class objects w/o memory pool
    gettimeofday(&start_tv, 0);
    for (int i = 0; i < 100000000; i++) {
        Test* t = new Test(i);
    }
    gettimeofday(&end_tv, 0);
    long elapsed = (end_tv.tv_sec - start_tv.tv_sec);
    printf("[RESULT] w/o memory pool, execution time: %ld sec\n", elapsed);

    // allocate 100 million class objects w/ memory pool
    MemoryPool mem_pool;
    gettimeofday(&start_tv, 0);
    for (int i = 0; i < 100000000; i++) {
        Test* t = new (mem_pool.get<MemoryPool>()) Test(i);
    }
    gettimeofday(&end_tv, 0);
    elapsed = (end_tv.tv_sec - start_tv.tv_sec);
    printf("[RESULT] w/ memory pool, execution time: %ld sec\n", elapsed);
}
