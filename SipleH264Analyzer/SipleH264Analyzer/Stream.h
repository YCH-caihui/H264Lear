#pragma once
#ifndef _STREAM_H
#define _STEAM_H
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include "global.h"
#include <vector>
#include "CNALUnit.h"


class CStreamFile
{
private:
	FILE* m_inputFile;
	TCHAR* m_filename;
	std::vector<uint8> m_nalVec;


	void file_info();
	void file_error(int idx);

	int  find_nal_prefix();

	void ebsp_to_sodb();



public:
	CStreamFile(TCHAR* fileName);


	//open api
	int parse_h264_bisttream();


	~CStreamFile();

};






#endif // _STREAM_H
