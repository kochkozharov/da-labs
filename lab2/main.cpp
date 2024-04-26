#include <iostream>
#include <string>
#include "patricia.h"
#include <cinttypes>

int main() {
    CaseInsensitiveString input;
    TPatriciaTrie trie;
    while (input.Scan(stdin)) {
        if (input == "+") {
            CaseInsensitiveString key;
            uint64_t value;
            key.Scan(stdin);
            scanf("%" SCNu64, &value);
            //std::cin >> key >> value;
            try {
                trie.Insert({key, value});
            } catch (std::exception& e) {
                std::cout << e.what() << '\n';
                continue;
            }
        } else if (input == "-") {
            CaseInsensitiveString key;
            key.Scan(stdin);
            try {
                trie.Erase(key);
            } catch (std::exception& e) {
                std::cout << e.what() << '\n';
                continue;
            }
        } else if (input == "!") {
            CaseInsensitiveString cmd;
            cmd.Scan(stdin);
            if (cmd == "Save") {
                CaseInsensitiveString path;
                path.Scan(stdin);
                std::ofstream file;
                try {
                    file.open(path.CStr());
                } catch (std::exception& e) {
                    std::cout << "ERROR: " << e.what() << '\n';
                }
                try {
                    trie.SaveToFile(file);
                } catch (std::exception& e) {
                    std::cout << e.what() << '\n';
                }
            } else if (cmd == "Load") {
                CaseInsensitiveString path;
                path.Scan(stdin);
                std::ifstream file;
                try {
                    file.open(path.CStr());
                } catch (std::exception& e) {
                    std::cout << "ERROR: " << e.what() << '\n';
                }
                try {
                    trie.LoadFromFile(file);
                } catch (std::exception& e) {
                    std::cout << e.what() << '\n';
                }
            }
            std::cout << "OK" << '\n';
            continue;
        } else {
            TPair<CaseInsensitiveString, uint64_t> p;
            try {
                p = trie.Find(input);
            } catch (std::exception& e) {
                std::cout << e.what() << '\n';
                continue;
            }
            std::cout << "OK: " << p.value << '\n';
            continue;
        }
        std::cout << "OK" << '\n';
    }

    return 0;
}
