#include <vector>

class MemoryPool
{
  public:
    static const size_t SEGMENT_SIZE = 64 * 1024 * 1024;  // 64 MB
    MemoryPool() {
        m_head = new Block();
    }

    class Block
    {
      public:
        Block() : m_next(NULL), m_allocated(0) {}

        void* allocate(size_t size) {
            void* pointer = (void*)(m_data + m_allocated);
            m_allocated += size;

            return pointer;
        }

        size_t remains() {
            return SIZE - m_allocated;
        }

        Block* getNext() {
            return m_next;
        }

        void setNext(Block* next) {
            m_next = next;
        }

      private:
        static const size_t SIZE = SEGMENT_SIZE
                                    - sizeof(Block*)       // m_next
                                    - sizeof(unsigned int);  // m_allocated

        Block*       m_next;
        unsigned int m_allocated;
        char         m_data[SIZE];
    };

    template <typename T>
    void* allocate() {
        size_t size = sizeof(T);
        Block* block = m_head;
        while (true) {
            if (block->remains() >= size) {
                // allocate from current segment
                return block->allocate(size);
            }

            if (block->getNext() != NULL) {
                // move to  next segment
                block = block->getNext();
            } else {
                // create new segment
                Block* new_block = new Block();
                new_block->setNext(m_head);
                m_head = new_block;

                return new_block->allocate(size);
            }
        }
    }

  private:
    Block* m_head;
};
