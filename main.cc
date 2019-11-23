// #pragma pack(push)
// #pragma pack(1)		//内存对其改为1个字节对齐模式

#include "huffmanencode.h";
#include "huffmandecode.h"

void visitCode(std::map<uchar, std::string>& codeMap) {
    for (auto m : codeMap) {
        printf("%c: %s\n", m.first, m.second.data());
    }
}

void testHuffmanTree() {
    std::string srcFilename = "lz.bmp";
    std::string destFilename = "lzeven.bmp";

    std::map<uchar, int> afMap;
    getAlphaFreq(srcFilename, afMap);

    huffmanTree htree(afMap);
    htree.watch();  
    printf("huffmanCode:\n");
    std::map<uchar, std::string> codeMap;
    htree.huffmanCode(codeMap);
    // visitCode(codeMap);
    std::cout << "" << std::endl;
    printf("write data to file!\n");
    
    huffmanEncode(srcFilename, destFilename, afMap, codeMap);
}

void testHuffmanDecode(const std::string& filename) {

    std::string destFilename = "de" + filename;
    huffmanDecode(filename.data(), destFilename.data());
}

int main()
{
    testHuffmanTree();
    testHuffmanDecode("lzeven.bmp");

    system("pause");
    return 0;
}

// #pragma pack(pop)
