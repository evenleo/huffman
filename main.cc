#include "huffencode.h";
#include "huffdecode.h"

using namespace huffman;

void testHuffTree(const std::string& filename) {
    std::string destFilename = "en" + filename;
    huffEncode he;
    he.encode(filename.data(), destFilename.data());
}

void testHuffDecode(const std::string& filename) {
    std::string destFilename = "de" + filename;
    huffDecode hd;
    hd.decode(filename.data(), destFilename.data());
}

int main()
{
    std::string filename = "lz.bmp";
    // testHuffTree(filename);
    // testHuffDecode("en" + filename);

    system("pause");
    return 0;
}
