
#if !defined(AES_HPP)
#define AES_HPP

#include "constants.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

class AES
{
public:
    typedef std::vector<uint8_t>::iterator iter;

    explicit AES(std::vector<uint8_t> key);
    void encrypt(std::vector<uint8_t> &plaintext);
    static std::vector<uint8_t> expand_key(std::vector<uint8_t> key);
    // rotates the word in-place
    static void rot_word(std::vector<uint8_t> &word);
    static void sub_word(std::vector<uint8_t> &word);
    static void rcon_word(std::vector<uint8_t> &word, int round);
    // xors second into first until last
    static void xor_words(iter first, iter last, iter second);
    static void mix_columns(std::vector<uint8_t> &block);
    static void shift_rows(std::vector<uint8_t> &block);

private:
    void add_round_key(std::vector<uint8_t> &block, int round);

private:
    // TODO: do I need it to be a member?
    std::vector<uint8_t> round_key;
};
#endif // AES_HPP