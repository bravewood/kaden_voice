// XLStringUtil.cpp: XLStringUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
//#include <string>
//#include <deque>
//#include <vector>
//#include <map>
//#include <cassert>
#ifdef _WINDOWS
#include <tchar.h>
#include <mbstring.h>
#endif
#include "XLStringUtil.h"
#include "XLFileUtil.h"

/*
�@�@ �@ �@ ʁR�^::::�R.�w===�
�@�@ �@ �@ {::{/��=��=��V:/
�@�@ �@ �@ >:�L:::::::::::::::::::::::::�M�R�A
�@�@�@ ��:::::::::::::::::::::::::::::::::::::::::�R
�@ _�^/::::::::::::::::::::::::::::::::::::::::::::::ʁ@�@�@�@�@�@���b�s�[�m���Ă��
.�@| ll ! :::::::l::::::/|�n::::::::��::::i :::::::i�@�@�@�@�@ �Ȃ񂾂��񂾌�����
�@ ��S|:::::::::|:::/�M�g-:::::/ _,X:j:::/:::l                      �݂��PHP����D���Ȃ��Ƃ��B
�@�@ �S:::::::::|�����@!�u�@�����@/::::/
�@�@�@ ��::::�g �g�@ �@ �@ �@ �g �:::/!
�@ �@ /::::�i�_�@�@ �['�@ �@/�P�j�@ |
�@�@�@ �@ |�@�M`�[�\�\�]''|�@�@�R�A.|
�@�@�@�@�@ �T�@�m�@�@�@�@ �R�@ �m�@|
�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P �@�@�@�@�@�@�@

*/

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLStringUtil::XLStringUtil()
{
}

XLStringUtil::~XLStringUtil()
{

}


//�݂�ȑ�D��PHP��strtoupper
std::string XLStringUtil::strtoupper(const std::string & str)
{
	if (str.empty())
	{
		return str;
	}
	std::string r = str;
	
	char * p = &r[0];
	for(; *p ; )
	{
		if (isMultiByte(p))
		{
			p = nextChar(p);
		}
		else
		{
			*p = toupper(*p);
			p ++;
		}
	}
	return r;
}
SEXYTEST("XLStringUtil::strtoupper�̂Ă���")
{
	{
		std::string a = XLStringUtil::strtoupper("aBcDefg");
		std::string b = "ABCDEFG";
		SEXYTEST_EQ(a ,b); 
	}
}

//�݂�ȑ�D��PHP��strtolower
std::string XLStringUtil::strtolower(const std::string & str)
{
	if (str.empty())
	{
		return str;
	}
	std::string r = str;
	
	char * p = &r[0];
	for(; *p ; )
	{
		if (isMultiByte(p))
		{
			p = nextChar(p);
		}
		else
		{
			*p = tolower(*p);
			p ++;
		}
	}
	return r;
}
SEXYTEST("XLStringUtil::strtolower�̂Ă���")
{
	{
		std::string a = XLStringUtil::strtolower("aBcDefg");
		std::string b = "abcdefg";
		SEXYTEST_EQ(a ,b); 
	}
}

int XLStringUtil::atoi(const std::string & str)
{
	return std::atoi(str.c_str() );
}


//HTTP�w�b�_�̃L������
std::string XLStringUtil::HeaderUpperCamel(const std::string & str)
{
	if (str.empty())
	{
		return str;
	}
	std::string r = str;
	
	char * p = &r[0];
	*p = toupper(*p);
	p++;

	for(; *p ; ++p)
	{
		//Content-Length �݂����� - �̂��Ƃ͂ł����������B
		if (*p == '-')
		{
			++p;
			*p = toupper(*p);
		}
		else
		{
			*p = tolower(*p);
		}
	}
	return r;
}
SEXYTEST("XLStringUtil::strtoupper�̂Ă���")
{
	{
		std::string a = XLStringUtil::HeaderUpperCamel("content-length");
		std::string b = "Content-Length";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::HeaderUpperCamel("Content-length");
		std::string b = "Content-Length";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::HeaderUpperCamel("content-Length");
		std::string b = "Content-Length";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::HeaderUpperCamel("server");
		std::string b = "Server";
		SEXYTEST_EQ(a ,b); 
	}
}

std::string XLStringUtil::join(const std::string& glue , const std::list<std::string> & pieces )
{
	std::string r;
	r.reserve(pieces.size() * 10);

	std::list<std::string>::const_iterator it = pieces.begin();
	std::list<std::string>::const_iterator et = pieces.end();
	if (it == et)
	{
		return r;
	}
	//�ŏ��̈ꔭ��.
	r += *it;
	++it;

	for( ; it != et ; ++it )
	{
		r += glue;
		r += *it;
	}
	return r;
}

//template ���Ēm���Ă邯�ǁA�Ƃ肠������ŁB
std::string XLStringUtil::join(const std::string& glue , const std::vector<std::string> & pieces )
{
	std::string r;
	r.reserve(pieces.size() * 10);

	std::vector<std::string>::const_iterator it = pieces.begin();
	std::vector<std::string>::const_iterator et = pieces.end();
	if (it == et)
	{
		return r;
	}
	//�ŏ��̈ꔭ��.
	r += *it;
	++it;

	for( ; it != et ; ++it )
	{
		r += glue;
		r += *it;
	}
	return r;
}

//key=value& �݂����Ȋ����� join
std::string XLStringUtil::crossjoin(const std::string& glue1 ,const std::string& glue2 , const std::map<std::string,std::string> & pieces )
{
	std::string r;
	r.reserve(pieces.size() * 10);

	std::map<std::string,std::string>::const_iterator it = pieces.begin();
	std::map<std::string,std::string>::const_iterator et = pieces.end();
	if (it == et)
	{
		return r;
	}
	//�ŏ��̈ꔭ��.
	r += it->first + glue1 + it->second;
	++it;

	for( ; it != et ; ++it )
	{
		r += glue2;
		r += it->first + glue1 + it->second;
	}
	return r;
}
//strstr�̃}���`�o�C�g�Z�[�t �����񌟍�
const char* XLStringUtil::strstr(const std::string& target, const std::string & need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbsstr( (unsigned char*) target.c_str() ,(unsigned char*) need.c_str() );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strstr( target.c_str() ,need.c_str() );
#endif
}

//strstr�̃}���`�o�C�g�Z�[�t �����񌟍�
const char* XLStringUtil::strstr(const char* target, const char* need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbsstr( (unsigned char*)target ,(unsigned char*) need );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strstr(target ,need );
#endif
}

//strstr�̃}���`�o�C�g�Z�[�t �����񌟍� //�኱�����
const char* XLStringUtil::strrstr(const std::string& target, const std::string & need )
{
	if (need.empty()) return NULL;
	char _need0 = need[0];

	const char * start = target.c_str();
	const char * p = start;

	while(start <= p)
	{
		const char * match = strrchr(p,_need0);
		if (!match) return NULL;
		if ( strcmp(match,need.c_str() ) == 0 )	return match;
		p = match - 1;
	}
	return NULL;
}

//strstr�̃}���`�o�C�g�Z�[�t �����񌟍� //�኱�����
const char* XLStringUtil::strrstr(const char* target, const char* need )
{
	if (!*need) return NULL;
	char _need0 = *need;

	const char * p = target;
	while(target <= p)
	{
		const char * match = strrchr(p,_need0);
		if (!match) return NULL;
		if ( strcmp(match,need ) == 0 )	return match;
		p = match - 1;
	}
	return NULL;
}

//stristr�̃}���`�o�C�g�Z�[�t �啶���������֌W�Ȃ��̌���
const char* XLStringUtil::stristr(const std::string& target, const std::string & need )
{
	std::string _target = XLStringUtil::strtolower(target);
	std::string _need = XLStringUtil::strtolower(need);
	const char * find = XLStringUtil::strstr(_target.c_str(),_need.c_str());
	if (!find)
	{
		return NULL;
	}
	const int offset = (int)(find - _target.c_str());
	return target.c_str() + offset;
}

