#include <iostream>
#include <chrono>
#include <array>
#include "adaptors.h"

using namespace std;

int main()
{
    constexpr int n = 1000000;
    std::array<int, n> data;
    std::iota(data.begin(), data.end(), 0);
    int s;

    // Offset traversal
    std::cout << "Offset" << std::endl;
    s = 0;
    auto start = std::chrono::system_clock::now();
    for (const int& x : offset(data.begin(), data.end(), 2))
        s += x;
    auto end = std::chrono::system_clock::now();
    std::cout << s << std::endl;
    std::chrono::duration<double> diff = end-start;
    std::cout << diff.count() << " s" << std::endl;

    // Pseudo random traversal
    std::cout << "\nPseudo random" << std::endl;
    s = 0;
    start = std::chrono::system_clock::now();
    for (const int& x : pseudoShuffled(data.begin(), data.end(), 2, 2147483647))
        s += x;
    end = std::chrono::system_clock::now();
    std::cout << s << std::endl;
    diff = end-start;
    std::cout << diff.count() << " s" << std::endl;

    // Random traversal
    std::cout << "\nRandom" << std::endl;
    s = 0;
    std::random_device rd;
    std::mt19937 g(rd());
    start = std::chrono::system_clock::now();
    for (const int& x : shuffled(data.begin(), data.end(), g))
        s += x;
    end = std::chrono::system_clock::now();
    std::cout << s << std::endl;
    diff = end-start;
    std::cout << diff.count() << " s" << std::endl;

    return 0;
}
