#include "AES.hpp"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

// const std::string in_filename = "../aes_sample.in";
// const std::string out_filename = "../aes_sample.out";
constexpr int block_len = 16;

int main()
{
    // char key[block_len];
    // std::vector<char> key(block_len);
    std::vector<uint8_t> block(block_len);

    // std::ifstream input(in_filename, std::ifstream::binary);
    // std::ofstream output(out_filename, std::ofstream::binary);

    // reading the key
    std::cin.read((char *)block.data(), block_len);

    AES aes(block);

    int dim = constants::nk;

    // for (int i = 0; i < block_num; i++)
    while (true)
    {
        // input.read((char *)block.data(), block_len);
        // now we read column by column
        // TODO: get rid of unnamed constants
        for (int col = 0; col < dim; col++)
        {
            for (int row = 0; row < dim; row++)
            {
                std::cin.get((char &)block[row * dim + col]);
                if (std::cin.eof())
                {
                    return 0;
                }
            }
        }

        aes.encrypt(block);

        for (int col = 0; col < dim; col++)
        {
            for (int row = 0; row < dim; row++)
            {
                std::cout.put((char &)block[row * dim + col]);
            }
        }
    }

    // input.close();
    // output.close();

    return 0;
}
