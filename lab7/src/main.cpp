#include <iostream>
#include <cmath>

int LengthOfNum(long long x){
    int len=1;
    while(x>9){ len++; x/=10; }
    return len;
}

long long CountMultiplesInRange(long long l, long long r, int m) {
    long long firstMultiple = (l % m == 0) ? l : l + (m - l % m);
    long long lastMultiple = (r % m == 0) ? r : r - (r % m);
    if (firstMultiple > lastMultiple) {
        return 0;
    }
    return (lastMultiple - firstMultiple) / m + 1;
}

long long Solve(long long n, int m) {
    int length = LengthOfNum(n);
    long long result = 0;
    long long a = pow(10, length-1);
    long long b = n; 
    for (int i = 0; i < length; ++i) {
        result += CountMultiplesInRange(a, b, m);
        a /= 10;
        b /= 10;
    }
    if (n % m == 0) {
        return result-1;
    }
    return result;
}

int main() {
    long long n;
    int m;
    std::cin >> n >> m;
    std::cout << Solve(n, m) << std::endl;
    return 0;
}