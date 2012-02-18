// XLStringUtil.h: XLStringUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
#define AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLStringUtil  
{
public:
	XLStringUtil();
	virtual ~XLStringUtil();


	//�݂�ȑ�D��PHP��strtoupper
	static std::string XLStringUtil::strtoupper(const std::string & str);

	//�݂�ȑ�D��PHP��strtolower
	static std::string XLStringUtil::strtolower(const std::string & str);

	//HTTP�w�b�_�̃L������
	static std::string XLStringUtil::HeaderUpperCamel(const std::string & str);

	//�݂�ȑ�D��PHP��join
	static std::string XLStringUtil::join(const std::string& glue , const std::map<std::string,std::string> & pieces );
	static std::string XLStringUtil::join(const std::string& glue , const std::list<std::string> & pieces );

	//key=value& �݂����Ȋ����� join
	static std::string XLStringUtil::crossjoin(const std::string& glue1 ,const std::string& glue2 , const std::map<std::string,std::string> & pieces );
	//split
	static std::list<std::string> XLStringUtil::split(const std::string& glue, const std::string & inTarget );
	//key=value& �݂����Ȋ����� join
	static std::map<std::string,std::string> XLStringUtil::crosssplit(const std::string& glue1 ,const std::string& glue2 , const std::string & inTarget );
	//stringmap ���m�̃}�[�W
	static std::map<std::string,std::string> XLStringUtil::merge(const std::map<std::string,std::string>& a ,const std::map<std::string,std::string>& b , bool overideB );

	//�݂�ȑ�D��PHP��urldecode
	static std::string XLStringUtil::urldecode(const std::string & inUrl);
	static std::string XLStringUtil::urlencode(const std::string &str) ;

	//�݂�ȑ�D��PHP��chop ���E�̋󔒂̏���
	static std::string XLStringUtil::chop(const std::string & str);

	//�Ȃ��� std::string �ɕW���ŗp�ӂ���Ă��Ȃ��u���B�ӂ������B
	static std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew);

	//�݂�ȑ�D��PHP��htmlspecialchars
	//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
	//http://search.net-newbie.com/php/function.htmlspecialchars.html
	static std::string XLStringUtil::htmlspecialchars(const std::string &inStr);
	static std::string XLStringUtil::getext(const std::string &inTarget);

	//�݂�ȑ�D��PHP��nl2br
	//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
	static std::string XLStringUtil::nl2br(const std::string &inStr);
	
	//�g���q���擾����. abc.cpp -> ".cpp" �̂悤�Ȋ����ɂȂ��
	static std::string XLStringUtil::baseext(const std::string &fullpath);
	//�x�[�X�f�B���N�g�����擾����  c:\\hoge\\hoge.txt -> c:\\hoge �ɂ���  �Ō��\\ �͏�����B
	static std::string XLStringUtil::basedir(const std::string &fullpath);
	//�t�@�C�������擾����  c:\\hoge\\hoge.txt -> hoge.txt
	static std::string XLStringUtil::basename(const std::string &fullpath);
	//���΃p�X�����΃p�X�ɕϊ�����
	static std::string XLStringUtil::pathcombine(const std::string& base,const std::string& dir,const std::string& pathsep = "");
	//�p�X�̋�؂蕶���𕽂�ɂ���.
	static std::string XLStringUtil::pathseparator(const std::string& path);
	//URL�p�����[�^�[�̒ǉ�.
	static std::string XLStringUtil::AppendURLParam(const std::string& url,const std::string& append);


	//inTarget �� inStart �` inEnd �܂ł��擾
	static std::string XLStringUtil::cut(const std::string &inTarget , const std::string & inStart , const std::string &inEnd , std::string * outNext );

	//�R�}���h���C�������p�[�X
	static std::list<std::string> XLStringUtil::parse_command(const std::string & command);

	//	static void test();

};

#endif // !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
