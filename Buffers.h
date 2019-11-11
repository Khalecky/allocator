#ifndef BUFALLOC_H_INCLUDED
#define BUFALLOC_H_INCLUDED


template<typename T>
struct Buffer
{
    T* _ptr = nullptr;
    size_t _countItems;
    size_t _currOffset = 0;

    Buffer(size_t countItems): _countItems(countItems)
    {
        //std::cout << "--- BUFFER MALLOC ---" << std::endl;
        auto p = std::malloc( _countItems * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        _ptr = reinterpret_cast<T*>(p);
    }

    ~Buffer()
    {
        //std::cout << "--- BUFFER FREE --- " << _ptr << std::endl;
        std::free(_ptr);
    }

    Buffer(const Buffer& other): _countItems(other._countItems)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Buffer& operator=(Buffer&& other)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::swap(_ptr, other._ptr);
        _countItems = other._countItems;
        return *this;
    }

    Buffer(Buffer&& other) noexcept: _countItems(other._countItems)
    {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::swap(_ptr, other._ptr);
    }

    T* endBufItem() const
    {
        return _ptr + _countItems;
    }

    T* nextBufItem()
    {
        T* p = nullptr;
        if (_currOffset < _countItems)
            p = _ptr + _currOffset;

        ++_currOffset;
        return p;
    }

    bool contains(const T* bufItem) const
    {
        return bufItem >= _ptr &&  bufItem < endBufItem()   //check: item is in range
                && (bufItem - _ptr) % sizeof(T) == 0;       //and items offset is integer
    }

    bool empty() const
    {
        return _countItems == 0;
    }

    //before remove need to check contains!!!
    void removeLastItem()
    {
        --_currOffset;
        -- _countItems;
    }
};

template<typename T, size_t BufferSize>
class PoolBuffers
{
    using Buffers = std::vector<Buffer<T> >;
    using BufAllocIter = typename Buffers::iterator;

    Buffers _buffers;


    bool empty()
    {
        return _buffers.empty();
    }

    void insertBuffer()
    {
        _buffers.emplace_back(BufferSize);
    }


public:

    T* createNextBufItem()
    {
        if (_buffers.empty())
            insertBuffer();

        auto& buffer = _buffers.back();

        T* newItem = buffer.nextBufItem();
        if (!newItem)
        {
            insertBuffer();
            auto& buffer = _buffers.back();
            newItem = buffer.nextBufItem();
        }
        return newItem;
    }

    void removeBufItem(T* bufItem)
    {
        BufAllocIter iter = std::find_if(_buffers.begin(), _buffers.end(), [&](const auto& buf) { return buf.contains(bufItem); } );

        if (iter != _buffers.end())
        {
            iter->removeLastItem();
            if (iter->empty())
                _buffers.erase(iter);
        }
    }



};


#endif // BUFALLOC_H_INCLUDED
