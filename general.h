#ifndef GENERAL_LAB1_H_GUARD
#define GENERAL_LAB1_H_GUARD

#include <iostream>
#include <fstream>
#include <string>

#include "data_struct/dynamic_array.h"
#include "data_struct/list.h"
#include "data_struct/flist.h"
#include "data_struct/stack.h"
#include "data_struct/queue.h"
#include "data_struct/hash_table.h"
#include "data_struct/complete_binary_tree.h"


extern data_struct::HashTable<std::string, data_struct::DynamicArray<std::string>> arrays;
extern data_struct::HashTable<std::string, data_struct::List<std::string>> lists;
extern data_struct::HashTable<std::string, data_struct::FList<std::string>> flists;
extern data_struct::HashTable<std::string, data_struct::Stack<std::string>> stacks;
extern data_struct::HashTable<std::string, data_struct::Queue<std::string>> queues;
extern data_struct::HashTable<std::string, data_struct::HashTable<std::string, std::string>> hashes;
extern data_struct::HashTable<std::string, data_struct::CompleteBinTree<std::string>> trees;


data_struct::List<std::string> split (std::string const&, char);


namespace data_struct
{
    template <>
    struct Hasher<std::string> {
        size_t operator() (std::string const& s) const {
            size_t hash = 2736712;

            for (auto c : s) {
                hash += 37 * c;
            }   
            return hash;
        }
    };

    template <>
    struct Hasher<int> {
        size_t operator() (int i) const {
            return i;
        }
    };
}

#endif
