#ifndef _HUFFMAN_DECODE_H
#define _HUFFMAN_DECODE_H

#include <map>
#include <fstream>
#include <string.h>
#include "huffman.h"
#include "huffmantree.h"


class huffmanDecode {

};

bool getAlphaFreq(const char* filename) {

   
    return true;
}

bool decode(const Node* node, std::map<std::string, uchar>& decodeMap,
            const char* srcFilename, const char* destFilename) {
    std::ifstream is(srcFilename, std::ios::binary);
    if (!is) {
        printf("read file failed! filename: %s", srcFilename);
        return false;
    }

    return true;
}

static bool is_leaf(Node* n) {
    return n->left == nullptr && n->right == nullptr;
}

long getFileSize(const char* strFileName)
{
    std::ifstream in(strFileName);
    if (!in.is_open()) return 0;

    in.seekg(0, std::ios_base::end);
    std::streampos sp = in.tellg();
    in.close();
    return sp;
}

bool huffmanDecode(const char* srcFilename, const char* destFilename) {

    std::map<uchar, int> afMap;
    huffmanFileHead fileHead;
    long fileSize = getFileSize(srcFilename);
    std::ifstream is(srcFilename, std::ios::binary);
    if (!is) {
        printf("read file failed! filename: %s", srcFilename);
        return false;
    }
    
    is.read((char*)&fileHead, sizeof(fileHead));
    for (int i = 0; i < static_cast<int>(fileHead.alphaVariety); ++i) {
        alphaFreq af;
        is.read((char*)&af, sizeof(af));
        afMap.insert(std::pair<char, int>(af.alpha, af.freq));
    }

    huffmanTree htree(afMap);
    htree.watch();  
    std::map<uchar, std::string> codeMap;
    htree.huffmanCode(codeMap);

    std::map<std::string, uchar> decodeMap;
    for (auto it : codeMap) {
        decodeMap.insert(std::pair<std::string, uchar>(it.second, it.first));
    }

    Node node = htree.getHuffmanTree();
    Node* pNode = &node;

    std::ofstream io(destFilename, std::ios::binary);
    if (!io) {
        printf("create file failed! filename: %s", destFilename);
        return false;
    }
    
    uchar value;
    std::string code;
    int index = 0;
    long curLocation = is.tellg();
    is.read((char*)&value, sizeof(uchar));
    while (1) {
        if (is_leaf(pNode)) {
            uchar alpha = decodeMap[code];
            io.write((char*)&alpha, sizeof(uchar));
            if (curLocation >= fileSize && index >= fileHead.lastValidBit) {
                break;
            }
            code.clear();
            pNode = &node;
        }

        if (GET_BYTE(value, index)) {
            code += '1';
            pNode = pNode->right;
        } else {
            pNode = pNode->left;
            code += '0';
        }
        if (++index >= 8) {
            index = 0;
            is.read((char*)&value, sizeof(uchar));
            curLocation = is.tellg();
        }
    }
    
    is.close();
    io.close();
    return true;
}

#endif

