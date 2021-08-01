// ExpColum.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <tchar.h>
#include <assert.h>

typedef unsigned char UINT8;



static int get_bit_at_position(UINT8* buf, UINT8& bytepotion, UINT8& bitPosition)
{
	UINT8 mask = 0, val = 0;

	mask = 1 << (7 - bitPosition);
	val = ((buf[bytepotion] & mask) != 0);
	if (++bitPosition > 7)
	{
		bytepotion++; 
		bitPosition = 0;
	}
	return val;
}


static int get_uev_code_num(UINT8* buf, UINT8& bytePosition, UINT8& bitPosition)
{
	assert(bitPosition < 8);
	UINT8 val = 0, prefixZeroCount = 0;
	int prefix = 0, surfaix = 0;

	while (true)
	{
		val = get_bit_at_position(buf, bytePosition, bitPosition);
		if (val == 0)
		{
			prefixZeroCount++;
		}
		else
		{
			break;
		}
	}

	prefix = (1 << prefixZeroCount) - 1;
	for (size_t i = 0; i < prefixZeroCount; i++)
	{
		val = get_bit_at_position(buf, bytePosition, bitPosition);
		surfaix += val * (1 << (prefixZeroCount - i - 1));
	}

	return val;
}


int _tmain(int argc, _TCHAR* argv[])
{
	UINT8 strArray[6] = { 0xA6, 0x42, 0x98, 0xE2, 0x04, 0x8A };
	UINT8 bytePosition = 0, bitPosition = 0;
	UINT8 dataLengthInBits = sizeof(strArray) / sizeof(UINT8) * 8;

	int codeNum = 0;

	while ((bytePosition * 8 + bitPosition) < dataLengthInBits)
	{
		codeNum = get_uev_code_num(strArray, bytePosition, bitPosition);
		printf("ExpoColumb codeNum= %d \n", codeNum);
	}

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
