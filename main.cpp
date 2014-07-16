#include <iostream>
#include <tuple>
#include <set>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <fstream>

using namespace std;

typedef int64_t inttype;
typedef inttype X;
typedef inttype Y;
typedef tuple<X, Y> Loc;
typedef set<Loc> World;

bool isAlive(const World &w, const Loc &l)
{
    return w.find(l) != w.end();
}

World neighbors(const Loc &l)
{
    World retval;

    auto x = get<0>(l);
    auto y = get<1>(l);

    retval.emplace(make_tuple(x-1, y-1)); retval.emplace(make_tuple(x  , y-1)); retval.emplace(make_tuple(x+1, y-1));
    retval.emplace(make_tuple(x-1, y  )); retval.emplace(make_tuple(x  , y  )); retval.emplace(make_tuple(x+1, y  ));
    retval.emplace(make_tuple(x-1, y+1)); retval.emplace(make_tuple(x  , y+1)); retval.emplace(make_tuple(x+1, y+1));

    return retval;
}

inttype countLivingNeighbors(const World &w, const Loc &l)
{
    auto neigh = neighbors(l);
    inttype retval;

    World intersect;

    set_intersection(w.begin(), w.end(), neigh.begin(), neigh.end(),
                     insert_iterator<World>(intersect, intersect.begin()));

    return intersect.size();
}

bool rule(bool self, inttype alive)
{
    return (self == true) ? (alive == 3 || alive == 4) : (alive == 3);
}

World candidates(const World &w)
{
    World retval;

    for(const auto &a : w)
    {
        auto foo = neighbors(a);
        retval.insert(foo.begin(), foo.end());
    }

    return retval;
}

World step(const World &w)
{
    auto cand = candidates(w);
    World retval;
    for(const auto &c : cand)
        if(rule(isAlive(w, c), countLivingNeighbors(w, c)))
            retval.insert(c);
    return retval;
}

void print(const World &w)
{
    auto maxX = get<0>(*max_element(w.begin(), w.end(), [] (const Loc &l, const Loc &r) { return get<0>(l) < get<0>(r); }));
    auto maxY = get<1>(*max_element(w.begin(), w.end(), [] (const Loc &l, const Loc &r) { return get<1>(l) < get<1>(r); }));
    auto minX = get<0>(*max_element(w.begin(), w.end(), [] (const Loc &l, const Loc &r) { return get<0>(l) > get<0>(r); }));
    auto minY = get<1>(*max_element(w.begin(), w.end(), [] (const Loc &l, const Loc &r) { return get<1>(l) > get<1>(r); }));

    cout << "\e[2J  \033[2J\033[;H";

    for(auto y = minY ; y <= maxY ; y++)
    {
        for(auto x = minX ; x <= maxX ; x++)
        {
            if (w.find(make_tuple(x,y)) == w.end())
                cout << ' ';
            else
                cout << '#';
        }
        cout << endl;
    }
}

int main(int argc, char **argv)
{
    World w;

    ifstream i(argv[1]);
    {
        char foo[100];
        i.getline(foo, 100);
    }

    for (;;)
    {
        inttype x,y;
        i >> x >> y;
        if (i.eof() || i.bad())
            break;
        w.emplace( make_tuple(x,y) );
    }

    for(;;)
    {
        print(w);
        w = step(w);
        usleep(50000);
    }
}
