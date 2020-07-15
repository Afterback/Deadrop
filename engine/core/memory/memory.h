#pragma once
#include <memory>
#include <vector>

namespace deadrop
{
    // alias to std::unique_ptr
    template<typename T>
    using uptr = std::unique_ptr<T>;

    // alias to std::shared_ptr
    template<typename T>
    using sptr = std::shared_ptr<T>;

    namespace memory
    {
        // this is a generic struct that is used to pass
        // blocks of memory to other functions
        struct MemoryBlock
        {
            size_t size;
            void* ptr;
        };

        // a function to clear the content of the vector and clears its allocated memory
        template<typename T>
        inline void deallocate(std::vector<T>& vec)
        {
            // call the destructor of all the items, but does not deallocate the memory allocated by the vector
            vec.clear(); 
            // create an empty vector and swap it with our vector which causes the vector to clear its memory
            std::vector<T>(vec).swap(vec);
        }
    }
}

