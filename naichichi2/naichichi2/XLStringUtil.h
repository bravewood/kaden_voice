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

	static int XLStringUtil::atoi(const std::string & str);

	//HTTP�w�b�_�̃L������
	static std::string XLStringUtil::HeaderUpperCamel(const std::string & str);

	//���̕������
	static inline const char* XLStringUtil::nextChar(const char * p)
	{
	#ifdef _WINDOWS
		if (((0x81 <= (unsigned char)(*p) && (unsigned char)(*p) <= 0x9f) || (0xe0 <= (unsigned char)(*p) && (unsigned char)(*p) <= 0xfc)))
		{
			return p + 2;
		}
		return p + 1;
	#else
		if ( (((unsigned char)*p) & 0x80) == 0) return p + 1; 
		if ( (((unsigned char)*p) & 0x20) == 0) return p + 2;
		if ( (((unsigned char)*p) & 0x10) == 0) return p + 3;
		if ( (((unsigned char)*p) & 0x08) == 0) return p + 4;
		if ( (((unsigned char)*p) & 0x04) == 0) return p + 5;
		return p + 6;
	#endif
	}
	//���̕������
	static inline char* XLStringUtil::nextChar(char * p)
	{
		return (char*)nextChar((const char*)p);
	}
	//�}���`�o�C�g���H
	static inline bool XLStringUtil::isMultiByte(const char * p)
	{
	#ifdef _WINDOWS
		return ((0x81 <= (unsigned char)(*p) && (unsigned char)(*p) <= 0x9f) || (0xe0 <= (unsigned char)(*p) && (unsigned char)(*p) <= 0xfc));
	#else
		return ( (((unsigned char)*p) & 0x80) != 0); 
	#endif
	}

	//strstr�̃}���`�o�C�g�Z�[�t �����񌟍�
	static const char* XLStringUtil::strstr(const std::string& target, const std::string & need );
	//strstr�̃}���`�o�C�g�Z�[�t �����񌟍�
	static const char* XLStringUtil::strstr(const char* target, const char* need );
	//strstr�̃}���`�o�C�g�Z�[�t �����񌟍� //�኱�����
	static const char* XLStringUtil::strrstr(const std::string& target, const std::string & need );
	//strstr�̃}���`�o�C�g�Z�[�t �����񌟍� //�኱�����
	static const char* XLStringUtil::strrstr(const char* target, const char* need );
	//stristr�̃}���`�o�C�g�Z�[�t �啶���������֌W�Ȃ��̌���
	static const char* XLStringUtil::stristr(const std::string& target, const std::string & need );
	//strchr�̃}���`�o�C�g�Z�[�t ������̍ŏ��猟�����čŏ����Ɏg���������̈ʒu
	static const char* XLStringUtil::strchr(const std::string& target, char need );
	//strchr�̃}���`�o�C�g�Z�[�t ������̍ŏ��猟�����čŏ����Ɏg���������̈ʒu
	static const char* XLStringUtil::strchr(const char* target, char need );
	//strrchr�̃}���`�o�C�g�Z�[�t ������̌�납�猟�����čŏ����Ɏg���������̈ʒu
	static const char* XLStringUtil::strrchr(const std::string& target, char need );
	//strrchr�̃}���`�o�C�g�Z�[�t ������̌�납�猟�����čŏ����Ɏg���������̈ʒu
	static const char* XLStringUtil::strrchr(const char* target, char need );
	//strpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
	//�ŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
	static int XLStringUtil::strpos(const std::string& target, const std::string & need );
	//strpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
	//�ŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
	static int XLStringUtil::strpos(const char* target, const char* need );
	//strrpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
	//�t���猟�����čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
	static int XLStringUtil::strrpos(const std::string& target, const std::string & need );
	//strrpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
	//�t���猟�����čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
	static int XLStringUtil::strrpos(const char* target, const char* need );
	//stripos�̃}���`�o�C�g�Z�[�t (stripos��PHP�ɃA�����)
	//�啶���������֌W�Ȃ��Ō������čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
	static int XLStringUtil::stripos(const std::string& target, const std::string & need );

	//�݂�ȑ�D��PHP��join
	static std::string XLStringUtil::join(const std::string& glue , const std::map<std::string,std::string> & pieces );
	static std::string XLStringUtil::join(const std::string& glue , const std::list<std::string> & pieces );
	//template ���Ēm���Ă邯�ǁA�Ƃ肠������ŁB
	static std::string XLStringUtil::join(const std::string& glue , const std::vector<std::string> & pieces );

	//key=value& �݂����Ȋ����� join
	static std::string XLStringUtil::crossjoin(const std::string& glue1 ,const std::string& glue2 , const std::map<std::string,std::string> & pieces );
	//split
	static std::list<std::string> XLStringUtil::split(const std::string& glue, const std::string & inTarget );
	//vector
	static std::vector<std::string> XLStringUtil::split_vector(const std::string& glue, const std::string & inTarget );
	//key=value& �݂����Ȋ����� join
	static std::map<std::string,std::string> XLStringUtil::crosssplit(const std::string& glue1 ,const std::string& glue2 , const std::string & inTarget );
	//stringmap ���m�̃}�[�W
	static std::map<std::string,std::string> XLStringUtil::merge(const std::map<std::string,std::string>& a ,const std::map<std::string,std::string>& b , bool overideB );

	//�݂�ȑ�D��PHP��urldecode
	static std::string XLStringUtil::urldecode(const std::string & inUrl);
	static std::string XLStringUtil::urlencode(const std::string &str) ;
	//base64�G���R�[�h
	static std::string XLStringUtil::base64encode(const std::string& src) ;
	//base64�G���R�[�h
	static std::string XLStringUtil::base64encode(const char* src,int len) ;
	//base64�f�R�[�h
	static std::string XLStringUtil::base64decode(const std::string& src) ;
	//base64�f�R�[�h
	static void XLStringUtil::base64decode(const std::string& src ,std::vector<char>* out) ;
	//���񂵂āA�֐� func ��K���B true ��Ԃ������̂�������������B
	static std::string XLStringUtil::string_filter(const std::list<std::string>& list,const std::function<bool(const std::string&)>& func);
	//���񂵂āA�֐� func ��K���B func�̖߂蕶������������܂��B
	static std::string XLStringUtil::string_map(const std::list<std::string>& list,const std::function<std::string (const std::string&)>& func);
	//���񂵂āA�֐� func ��K���B true ��Ԃ������̂�����Ԃ��B
	static std::list<std::string> XLStringUtil::array_filter(const std::list<std::string>& list,const std::function<bool(const std::string&)>& func);
	//���񂵂āA�֐� func ��K���B func�̖߂�Ŕz�����ĕԂ��܂��B
	static std::list<std::string> XLStringUtil::array_map(const std::list<std::string>& list,const std::function<std::string (const std::string&)>& func);


	//�݂�ȑ�D��PHP��chop ���E�̋󔒂̏���
	static std::string XLStringUtil::chop(const std::string & str,const char * replaceTable = NULL);

	//�Ȃ��� std::string �ɕW���ŗp�ӂ���Ă��Ȃ��u���B�ӂ������B
	static std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew);
	//�}���`�o�C�g��Ή� �̕�����u��
	static std::string XLStringUtil::replace_low(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew);

	//�݂�ȑ�D��PHP��htmlspecialchars
	//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
	//http://search.net-newbie.com/php/function.htmlspecialchars.html
	static std::string XLStringUtil::htmlspecialchars(const std::string &inStr);
	//�}���`�o�C�g��Ή� �^�O���G�X�P�[�v
	static std::string XLStringUtil::htmlspecialchars_low(const std::string &inStr);

	//�݂�ȑ�D��PHP��nl2br
	//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
	static std::string XLStringUtil::nl2br(const std::string &inStr);
	//�}���`�o�C�g��Ή� \n��<br>��
	static std::string XLStringUtil::nl2br_low(const std::string &inStr);
	
	//�g���q���擾����. abc.cpp -> ".cpp" �̂悤�Ȋ����ɂȂ��
	static std::string XLStringUtil::baseext(const std::string &fullpath);
	//�g���q���擾����. abc.cpp -> "cpp" �̂悤�Ȋ����ɂȂ�� . �����Ȃ�
	static std::string XLStringUtil::baseext_nodot(const std::string &fullpath);
	//�g���q���擾����. abc.Cpp -> "cpp" �̂悤�Ȋ����ɂȂ�� . �����Ȃ��ŏ�����
	static std::string XLStringUtil::baseext_nodotsmall(const std::string &fullpath);

	//�x�[�X�f�B���N�g�����擾����  c:\\hoge\\hoge.txt -> c:\\hoge �ɂ���  �Ō��\\ �͏�����B
	static std::string XLStringUtil::basedir(const std::string &fullpath);
	//�t�@�C�������擾����  c:\\hoge\\hoge.txt -> hoge.txt
	static std::string XLStringUtil::basename(const std::string &fullpath);
	//�t�@�C��������(�g���q�Ȃ�)���擾����  c:\\hoge\\hoge.txt -> hoge
	static std::string XLStringUtil::basenameonly(const std::string &fullpath);
	//���΃p�X�����΃p�X�ɕϊ�����
	static std::string XLStringUtil::pathcombine(const std::string& base,const std::string& dir,const std::string& pathsep = "");
	//�p�X�̋�؂蕶���𕽂�ɂ���.
	static std::string XLStringUtil::pathseparator(const std::string& path,const std::string& pathsep = "");
	//URL�p�����[�^�[�̒ǉ�.
	static std::string XLStringUtil::AppendURLParam(const std::string& url,const std::string& append);


	//inTarget �� inStart �` inEnd �܂ł��擾
	static std::string XLStringUtil::cut(const std::string &inTarget , const std::string & inStart , const std::string &inEnd , std::string * outNext );

	//�R�}���h���C�������p�[�X
	static std::list<std::string> XLStringUtil::parse_command(const std::string & command);

	//bigram�ɂ�镶���񕪊�
	static std::list<std::string> XLStringUtil::makebigram(const std::string & words);

	//�w�肵�����Ŋۂ߂�
	static std::string XLStringUtil::strimwidth(const std::string &  str , int startMoji , int widthMoji ,const std::string& trimmarker);

	//�}���`�o�C�g�Ή� �_�u���N�E�H�[�g
	static std::string doublequote(const std::string& str);
	//��}���`�o�C�g�̃_�u���N�E�H�[�g
	static std::string doublequote_low(const std::string& str);
	//�d���폜
	static std::list<std::string> XLStringUtil::unique(const std::list<std::string>& list);
	//�}���`�o�C�g�Ή� inOld�Ƀ}�b�`�������̂�������������܂�
	static std::string XLStringUtil::remove(const std::string &inTarget ,const std::string &inOld );
	//�}���`�o�C�g�Ή� �����̌����ꊇ�u�� const char * replacetable[] = { "A","��"  ,"I","��"  , "��","����"  , NULL , NULL}  //�K��2�����ŏ����Ă�
	static std::string XLStringUtil::replace(const std::string &inTarget ,const char** replacetable,bool isrev = false);
	//remove �����̌����ꊇ�폜  const char * replacetable[] = {"A","B","��","������" , NULL}
	static std::string XLStringUtil::remove(const std::string &inTarget ,const char** replacetable);
	//typo�C��
	//r	 �u���[�}���v���u�Ђ炪�ȁv�ɕϊ����܂��B
	//R	 �u�Ђ炪�ȁv���u���[�}���v�ɕϊ����܂��B
	//k	 �u���ȓ���typo�v���u�Ђ炪�ȁv�ɕϊ����܂��B
	//K	 �u�Ђ炪�ȁv���u���ȓ���typo�v�ɕϊ����܂��B
	static std::string XLStringUtil::mb_convert_typo(const std::string &inTarget,const std::string& option);
	//�݂�ȑ�D�� PHP��mb_convert_kana�̈ڐA
	//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
	//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
	//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
	//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
	//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂��iU+3000 -> U+0020�j�B
	//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂��iU+0020 -> U+3000�j�B
	//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
	//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
	//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
	//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
	//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	static std::string XLStringUtil::mb_convert_kana(const std::string &inTarget,const std::string& option);
	//�݂�ȑ�D��PHP��escapeshellarg
	static std::string XLStringUtil::escapeshellarg(const std::string &inStr);

};

#endif // !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
