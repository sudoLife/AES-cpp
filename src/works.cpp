#include "AES.hpp"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

const std::string in_filename = "../aes_sample.in";
const std::string out_filename = "../aes_sample.out";
constexpr int block_len = 16;

int main()
{
    // char key[block_len];
    // std::vector<char> key(block_len);
    std::vector<uint8_t> block(block_len);

    std::ifstream input(in_filename, std::ifstream::binary);
    std::ofstream output(out_filename, std::ofstream::binary);

    // reading the key
    input.read((char *)block.data(), block_len);

    AES aes(block);

    // count the number of bytes in the file
    auto block_start = input.tellg();

    input.seekg(0, std::ifstream::end);

    auto block_end = input.tellg();

    input.clear();
    input.seekg(block_start, std::ifstream::beg);

    int block_num = (block_end - block_start) / block_len;

    std::cout << "Blocks to encrypt: " << block_num << '\n';
    int dim = constants::nk;

    for (int i = 0; i < block_num; i++)
    {
        // input.read((char *)block.data(), block_len);
        // now we read column by column
        // TODO: get rid of unnamed constants
        for (int col = 0; col < dim; col++)
        {
            for (int row = 0; row < dim; row++)
            {
                input.get((char &)block[row * dim + col]);
            }
        }

        aes.encrypt(block);

        for (int col = 0; col < dim; col++)
        {
            for (int row = 0; row < dim; row++)
            {
                output.put((char &)block[row * dim + col]);
            }
        }
        // output.write((char *)block.data(), block_len);
    }

    input.close();
    output.close();

    return 0;
}