//strchr�̃}���`�o�C�g�Z�[�t ������̍ŏ��猟�����čŏ����Ɏg���������̈ʒu
const char* XLStringUtil::strchr(const std::string& target, char need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbschr( (unsigned char*)target.c_str() ,(unsigned char) need );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strchr( target ,need );
#endif
}

//strchr�̃}���`�o�C�g�Z�[�t ������̍ŏ��猟�����čŏ����Ɏg���������̈ʒu
const char* XLStringUtil::strchr(const char* target, char need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbschr( (unsigned char*)target ,(unsigned char) need );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strchr( target ,need );
#endif
}

//strrchr�̃}���`�o�C�g�Z�[�t ������̌�납�猟�����čŏ����Ɏg���������̈ʒu
const char* XLStringUtil::strrchr(const std::string& target, char need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbsrchr( (unsigned char*)target.c_str() ,(unsigned char) need );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strrchr( target ,need );
#endif
}

//strrchr�̃}���`�o�C�g�Z�[�t ������̌�납�猟�����čŏ����Ɏg���������̈ʒu
const char* XLStringUtil::strrchr(const char* target, char need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbsrchr( (unsigned char*)target ,(unsigned char) need );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strrchr( target ,need );
#endif
}

//strpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
//�ŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
int XLStringUtil::strpos(const std::string& target, const std::string & need )
{
	const char * p = XLStringUtil::strstr(target.c_str(),need.c_str());
	if (!p) return -1;

	return p - target.c_str();
}

//strpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
//�ŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
int XLStringUtil::strpos(const char* target, const char* need )
{
	const char * p = XLStringUtil::strstr(target,need);
	if (!p) return -1;

	return p - target;
}
//strrpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
//�t���猟�����čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
int XLStringUtil::strrpos(const std::string& target, const std::string & need )
{
	const char * p = strrstr(target.c_str(),need.c_str());
	if (!p) return -1;

	return p - target.c_str();
}

//strrpos�̃}���`�o�C�g�Z�[�t (strpos��PHP�ɃA�����)
//�t���猟�����čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
int XLStringUtil::strrpos(const char* target, const char* need )
{
	const char * p = strrstr(target,need);
	if (!p) return -1;

	return p - target;
}

//stripos�̃}���`�o�C�g�Z�[�t (stripos��PHP�ɃA�����)
//�啶���������֌W�Ȃ��Ō������čŏ��Ɍ��������ꏊ�B������Ȃ���� -1 ��Ԃ��܂��B
int XLStringUtil::stripos(const std::string& target, const std::string & need )
{
	const char * p = XLStringUtil::stristr(target,need);
	if (!p) return -1;

	return p - target.c_str();
}


//split
std::list<std::string> XLStringUtil::split(const std::string& glue, const std::string & inTarget )
{
	std::list<std::string> r;

	int oldpos = 0;
	int pos = 0;
	while( (pos = inTarget.find( glue , oldpos)) != -1 )
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);

		r.push_back(k);

		oldpos = pos+glue.size();
	}
	//�Ō�̎c��
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);
		r.push_back(k);
	}
	return r;
}
//vector
std::vector<std::string> XLStringUtil::split_vector(const std::string& glue, const std::string & inTarget )
{
	std::vector<std::string> r;

	int oldpos = 0;
	int pos = 0;
	while( (pos = inTarget.find( glue , oldpos)) != -1 )
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);

		r.push_back(k);

		oldpos = pos+glue.size();
	}
	//�Ō�̎c��
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);
		r.push_back(k);
	}
	return r;
}

//key=value& �݂����Ȋ����� split
std::map<std::string,std::string> XLStringUtil::crosssplit(const std::string& glue1 ,const std::string& glue2 , const std::string & inTarget )
{
	std::map<std::string,std::string> r;

	int oldpos = 0;
	int pos = 0;
	while( (pos = inTarget.find( glue1 , oldpos)) != -1 )
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);

		oldpos = pos + glue1.size();

		int vpos = k.find( glue2 );
		if (vpos < 0)
		{
			r.insert( std::pair<std::string,std::string>(k,"") );
			continue;
		}

		std::string v = k.substr( vpos + glue2.size() );
		k = k.substr(0 , vpos);
		r.insert( std::pair<std::string,std::string>(k,v) );

	}

	//�Ō�̎c��
	{
		std::string k = inTarget.substr(oldpos);
		int vpos = k.find( glue2 );
		if (vpos < 0)
		{
			r.insert( std::pair<std::string,std::string>(k,"") );
			return r;
		}

		std::string v = k.substr( vpos + glue2.size() );
		k = k.substr(0 , vpos);
		r.insert( std::pair<std::string,std::string>(k,v) );
	}
	return r;
}

SEXYTEST("XLStringUtil::crosssplit�̂Ă���")
{
	
	{
		std::map<std::string,std::string> a = XLStringUtil::crosssplit("&","=","room=&menu�G�A�R��&action����ڂ�MAX");
		std::map<std::string,std::string> b ; b["room"]=""; b["menu�G�A�R��"] = ""; b["action����ڂ�MAX"]="";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::map<std::string,std::string> a = XLStringUtil::crosssplit("&","=","a=1&bb=22&ccc=333");
		std::map<std::string,std::string> b ; b["a"]="1"; b["bb"]="22"; b["ccc"]="333";
		SEXYTEST_EQ(a ,b); 
	}
}


std::map<std::string,std::string> XLStringUtil::merge(const std::map<std::string,std::string>& a ,const std::map<std::string,std::string>& b , bool overideB = true )
{
	if (overideB)
	{
		std::map<std::string,std::string> r = b;
		r.insert(a.begin(),a.end());
		return r;
	}

	std::map<std::string,std::string> r = a;
	r.insert(b.begin(),b.end());
	return r;
}


//�݂�ȑ�D��PHP��urldecode
std::string XLStringUtil::urldecode(const std::string & inUrl)
{
	static const char xc[] = "0123456789ABCDEF";

	const char* url = inUrl.c_str();
	std::vector<char> buffer;
	buffer.resize(inUrl.size() + 1);

	char* start = &buffer[0];
	char* nomal = start;


	for( ; *url ; url++ , nomal++)
	{
		if (*url != '%')
		{
			if (*url == '+')	*nomal = ' ';
			else				*nomal = *url;
		}
		else
		{
			//% �����ŏI����Ă��镶����̔r��
			if ( *(url+1) == 0 ){	*nomal = '%';	continue;	}

			//%Z �Ƃ��̂킯�킩�߂̔r��
			const char* first = strchr(xc,*(url+1));
			if ( first == NULL ) {	*nomal = '%';	continue;	}

			//%A �����ŏI����Ă��镶����̔r��
			if ( *(url+2) == 0 ){	*nomal = '%';	continue;	}

			//%AZ �Ƃ��̂킯�킩�߂̔r��
			const char* second = strchr(xc,*(url+2));
			if ( second == NULL ) {	*nomal = '%';	continue;	}

			*nomal = 16 * (first - xc) + (second - xc);
			url ++;
			url ++;
		}
	}
	*nomal = '\0';

	return start;
}

	
SEXYTEST("XLStringUtil::base64encode�̂Ă���")
{
	{
		std::string a = XLStringUtil::urldecode("search=%41%42C%44");
		std::string b = "search=ABCD";
		SEXYTEST_EQ(a ,b); 
	}
}


//http://d.hatena.ne.jp/ytakano/20081016/urlencode ���
std::string XLStringUtil::urlencode(const std::string &str) 
{
    std::ostringstream os;

    for (unsigned int i = 0; i < str.size(); i++) {
        char c = str[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~') {
            os << c;
        } else {
            char s[4];
            _snprintf(s, sizeof(s), "%%%02x", c & 0xff);
            os << s;
        }
    }

    return os.str();
}

