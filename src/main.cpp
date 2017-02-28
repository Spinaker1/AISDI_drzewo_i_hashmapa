#include <cstddef>
#include <cstdlib>
#include <string>

#include "TreeMap.h"
#include "HashMap.h"



int main()
{
    srand (time(NULL));
    aisdi::HashMap<int,char> hashmap;
    aisdi::TreeMap<int,char> tree;

    clock_t czas=clock();

    int tab[10000];

    for (int i=0; i<10000; i++)
    {
        tab[i]=rand();
    }

    for (int i=0; i<10000; i++)
    {
        hashmap[tab[i]]='A'+(rand()%26);
    }
    std::cout << "Dodawanie nowych elementow do struktury HashMap trwalo " << clock()-czas << std::endl;

    czas=clock();
    for (int i=0; i<10000; i++)
    {
        tree[tab[i]]='A'+(rand()%26);
    }
    std::cout << "Dodawanie nowych elementow do struktury TreeMap trwalo " << clock()-czas << std::endl;

    czas=clock();
    for (int i=0; i<10000; i++)
    {
        hashmap.valueOf(tab[i]);
    }
    std::cout << "Odnajdywanie wartości po kluczu w strukturze HashMap trwalo " << clock()-czas << std::endl;

    czas=clock();
    for (int i=0; i<10000; i++)
    {
        tree.valueOf(tab[i]);
    }
    std::cout << "Odnajdywanie wartości po kluczu w strukturze TreeMap trwalo  " << clock()-czas << std::endl;

    czas=clock();
    for (int i=0; i<10000; i++)
    {
        hashmap.remove(tab[i]);
    }
    std::cout << "Usuwanie elementow ze struktury HashMap trwalo " << clock()-czas << std::endl;

    czas=clock();
    for (int i=0; i<10000; i++)
    {
        tree.remove(tab[i]);
    }
    std::cout << "Usuwanie elementow ze struktury TreeMap trwalo " << clock()-czas << std::endl;
}
