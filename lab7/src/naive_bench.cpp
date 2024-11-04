#include <iostream>
#include <string>


int main() {
    long long n;
    int m;
    double start_time =  clock();
    while (std::cin >> n >> m){
        long long count = 0;
        for (long long multiple = m; multiple < n; multiple += m) {
            if (std::to_string(multiple) < std::to_string(n)) {
                count++;
            }
        }
    }
    double end_time = clock();
    double search_time = end_time - start_time;
    printf("Naive Bench\nTime: %f sec\n", (double)search_time/CLOCKS_PER_SEC);
    return 0;
}