//base64�G���R�[�h
std::string XLStringUtil::base64encode(const std::string& src) 
{
	return base64encode(src.c_str() , src.size() );
}
SEXYTEST("XLStringUtil::base64encode�̂Ă���")
{
	{
		std::string a = XLStringUtil::base64encode("IE4+, NN4.06+, Gecko, Opera6+");
		std::string b = "SUU0KywgTk40LjA2KywgR2Vja28sIE9wZXJhNis=";
		SEXYTEST_EQ(a ,b); 

		std::string c = XLStringUtil::base64decode(b);
		SEXYTEST_EQ(c ,"IE4+, NN4.06+, Gecko, Opera6+"); 
	}
	{
		std::string a = XLStringUtil::base64encode("ABCDEFG");
		std::string b = "QUJDREVGRw==";
		SEXYTEST_EQ(a ,b); 

		std::string c = XLStringUtil::base64decode(b);
		SEXYTEST_EQ(c ,"ABCDEFG"); 
	}
	{
		std::string a = XLStringUtil::base64encode("http://user1.matsumoto.ne.jp/~goma/js/base64.html");
		std::string b = "aHR0cDovL3VzZXIxLm1hdHN1bW90by5uZS5qcC9+Z29tYS9qcy9iYXNlNjQuaHRtbA==";
		SEXYTEST_EQ(a ,b); 

		std::string c = XLStringUtil::base64decode(b);
		SEXYTEST_EQ(c ,"http://user1.matsumoto.ne.jp/~goma/js/base64.html"); 
	}
	{
		std::vector<char> file;
		XLFileUtil::cat_b("./config/testdata/badimage.jpg",&file);
		SEXYTEST_EQ(file.empty() ,false); 

		//�G���R�[�h
		std::string base64 = XLStringUtil::base64encode(&file[0],file.size() );

		//�f�R�[�h
		std::vector<char> file2;
		XLStringUtil::base64decode(base64 , &file2);

		//��r.
		SEXYTEST_EQ(file.size() ,file2.size()); 
	}
}

//base64�G���R�[�h
std::string XLStringUtil::base64encode(const char* src,int len) 
{
	static const char *base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	std::vector<char> v;
	v.resize( len * 2 );
	unsigned char * d = (unsigned char *) (&v[0]);

	const unsigned char * p = (unsigned char*) (src);
	const unsigned char * end = (unsigned char*) (p + (len / 3 * 3) );
	for(; p < end ; p += 3 , d += 4)
	{
		*(d + 0)  = base64char[ ((*(p + 0) & 0xfc) >> 2) ];

		*(d + 1)  = base64char[ ((*(p + 0) & 0x03) << 4) |  ((*(p + 1) & 0xf0) >> 4) ];

		*(d + 2)  = base64char[ ((*(p + 1) & 0x0f) << 2) |  ((*(p + 2) & 0xc0) >> 6) ];

		*(d + 3)  = base64char[ ((*(p + 2) & 0x3f)) ];
	}
	//�[���������܂��B
	switch ( len % 3 )
	{
	case 2: //1��������Ȃ�
		*(d + 0)  = base64char[ ((*(p + 0) & 0xfc) >> 2) ];

		*(d + 1)  = base64char[ ((*(p + 0) & 0x03) << 4) |  ((*(p + 1) & 0xf0) >> 4) ];

		*(d + 2)  = base64char[ ((*(p + 1) & 0x0f) << 2) ];

		*(d + 3)  = '=';

		d += 4;
		break;
	case 1: //2��������Ȃ�
		*(d + 0)  = base64char[ ((*(p + 0) & 0xfc) >> 2) ];

		*(d + 1)  = base64char[ ((*(p + 0) & 0x03) << 4) ];

		*(d + 2)  = '=';

		*(d + 3)  = '=';

		d += 4;
		break;
	case 0:
		break;
	}

	*d = '\0';

	return &v[0];
}

//base64�f�R�[�h
std::string XLStringUtil::base64decode(const std::string& src) 
{
	std::vector<char> out;
	base64decode(src,&out);

	return std::string(&out[0] , 0 , out.size() );
}

//base64�f�R�[�h
void XLStringUtil::base64decode(const std::string& src ,std::vector<char>* out) 
{
	auto base64int = [](unsigned char pp) -> int {
		if (pp >= 'A' && pp <= 'Z') return   pp - 'A';
		else if (pp >= 'a' && pp <= 'z') return   pp - 'a' + 26;
		else if (pp >= '0' && pp <= '9') return   pp - '0' + 52;
		else if (pp == '+') return   62;
		else if (pp == '/') return   63;
		else return 255;
	};

	out->resize( src.size() );
	unsigned char * d = (unsigned char *) (&out->operator[](0) );
	const unsigned char * start = d;
	unsigned int conv0 = 255;
	unsigned int conv1 = 255;
	unsigned int conv2 = 255;
	unsigned int conv3 = 255;

	unsigned char * p = (unsigned char *) src.c_str();
	while(*p)
	{
		conv0 = base64int(*(p + 0));
		if (conv0 == 255) break;

		conv1 = base64int(*(p + 1));
		if (conv1 == 255) break;

		conv2 = base64int(*(p + 2));
		if (conv2 == 255) break;

		conv3 = base64int(*(p + 3));
		if (conv3 == 255) break;

		*(d + 0) = ((conv0) << 2) + ((conv1 & 0x30) >> 4);
		*(d + 1) = ((conv1 & 0x0f) << 4) + ((conv2 & 0x3c) >> 2);
		*(d + 2) = ((conv2 & 0x03) << 6) + ((conv3));

		p += 4;
		d += 3;
	}
	if (conv0 == 255)
	{
	}
	else if (conv1 == 255)
	{
		*(d + 0) = ((conv0) << 2);

		d += 1;
	}
	else if (conv2 == 255)
	{
		*(d + 0) = ((conv0) << 2) + ((conv1 & 0x30) >> 4);
		*(d + 1) = ((conv1 & 0x0f) << 4) ;

		d += 2;
	}
	else if (conv3 == 255)
	{
		*(d + 0) = ((conv0) << 2) + ((conv1 & 0x30) >> 4);
		*(d + 1) = ((conv1 & 0x0f) << 4) + ((conv2 & 0x3c) >> 2);
		*(d + 2) = ((conv2 & 0x03) << 6);

		d += 3;
	}

	//�T�C�Y����
	out->resize((int) (d - start));
}

//�݂�ȑ�D��PHP��chop ���E�̋󔒂̏��� str�̂݃}���`�o�C�g�Ή�  replaceTable�͔��p�̂�
std::string XLStringUtil::chop(const std::string & str,const char * replaceTable)
{
	if (replaceTable == NULL)
	{
		replaceTable = " \t\r\n";
	}

	const char * p = str.c_str();
	//���o��
	while(*p)
	{
		if (isMultiByte(p))
		{
			break;
		}
		const char * rep = replaceTable;
		for(; *rep ; ++rep)
		{
			if ( *rep == *p )
			{
				break;
			}
		}
		if (*rep == NULL)
		{//not match
			break;
		}
		p ++;
	}

	const char * start = p;
	const char * lastEffectvie = p;

	//�I�[���
	while(*p)
	{
		if (isMultiByte(p))
		{
			p = nextChar(p);
			lastEffectvie = p;
			continue;
		}
		const char * rep = replaceTable;
		for(; *rep ; ++rep)
		{
			if ( *rep == *p )
			{
				break;
			}
		}
		if (*rep == NULL)
		{//not match
			++p;
			lastEffectvie = p;
		}
		else
		{//match
			++p;
		}
	}
	return std::string(start , 0 , (lastEffectvie - start) );
}
SEXYTEST("XLStringUtil::chop�̂Ă���")
{
	{
		std::string a = XLStringUtil::chop("�R���s���[�^");
		std::string b = "�R���s���[�^";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop("�R���s���[�^ ");
		std::string b = "�R���s���[�^";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" �R���s���[�^");
		std::string b = "�R���s���[�^";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" �R���s���[�^ ");
		std::string b = "�R���s���[�^";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" �R���s���[�^ A ");
		std::string b = "�R���s���[�^ A";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop("abc");
		std::string b = "abc";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" abc");
		std::string b = "abc";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" abc  ");
		std::string b = "abc";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop(" abc \r\n");
		std::string b = "abc";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::chop("			abc \r\n");
		std::string b = "abc";
		SEXYTEST_EQ(a ,b); 
	}
}

