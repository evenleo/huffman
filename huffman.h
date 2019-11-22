#ifndef _HUFFMAN_H
#define _HUFFMAN_H

#include <string>

//得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))


struct huffmanFileHead {
	unsigned char flag[4];				//压缩二进制文件头部标志 ycy
	unsigned char alphaVariety;		//字符种类
	unsigned char lastValidBit;		//最后一个字节的有效位数
	unsigned char unused[11];			//待用空间
};								//这个结构体总共占用16个字节的空间

struct alphaFreq {
	unsigned char alpha;		//字符,考虑到文件中有汉字，所以定义成unsigned char
	int freq;	               //字符出现的频度
    alphaFreq() {}
	alphaFreq(const std::pair<char, int>& x) 
	  : alpha(x.first), freq(x.second) {}	
};

bool isFileExist(const std::string& filename) {
	FILE *fp;
	fp = fopen(filename.data(), "rb");
	if (nullptr == fp) {
		return false;
	}
	fclose(fp);
	return true;
}


#endif