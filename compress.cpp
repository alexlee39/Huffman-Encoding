#include <vector>
#include <cstring>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;


/**We will use nonzero counts for our frequency vector when building
 * Huffman Tree, but we need to find an optimal way to find those frequencies
 * in the respective frequency vector for the Huffman Tree building process,
 * when encoding those bytes and outputting it. 
 * 
 * On empty file, when we compress, we dont need the huffman tree, because 
 */
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        error("Usage: ./compress <original_file> <compressed_file> ");
    }
    vector<int> freqs(256,0);
    vector<int> nonZeroFreqs;
    FancyInputStream input(argv[1]);
    FancyOutputStream output(argv[2]);

    if(!input.good())
    {
        error("Input file not found.");
    }

    if(input.filesize() == 0){
        return 0;
    }

    int readByte = input.read_byte();
    while(readByte != -1)
    {
        //Count occurences of each byte 
        freqs[readByte] += 1;
        readByte = input.read_byte();
    }
    
    for(unsigned int i = 0; i < freqs.size(); i++)
    {
        //Write header for output to store the frequencies for HCTree
        /* Inefficient Header */
        // output.write_int(freqs[i]);

        /* Store nonZeroFreqs for Efficient Header*/
        if(freqs[i] > 0){
            // nonZeroFreqs.push_back(i);
            output.write_int(freqs[i]); //Write the count
            output.write_byte((unsigned char)i);
        }
    }
    output.write_int(-1);
    /* Efficient Header
     * Use nonZeroFreqs, and write out 1 byte for each symbol, and write out an 
     * int for their count.
     * Byte-Symbol 
     * Note: This header is only effective for files with less than 200 unique bytes
     * Note: Write a -1 at the end of header to show where huffman Tree ends?
    */

    // if(nonZeroFreqs.size() < 200){
        // for(unsigned int i = 0; i < nonZeroFreqs.size(); i++){
        //     output.write_int(i);
        //     output.write_byte((unsigned char)i);
        // }
        // output.write_int(-1);
    // }
    // else{
    //     for(unsigned int i = 0; i < freqs.size(); i++)
    //     {
    //         //Write header for output to store the frequencies for HCTree
    //         /* Inefficient Header */
    //         output.write_int(freqs[i]);
    //     }
    // }
    HCTree tree;
    tree.build(freqs);
    // tree.treeTraversal();
    input.reset();
    readByte = input.read_byte();
    while(readByte != -1)
    {
        tree.encode(readByte,output);
        readByte = input.read_byte();
    }
    return 0;
}
