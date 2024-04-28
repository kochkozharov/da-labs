#include <unistd.h>

#include <chrono>
#include <climits>
#include <iostream>
#include <map>

#include "patricia.h"

using duration_t = std::chrono::microseconds;

std::string GenRandomStr(const int len) {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "lab2_benchmark [NUM_OF_KEYS]");
        exit(-1);
    }
    char *p;
    long int times = strtol(argv[1], &p, 10);
    if (errno != 0 || *p != '\0' || times > INT_MAX || times < INT_MIN) {
        fprintf(stderr, "Wrong arg");
        exit(-1);
    }
    srand((unsigned)time(NULL) * getpid());
    std::map<std::string, uint64_t> rb_tree;
    TPatriciaTrie t;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    int64_t rb_ts = 0, pat_ts = 0;
    for (unsigned long i = 0; i < times; ++i) {
        std::string key = GenRandomStr(100);
        CaseInsensitiveString insKey(key.c_str()); 
        {
            start = std::chrono::system_clock::now();
            rb_tree.insert({key, i});
            end = std::chrono::system_clock::now();
            rb_ts +=
                std::chrono::duration_cast<duration_t>(end - start).count();
        }
        {
            start = std::chrono::system_clock::now();
            try {
                t.Insert({insKey, i});
            } catch (std::exception &e) {
            }
            end = std::chrono::system_clock::now();
            pat_ts +=
                std::chrono::duration_cast<duration_t>(end - start).count();
        }
    }
    std::cout << "std::map ms=" << rb_ts << "\npatricia ms=" << pat_ts << std::endl;
}