//���񂵂āA�֐� func ��K���B true ��Ԃ������̂�������������B
std::string XLStringUtil::string_filter(const std::list<std::string>& list,const std::function<bool(const std::string&)>& func)
{
	std::string ret;
	for(auto it = list.begin() ; it != list.end() ; ++it )
	{
		if ( func(*it) )
		{
			ret += *it;
		}
	}
	return ret;
}

//���񂵂āA�֐� func ��K���B func�̖߂蕶������������܂��B
std::string XLStringUtil::string_map(const std::list<std::string>& list,const std::function<std::string (const std::string&)>& func)
{
	std::string ret;
	for(auto it = list.begin() ; it != list.end() ; ++it )
	{
		ret += func(*it);
	}
	return ret;
}

//���񂵂āA�֐� func ��K���B true ��Ԃ������̂�����Ԃ��B
std::list<std::string> XLStringUtil::array_filter(const std::list<std::string>& list,const std::function<bool(const std::string&)>& func)
{
	std::list<std::string> ret;
	for(auto it = list.begin() ; it != list.end() ; ++it )
	{
		if ( func(*it) )
		{
			ret.push_back(*it);
		}
	}
	return ret;
}

//���񂵂āA�֐� func ��K���B func�̖߂�Ŕz�����ĕԂ��܂��B
std::list<std::string> XLStringUtil::array_map(const std::list<std::string>& list,const std::function<std::string (const std::string&)>& func)
{
	std::list<std::string> ret;
	for(auto it = list.begin() ; it != list.end() ; ++it )
	{
		ret.push_back(func(*it));
	}
	return ret;
}
//�}���`�o�C�g�Ή� �Ȃ��� std::string �ɕW���ŗp�ӂ���Ă��Ȃ��u���B�ӂ������B
std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	const char * p = inTarget.c_str();
	const char * match;
	while( match = XLStringUtil::strstr( p , inOld.c_str() ) )
	{
		//ret += std::string(p,0,(int)(match - p));
		ret.append(p,(int)(match - p));
		ret += inNew;

		p = match + inOld.size();
	}
	//�c��𑫂��Ă����܂�.
	return ret + p;
}


//�}���`�o�C�g��Ή� �̕�����u��
std::string XLStringUtil::replace_low(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	const char * p = inTarget.c_str();
	const char * match;
	while( match = std::strstr( p , inOld.c_str() ) )
	{
//		ret += std::string(p,0,(int)(match - p));
		ret.append(p,(int)(match - p));
		ret += inNew;

		p = match + inOld.size();
	}
	//�c��𑫂��Ă����܂�.
	return ret + p;
}

SEXYTEST("XLStringUtil::replace")
{
	{
		std::string a = XLStringUtil::replace("�ɂ��[��","��[" , "�ρ[�[");
		SEXYTEST_EQ(a ,"�ɂ�ρ[�[��");
	}
	{
		std::string a = XLStringUtil::replace("ABCDEFG","BCD" , "XYZ");
		SEXYTEST_EQ(a ,"AXYZEFG");
	}
}


