#include "general.h"

using namespace std;
using namespace data_struct;


template <typename HashCont>
auto& get_container (string const& name, HashCont& hs, DynamicArray<string> const& setComm, string const& comm) {
    auto it = algs::find (setComm.begin(), setComm.end(), comm);
    auto contIt = hs.find (name);

    if (contIt == hs.end()) {
        if (it != setComm.end()) {
            hs.add (name);
            return hs[name];
        }

        throw std::runtime_error (
            "не существует контейнера с именем \"" + name + "\"\n"
        );
    }

    return hs[name];
}


template <typename Container>
void write_default (ostream& out, Container const& cont) {
    out << string (4, ' ');

    bool isFirst = true;
    for (auto& el : cont) {
        out << (isFirst ? "" : ", ");
        isFirst = false;
        out << el;
    }

    out << ";\n";
}


void write_hash_table (ostream& out, HashTable<string, string> const& table) {
    out << string (4, ' ');
    bool isFirst = true;
    for (auto& [k, v] : table) {
        out << (isFirst ? "" : ", ");
        isFirst = false;
        out << k << " : " << v;
    }

    out << ";\n";
}


void write_tree (ostream& out, CompleteBinTree<string> const& tree) {
    out << string (4, ' ');
    bool isFirst = true;
    int i = 1;
    for (auto& el : tree) {
        out << (isFirst ? "" : ", ");

        if (log2f(i) == int(log2 (i))) {
            if (not isFirst) {
                out << endl << string (4, ' ');
            }
        }
        ++i;

        isFirst = false;
        out << el; 
    }

    out << ";\n";
}


void write_file (string const& fileName) {
    ofstream out (fileName);

    for (auto& [name, arr] : arrays) {
        out << "Array" << " " << name << " >\n";
        write_default (out, arr);
        out << endl;
    }

    for (auto& [name, list] : lists) {
        out << "List" << " " << name << " >\n";
        write_default (out, list);
        out << endl;
    }

    for (auto& [name, flist] : flists) {
        out << "Flist" << " " << name << " >\n";
        write_default (out, flist);
        out << endl;
    }

    for (auto& [name, st] : stacks) {
        out << "Stack" << " " << name << " >\n";
        write_default (out, st);
        out << endl;
    }

    for (auto& [name, que] : queues) {
        out << "Queue" << " " << name << " >\n";
        write_default (out, que);
        out << endl;
    }

    for (auto& [name, table] : hashes) {
        out << "HashTable" << " " << name << " >\n";
        write_hash_table (out, table);
        out << endl;
    }

    for (auto& [name, tree] : trees) {
        out << "Tree" << " " << name << " >\n";
        write_tree (out, tree);
        out << endl;
    }

    out.close();
}


void array_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"PUSH"};
    auto& arr = get_container (name, arrays, setCommands, command);

    auto check_index = [&] (auto ind) {
        if (ind >= arr.size()) throw std::runtime_error (
            "недопустимое значение индекса: " + to_string(ind) + "\n"
        );
    };

    if (command == "PUSH") {
        arr.push_back (args.front());
    } else if (command == "INSIND") {
        auto ind = stoi(args.front()); args.pop_front();
        check_index (ind);
        auto it = arr.begin() + ind;
        arr.insert (it, args.front());
    } else if (command == "GETIND") {
        auto ind = stoi(args.front()); args.pop_front();
        check_index (ind);
        cout << arr[ind] << endl;
    } else if (command == "DELIND") {
        auto ind = stoi(args.front()); args.pop_front();
        check_index (ind);
        auto it = arr.begin() + ind;
        arr.erase (it);
    } else if (command == "RESIND") {
        auto ind = stoi(args.front()); args.pop_front();
        arr[ind] = args.front();
    } else if (command == "PRINT") {
        write_default (cout, arr);
    } else {
        throw std::runtime_error (
            "для массива не определенна команда \"" + command + "\"\n"
        );
    }
}


