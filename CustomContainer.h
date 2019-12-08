#ifndef CUSTOMCONTAINER_H_INCLUDED
#define CUSTOMCONTAINER_H_INCLUDED

#include <algorithm>

template< class T, class Allocator = std::allocator<T> >
class CustomContainer
{

    Allocator alloc;

    size_t capacity = 1;
    size_t size = 0;

    T** objects = nullptr;

    //
    class Iterator {

        CustomContainer &owner;
        size_t offset = 0;

    public:
        explicit Iterator(CustomContainer &owner_, size_t offset_ = 0): owner(owner_), offset(offset_) {}

        auto operator!=(const Iterator &iter) const {
            return owner.size > 0 && offset != iter.offset;
        }

        auto operator++() { return ++offset; }
        auto &operator*() const { return *(owner.objects[offset]); }
    };
    //

public:

    CustomContainer() {

        if (!objects)
            objects = new T* [capacity];
    }

    ~CustomContainer()
    {
        for (size_t i = 0; i < size; ++i) {
            alloc.destroy(objects[i]);
            alloc.deallocate(objects[i], 1);
        }

        delete[] objects;
    }

    template<typename ...Args>
    void push_emplace(Args&& ...args)
    {
        T* p = alloc.allocate(1);
        alloc.construct(p, std::forward<Args>(args)...);

        //append
        if (size + 1 > capacity) {

            capacity *= 2;

            T** newObjects = new T* [capacity];
            std::copy(objects, objects + size, newObjects);
            std::swap(objects, newObjects);
            delete [] newObjects;
            //
        }
        objects[size++] = p;
    }

    //
    auto begin() { return Iterator{*this}; }
    auto end() {
        return Iterator{*this, size}; //nullptr
    }


};

#endif // CUSTOMCONTAINER_H_INCLUDED
