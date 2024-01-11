#include <vector>
#include <cstring>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        error("Usage: ./compress <compressed_file> <file> ");
    }

    vector<int> freqs(256,0);
    FancyInputStream input(argv[1]);
    FancyOutputStream output(argv[2]);

    if(!input.good())
    {
        error("Input file not found.");
    }

    
    if(input.filesize() == 0){
        return 0;
    }

    /* Reading Efficient Header*/
    int count = input.read_int();
    unsigned char symbol = input.read_byte();
    while(true){
        freqs[+symbol] = count;
        count = input.read_int();
        if(count == -1){
            break;
        }
        symbol = input.read_byte();
    }

    /* Reading in Inefficient Header */
    // for(unsigned int i = 0; i < freqs.size();i++){
    //     int readInt = input.read_int();
    //     freqs[i] = readInt;
    // }

    HCTree tree;
    tree.build(freqs);
    // tree.treeTraversal();
    // Read in bit by bit, to slowly decode message using Huffman Tree
    // How do we know when we reached end of file? 
    while(true){
        int outputChar = tree.decode(input);
        if(outputChar == -1){
            break;
        }
        output.write_byte(outputChar);
    }

    return 0;
}

