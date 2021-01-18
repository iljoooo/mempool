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

        Block*     m_next;
        unsigned int m_allocated;
        char         m_data[SIZE];
    };

    template <typename T>
    void* get() {
        return allocate(sizeof(T));
    }

  private:
    void* allocate(size_t size) {
        Block* sgmt = m_head;
        while (true) {
            if (sgmt->remains() >= size) {
                // allocate from current segment
                return sgmt->allocate(size);
            }

            if (sgmt->getNext() != NULL) {
                // move to  next segment
                sgmt = sgmt->getNext();
            } else {
                // create new segment
                Block* new_sgmt = new Block();
                new_sgmt->setNext(m_head);
                m_head = new_sgmt;

                return new_sgmt->allocate(size);
            }
        }
    }

    Block* m_head;
};
