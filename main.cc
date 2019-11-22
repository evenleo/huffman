#include "huffmantree.h"

void visitCode(std::map<char, std::string>& codeMap) {
    for (auto m : codeMap) {
        std::cout << m.first << ":" << m.second << std::endl;
    }
}

void testHuffmanTree() {
    huffmanTree htree = {   {'A', 20},
                            {'B', 30},
                            {'C', 60},
                            {'D', 120},
                            {'E', 350},
                            {'F', 450}
                        };
    htree.watch();  
    std::cout << "huffmanCode:" << std::endl;
    std::map<char, std::string> codeMap;
    htree.huffmanCode(codeMap);
    visitCode(codeMap);
}

int main()
{
    testHuffmanTree();

    system("pause");
    return 0;
}