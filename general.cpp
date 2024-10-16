#include "general.h"

using namespace std;
using namespace data_struct;


HashTable<string, DynamicArray<string>> arrays;
HashTable<string, List<string>> lists;
HashTable<string, FList<string>> flists;
HashTable<string, Stack<string>> stacks;
HashTable<string, Queue<string>> queues;
HashTable<string, HashTable<string, string>> hashes;
HashTable<string, CompleteBinTree<string>> trees;


string spaces_delete (string s) {

    auto it = algs::find_if (s.begin(), s.end(), [] (auto c) {
        return c != ' ';
    });
    s.erase(s.begin(), it);

    auto rbeg = ReverseIterator<decltype(it)> (--s.end());
    auto rend = ReverseIterator<decltype(it)> (--s.begin());

    auto it2 = algs::find_if (rbeg, rend, [] (auto c) {
        return c != ' ';
    });
    s.erase(++it2.real(), s.end());

    return s;
}

List<string> split (string const& str, char separator) {
    List<string> res;
    auto subStrBeg = str.begin();

    while (subStrBeg != str.end()) {
        if (subStrBeg != str.begin()) {
            ++subStrBeg;
        }
        auto subStrEnd = algs::find (subStrBeg, str.end(), separator);
        res.push_back (
            spaces_delete ({subStrBeg, subStrEnd})
        );
        subStrBeg = subStrEnd;
    }

    return res;
}
