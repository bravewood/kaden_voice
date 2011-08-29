// RStdioFile.cpp: RStdioFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStdioFile::RStdioFile()
{
	FilePointer = NULL;
}

RStdioFile::~RStdioFile()
{
	Close();
}

//�J��.
void RStdioFile::Open(string inFilename,string inMode) throw (RIOCannotOpenException)
{
	Close();

	FilePointer = ::fopen(inFilename.c_str() , inMode.c_str() );
	if (!FilePointer)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "�t�@�C�� " + inFilename + " ���J���܂���" );
	}

#ifndef _WIN32_WCE
	Buffer.reserve(32768);
	setvbuf(FilePointer , &Buffer[0] , _IOFBF , 32768);
#endif
}

//����.
void RStdioFile::Close()
{
	if (FilePointer)
	{
		fclose(FilePointer);
		FilePointer = NULL;
	}
}


//�ǂݍ���.
string RStdioFile::ReadLine() throw (RIOCannotReadException)
{
	ASSERT(FilePointer != NULL);

	string ret;

	char buf[MAX_PATH * 10];
	while(!feof(FilePointer))
	{
		if (  ::fgets( buf , MAX_PATH * 10, FilePointer) == NULL)
		{
			if (feof(FilePointer))	break;	//����I��.
			throw RIOCannotReadException("�ǂݍ��ݒ��ɃG���[���������܂���");
		}

		int skip = ret.size() ;

		//�ǂݍ��񂾃o�b�t�@�ɘA��.
		ret += buf;
		//���s������܂���?
		if ( ret.find( string("\r\n") , skip) != -1) break;
	}
	return ret;
}

//��������
void RStdioFile::WriteLine(string inWrittingString) throw (RIOCannotWriteException)
{
	ASSERT(FilePointer != NULL);

	int count = ::fwrite( inWrittingString.c_str() , sizeof(char) , inWrittingString.size(), FilePointer);
	if (count != inWrittingString.size())
		throw RIOCannotWriteException("�������ݒ��ɃG���[���������܂���");
}

//����������?
bool RStdioFile::IsEof()
{
	return feof(FilePointer) != 0;
}
