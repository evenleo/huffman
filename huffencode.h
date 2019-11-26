#ifndef _HUFF_ENCODE_H
#define _HUFF_ENCODE_H

#include <map>
#include <fstream>

#include "huffman.h"
#include "huffTree.h"

namespace huffman {

class huffEncode {
public:
	bool encode(const char* srcFilename, const char* destFilename) {
		if (!_getAlphaFreq(srcFilename)) return false;
		huffTree htree(_afMap);
		htree.huffmanCode(_codeMap);
		return _encode(srcFilename, destFilename);
	}
private:
	int _getLastValidBit() {
		int sum = 0;
		for (auto it : _codeMap) {
			sum += it.second.size() * _afMap.at(it.first);
			sum &= 0xFF;
		}
		sum &= 0x7;
		return sum == 0 ? 8 : sum;
	}
	bool _getAlphaFreq(const char* filename) {
		uchar ch;
		std::ifstream is(filename, std::ios::binary);
		if (!is.is_open()) {
            printf("read file failed! filename: %s", filename);
			return false;
		}
		is.read((char*)&ch, sizeof(uchar));
		while (!is.eof()) {
			_afMap[ch]++;
			is.read((char*)&ch, sizeof(uchar));
		}
		is.close();
		return true;
	}
	bool _encode(const char* srcFilename, const char* destFilename) {
		uchar ch;
		uchar value;
		int bitIndex = 0;
		fileHead filehead = {'e', 'v', 'e', 'n'};
		filehead.alphaVariety = (uchar) _afMap.size();
		filehead.lastValidBit = _getLastValidBit();

		std::ifstream is(srcFilename, std::ios::binary);
		if (!is.is_open()) {
            printf("read file failed! filename: %s", srcFilename);
			return false;
		}
		std::ofstream io(destFilename, std::ios::binary);
		if (!io.is_open()) {
            printf("read file failed! filename: %s", destFilename);
			return false;
		}

		io.write((char*)&filehead, sizeof(fileHead));
		for (auto i : _afMap) {
			alphaFreq af(i);
			io.write((char*)&af, sizeof(alphaFreq));
		}

		is.read((char*)&ch, sizeof(uchar));
		while (!is.eof()) {
			std::string code = _codeMap.at(ch);
			for (auto c : code) {
				if ('0' == c) {
					CLR_BYTE(value, bitIndex);
				} else {
					SET_BYTE(value, bitIndex);
				}
				++bitIndex;
				if (bitIndex >= 8) {
					bitIndex = 0;
					io.write((char*)&value, sizeof(uchar));
				}
			} 
			is.read((char*)&ch, sizeof(uchar));
		}

		if (bitIndex) {
			io.write((char*)&value, sizeof(uchar));
		}
		is.close();
		io.close();
		return true;
	}
private:
	std::map<uchar, int> _afMap;
	std::map<uchar, std::string> _codeMap;
};

}

#endif