// XLFileUtil.h: XLFileUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_)
#define AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLFileUtil  
{
public:
	XLFileUtil();
	virtual ~XLFileUtil();

	//�t�@�C�������݂��邩?
	static bool XLFileUtil::Exist(const std::string & inFileName);
	//�폜
	static bool XLFileUtil::unlink(const std::string & inFileName);
	static bool XLFileUtil::copy(const std::string & inFileNameA,const std::string & inFileNameB);
	static bool XLFileUtil::move(const std::string & inFileNameA,const std::string & inFileNameB);
	//�t�@�C�������ׂ� string �ɓǂݍ���.
	static std::string XLFileUtil::cat(const std::string & inFileName);
	//�t�@�C�������ׂ� std::vector<char> �ɓǂݍ���.
	static std::vector<char> XLFileUtil::cat_b(const std::string & inFileName);
	//�t�@�C�������ׂ� out �ɓǂݍ���
	static bool XLFileUtil::cat_b(const std::string & inFileName , std::vector<char>* out);

	//inStr �� �t�@�C���ɏ�������
	static bool XLFileUtil::write(const std::string & inFileName,const std::string & inStr );
	//inBuffer �� �t�@�C���ɏ�������
	static bool XLFileUtil::write(const std::string & inFileName,const std::vector<char> & inBuffer);
	//inBuffer �� �t�@�C���ɏ�������
	static bool XLFileUtil::write(const std::string & inFileName,const char* data , int size);

};

#endif // !defined(AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_)
