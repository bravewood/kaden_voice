// XLFileUtil.cpp: XLFileUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "XLFileUtil.h"
#include "XLStringUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLFileUtil::XLFileUtil()
{

}

XLFileUtil::~XLFileUtil()
{

}


//�t�@�C�������݂��邩?
bool XLFileUtil::Exist(const std::string & inFileName)
{
	return (0xFFFFFFFF != GetFileAttributes(XLStringUtil::pathseparator(inFileName).c_str() ));
}

//�폜
bool XLFileUtil::unlink(const std::string & inFileName)
{
	::unlink(XLStringUtil::pathseparator(inFileName).c_str());
	return true;
}
bool XLFileUtil::copy(const std::string & inFileNameA,const std::string & inFileNameB)
{
//	::copy(inFileNameA.c_str(),inFileNameB.c_str());
	::CopyFile(XLStringUtil::pathseparator(inFileNameA).c_str(),XLStringUtil::pathseparator(inFileNameB).c_str(),TRUE);
	return true;
}
bool XLFileUtil::move(const std::string & inFileNameA,const std::string & inFileNameB)
{
	::rename(XLStringUtil::pathseparator(inFileNameA).c_str(),XLStringUtil::pathseparator(inFileNameB).c_str());
	return true;
}


//�t�@�C�������ׂ� std::string �ɓǂݍ���.
std::string XLFileUtil::cat(const std::string & inFileName)
{
	std::vector<char> out;
	FILE * fp = fopen(inFileName.c_str() , "rb");
	//���݂��Ȃ��ꍇ�͋�
	if (fp == NULL) return "";

	//�P�c�Ɏ����Ă�����.
	fseek(fp , 0 ,SEEK_END);

	//����ŃT�C�Y���킩��.
	unsigned long size = ftell(fp);

	//�擪�ɖ߂�.
	fseek(fp , 0 ,SEEK_SET);

	//�̈���m�ۂ��ēǂݍ���
	out.resize(size + 1);
	fread( &out[0] , 1 , size  , fp);
	out[size] = '0';	//�I�[.

	fclose(fp);

	return (const char*) (&out[0]);
}

//�t�@�C�������ׂ� std::vector<char> �ɓǂݍ���.
std::vector<char> XLFileUtil::cat_b(const std::string & inFileName)
{
	std::vector<char> out;
	cat_b(inFileName,&out);
	return out;
}

//�t�@�C�������ׂ� std::vector<char> �ɓǂݍ���.
bool XLFileUtil::cat_b(const std::string & inFileName , std::vector<char>* out)
{
	out->clear();

	FILE * fp = fopen(inFileName.c_str() , "rb");
	//���݂��Ȃ��ꍇ�͋�
	if (fp == NULL) return false;

	//�P�c�Ɏ����Ă�����.
	fseek(fp , 0 ,SEEK_END);

	//����ŃT�C�Y���킩��.
	unsigned long size = ftell(fp);

	//�擪�ɖ߂�.
	fseek(fp , 0 ,SEEK_SET);

	//�̈���m�ۂ��ēǂݍ���
	out->resize(size);
	fread( &((*out)[0]) , 1 , size  , fp);
	
	fclose(fp);

	return true;
}


//inStr �� �t�@�C���ɏ�������
bool XLFileUtil::write(const std::string & inFileName,const std::string & inStr )
{
	return write( inFileName , inStr.c_str() , inStr.size());
}

//inBuffer �� �t�@�C���ɏ�������
bool XLFileUtil::write(const std::string & inFileName,const std::vector<char> & inBuffer)
{
	return write(inFileName,&inBuffer[0] ,  inBuffer.size());
}
//inBuffer �� �t�@�C���ɏ�������
bool XLFileUtil::write(const std::string & inFileName,const char* data , int size)
{
	FILE * fp = fopen(inFileName.c_str() , "wb");
	if (fp == NULL)
	{
		assert(0);
		return false;
	}

	fwrite( data , 1, size  , fp);

	fclose(fp);
	return true;
}