#include "suffix_tree.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include <algorithm>
#include <random>

using duration_t = std::chrono::microseconds;


using namespace std;

string generateRandomString(int length)
{
    // Define the list of possible characters
    const string CHARACTERS
        = "abcdefghijklmnopqrstuvwxyz";

    // Create a random number generator
    random_device rd;
    mt19937 generator(rd());

    // Create a distribution to uniformly select from all
    // characters
    uniform_int_distribution<> distribution(
        0, CHARACTERS.size() - 1);

    // Generate the random string
    string random_string;
    for (int i = 0; i < length; ++i) {
        random_string
            += CHARACTERS[distribution(generator)];
    }

    return random_string;
}



int main() {

    string text=generateRandomString(10000);
    long t1=0;
    long t2=0;
    auto start = std::chrono::system_clock::now();
    Trie trie(text); 
    auto end = std::chrono::system_clock::now();
    t1 += std::chrono::duration_cast<duration_t>(end - start).count();

    for (int i = 0; i < 10000; ++i) {
        string pattern = generateRandomString(10);
        auto start = std::chrono::system_clock::now();
        vector<size_t> res1 = trie.Search(pattern);
        auto end = std::chrono::system_clock::now();
        t1 += std::chrono::duration_cast<duration_t>(end - start).count();


        vector<size_t> res2;
        start = std::chrono::system_clock::now();
        size_t pos = text.find(pattern, 0);
        while (pos != string::npos) {
            res2.push_back(pos);
            pos = text.find(pattern, pos + 1);
        }
        end = std::chrono::system_clock::now();
        t2 += std::chrono::duration_cast<duration_t>(end - start).count();
        sort(res1.begin(), res1.end());
        sort(res2.begin(), res2.end());


        assert(res1 == res2);
    }
    cout << "suf_tree: " << t1 << '\n' << "naive: "  << t2 << '\n';
}