//�݂�ȑ�D��PHP��htmlspecialchars
//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
std::string XLStringUtil::htmlspecialchars(const std::string &inStr)
{
	return replace(replace(replace(replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;"), "'","&apos;");
}

//�}���`�o�C�g��Ή� �^�O���G�X�P�[�v
std::string XLStringUtil::htmlspecialchars_low(const std::string &inStr)
{
	return replace_low(replace_low(replace_low(replace_low(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;"), "'","&apos;");
}

//�݂�ȑ�D��PHP��nl2br
//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
std::string XLStringUtil::nl2br(const std::string &inStr)
{
	return replace(inStr , "\r\n" , "<br>");
}

//�}���`�o�C�g��Ή� \n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
std::string XLStringUtil::nl2br_low(const std::string &inStr)
{
	return replace_low(inStr , "\r\n" , "<br>");
}

//inTarget �� inStart �` inEnd �܂ł��擾
std::string XLStringUtil::cut(const std::string &inTarget , const std::string & inStart , const std::string &inEnd , std::string * outNext )
{
	const char * p = NULL;
	if ( !inStart.empty() )
	{
		p = strstr(inTarget.c_str() , inStart.c_str() );
		if (p == NULL ) return "";
		p += inStart.size();
	}
	else
	{
		p = inTarget.c_str();
	}

	const char * p2 = NULL;
	if ( ! inEnd.empty() )
	{
		p2 = strstr(p , inEnd.c_str() );
		if (p2 == NULL ) return "";
	}
	else
	{
		p2 = p + strlen(p);
	}

	std::string ret = std::string( p , 0 , p2 - p );
	if (outNext)
	{
		*outNext = std::string(p2 + inEnd.size());
	}

	return ret;
}

//�g���q���擾����. abc.cpp -> ".cpp" �̂悤�Ȋ����ɂȂ��
std::string XLStringUtil::baseext(const std::string &fullpath)
{
	const char * p = XLStringUtil::strrchr(fullpath.c_str() , '.');
	if (!p) return "";
	return p;
}
SEXYTEST("XLStringUtil::baseext�̂Ă���")
{
	{
		std::string a = XLStringUtil::baseext("c:\\aaa\\bbb\\ccc.exe");
		std::string b = ".exe";
		SEXYTEST_EQ(a ,b); 
	}
}
//�g���q���擾����. abc.cpp -> "cpp" �̂悤�Ȋ����ɂȂ�� . �����Ȃ�
std::string XLStringUtil::baseext_nodot(const std::string &fullpath)
{
	const char * p = XLStringUtil::strrchr(fullpath.c_str() , '.');
	if (!p) return "";
	return p + 1;
}
SEXYTEST("XLStringUtil::baseext_nodot�̂Ă���")
{
	{
		std::string a = XLStringUtil::baseext_nodot("c:\\aaa\\bbb\\ccc.exe");
		std::string b = "exe";
		SEXYTEST_EQ(a ,b); 
	}
}

//�g���q���擾����. abc.Cpp -> "cpp" �̂悤�Ȋ����ɂȂ�� . �����Ȃ��ŏ�����
std::string XLStringUtil::baseext_nodotsmall(const std::string &fullpath)
{
	const char * p = XLStringUtil::strrchr(fullpath.c_str() , '.');
	if (!p) return "";
	return XLStringUtil::strtolower(p + 1);
}

//�x�[�X�f�B���N�g�����擾����  c:\\hoge\\hoge.txt -> c:\\hoge �ɂ���  �Ō��\\ �͏�����B
std::string XLStringUtil::basedir(const std::string &fullpath)
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = XLStringUtil::strrchr(fullpath.c_str() , '\\');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = XLStringUtil::strrchr(fullpath.c_str() , '/');
#endif
	if (!p) return "";
	return fullpath.substr(0, (unsigned int) (p - fullpath.c_str()) );
}
SEXYTEST("XLStringUtil::basedir�̂Ă���")
{
	{
		std::string a = XLStringUtil::basedir("c:\\aaa\\bbb\\ccc.exe");
		std::string b = "c:\\aaa\\bbb";
		SEXYTEST_EQ(a ,b); 
	}
}

//�t�@�C�������擾����  c:\\hoge\\hoge.txt -> hoge.txt
std::string XLStringUtil::basename(const std::string &fullpath)
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = XLStringUtil::strrchr(fullpath, '\\');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = XLStringUtil::strrchr(fullpath, '/');
#endif
	if (!p) return fullpath;
	return p + 1;
}
SEXYTEST("XLStringUtil::basename�̂Ă���")
{
	{
		std::string a = XLStringUtil::basename("c:\\aaa\\bbb\\ccc.exe");
		std::string b = "ccc.exe";
		SEXYTEST_EQ(a ,b); 
	}
}

//�t�@�C��������(�g���q�Ȃ�)���擾����  c:\\hoge\\hoge.txt -> hoge
std::string XLStringUtil::basenameonly(const std::string &fullpath)
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = XLStringUtil::strrchr(fullpath, '\\');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = XLStringUtil::strrchr(fullpath, '/');
#endif
	if (!p) p = fullpath.c_str();
	else p = p + 1;

	const char* ext = XLStringUtil::strrchr(p , '.');
	if (ext == NULL) return p;

	return std::string(p , 0 , ext - p );
}

SEXYTEST("XLStringUtil::basenameonly�̂Ă���")
{
	{
		std::string a = XLStringUtil::basenameonly("c:\\aaa\\bbb\\ccc.exe");
		std::string b = "ccc";
		SEXYTEST_EQ(a ,b); 
	}
}


std::string XLStringUtil::pathcombine(const std::string& base,const std::string& dir,const std::string& pathsep)
{
#ifdef _WINDOWS
	std::string _pathsep = pathsep.empty() ? "\\" : pathsep;
#else
	std::string _pathsep = pathsep.empty() ? "/" : pathsep;
#endif
	std::string _base = pathseparator(base,pathsep);
	std::string _dir = pathseparator(dir,pathsep);

	std::list<std::string> nodes = split(_pathsep,_base + _pathsep + _dir);
	std::list<std::string>::iterator i = nodes.begin();
	std::list<std::string> useNodes;
	for( ; i != nodes.end() ; ++ i )
	{
		if ( i->empty() )
		{
			continue;
		}
		if (*i == ".")
		{
			if ( i == nodes.begin() )
			{//�擪�ɂ��� ./ �����͎c��
				useNodes.push_back(*i);
			}
			else
			{//����ȊO�� . �͖���
			}
		}
		else if (*i == "..")
		{
			if (useNodes.size() == 1)
			{
#ifdef _WINDOWS
				auto topnode = useNodes.begin();
				if ( topnode->size() >= 2 && (topnode->c_str())[1] == ':' )
				{//windows��c: ����ɂ͏オ��Ȃ��̂Ŗ���
					continue;
				}
#else
				
#endif
				useNodes.pop_back();
				useNodes.push_back("");	//  /��ǉ�.
			}
			else
			{
				useNodes.pop_back();
			}
		}
		else
		{
			useNodes.push_back(*i);
		}
	}
	return join(_pathsep,useNodes);
}
SEXYTEST("XLStringUtil::pathcombine")
{
	{
		std::string a = XLStringUtil::pathcombine("./config/webroot" , "./hello.tpl","\\");
		SEXYTEST_EQ(a ,".\\config\\webroot\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("config/webroot" , "./hello.tpl","\\");
		SEXYTEST_EQ(a ,"config\\webroot\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("c:\\Program Files\\hogehoge" , "./hello.tpl","\\");
		SEXYTEST_EQ(a ,"c:\\Program Files\\hogehoge\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("c:\\Program Files\\hogehoge" , "../hello.tpl","\\");
		SEXYTEST_EQ(a ,"c:\\Program Files\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("c:\\Program Files\\hogehoge" , "../../hello.tpl","\\");
		SEXYTEST_EQ(a ,"c:\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("c:\\Program Files\\hogehoge" , "../../../hello.tpl","\\");
		SEXYTEST_EQ(a ,"c:\\hello.tpl");
	}
	{
		std::string a = XLStringUtil::pathcombine("/var/www/html" , "../../../hello.tpl","\\");
		SEXYTEST_EQ(a ,"\\hello.tpl");
	}
}

//�p�X�̋�؂蕶���𕽂�ɂ���.
std::string XLStringUtil::pathseparator(const std::string& path,const std::string& pathsep)
{
	if ( pathsep.empty() )
	{
#ifdef _WINDOWS
		return XLStringUtil::replace(path , "/" , "\\");
#else
		return XLStringUtil::replace(path , "\\" , "/");
#endif
	}
	if ( pathsep == "/" )
	{
		return XLStringUtil::replace(path , "\\" , "/");
	}
	else if ( pathsep == "\\" )
	{
		return XLStringUtil::replace(path , "/" , "\\");
	}
	else
	{
		assert(0);
		return path;
	}
}


//URL�p�����[�^�[�̒ǉ�.
std::string XLStringUtil::AppendURLParam(const std::string& url,const std::string& append)
{
	if (url.find("?") != -1 )
	{
		return url + "&" + append;
	}
	return url + "?" + append;
}
SEXYTEST("XLStringUtil::AppendURLParam�̂Ă���")
{
	{
		std::string a = XLStringUtil::AppendURLParam("http://127.0.0.1:15550/media_start","accesstoken=kaede");
		std::string b = "http://127.0.0.1:15550/media_start?accesstoken=kaede";
		SEXYTEST_EQ(a ,b); 
	}
}

//�R�}���h���C�������p�[�X
std::list<std::string> XLStringUtil::parse_command(const std::string & command)
{
	enum state
	{
		 NO
		,TARGET
		,QUOTE_SINGLE
		,QUOTE_DOUBLE
	};
	state s = NO;
	const char* targetStart = NULL;
	std::list<std::string> ret;

	const char * p = command.c_str();
	for( ; *p ; ++p)
	{
		switch (s)
		{
		case NO:
			{
				if (*p == '"')
				{
					s = QUOTE_DOUBLE;
				}
				else if (*p == '\'')
				{
					s = QUOTE_SINGLE;
					targetStart = p + 1;
				}
				else if (*p == ' ' || *p == '\t')
				{
					//nop
				}
				else
				{
					s = TARGET;
					targetStart = p;
				}
				break;
			}
		case QUOTE_DOUBLE:
			{
				if (*p == '"')
				{
					assert(targetStart != NULL);
					s = NO;
					ret.push_back( std::string(targetStart ,0 , (int) (p - targetStart - 1) ) );
					targetStart = NULL;
				}
				else if (*p == '\\' && *(p+1) == '"')
				{
					//�N�E�H�[�g�X�L�b�v
					p++;
				}
			}
		case QUOTE_SINGLE:
			{
				if (*p == '\'')
				{
					assert(targetStart != NULL);
					s = NO;
					ret.push_back( std::string(targetStart ,0 , (int) (p - targetStart - 1) ) );
					targetStart = NULL;
				}
				else if (*p == '\\' && *(p+1) == '\'')
				{
					//�N�E�H�[�g�X�L�b�v
					p++;
				}
			}
		case TARGET:
			{
				if (*p == ' ' || *p == '\t')
				{
					assert(targetStart != NULL);
					s = NO;
					ret.push_back( std::string(targetStart ,0 , (int) (p - targetStart) ) );
					targetStart = NULL;
				}
				else if (*p == '\\' && (*(p+1) == ' ' && *(p+1) == '\t') )
				{
					//�N�E�H�[�g�X�L�b�v
					p++;
				}
				break;
			}
		}
	}
	if (s == QUOTE_SINGLE || s == QUOTE_DOUBLE  || s == TARGET)
	{
		assert(targetStart != NULL);
		ret.push_back( std::string(targetStart ,0 , (int) (p - targetStart) ) );
	}
	return ret;
}

//bigram�ɂ�镶���񕪊�
std::list<std::string> XLStringUtil::makebigram(const std::string & words)
{
	std::list<std::string> ret;
	const char * s = NULL;
	const char * n = NULL;
	for(const char * p = words.c_str() ; *p ; )
	{
		s = nextChar(p);
		if (*s == 0x00)
		{
			break;
		}
		n = nextChar(s);

		ret.push_back( std::string(p,0,(int)(n-p) ) );

		p = s;
	}
	return ret;
}
SEXYTEST("XLStringUtil::makebigram�̂Ă���")
{
	{
		std::list<std::string> a = XLStringUtil::makebigram("abc");
		std::list<std::string> b ; b.push_back("ab"); b.push_back("bc");
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::list<std::string> a = XLStringUtil::makebigram("���Q���j");
		std::list<std::string> b ; b.push_back("���Q"); b.push_back("�Q��"); b.push_back("���j");
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::list<std::string> a = XLStringUtil::makebigram("ai�Q���j");
		std::list<std::string> b ; b.push_back("ai"); b.push_back("i�Q"); b.push_back("�Q��"); b.push_back("���j");
		SEXYTEST_EQ(a ,b); 
	}
}

//�w�肵�����Ŋۂ߂�
std::string XLStringUtil::strimwidth(const std::string &  str , int startMoji , int widthMoji ,const std::string& trimmarker)
{
	int countMoji = 0;
	std::string ret;

	//�擪���ȗ�����ꍇ
	if (startMoji >= 1)
	{
		ret = trimmarker;
	}

	//�J�n�ʒu��������
	const char * p;
	for(p = str.c_str() ; *p ;)
	{
		if (countMoji >= startMoji)
		{
			break;
		}
		p = nextChar(p);
		countMoji++;
	}
	const char * start = p;

	//�ȗ��ʒu��������
	for( ; *p ; )
	{
		if (countMoji >= startMoji + widthMoji)
		{
			break;
		}
		p = nextChar(p);
		countMoji++;
	}

	//���C���ƂȂ镶������擾
	ret += std::string(start , 0 ,(int) (p - start));

	//�����ȗ����Ă���ꍇ
	if (countMoji >= startMoji + widthMoji)
	{
		ret += trimmarker;
	}
	return ret;
}

SEXYTEST("XLStringUtil::strimwidth�̂Ă���")
{
	{
		std::string a = XLStringUtil::strimwidth("��������������",2,3,"!!");
		std::string b = "!!������!!";
		SEXYTEST_EQ(a ,b); 
	}
}

//�}���`�o�C�g�Ή� �_�u���N�E�H�[�g
std::string XLStringUtil::doublequote(const std::string& str)
{
	return replace(str , "\"" , "\\\"" );
}

//��}���`�o�C�g�̃_�u���N�E�H�[�g
std::string XLStringUtil::doublequote_low(const std::string& str)
{
	return replace_low(str , "\"" , "\\\"" );
}


//�d���폜
std::list<std::string> XLStringUtil::unique(const std::list<std::string>& list)
{
	//�P���ȃA���S���Y���Ȃ̂Œx���ł��B �C�ɂ��Ȃ�!
	std::list<std::string> ret;
	for(std::list<std::string>::const_iterator it = list.begin() ; it != list.end() ; ++it) 
	{
		if ( ret.empty() )
		{
			ret.push_back(*it);
			continue;
		}

		std::list<std::string>::const_iterator itf = ret.begin();
		for( ; itf != ret.end() ; ++itf) 
		{
			if (*it == *itf)
			{
				break;
			}
		}
		if (itf == ret.end())
		{
			ret.push_back(*it);
		}
	}
	return ret;
}
//�}���`�o�C�g�Ή� inOld�Ƀ}�b�`�������̂�������������܂�
std::string XLStringUtil::remove(const std::string &inTarget ,const std::string &inOld )
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	const char * p = inTarget.c_str();
	const char * match;
	while( match = XLStringUtil::strstr( p , inOld.c_str() ) )
	{
		ret += std::string(p,0,(int)(match - p));
		p = match + inOld.size();
	}
	//�c��𑫂��Ă����܂�.
	return ret + p;
}

//�}���`�o�C�g�Ή� �����̌����ꊇ�u�� const char * replacetable[] = { "A","��"  ,"I","��"  , "��","����" , NULL , NULL}  //�K��2�����ŏ����Ă�
std::string XLStringUtil::replace(const std::string &inTarget ,const char** replacetable,bool isrev)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	if (inTarget.empty())
	{
		return inTarget;
	}

	const char * p = inTarget.c_str();
	for(; *p ; )
	{
		const char * pp = p;
		p = nextChar(p);

		int compareIndex = isrev == false ? 0 : 1;
		int replaceIndex = isrev == false ? 1 : 0;
		const char ** r1 = replacetable;
		for( ; *r1 != NULL ; r1+=2)
		{
			const char * ppp = pp;
			const char * r2 = *(r1+compareIndex);
			for( ; 1 ; ++r2,++ppp )
			{
				if ( *r2 == NULL || *ppp != *r2)
				{
					break;
				}
			}
			if (*r2 == NULL)  //������r������̕����I�[�ɂ��ǂ����
			{
				ret.append(*(r1+replaceIndex));
				p = ppp;
				break;
			}
		}
		if ( *r1 == NULL )
		{
			ret.append(pp,(int) (p - pp));
		}
	}
	return ret;
}
SEXYTEST("XLStringUtil::replace�̂Ă���")
{
	{
		const char *replaceTable[] = {
			 "�A�C","����"
			,"�C","��"
			,NULL,NULL
		};
		std::string a = XLStringUtil::replace("���A�C��",replaceTable);
		std::string b = "��������";
		SEXYTEST_EQ(a ,b); 
	}
}

//remove �����̌����ꊇ�폜  const char * replacetable[] = {"A","B","��","������" , NULL} �S�������܂�
std::string XLStringUtil::remove(const std::string &inTarget ,const char** replacetable)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	if (inTarget.empty())
	{
		return inTarget;
	}

	const char * p = inTarget.c_str();
	for(; *p ; )
	{
		const char * pp = p;
		p = nextChar(p);

		const char ** r1 = replacetable;
		for( ; *r1 != NULL ; r1++)
		{
			const char * ppp = pp;
			const char * r2 = *(r1);
			for( ; 1 ; ++r2,++ppp )
			{
				if ( *r2 == NULL || *ppp != *r2)
				{
					break;
				}
			}
			if (*r2 == NULL)  //������r������̕����I�[�ɂ��ǂ����
			{
				p = ppp;
				break;
			}
		}
		if ( *r1 == NULL )
		{
			ret.append(pp,(int) (p - pp));
		}
	}
	return ret;
}


//typo�C��
//r	 �u���[�}���v���u�Ђ炪�ȁv�ɕϊ����܂��B
//R	 �u�Ђ炪�ȁv���u���[�}���v�ɕϊ����܂��B
//k	 �u���ȓ���typo�v���u�Ђ炪�ȁv�ɕϊ����܂��B
//K	 �u�Ђ炪�ȁv���u���ȓ���typo�v�ɕϊ����܂��B
std::string XLStringUtil::mb_convert_typo(const std::string &inTarget,const std::string& option)
{
	static const char *replaceTableRoma[] = {
		 "ltsu","��"
		,"whu","��"
		,"lyi","��"
		,"xyi","��"
		,"lye","��"
		,"xye","��"
		,"wha","����"
		,"whi","����"
		,"whe","����"
		,"who","����"
		,"kyi","����"
		,"kye","����"
		,"kya","����"
		,"kyu","����"
		,"kyo","����"
		,"kwa","����"
		,"qwa","����"
		,"qwi","����"
		,"qyi","����"
		,"qwu","����"
		,"qwe","����"
		,"qye","����"
		,"qwo","����"
		,"qya","����"
		,"qyu","����"
		,"qyo","����"
		,"syi","����"
		,"swi","����"
		,"sha","����"
		,"shu","����"
		,"sho","����"
		,"syi","����"
		,"sya","����"
		,"syu","����"
		,"syo","����"
		,"shi","��"
		,"swa","����"
		,"swi","����"
		,"swu","����"
		,"swe","����"
		,"swo","����"
		,"cyi","����"
		,"tyi","����"
		,"che","����"
		,"cye","����"
		,"tye","����"
		,"cha","����"
		,"cya","����"
		,"tya","����"
		,"chu","����"
		,"cyu","����"
		,"tyu","����"
		,"cho","����"
		,"cyo","����"
		,"tyo","����"
		,"chi","��"
		,"tsa","��"
		,"tsi","��"
		,"tse","��"
		,"tso","��"
		,"tsu","��"
		,"ltu","��"
		,"xtu","��"
		,"thi","�Ă�"
		,"the","�Ă�"
		,"tha","�Ă�"
		,"thu","�Ă�"
		,"tho","�Ă�"
		,"twa","�Ƃ�"
		,"twi","�Ƃ�"
		,"twu","�Ƃ�"
		,"twe","�Ƃ�"
		,"two","�Ƃ�"
		,"nyi","�ɂ�"
		,"nye","�ɂ�"
		,"nya","�ɂ�"
		,"nyu","�ɂ�"
		,"nyo","�ɂ�"
		,"hyi","�Ђ�"
		,"hye","�Ђ�"
		,"hya","�Ђ�"
		,"hyu","�Ђ�"
		,"hyo","�Ђ�"
		,"fwa","�ӂ�"
		,"fwi","�ӂ�"
		,"fyi","�ӂ�"
		,"fwu","�ӂ�"
		,"few","�ӂ�"
		,"fye","�ӂ�"
		,"fwo","�ӂ�"
		,"fya","�ӂ�"
		,"fyu","�ӂ�"
		,"fyo","�ӂ�"
		,"myi","�݂�"
		,"mye","�݂�"
		,"mya","�݂�"
		,"myu","�݂�"
		,"myo","�݂�"
		,"lya","��"
		,"xya","��"
		,"lyu","��"
		,"xyu","��"
		,"lyo","��"
		,"xyo","��"
		,"ryi","�股"
		,"rye","�肥"
		,"rya","���"
		,"ryu","���"
		,"ryo","���"
		,"gyi","����"
		,"gye","����"
		,"gya","����"
		,"gyu","����"
		,"gyo","����"
		,"gwa","����"
		,"gwi","����"
		,"gwu","����"
		,"gwe","����"
		,"gwo","����"
		,"jyi","����"
		,"zyi","����"
		,"jye","����"
		,"zye","����"
		,"jya","����"
		,"zya","����"
		,"lwa","��"
		,"xwa","��"
		,"jyu","����"
		,"zyu","����"
		,"jyo","����"
		,"zyo","����"
		,"dyi","����"
		,"dye","����"
		,"dya","����"
		,"dyu","����"
		,"dyo","����"
		,"dhi","�ł�"
		,"dhe","�ł�"
		,"dha","�ł�"
		,"dhu","�ł�"
		,"dho","�ł�"
		,"dwa","�ǂ�"
		,"dwi","�ǂ�"
		,"dwu","�ǂ�"
		,"dwe","�ǂ�"
		,"dwo","�ǂ�"
		,"byi","�т�"
		,"bye","�т�"
		,"bya","�т�"
		,"byu","�т�"
		,"byo","�т�"
		,"pyi","�҂�"
		,"pye","�҂�"
		,"pya","�҂�"
		,"pyu","�҂�"
		,"pyo","�҂�"
		,"lka","��" //��
		,"xka","��" //��
		,"lke","��" //��
		,"xke","��" //��
		,"vyi","���J��"
		,"vye","���J��"
		,"vya","���J��"
		,"vyu","���J��"
		,"vyo","���J��"
		,"wu","��"
		,"la","��"
		,"li","��"
		,"xi","��"
		,"lu","��"
		,"xu","��"
		,"le","��"
		,"xe","��"
		,"lo","��"
		,"xo","��"
		,"ye","����"
		,"ka","��"
		,"ca","��"
		,"ki","��"
		,"qa","����"
		,"qi","����"
		,"qe","����"
		,"qo","����"
		,"ku","��"
		,"cu","��"
		,"qu","��"
		,"ke","��"
		,"ko","��"
		,"co","��"
		,"sa","��"
		,"si","��"
		,"ci","��"
		,"su","��"
		,"se","��"
		,"ce","��"
		,"so","��"
		,"ta","��"
		,"ti","��"
		,"tu","��"
		,"te","��"
		,"to","��"
		,"na","��"
		,"ni","��"
		,"nu","��"
		,"ne","��"
		,"no","��"
		,"ha","��"
		,"hi","��"
		,"fa","�ӂ�"
		,"fa","�ӂ�"
		,"fi","�ӂ�"
		,"fe","�ӂ�"
		,"fo","�ӂ�"
		,"hu","��"
		,"fu","��"
		,"he","��"
		,"ho","��"
		,"ma","��"
		,"mi","��"
		,"mu","��"
		,"me","��"
		,"mo","��"
		,"ya","��"
		,"yu","��"
		,"yo","��"
		,"ra","��"
		,"ri","��"
		,"ru","��"
		,"re","��"
		,"wa","��"
		,"wo","��"
		,"nn","��"
		,"xn","��"
		,"ga","��"
		,"gi","��"
		,"gu","��"
		,"ge","��"
		,"go","��"
		,"za","��"
		,"je","����"
		,"ja","����"
		,"ju","����"
		,"jo","����"
		,"zi","��"
		,"ji","��"
		,"zu","��"
		,"ze","��"
		,"zo","��"
		,"ji","��"
		,"da","��"
		,"di","��"
		,"du","��"
		,"de","��"
		,"do","��"
		,"ba","��"
		,"bi","��"
		,"bu","��"
		,"be","��"
		,"bo","��"
		,"pa","��"
		,"pi","��"
		,"pu","��"
		,"pe","��"
		,"po","��"
		,"va","���J��"
		,"vi","���J��"
		,"ve","���J��"
		,"vo","���J��"
		,"vu","���J"
		,"a","��"
		,"i","��"
		,"u","��"
		,"e","��"
		,"o","��"
		,NULL,NULL
	};
	static const char *replaceTableKana[] = {
		 "4@","���J"
		,"a","��"
		,"e","��"
		,"4","��"
		,"5","��"
		,"6","��"
		,"t","��"
		,"g","��"
		,"h","��"
		,":","��"
		,"b","��"
		,"x","��"
		,"d","��"
		,"r","��"
		,"p","��"
		,"c","��"
		,"q","��"
		,"a","��"
		,"z","��"
		,"w","��"
		,"s","��"
		,"u","��"
		,"i","��"
		,"1","��"
		,",","��"
		,"k","��"
		,"f","��"
		,"v","��"
		,"2","��"
		,"^","��"
		,"-","��"
		,"j","��"
		,"n","��"
		,"]","��"
		,"/","��"
		,"m","��"
		,"7","��"
		,"8","��"
		,"9","��"
		,"o","��"
		,"l","��"
		,".","��"
		,";","��"
		,"\\","��"
		,"0","��"
		//,"","��"
		,"y","��"
		,"#","��"
		,"E","��"
		,"$","��"
		,"%","��"
		,"&","��"
		,"t@","��"
		,"g@","��"
		,"h@","��"
		,":@","��"
		,"b@","��"
		,"x@","��"
		,"d@","��"
		,"r@","��"
		,"p@","��"
		,"c@","��"
		,"q@","��"
		,"a@","��"
		,"z@","��"
		,"w@","��"
		,"s@","��"
		,"f@","��"
		,"v@","��"
		,"2@","��"
		,"^@","��"
		,"-@","��"
		,"f[","��"
		,"v[","��"
		,"2[","��"
		,"^[","��"
		,"-[","��"
		,"'","��"
		,"(","��"
		,")","��"
		,"Z","��"
		//,"��","��"
		,NULL,NULL
	};
	std::string ret = inTarget;
	//r	 �u���[�}���v���u�Ђ炪�ȁv�ɕϊ����܂��B
	//R	 �u�Ђ炪�ȁv���u���[�}���v�ɕϊ����܂��B
	if ( option.find("r") != -1  )
	{
		ret = XLStringUtil::replace(ret ,replaceTableRoma,true );
	}
	if ( option.find("R") != -1  )
	{
		ret = XLStringUtil::replace(ret ,replaceTableRoma,false );
	}

	//k	 �u���ȓ���typo�v���u�Ђ炪�ȁv�ɕϊ����܂��B
	//K	 �u�Ђ炪�ȁv���u���ȓ���typo�v�ɕϊ����܂��B
	if ( option.find("k") != -1  )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKana,true );
	}
	if ( option.find("K") != -1  )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKana,false );
	}
	return ret;
}



//�݂�ȑ�D�� PHP��mb_convert_kana�̈ڐA
//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂�
//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂��iU+0020 -> U+3000�j�B
//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
std::string XLStringUtil::mb_convert_kana(const std::string &inTarget,const std::string& option)
{
	std::string ret = inTarget;
	static const char *replaceTableAplha[] = {
		 "�`","A"
		,"�a","B"
		,"�b","C"
		,"�c","D"
		,"�d","E"
		,"�e","F"
		,"�f","G"
		,"�g","H"
		,"�h","I"
		,"�i","J"
		,"�j","K"
		,"�k","L"
		,"�l","M"
		,"�m","N"
		,"�n","O"
		,"�o","P"
		,"�p","Q"
		,"�q","R"
		,"�r","S"
		,"�s","T"
		,"�t","U"
		,"�u","V"
		,"�v","W"
		,"�w","X"
		,"�x","Y"
		,"�y","Z"
		,"��","a"
		,"��","b"
		,"��","c"
		,"��","d"
		,"��","e"
		,"��","f"
		,"��","g"
		,"��","h"
		,"��","i"
		,"��","j"
		,"��","k"
		,"��","l"
		,"��","m"
		,"��","n"
		,"��","o"
		,"��","p"
		,"��","q"
		,"��","r"
		,"��","s"
		,"��","t"
		,"��","u"
		,"��","v"
		,"��","w"
		,"��","x"
		,"��","y"
		,"��","z"
		,"�","�["
		,"�e","'"
		,"�f","'"
		,"�g","\""
		,"�h","\""
		,"�i","("
		,"�j",")"
		,"�k","["
		,"�l","]"
		,"�m","["
		,"�n","]"
		,"�o","{"
		,"�p","}"
		,"�q","<"
		,"�r",">"
		,"�s","<"
		,"�t",">"
		,"�u","{"
		,"�v","}"
		,"�w","{"
		,"�x","}"
		,"�y","["
		,"�z","]"
		,"�E","�"
		,"�I","!"
		,"��","#"
		,"��","&"
		,"��","$"
		,"�H","?"
		,"�F",":"
		,"�G",";"
		,"�^","/"
		,"�_","\\"
		,"��","@"
		,"�b","|"
		,"�|","-"
		,"��","="
		,"��","="
		,"��","%"
		,"�{","+"
		,"�|","-"
		,"��","/"
		,"��","*"
		,"�`","~" //UTF-8���ƕʂ́`�����邩�画�f������E�E�E
		,NULL,NULL
	};
//r	 �u�S�p�v�p�����u���p�v�ɕϊ����܂��B
//R	 �u���p�v�p�����u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
	if ( option.find("r") != -1 ||   option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,false );
	}
	else if ( option.find("R") != -1 ||  option.find("A") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,true );
	}

	static const char *replaceTableNum[] = {
		 "�P","1"
		,"�Q","2"
		,"�R","3"
		,"�S","4"
		,"�T","5"
		,"�U","6"
		,"�V","7"
		,"�W","8"
		,"�X","9"
		,"�O","0"
		,NULL,NULL
	};
//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
	if ( option.find("n") != -1 ||  option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,false );
	}
	else if ( option.find("N") != -1 ||  option.find("A") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,true );
	}

	static const char *replaceTableSpace[] = {
		 "�@"," "
		,NULL,NULL
	};
