#include "AES.hpp"
#include "constants.hpp"
#include <cassert>

AES::AES(std::vector<uint8_t> key)
{
    round_key = AES::expand_key(key);
}

void AES::encrypt(std::vector<uint8_t> &block)
{
    assert(block.size() == constants::block_len);

    add_round_key(block, 0);

    for (int round = 1; round < constants::rounds - 1; round++)
    {
        // S-box substitution
        sub_word(block);
        shift_rows(block);
        mix_columns(block);
        add_round_key(block, round);
    }

    sub_word(block);
    shift_rows(block);
    add_round_key(block, constants::rounds - 1);
}

// STATIC functions
std::vector<uint8_t> AES::expand_key(std::vector<uint8_t> key)
{
    std::vector<uint8_t> expanded_key(constants::rounds * constants::block_len);
    std::vector<uint8_t> word(constants::word_len);

    std::copy(key.begin(), key.end(), expanded_key.begin());

    for (int i = constants::nk; i < constants::nk * constants::rounds; i++)
    {
        // okay now it's better, we have ith word

        // now we copy into our temporary word vector to perform all of those operations
        iter it = expanded_key.begin() + constants::word_len * (i - 1);
        std::copy(it, it + constants::word_len, word.begin());

        if (i % constants::nk == 0)
        {
            // TODO: possible improvements: do the sub work in-place in the expanded key itself
            // we don't really need this word
            rot_word(word);
            sub_word(word);
            rcon_word(word, i / constants::nk);
        }

        // now we xor with w_{i - N_k}
        xor_words(word.begin(), word.end(), expanded_key.begin() + constants::word_len * (i - constants::nk));
        // finally, the word is ready to be put in its place
        std::copy(word.begin(), word.end(), expanded_key.begin() + i * constants::word_len);
    }

    return expanded_key;
}

// TODO: modify this so it operates on an arbitrary chunk?
// maybe it's not worth it, let's just implement another method
void AES::rot_word(std::vector<uint8_t> &word)
{
    // each word is 32 bytes
    assert(word.size() == constants::word_len);
    uint8_t temp = word[0];
    // TODO: should I have anything more efficient than this?
    for (int i = 1; i < constants::word_len; i++)
    {
        word[i - 1] = word[i];
    }
    word[3] = temp;
}

void AES::sub_word(std::vector<uint8_t> &word)
{
    for (auto &chr : word)
    {
        chr = constants::sbox[chr];
    }
}

void AES::rcon_word(std::vector<uint8_t> &word, int round)
{
    // this only modifies the LSByte
    word[0] ^= constants::rcon[round - 1];
}

void AES::xor_words(iter first, iter last, iter second)
{
    for (; first != last; first++, second++)
    {
        *first ^= *second;
    }
}

void AES::shift_rows(std::vector<uint8_t> &block)
{
    assert(block.size() == constants::block_len);

    // I know how this is a shit deal here, but meh
    // first row is not shifted
    // second row is shifted by 1
    int i = constants::word_len;
    std::swap(block[i], block[i + 1]);
    std::swap(block[i + 1], block[i + 2]);
    std::swap(block[i + 2], block[i + 3]);

    // third row is shifted by 2
    i = constants::word_len * 2;
    std::swap(block[i], block[i + 2]);
    std::swap(block[i + 1], block[i + 3]);

    // fourth row is shifted by 3
    i = constants::word_len * 3;
    std::swap(block[i], block[i + 1]);
    std::swap(block[i], block[i + 2]);
    std::swap(block[i], block[i + 3]);
}

void AES::add_round_key(std::vector<uint8_t> &block, int round)
{
    auto nk = constants::nk;
    int offset = round * constants::block_len;

    for (int row = 0; row < nk; row++)
    {
        for (int col = 0; col < nk; col++)
        {
            block[row * nk + col] ^= round_key[offset + col * nk + row];
        }
    }
}

void AES::mix_columns(std::vector<uint8_t> &block)
{
    auto nk = constants::nk;

    // std::vector<uint8_t> result(constants::block_len, 0);
    std::vector<uint8_t> a(nk);
    std::vector<uint8_t> b(nk);

    uint8_t r;
    uint8_t h;

    for (int col = 0; col < nk; col++)
    {
        for (int row = 0; row < nk; row++)
        {
            r = block[row * nk + col];
            a[row] = r;
            h = (r >> 7) & 1;   /* arithmetic right shift, thus shifting in either zeros or ones */
            b[row] = r << 1;    /* implicitly removes high bit because b[row] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
            b[row] ^= h * 0x1B; /* Rijndael's Galois field */
        }

        block[0 * nk + col] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
        block[1 * nk + col] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
        block[2 * nk + col] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
        block[3 * nk + col] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
    }
}
