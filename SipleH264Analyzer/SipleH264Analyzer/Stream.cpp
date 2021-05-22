#include "Stream.h"

using namespace std;

CStreamFile::CStreamFile(TCHAR* fileName)
{
	m_filename = fileName;
	file_info();
	_tfopen_s(&m_inputFile, m_filename, _T("rb"));
	if (NULL == m_inputFile)
	{
		file_error(0);
	}

}

void CStreamFile::file_info()
{
	if (m_filename)
	{
		wcout << L"File name:" << m_filename << endl;
	}
}


void CStreamFile::file_error(int idx)
{
	switch (idx)
	{
	case 0:
		wcout << L"Error: opening input file failed" << endl;
		break;
	}
}


int CStreamFile::find_nal_prefix()
{
    FILE* pFile = m_inputFile;
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
    m_nalVec.clear();
    int pos = 0, getPrefix = 0;
    for (int idx = 0; idx < 3; idx++)
    {
        prefix[idx] = getc(pFile);
        m_nalVec.push_back(prefix[idx]);
    }

    while (!feof(pFile))
    {
        if ((prefix[pos % 3] == 0) && (prefix[(pos + 1) % 3] == 0) && (prefix[(pos + 2) % 3] == 1))
        {
            //0x 00 00 01 found
            getPrefix = 1;
            m_nalVec.pop_back();
            m_nalVec.pop_back();
            m_nalVec.pop_back();
            break;

        }
        else if ((prefix[pos % 3] == 0) && (prefix[(pos + 1) % 3] == 0) && (prefix[(pos + 2) % 3] == 1))
        {
            if (1 == getc(pFile))
            {
                //0x 00 00 00 01 found
                getPrefix = 2;
                m_nalVec.pop_back();
                m_nalVec.pop_back();
                m_nalVec.pop_back();;
                break;
            }
        }
        else
        {
            fileByte = getc(pFile);
            prefix[(pos++) % 3] = fileByte;
            m_nalVec.push_back(fileByte);

        }
    }


    return getPrefix;
}


int CStreamFile::parse_h264_bisttream()
{
    int ret = 0;
    do
    {
        ret = find_nal_prefix();
        //解析nall unit
        if (m_nalVec.size())
        {
            uint8 nalType = m_nalVec[0] & 0x1F;
            wcout << L"Nall unity type:" << nalType << endl; 
            ebsp_to_sodb();
        }

    } while (ret);
    return 0;
}


void CStreamFile::ebsp_to_sodb()
{
    // 00 00 xxxxxx 03
    if (m_nalVec.size() < 3)
    {
        return;
    }

    for (vector<uint8>::iterator itor = m_nalVec.begin() + 2; itor != m_nalVec.end(); )
    {
        if ((3 == *itor) && (0 == *(itor - 1)) && (0 == *(itor - 2)))
        {
            vector<uint8>::iterator temp = m_nalVec.erase(itor);
            itor = temp;
        }
        else 
        {
            itor++;
        }
    }
}

CStreamFile::~CStreamFile()
{
	if (NULL != m_inputFile)
	{
		fclose(m_inputFile);
		m_inputFile = nullptr;
	}
}