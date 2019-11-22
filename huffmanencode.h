#include <map>

#include "huffman.h"

//https://blog.csdn.net/weixin_38214171/article/details/81626498


void getAlphaFreq(const std::string& filename, 
                  std::map<char, int>& afMap) {
	FILE *fpIn = fopen(filename.data(), "rb");
	int ch = fgetc(fpIn);
	while (!feof(fpIn)) {
		afMap[ch]++;
		ch = fgetc(fpIn);
	}
	fclose(fpIn);
}

int getlastValidBit(const std::map<char, int>& afMap, 
                    const std::map<char, std::string>& codeMap) {
	int sum = 0;
	for (auto it : codeMap) {
		sum += it.second.size() * afMap.at(it.first);
		sum &= 0xFF;
	}
	sum &= 0x7;
	return sum == 0 ? 8 : sum;
}

void huffmanEncode(const std::string& srcFilename, 
                   const std::string& destFilename,
				   const std::map<char, int>& afMap, 
                   const std::map<char, std::string>& codeMap) {
    FILE *fpIn;
	FILE *fpOut;
	int ch;
	unsigned char value;
	int bitIndex = 0;
    huffmanFileHead fileHead = {'e', 'v', 'e', 'n'};
	fileHead.alphaVariety = (unsigned char) afMap.size();
	fileHead.lastValidBit = getlastValidBit(afMap, codeMap);

    fpIn = fopen(srcFilename.data(), "rb");
	fpOut = fopen(destFilename.data(), "wb");

	fwrite(&fileHead, sizeof(fileHead), 1, fpOut);
	for (auto i : afMap) {
		alphaFreq af(i);
		fwrite(&af, sizeof(af), 1, fpOut);
	}

	ch = fgetc(fpIn);
	while (!feof(fpIn)) {
		std::string code = codeMap.at(ch);
		for (auto c : code) {
			if ('0' == c) {
				CLR_BYTE(value, bitIndex);
			} else {
				SET_BYTE(value, bitIndex);
			}
			++bitIndex;
			if (bitIndex >= 8) {
				bitIndex = 0;
				fwrite(&value, sizeof(unsigned char), 1, fpOut);
			}
		} 
		ch = fgetc(fpIn);
	}
	if (bitIndex) {
		fwrite(&value, sizeof(unsigned char), 1, fpOut);
	}

	fclose(fpIn);
	fclose(fpOut);
}
