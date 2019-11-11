#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include <vector>
#include <algorithm>
#include <typeinfo>

#include "Buffers.h"




template<typename T, size_t ReserveStep=1>
struct CustomAllocator
{

    using value_type = T;

    CustomAllocator() {}


    template<typename U>
    struct rebind {
        using other = CustomAllocator<U, ReserveStep>;
    };


    T *allocate([[gnu::unused]] std::size_t n)
    {
        auto p = _buffers.createNextBufItem();
        //std::cout << "--- ALLOCATE ---" << p << std::endl;
        return p;
    }

    void deallocate(T *p, [[gnu::unused]] std::size_t n)
    {
        //std::cout << "--- DEALLOCATE ---" << " p = " << p << std::endl;
        _buffers.removeBufItem(p);
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

    PoolBuffers<T, ReserveStep> _buffers;
};


#endif // ALLOCATOR_H_INCLUDED
