#pragma once

namespace deadrop
{
    namespace memory
    {
        // provides an easy way to create a memory view of a specific type,
        // only valid while the memory still exists
        template<class T>
        class ReadOnlyView
        {
        public:
            // aliases
            using size_type = std::size_t;
            using const_pointer = const T*;
            using const_reference = const T&;

            // delete default constructor
            ReadOnlyView() = delete;

            // constructor
            explicit ReadOnlyView(const_pointer ptr, const size_type byte_size) :
                m_ptr(ptr), m_size(byte_size) {}

            // return the byte size
            constexpr size_type size_bytes() const { return m_size; }

            // provides an index-based read-only access to the pointed memory
            constexpr const_reference operator [](size_type idx)
            {
                return m_ptr[idx];
            }

            // returns whether ptr points to a non-nullptr memory address, and checks if the size is non-zero
            constexpr bool is_valid() const
            {
                return m_ptr != nullptr && m_size != 0;
            }

        private:
            // stores a pointer
            const_pointer m_ptr = nullptr;
            // stores the byte size
            size_type m_size = 0;
        };
    }
}