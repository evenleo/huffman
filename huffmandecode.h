#ifndef _HUFFMAN_DECODE_H
#define _HUFFMAN_DECODE_H

#include <map>
#include <fstream>
#include <string.h>
#include "huffman.h"
#include "huffmantree.h"

static bool is_leaf(Node* n) {
    return n->left == nullptr && n->right == nullptr;
}

class huffmanDecode {
public:
    huffmanDecode() {
        _fileHead = new huffmanFileHead();
    }
    ~huffmanDecode() {
        delete _fileHead;
        delete _htree;
    }
    

private:
    long getFileSize(const char* strFileName) {
        std::ifstream in(strFileName);
        if (!in.is_open()) return 0;

        in.seekg(0, std::ios_base::end);
        std::streampos sp = in.tellg();
        in.close();
        return sp;
    }
    bool getAlphaFreq(const char* filename) {
        std::ifstream is(filename, std::ios::binary);
        if (!is) {
            printf("read file failed! filename: %s", filename);
            return false;
        }
        
        is.read((char*)_fileHead, sizeof(huffmanFileHead));
        for (int i = 0; i < static_cast<int>(_fileHead->alphaVariety); ++i) {
            alphaFreq af;
            is.read((char*)&af, sizeof(af));
            _afMap.insert(std::pair<char, int>(af.alpha, af.freq));
        }
        is.close();
        return true;
    }
    bool _decode(const char* srcFilename, 
                 const char* destFilename) {
        long fileSize = getFileSize(srcFilename);
        
        std::ifstream is(srcFilename, std::ios::binary);
        if (!is) {
            printf("read file failed! filename: %s", srcFilename);
            return false;
        }
        is.seekg(sizeof(huffmanFileHead) + sizeof(alphaFreq) * _fileHead->alphaVariety);

        Node node = _htree->getHuffmanTree();
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
                uchar alpha = _decodeMap[code];
                io.write((char*)&alpha, sizeof(uchar));
                if (curLocation >= fileSize && index >= _fileHead->lastValidBit) {
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
public:
    bool decode(const char* srcFilename, const char* destFilename) {

        getAlphaFreq(srcFilename);

        long fileSize = getFileSize(srcFilename);
        _htree = new huffmanTree(_afMap);
        _htree->watch();  
        _htree->huffmanCode(_codeMap);

        for (auto it : _codeMap) {
            _decodeMap.insert(std::pair<std::string, uchar>(it.second, it.first));
        }

        return _decode(srcFilename, destFilename);
    }
private:
    huffmanFileHead *_fileHead;
    huffmanTree *_htree;
    std::map<uchar, int> _afMap;
    std::map<uchar, std::string> _codeMap;
    std::map<std::string, uchar> _decodeMap;
};

#endif

