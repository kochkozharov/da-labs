#include <cinttypes>

#include "patricia.h"

int main() {
    TCaseInsensitiveString input;
    TPatriciaTrie t;
    while (input.Scan()) {
        if (input == "+") {
            TCaseInsensitiveString key;
            uint64_t value;
            key.Scan();
            scanf("%" SCNu64, &value);
            if (!t.Insert({key, value})) {
                printf("Exist\n");
                continue;
            }
        } else if (input == "-") {
            TCaseInsensitiveString key;
            key.Scan();
            if (!t.Erase(key)) {
                printf("NoSuchWord\n");
                continue;
            }
        } else if (input == "!") {
            TCaseInsensitiveString cmd;
            cmd.Scan();
            if (cmd == "Save") {
                TCaseInsensitiveString path;
                path.Scan();
                TFile file(path, TFile::FileType::Save);
                if (!file.check()) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
                if (!t.SaveToFile(file.GetFile())) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
            } else if (cmd == "Load") {
                TCaseInsensitiveString path;
                path.Scan();
                TFile file(path, TFile::FileType::Load);
                if (!file.check()) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
                if (!t.LoadFromFile(file.GetFile())) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
            }
        } else {
            const TPair<TCaseInsensitiveString, uint64_t>* p;
            p = t.Find(input.CStr());
            if (!p) {
                printf("NoSuchWord\n");
                continue;
            }
            printf("OK: %" PRIu64 "\n", p->value);
            continue;
        }
        printf("OK\n");
    }
    return 0;
}