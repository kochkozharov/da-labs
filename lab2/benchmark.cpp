#include <iostream>
#include <map>
#include <chrono>

#include "patricia.h"

using duration_t = std::chrono::microseconds;

int main() {
    std::map<std::string, uint64_t> rb_tree;
    TPatriciaTrie t;
    char action;
    std::string key;
    uint64_t value;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    int64_t rb_ts = 0, bst_ts = 0;
    while (std::cin >> action) {
        if( action == '+' ) {
            std::cin >> key >> value;
            {
                start = std::chrono::system_clock::now();
                rb_tree.insert( {key, value} );
                end = std::chrono::system_clock::now();
                rb_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
            {
                start = std::chrono::system_clock::now();
                try{
                    t.Insert({key, value});
                } catch(std::exception &e) {}
                end = std::chrono::system_clock::now();
                bst_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
        }
    }
    std::cout << "std::map ms=" << rb_ts << "\nbst ms=" << bst_ts << std::endl;
}
