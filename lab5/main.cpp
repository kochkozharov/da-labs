#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "suffix_tree.h"

using namespace std;


int main() {
    string text;
    cin >> text;
    Trie trie(text);

    string pattern;
    int counter = 1;
    while (cin >> pattern) {
        vector<int> res = trie.Search(pattern);
        if (!res.empty()) {
            printf("%d: ", counter);
            sort(res.begin(), res.end());
            for (size_t i = 0; i < res.size(); ++i) {
                if (i != 0) {
                    printf(", ");
                }
                printf("%d", res[i] + 1);
            }
            printf("\n");
        }
        ++counter;
    }

    return 0;
}
