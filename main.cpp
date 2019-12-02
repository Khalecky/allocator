#include <iostream>
#include <vector>
#include <map>

#include "CustomAllocator.h"
#include "CustomContainer.h"

int getFact(int n)
{
    return n == 0 ? 1: n * getFact(n-1);
}

int getFib(int n)
{
    return n < 2 ? n: getFib(n-1) + getFib(n-2);
}

struct Hard
{
    int fa;
    int fi;
    Hard(int fa, int fi) : fa(fa), fi(fi) {}

    Hard(const Hard& inst): fa(inst.fa), fi(inst.fi) {}

    Hard(Hard&& inst) /*noexcept*/: fa(inst.fa), fi(inst.fi) {}

    ~Hard() = default;
};


int main()
{

    {
        auto m = std::map<int, Hard>{};
        for (int i = 0; i < 10; ++i)
        {
            auto fa = getFact(i);
            auto fi = getFib(i);
            m.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(fa, fi));
        }
    }

    //

    {
        auto m = std::map<int, Hard,std::less<int>,  CustomAllocator<std::pair<const int, Hard>, 10> >{};
        for (int i = 0; i < 10; ++i)
        {
            auto fa = getFact(i);
            auto fi = getFib(i);
            m.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(fa, fi));
        }
        //print map
        for (const auto& pair: m)
        {
            const auto& i = pair.first;
            const Hard& hard = pair.second;
            std::cout << i << ' ' << hard.fa << ' ' << hard.fi << std::endl;
        }

        //inducing expansion
        auto fa = getFact(10);
        auto fi = getFib(10);
        m.emplace(std::piecewise_construct, std::forward_as_tuple(10), std::forward_as_tuple(fa, fi));
        //
    }
    std::cout << std::endl;

    //

    auto hards = CustomContainer<Hard, CustomAllocator<Hard, 10> >{};
    for (int i = 0; i < 10; ++i)
    {
        auto fa = getFact(i);
        auto fi = getFib(i);
        hards.push_emplace(fa, fi);
    }


    int i = 0;
    for(const auto &hard: hards) {
        std::cout << (i++) << ' ' << hard.fa << ' ' << hard.fi << std::endl;
    }


    //inducing expansion
    auto fa = getFact(10);
    auto fi = getFib(10);
    hards.push_emplace(fa, fi);


    return 0;

}

