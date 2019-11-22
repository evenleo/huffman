#include "huffmantree.h"
#include "huffmanencode.h";
#include "huffmandecode.h"

void visitCode(std::map<char, std::string>& codeMap) {
    for (auto m : codeMap) {
        printf("%c: %s\n", m.first, m.second.data());
    }
}

void testHuffmanTree() {
    std::string srcFilename = "lz.bmp";
    std::string destFilename = "lzeven.bmp";

    std::map<char, int> afMap;
    getAlphaFreq(srcFilename, afMap);

    huffmanTree htree(afMap);
    htree.watch();  
    std::cout << "huffmanCode:" << std::endl;
    std::map<char, std::string> codeMap;
    htree.huffmanCode(codeMap);
    visitCode(codeMap);
    std::cout << "write data to file!" << std::endl;
    
    // huffmanEncode(srcFilename, destFilename, afMap, codeMap);
}


int main()
{
    // testHuffmanTree();
    huffmanDecode("lzeven.bmp");
    system("pause");
    return 0;
}