void list_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"PUSHHEAD", "PUSHTAIL"};
    auto& list = get_container (name, lists, setCommands, command);

    if (command == "PUSHTAIL") {
        list.push_back (args.front());
    } else if (command == "PUSHHEAD") {
        list.push_front (args.front());
    } else if (command == "DELHEAD") {
        list.pop_front();
    } else if (command == "DELTAIL") {
        list.pop_back();
    } else if (command == "DEL") {
        auto it = algs::find (list.begin(), list.end(), args.front());

        if (it == list.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в списке отсутствует" +"\n"
        );
        list.erase (it);
    } else if (command == "GET") {
        auto it = algs::find (list.begin(), list.end(), args.front());

        if (it == list.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в списке отсутствует" +"\n"
        );

        cout << *it << endl;
    } else if (command == "PRINT") {
        write_default (cout, list);
    } else {
        throw std::runtime_error (
            "для списка не определенна команда \"" + command + "\"\n"
        );
    }
}


void flist_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"PUSHHEAD"};
    auto& flist = get_container (name, flists, setCommands, command);

    if (command == "PUSHHEAD") {
        flist.push_front (args.front());
    } else if (command == "DELHEAD") {
        flist.pop_front();
    } else if (command == "DEL") {
        auto it = flist.find_prev (args.front());

        if (it == flist.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в списке отсутствует" +"\n"
        );
        flist.erase_after (it);
    } else if (command == "GET") {
        auto it = flist.find_prev (args.front());

        if (it == flist.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в списке отсутствует" +"\n"
        );

        cout << *++it << endl;
    } else if (command == "PRINT") {
        write_default (cout, flist);
    } else {
        throw std::runtime_error (
            "для списка не определенна команда \"" + command + "\"\n"
        );
    }
}


void queue_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"PUSH"};
    auto& queue = get_container (name, queues, setCommands, command);

    if (command == "PUSH") {
        queue.push_back (args.front());
    } else if (command == "POP") {
        queue.pop_front();
    } else if (command == "PRINT") {
        write_default (cout, queue);
    } else {
        throw std::runtime_error (
            "для стека не определенна команда \"" + command + "\"\n"
        );
    }
}


void hashes_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"ADD"};
    auto& ht = get_container (name, hashes, setCommands, command);

    if (command == "ADD") {
        auto key = args.front(); args.pop_front();
        ht.add (key, args.front());
    } else if (command == "DEL") {
        auto it = ht.find (args.front());

        if (it == ht.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в хэш-таблице отсутствует" +"\n"
        );
        ht.erase (args.front());
    } else if (command == "PRINT") {
        write_hash_table (cout, ht);
    } else {
        throw std::runtime_error (
            "для хэш-таблицы не определенна команда \"" + command + "\"\n"
        );
    }
}


void stack_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"PUSH"};
    auto& stack = get_container (name, stacks, setCommands, command);

    if (command == "PUSH") {
        stack.push (args.front());
    } else if (command == "POP") {
        stack.pop();
    } else if (command == "PRINT") {
        write_default (cout, stack);
    } else {
        throw std::runtime_error (
            "для стека не определенна команда \"" + command + "\"\n"
        );
    }
}


void tree_func (string const& command, string const& name, List<string> args) {
    DynamicArray<string> setCommands = {"ADD"};
    auto& tree = get_container (name, trees, setCommands, command);

    if (command == "ADD") {
        tree.add (args.front());
    } else if (command == "POP") {
        tree.pop_back ();
    } else if (command == "GET") {
        auto it = algs::find (tree.begin(), tree.end(), args.front());

        if (it == tree.end()) throw std::runtime_error (
            "\"" + args.front() + "\"" + " в дереве отсутствует" +"\n"
        );

        cout << *it << endl;
    } else if (command == "PRINT") {
        write_tree (cout, tree);
    } else {
        throw std::runtime_error (
            "для дерева не определенна команда \"" + command + "\"\n"
        );
    }
}


void parse_input (string const& inputStr) {
    auto tokens = split (inputStr, ' ');
    char firstLetter = tokens.front().front();
    
    tokens.front().erase(0, 1);
    auto command = tokens.front(); tokens.pop_front();
    auto name = tokens.front(); tokens.pop_front();

    switch (firstLetter)
    {
    case 'M':
        array_func (command, name, tokens);
        break;

    case 'L':
        list_func (command, name, tokens);
        break;

    case 'F':
        flist_func (command, name, tokens);
        break;

    case 'S':
        stack_func (command, name, tokens);
        break;

    case 'Q':
        queue_func (command, name, tokens);
        break;

    case 'H':
        hashes_func (command, name, tokens);
        break;

    case 'T':
        tree_func (command, name, tokens);
        break;
    
    default:
        throw std::runtime_error (
            "неизвестный код контейнера: "s 
          + "\"" + string {firstLetter} +"\"\n"
        );
    }
}