//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂�
//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂�
	if ( option.find("s") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,false );
	}
	else if ( option.find("S") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,true );
	}

	static const char *replaceTableHankanaToHiragana[] = {
		 "��","���J"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","�["
		,NULL,NULL
	};
	static const char *replaceTableHankanaToKatakana[] = {
		 "��","��"
		,"��","�K"
		,"��","�M"
		,"��","�O"
		,"��","�Q"
		,"��","�S"
		,"��","�U"
		,"��","�W"
		,"��","�Y"
		,"��","�["
		,"��","�]"
		,"��","�_"
		,"��","�a"
		,"��","�d"
		,"��","�["
		,"��","�]"
		,"��","�o"
		,"��","�r"
		,"��","�u"
		,"��","�x"
		,"��","�{"
		,"��","�p"
		,"��","�s"
		,"��","�v"
		,"��","�y"
		,"��","�|"
		,"�","�A"
		,"�","�C"
		,"�","�E"
		,"�","�G"
		,"�","�I"
		,"�","�J"
		,"�","�L"
		,"�","�N"
		,"�","�P"
		,"�","�R"
		,"�","�T"
		,"�","�V"
		,"�","�X"
		,"�","�Z"
		,"�","�\"
		,"�","�^"
		,"�","�`"
		,"�","�c"
		,"�","�e"
		,"�","�g"
		,"�","�i"
		,"�","�j"
		,"�","�k"
		,"�","�l"
		,"�","�m"
		,"�","�n"
		,"�","�q"
		,"�","�t"
		,"�","�w"
		,"�","�z"
		,"�","�}"
		,"�","�~"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","�@"
		,"�","�B"
		,"�","�D"
		,"�","�F"
		,"�","�H"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","�b"
		,"�","�["
		,NULL,NULL
	};
	static const char *replaceTableKatakanaToHiragana[] = {
		 "��","���J"
		,"�A","��"
		,"�C","��"
		,"�E","��"
		,"�G","��"
		,"�I","��"
		,"�J","��"
		,"�L","��"
		,"�N","��"
		,"�P","��"
		,"�R","��"
		,"�T","��"
		,"�V","��"
		,"�X","��"
		,"�Z","��"
		,"�\","��"
		,"�^","��"
		,"�`","��"
		,"�c","��"
		,"�e","��"
		,"�g","��"
		,"�i","��"
		,"�j","��"
		,"�k","��"
		,"�l","��"
		,"�m","��"
		,"�n","��"
		,"�q","��"
		,"�t","��"
		,"�w","��"
		,"�z","��"
		,"�}","��"
		,"�~","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�@","��"
		,"�B","��"
		,"�D","��"
		,"�F","��"
		,"�H","��"
		,"�K","��"
		,"�M","��"
		,"�O","��"
		,"�Q","��"
		,"�S","��"
		,"�U","��"
		,"�W","��"
		,"�Y","��"
		,"�[","��"
		,"�]","��"
		,"�_","��"
		,"�a","��"
		,"�d","��"
		,"�f","��"
		,"�h","��"
		,"�o","��"
		,"�r","��"
		,"�u","��"
		,"�x","��"
		,"�{","��"
		,"�p","��"
		,"�s","��"
		,"�v","��"
		,"�y","��"
		,"�|","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�b","��"
		,"��","��"
		,NULL,NULL
	};
//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	if ( option.find("k") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToKatakana,true );
	}
	else if ( option.find("K") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToKatakana,false );
	}

