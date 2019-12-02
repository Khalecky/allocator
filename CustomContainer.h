#ifndef CUSTOMCONTAINER_H_INCLUDED
#define CUSTOMCONTAINER_H_INCLUDED


template< class T, class Allocator = std::allocator<T> >
class CustomContainer
{
    Allocator alloc;
    size_t count = 0;   //counter of objects

    //
    class Iterator {

        Allocator *pAlloc = nullptr;
        size_t offset = 0;

    public:
        explicit Iterator(Allocator *alloc, size_t offset_ = 0): pAlloc(alloc), offset(offset_) {}

        auto operator!=(const Iterator &iter) const { return offset != iter.offset;}
        auto operator++() { return ++offset; }
        auto &operator*() const { return *(pAlloc->at(offset));}
    };
    //

public:

    CustomContainer() {}

    ~CustomContainer()
    {
        for (size_t i = 0; i < count; ++i) {
            alloc.destroy(alloc[i]);
            alloc.deallocate(alloc[i], 1);
        }

    }

    template<typename ...Args>
    void push_emplace(Args&& ...args)
    {
        T* p = alloc.allocate(1);
        alloc.construct(p, std::forward<Args>(args)...);
        ++count;
    }

    //
    auto begin() { return Iterator{&alloc}; }
    auto end() {
        return Iterator{&alloc, count}; //nullptr
    }


};

#endif // CUSTOMCONTAINER_H_INCLUDED
