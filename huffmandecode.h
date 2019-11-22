#ifndef _HUFFMAN_DECODE_H
#define _HUFFMAN_DECODE_H

#include <map>
#include <fstream>
#include <string.h>
#include "huffmantree.h"
#include "huffman.h"



bool huffmanDecode(const char* filename) {

    char flag[5];
    memset(flag, 0, sizeof(flag));
    unsigned char alphaVariety;
    unsigned char lastValidBit;
    std::map<char, int> afMap;

    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        printf("read file failed! filename: %s", filename);
        return false;
    }
    
    in.read(flag, sizeof(flag) - 1);
    in.read((char*)&alphaVariety, sizeof(alphaVariety));
    in.read((char*)&lastValidBit, sizeof(lastValidBit));
    for (int i = 0; i < static_cast<int>(alphaVariety); ++i) {
        alphaFreq af;
        in.read((char*)&af, sizeof(af));
        afMap.insert(std::pair<char, int>(af.alpha, af.freq));
    }


    in.close();

    printf("%s\n", flag);
    printf("map size: %d\n", afMap.size());

    


}

#endif