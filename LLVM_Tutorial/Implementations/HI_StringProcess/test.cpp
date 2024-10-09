#include <iostream>

using namespace std;

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t hash_(char const *str)
{
    hash_t ret{basis};

    while (*str)
    {
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}

void print(char const *str)
{
    while (*str)
    {
        cout << *str << endl;
        str++;
    }
}

int main()
{
    string str = "qwertasdf";
    // cout << hash_(str.c_str()) << endl;
    print(str.c_str());
    return 0;
}