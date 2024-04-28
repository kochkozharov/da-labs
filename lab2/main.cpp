#include <cinttypes>
#include <iostream>

#include "patricia.h"

int main() {
    CaseInsensitiveString input;
    TPatriciaTrie t;
    while (input.Scan()) {
        if (input == "+") {
            CaseInsensitiveString key;
            uint64_t value;
            key.Scan();
            scanf("%" SCNu64, &value);
            try {
                t.Insert({key, value});
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
        } else if (input == "-") {
            CaseInsensitiveString key;
            key.Scan();
            try {
                t.Erase(key);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
        } else if (input == "!") {
            CaseInsensitiveString cmd;
            cmd.Scan();
            if (cmd == "Save") {
                CaseInsensitiveString path;
                path.Scan();
                CFile file(path, CFile::FileType::Save);
                try {
                    t.SaveToFile(file.GetFile());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            } else if (cmd == "Load") {
                CaseInsensitiveString path;
                path.Scan();
                CFile file(path, CFile::FileType::Load);
                try {
                    t.LoadFromFile(file.GetFile());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            std::cout << "OK" << std::endl;
            continue;
        } else {
            TPair<CaseInsensitiveString, uint64_t> p;
            try {
                p = t.Find(input.CStr());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            std::cout << "OK: " << p.value << std::endl;
            continue;
        }
        std::cout << "OK" << std::endl;
    }

    return 0;
}