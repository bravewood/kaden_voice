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
#endif
#include "XLStringUtil.h"


//USE_TEST(XLStringUtil)

/*
�@�@ �@ �@ ʁR�^::::�R.�w===�
�@�@ �@ �@ {::{/��=��=��V:/
�@�@ �@ �@ >:�L:::::::::::::::::::::::::�M�R�A
�@�@�@ ��:::::::::::::::::::::::::::::::::::::::::�R
�@ _�^/::::::::::::::::::::::::::::::::::::::::::::::ʁ@�@�@�@�@�@���b�s�[�m���Ă��
.�@| ll ! :::::::l::::::/|�n::::::::��::::i :::::::i�@�@�@�@�@ �݂�ȂȂ񂾂��񂾌�����
�@ ��S|:::::::::|:::/�M�g-:::::/ _,X:j:::/:::l                      PHP����D���Ȃ��Ƃ��B
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
	for(; *p ; ++p)
	{
		*p = toupper(*p);
	}
	return r;
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
	for(; *p ; ++p)
	{
		*p = tolower(*p);
	}
	return r;
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

//key=value& �݂����Ȋ����� split
std::map<std::string,std::string> XLStringUtil::crosssplit(const std::string& glue1 ,const std::string& glue2 , const std::string & inTarget )
{
	std::map<std::string,std::string> r;

	int oldpos = 0;
	int pos = 0;
	while( (pos = inTarget.find( glue1 , oldpos)) != -1 )
	{
		std::string k = inTarget.substr(oldpos , pos - oldpos);
		int vpos = k.find( glue2 );
		if (vpos < 0)
		{
			continue;
		}

		std::string v = k.substr( vpos + glue2.size() );
		k = k.substr(0 , vpos);
		r.insert( std::pair<std::string,std::string>(k,v) );

		oldpos = pos + glue1.size();
	}

	//�Ō�̎c��
	{
		std::string k = inTarget.substr(oldpos);
		int vpos = k.find( glue2 );
		if (vpos < 0)
		{
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

#define TRIM_SPACE(x)	((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')

//�݂�ȑ�D��PHP��urldecode
std::string XLStringUtil::urldecode(const std::string & inUrl)
{
	static const char xc[] = "0123456789ABCDEF";

	const char* url = inUrl.c_str();
	std::vector<char> buffer;
	buffer.resize(inUrl.size() + 1);

	char* orinal = &buffer[0];
	char* nomal = orinal;


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

	return nomal;
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

//�݂�ȑ�D��PHP��chop ���E�̋󔒂̏���
std::string XLStringUtil::chop(const std::string & str)
{
	const char * p = str.c_str();
	int size = str.size();
	int l;
	for(l = 0 ; l < size ; l++)
	{
		if ( !TRIM_SPACE(p[l]) )
		{
			break;
		}
	}
	if (l == size)
	{
		return "";	//���ׂċ�
	}

	int r;
	for(r = size-1 ; r >= 0 ; r--)
	{
		if ( !TRIM_SPACE(p[r]) )
		{
			break;
		}
	}
//	assert (l  <= r);

	return str.substr(l , r - l + 1);
}

//�Ȃ��� std::string �ɕW���ŗp�ӂ���Ă��Ȃ��u���B�ӂ������B
std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;
	//�u������镶����̒���
	int replaceTargetSize = inOld.size();

	while( (pos = inTarget.find( inOld , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos , pos - oldpos);
		ret += inNew;

		oldpos = pos + replaceTargetSize ;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;

}

//�݂�ȑ�D��PHP��htmlspecialchars
//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
std::string XLStringUtil::htmlspecialchars(const std::string &inStr)
{
	return replace(replace(replace(replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;"), "'","&apos;");
}


//�݂�ȑ�D��PHP��nl2br
//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
std::string XLStringUtil::nl2br(const std::string &inStr)
{
	return replace(inStr , "\r\n" , "<br>");
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
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = _tcsrchr(fullpath.c_str() , '.');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = strrchr(fullpath.c_str() , '.');
#endif
	if (!p) return "";
	return p;
}

//�x�[�X�f�B���N�g�����擾����  c:\\hoge\\hoge.txt -> c:\\hoge �ɂ���  �Ō��\\ �͏�����B
std::string XLStringUtil::basedir(const std::string &fullpath)
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = _tcsrchr(fullpath.c_str() , '\\');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = strrchr(fullpath.c_str() , '/');
#endif
	if (!p) return "";
	return fullpath.substr(0, (unsigned int) (p - fullpath.c_str()) );
}

//�t�@�C�������擾����  c:\\hoge\\hoge.txt -> hoge.txt
std::string XLStringUtil::basename(const std::string &fullpath)
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	const char * p = _tcsrchr(fullpath.c_str() , '\\');
#else
	//UTF-8���Ɖ��肵�Ă���w
	const char * p = strrchr(fullpath.c_str() , '/');
#endif
	if (!p) return "";
	return p + 1;
}

std::string XLStringUtil::pathcombine(const std::string& base,const std::string& dir,const std::string& pathsep)
{
#ifdef _WINDOWS
	std::string _pathsep = pathsep.empty() ? "\\" : pathsep;
#else
	std::string _pathsep = pathsep.empty() ? "/" : pathsep;
#endif
	
	std::list<std::string> nodes = split(_pathsep,base + _pathsep + dir);
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
			//����
		}
		else if (*i == "..")
		{
			useNodes.pop_back();
		}
		else
		{
			useNodes.push_back(*i);
		}
	}
	return join(_pathsep,useNodes);
}

//�p�X�̋�؂蕶���𕽂�ɂ���.
std::string XLStringUtil::pathseparator(const std::string& path)
{
#ifdef _WINDOWS
	return XLStringUtil::replace(path , "/" , "\\");
#else
	return XLStringUtil::replace(path , "\\" , "/");
#endif
}


//URL�p�����[�^�[�̒ǉ�.
std::string XLStringUtil::AppendURLParam(const std::string& url,const std::string& append)
{
	if (url.find("?") >= 0 )
	{
		return url + "&" + append;
	}
	return url + "?" + append;
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


/*
void XLStringUtil::test()
{
	{
		std::string r;
		ASSERT( (r = chop("abc") ) == "abc");
		ASSERT( (r = chop(" abc") ) == "abc");
		ASSERT( (r = chop(" abc  ") ) == "abc");
		ASSERT( (r = chop(" abc \r\n") ) == "abc");
		ASSERT( (r = chop("			abc \r\n") ) == "abc");
	}
}
*/