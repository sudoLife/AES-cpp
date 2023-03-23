#include "AES.hpp"
#include "constants.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

inline void print(const std::vector<uint8_t> &block)
{
    auto nk = constants::nk;
    for (int i = 0; i < nk; i++)
    {
        for (int j = 0; j < nk; j++)
        {
            std::cout << (unsigned int)block[nk * i + j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void gmix_column(unsigned char *r)
{
    unsigned char a[4];
    unsigned char b[4];
    unsigned char row;
    unsigned char h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */
    for (row = 0; row < 4; row++)
    {
        a[row] = r[row];
        /* h is 0xff if the high bit of r[row] is set, 0 otherwise */
        h = (r[row] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[row] = r[row] << 1;  /* implicitly removes high bit because b[row] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[row] ^= h * 0x1B;    /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

int main()
{
    // std::vector<uint8_t> test{0xb3, 'B', 'C', 'D'};
    std::vector<uint8_t> test{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // // AES::rot_word(test);
    // AES::mix_columns(test);
    print(test);
    AES::mix_columns(test);
    // AES::shift_rows(test);
    print(test);

    return 0;
}
