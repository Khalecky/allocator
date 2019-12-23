#ifndef BUFALLOC_H_INCLUDED
#define BUFALLOC_H_INCLUDED


template<typename T>
struct Buffer
{
    T* _ptr = nullptr;
    size_t _capacity = 0;
    size_t _count = 0;

    Buffer(size_t capacity): _capacity(capacity)
    {
        //std::cout << "--- BUFFER MALLOC ---" << _capacity <<std::endl;
        auto p = std::malloc( _capacity * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        _ptr = reinterpret_cast<T*>(p);
    }

    ~Buffer()
    {
        //std::cout << "--- BUFFER FREE --- " << _ptr << std::endl;
        std::free(_ptr);
    }

    Buffer(const Buffer& other): _capacity(other._capacity)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Buffer& operator=(Buffer&& other)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::swap(_ptr, other._ptr);
        _capacity = other._capacity;
        return *this;
    }

    Buffer(Buffer&& other) noexcept: _capacity(other._capacity)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::swap(_ptr, other._ptr);
    }

    T* endBlock() const
    {
        return _ptr + _capacity;
    }


    T* push(size_t nBlocks = 1)
    {
        T* p = nullptr;
        if (_count >= _capacity)
            return p;

        p = _ptr + _count;
        _count += nBlocks;
        return p;
    }

    void pop(size_t nBlocks)
    {
        if (nBlocks > _count)
            return;

        _count -= nBlocks;
    }


    auto capacity() const
    {
        return _capacity;
    }

    auto size() const
    {
        return _count;
    }


    bool contains(const T* block) const
    {
        return block >= _ptr &&  block < endBlock()   //check: block is in range
                && (block - _ptr) % sizeof(T) == 0;       //and blocks offset is integer
    }

    T* operator[](size_t i) const
    {
        return _ptr + i;
    }


    bool empty() const
    {
        return _count == 0;
    }


};

#endif // BUFALLOC_H_INCLUDED
