#include <map>

//https://blog.csdn.net/weixin_38214171/article/details/81626498

typedef struct huffFileHead {
	unsigned char flag[3];				//压缩二进制文件头部标志 ycy
	unsigned char alphaVariety;		//字符种类
	unsigned char lastValidBit;		//最后一个字节的有效位数
	unsigned char unused[11];			//待用空间
} huffFileHead;								//这个结构体总共占用16个字节的空间

typedef struct ALPHA_FREQ {
	unsigned char alpha;		//字符,考虑到文件中有汉字，所以定义成unsigned char
	int freq;								//字符出现的频度
} ALPHA_FREQ;

typedef struct HUFFMAN_TAB {
	ALPHA_FREQ alphaFreq;
	int leftChild;
	int rightChild;
	boolean visited;
	char *code;
} HUFFMAN_TAB;

void huffmanEncode(const std::string& srcFilename, const std::string& destFilename
                   std::map<char, std::string>& codeMap) {
    FILE *fpIn;
	FILE *fpOut;
    huffFileHead fileHead = {'y', 'c', 'y'};
    fpIn = fopen(srcFilename.data(), "rb");
	fpOut = fopen(destFilename.data(), "wb");

    

}


//hufIndex下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标，形成键值对。在把字符串转换为哈夫曼编码时可以实现快速查找
void huffmanEncoding(HUFFMAN_TAB *huffmanTab, char *sourceFileName, char *targetFileName, int *hufIndex,
										 int alphaVariety, ALPHA_FREQ *alphaFreq) {
	FILE *fpIn;
	FILE *fpOut;
	int ch;
	unsigned char value;
	int bitIndex = 0;
	int i;
	char *hufCode = NULL;
	huffFileHead fileHead = {'y', 'c', 'y'};
	
	fpIn = fopen(sourceFileName, "rb");
	fpOut = fopen(targetFileName, "wb");
	
	fileHead.alphaVariety = (unsigned char) alphaVariety;
	fileHead.lastValidBit = getlastValidBit(huffmanTab, alphaVariety);
	 
//	size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
//	ptr -- 这是指向要被写入的元素数组的指针。
//	size -- 这是要被写入的每个元素的大小，以字节为单位。
//	nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
//	stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
	//给文件头部写入元数据
	fwrite(&fileHead, sizeof(huffFileHead), 1, fpOut);
	//给元数据后写字符种类和频度，解压缩时需要用这些生成一模一样新的哈夫曼树
	fwrite(alphaFreq, sizeof(ALPHA_FREQ), alphaVariety, fpOut);
	
	ch = fgetc(fpIn);
	while(!feof(fpIn)) {
		hufCode = huffmanTab[hufIndex[ch]].code;
		//把每个字符的哈夫曼编码一个一个过。
		//如果是字符'0'，就转换为二进制的0
		//如果是字符'1'，就转换为二进制的1
		for(i = 0; hufCode[i]; i++) {
			if('0' == hufCode[i]) {
				//value为一个字节
				//从第1位依次赋值，若大于八位（一个字节）了，就写入文件中
				CLR_BYTE(value, bitIndex);
			} else {
				SET_BYTE(value, bitIndex);
			}
			bitIndex++;
			if(bitIndex >= 8) {
				bitIndex = 0;
				fwrite(&value, sizeof(unsigned char), 1, fpOut);
			}
		}
		ch = fgetc(fpIn);
	}
	//如果最后一次不满一个字节，依然需要写到文件中，注意：写入的最后一个字节可能会存在垃圾位
	if(bitIndex) {
		fwrite(&value, sizeof(unsigned char), 1, fpOut);
	}
	
	fclose(fpIn);
	fclose(fpOut);
}