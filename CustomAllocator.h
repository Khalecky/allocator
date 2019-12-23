#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include <vector>
#include <algorithm>
#include <typeinfo>

#include "Buffers.h"


template<typename T, size_t BufferSize=1>
struct CustomAllocator
{

    using value_type = T;
    //
    using Buffers = std::vector<Buffer<T> >;
    //

    CustomAllocator() {}
    ~CustomAllocator() = default;


    template<typename U>
    struct rebind {
        using other = CustomAllocator<U, BufferSize>;
    };


    T *allocate(std::size_t n)
    {

        T *p = nullptr;

        if (_buffers.empty())
            appendBuffer();

        size_t countToAlloc = n;

        while(true)
        {
            auto& buffer = _buffers.back();

            size_t cAvailible = buffer.capacity() - buffer.size();

            if (cAvailible > 0)
            {
                auto buf = buffer.push( cAvailible <= countToAlloc ? cAvailible: countToAlloc );
                if (!p) //will return only first allocated mem pointer
                    p = buf;

                if (countToAlloc <= cAvailible)
                    break;

                countToAlloc -= cAvailible;
            }

            appendBuffer();
        }

        return p;
    }

    void appendBuffer()
    {
        _buffers.emplace_back(BufferSize);
    }


    void deallocate(T *p, std::size_t n)
    {
        //std::cout << "--- DEALLOCATE ---" << " p = " << p << " n = " << n << std::endl;

        auto iter = std::find_if(_buffers.begin(), _buffers.end(), [&](const auto& buf) { return buf.contains(p); } );
        if (iter == _buffers.end())
            return;

        iter->pop(n);

        //removeEmptyBuffers
        iter = std::remove_if(_buffers.begin(), _buffers.end(), [&](const auto& buf) { return buf.empty();} );
        _buffers.erase(iter, _buffers.end());
    }


    template<typename U, typename ...Args>
    void construct(U *p, Args&& ...args) const
    {
        new(p) U(std::forward<Args>(args)...);
    }


    template<typename U>
    void destroy(U *p) const
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~U();
    }

private:
    Buffers _buffers;

};


#endif // ALLOCATOR_H_INCLUDED