//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	if ( option.find("c") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,false );
	}
	else if ( option.find("C") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,true );
	}

//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
	if ( option.find("h") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,true );
	}
	else if ( option.find("H") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,false );
	}

	return ret;
}

SEXYTEST("XLStringUtil::mb_convert_kana�̂Ă���")
{
	{
//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂�
//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂�
//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","h"); //�u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
		std::string b = "������A�C�E�G�I������@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","H"); //�u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I�����������@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","c"); //�u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
		std::string b = "��������������������������@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","C"); //�u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
		std::string b = "�A�C�E�G�I�A�C�E�G�I������@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","k"); //�u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
		std::string b = "���������������������@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","K"); //�u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I�A�C�E�G�I�@123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","n"); //n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I������@123123 ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","N"); //N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I������@�P�Q�R�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","a"); //a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I������@123123 ABCABC";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","A"); //A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂��B
		std::string b = "�����������A�C�E�G�I������@�P�Q�R�P�Q�R �`�a�b�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","s"); //s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂�
		std::string b = "�����������A�C�E�G�I����� 123�P�Q�R ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
	{
		std::string a = XLStringUtil::mb_convert_kana("�����������A�C�E�G�I������@123�P�Q�R ABC�`�a�b","S"); //S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂�
		std::string b = "�����������A�C�E�G�I������@123�P�Q�R�@ABC�`�a�b";
		SEXYTEST_EQ(a ,b); 
	}
}



//�݂�ȑ�D��PHP��escapeshellarg
std::string XLStringUtil::escapeshellarg(const std::string &inStr)
{
	return "\"" + replace(inStr , "\"" , "\\\"") + "\"";
}
