#include <cinttypes>

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
            if (!t.Insert({key, value})) {
                printf("Exist\n");
                continue;
            }
        } else if (input == "-") {
            CaseInsensitiveString key;
            key.Scan();
            if (!t.Erase(key)) {
                printf("NoSuchWord\n");
                continue;
            }
        } else if (input == "!") {
            CaseInsensitiveString cmd;
            cmd.Scan();
            if (cmd == "Save") {
                CaseInsensitiveString path;
                path.Scan();
                CFile file(path, CFile::FileType::Save);
                if (!file.check()) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
                if (!t.SaveToFile(file.GetFile())) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
            } else if (cmd == "Load") {
                CaseInsensitiveString path;
                path.Scan();
                CFile file(path, CFile::FileType::Load);
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
            const TPair<CaseInsensitiveString, uint64_t>* p;
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