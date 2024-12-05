#include <algorithm>
#include <iostream>
#include "suffix_tree.h"

using namespace std;

int main() {
    string text;
    cin >> text;
    SuffixTrie SuffixTrie(text);

    string pattern;
    size_t counter = 1;
    while (cin >> pattern) {
        vector<size_t> res = SuffixTrie.Search(pattern);
        if (!res.empty()) {
            printf("%zu: ", counter);
            sort(res.begin(), res.end());
            for (size_t i = 0; i < res.size(); ++i) {
                if (i != 0) {
                    printf(", ");
                }
                printf("%zu", res[i] + 1);
            }
            printf("\n");
        }
        ++counter;
    }

    return 0;
}
