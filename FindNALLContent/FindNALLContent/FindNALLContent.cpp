// FindNALLContent.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
#include<tchar.h>

#include<vector>

typedef unsigned char uint8;
using namespace std;

static int find_nal_prefix(FILE ** pFileIn, vector<uint8> &nallBytes)
{
    FILE* pFile = *pFileIn;
    /*
    * 寻找  00 00 00 01 xxxx  00 00 00 01
    */
    uint8 prefix[3] = { 0 };
    uint8 fileByte;

    /*
    * 查询开头 00 00 00 01
      [0][1][2] = {0  0  0}   -> [1][2][0] == {0 0 0}  -> [2][0][1] = {0,0,0} 
      getc() = 1

      //查询开头 00 00 01
      [0][1][2] = {0 0 1}   [1][2][0] = {0 0 1}  [2][0][1] = { 0  0 1 } 
    */
    nallBytes.clear();
    int pos = 0, getPrefix = 0;
    for (int idx = 0; idx < 3; idx++)
    {
        prefix[idx] = getc(pFile);
        nallBytes.push_back(prefix[idx]);
    }

    while (!feof(pFile))
    {
        if ((prefix[pos % 3] == 0) && (prefix[(pos + 1) % 3] == 0) && (prefix[(pos + 2) % 3] == 1))
        {
            //0x 00 00 01 found
            getPrefix = 1;
            nallBytes.pop_back();
            nallBytes.pop_back();
            nallBytes.pop_back();
            break;

        }
        else if((prefix[pos % 3] == 0) && (prefix[(pos + 1) % 3] == 0) && (prefix[(pos + 2) % 3] == 1))
        {
            if (1 == getc(pFile))
            {
                //0x 00 00 00 01 found
                getPrefix = 2;
                nallBytes.pop_back();
                nallBytes.pop_back();
                nallBytes.pop_back();;
                break;
            }
        }
        else
        {
            fileByte = getc(pFile);
            prefix[(pos++) % 3] = fileByte;
            nallBytes.push_back(fileByte);

        }
    } 
    

    return 0;
}


int _tmain(int argc, _TCHAR * argv[])
{
    //打开h.264文件
    FILE* pFile_in = NULL;
    _tfopen_s(&pFile_in, argv[1], _T("rb"));
    if (!pFile_in)
    {
        printf("Error: Opening input  file failed \n");
    
    }

    vector<uint8> nallBytes;

    find_nal_prefix(&pFile_in, nallBytes);

    for (int idx = 0; idx < nallBytes.size(); idx++)
    {
        printf("%x ", nallBytes.at(idx));
    }

    fclose(pFile_in);
    return 0;
}

