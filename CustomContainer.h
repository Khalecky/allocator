#ifndef CUSTOMCONTAINER_H_INCLUDED
#define CUSTOMCONTAINER_H_INCLUDED


template< class T, class Allocator = std::allocator<T> >
class CustomContainer
{
    Allocator alloc;
    std::vector<T*> objects;

public:

    CustomContainer() {}

    ~CustomContainer()
    {
        for(T* p: objects)
            alloc.destroy(p);
    }

    template<typename ...Args>
    void push_emplace(Args&& ...args)
    {
        T* p = alloc.allocate(1);
        alloc.construct(p, std::forward<Args>(args)...);
        objects.push_back(p);

        //p->print();
    }

    auto begin() const
    {
        return objects.begin();
    }

    auto end() const
    {
        return objects.end();
    }








};



#endif // CUSTOMCONTAINER_H_INCLUDED
