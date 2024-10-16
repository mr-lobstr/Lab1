#include "general.h"

using namespace std;
using namespace data_struct;


void read_array (DynamicArray<string>& arr, string const& data) {
    auto dataList = split (data, ',');
    arr = DynamicArray<string> {dataList.begin(), dataList.end()};
}


void read_list (List<string>& list, string const& data) {
    list = split (data, ',');
}


void read_flist (FList<string>& flist, string const& data) {
    auto dataList = split (data, ',');
    flist = FList<string> {dataList.begin(), dataList.end()};
}


void read_stack (Stack<string>& stack, string const& data) {
    auto dataList = split (data, ',');
    stack = Stack<string> {dataList.begin(), dataList.end()};
}


void read_queue (Queue<string>& queue, string const& data) {
    auto dataList = split (data, ',');
    queue = Queue<string> {dataList.begin(), dataList.end()};
}


void read_tree (CompleteBinTree<string>& tree, string const& data) {
    auto dataList = split (data, ',');
    tree = CompleteBinTree<string> {dataList.begin(), dataList.end()};
}


void read_hash_table (HashTable<string, string>& table, string const& data) {
    auto dataList = split (data, ',');
    
    for (auto& pairStr : dataList) {
        auto pair = split (pairStr, ':');
        auto& key = pair.front();
        auto& value = pair.back();
        table.add (key, value);
    }
}


string read_file_to_str (string const& fileName) {
    ifstream in (fileName);
    string s, res;

    while (getline (in, s)) {
        res += s;
    }

    in.close();
    return res;
}


void parse_container (string const& containerStr) {
    auto contAndData = split (containerStr, '>');
    auto typeAndName = split (contAndData.front(), ' ');

    auto type = typeAndName.front();
    auto name = typeAndName.back();
    auto data = contAndData.back();

    if (type == "Array") {
        arrays.add (name);
        read_array (arrays[name], data);
    } else if (type == "List") {
        lists.add (name);
        read_list (lists[name], data);
    } else if (type == "Flist") {
        flists.add (name);
        read_flist (flists[name], data);
    } else if (type == "Stack") {
        stacks.add (name);
        read_stack (stacks[name], data);
    } else if (type == "Queue") {
        queues.add (name);
        read_queue (queues[name], data);
    } else if (type == "HashTable") {
        hashes.add (name);
        read_hash_table (hashes[name], data);
    } else if (type == "Tree") {
        trees.add (name);
        read_tree (trees[name], data);
    } else {
        throw std::runtime_error (
            "неизвестный контейнер: "s 
          + "\"" + type +"\"\n"
        );
    }
}


void parse_file (string const& fileName) {
    auto fileStr = read_file_to_str(fileName);
    auto containerList = split (fileStr, ';');
    containerList.pop_back();

    for (auto& contStr : containerList) {
        parse_container (contStr);
    }
}
