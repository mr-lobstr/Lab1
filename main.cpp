#include "main.h"
using namespace std;

int main (int argc, char* argv[])
{
    if (argc < 3) {
        cout << "ожидалось 3 и более аргументов, получено: " << argc << endl;
        return 0;
    }

    string flag = argv[1];
    string fileName = argv[2];

    if (flag != "--file") {
        cout << "ожидался флаг --line, получен: " << flag << endl;
        return 0;
    }

    parse_file (fileName);
    string s;

    while (getline (cin, s)) {
        if (s.size() < 3) {
            continue;
        }

        if (s == "exit") {
            break;
        }

        if (s == "write") {
            write_file (fileName);
            continue;
        }

        try {
            parse_input (s);
        } catch (std::runtime_error const& e) {
            cout << e.what();
            getline (cin, s);
        }
        cout << endl;
    }

    write_file (fileName);
}