#include "common.h"
#include "ActionImplement.h"
#include "XLHttpRequerst.h"
#include "XLStringUtil.h"

ActionImplement::ActionImplement(void)
{
}


ActionImplement::~ActionImplement(void)
{
}

xreturn::r<std::string> ActionImplement::Telnet(const std::string& host,int port,const std::string& wait,const std::string& send,const std::string& recv)
{
	//ASIO�쐬
	boost::asio::io_service io_service;

	 //���O����
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host , num2str( port ));
	boost::asio::ip::tcp::endpoint endpoint(*resolver.resolve(query));

    boost::asio::ip::tcp::socket socket(io_service);

	//�ڑ�
	boost::system::error_code error ;
	socket.connect(endpoint,error);
    if (error)
	{
		return xreturn::error("�z�X�g�ɐڑ��ł��܂��� host:" + host + " port:" + num2str(port) );
	}

	//�܂��͑���̌������𕷂��܂�.
	if (!wait.empty())
	{
		boost::asio::streambuf wait_body;
		boost::asio::read_until(socket, wait_body,wait ,error );
	}

	//���M
	if (!send.empty() )
	{
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << send << std::flush;
		boost::asio::write(socket, request);
	}

	//�Ō�ɉ�Ă̌������𕷂��Ă���ʐM��؂�܂��B
	if (!recv.empty())
	{
		//�M���̈ӌ��́H
		boost::asio::streambuf respons_body;
		boost::asio::read_until(socket, respons_body,recv ,error );
		return boost::asio::buffer_cast<const char*>(respons_body.data());
	}
	return "";
}

xreturn::r<std::string> ActionImplement::HttpGet(const std::string& url)
{
	XLHttpRequerst s;
	std::map<std::string,std::string> header;
	std::map<std::string,std::string> option;
	return s.GetContents(url ,header,option );
}

xreturn::r<std::string> ActionImplement::HttpPost(const std::string& url,const std::string& postdata)
{
	XLHttpRequerst s;
	std::map<std::string,std::string> header;
	std::map<std::string,std::string> option;
	option["method"] = "POST";
	option["data"] = postdata;

	return s.GetContents(url ,header,option );
}

xreturn::r<bool> ActionImplement::Execute(const std::string& targetpc,const std::string& command,const std::string& args,const std::string& directory)
{
	std::string _command = XLStringUtil::pathcombine(directory , command);
	std::string _args = XLStringUtil::pathseparator(args);

	::ShellExecute(NULL,"open",_command.c_str(),_args.c_str(),NULL,SW_SHOWNORMAL);

	//�������s�̔��肪���܂������Ȃ���������̂ŁA�Ƃ肠��������ŁB
	return true;
}

xreturn::r<bool> ActionImplement::SendKeydown(const std::string& targetpc,const std::string& windowname,int key , int keyoption)
{
	HWND hwnd = ::GetTopWindow(::GetDesktopWindow());
	do
	{
		char title[ MAX_PATH ]= {0};
		::GetWindowText( hwnd, title, sizeof(title) );

		if ( strstr(title , windowname.c_str() ) == NULL )
		{
			continue;
		}
		
		//keybd_event
		if (keyoption & 1)  ::SendMessage(hwnd , WM_KEYDOWN ,VK_CONTROL ,   0 );
		if (keyoption & 2)  ::SendMessage(hwnd , WM_KEYDOWN ,VK_LMENU     ,   0 );
		if (keyoption & 4)  ::SendMessage(hwnd , WM_KEYDOWN ,VK_LSHIFT  ,   0 );
		::SendMessage(hwnd , WM_KEYDOWN ,key, 0 );
		::SendMessage(hwnd , WM_CHAR    ,key, 0 );
		::SendMessage(hwnd , WM_KEYUP   ,key, 0 );
		if (keyoption & 4)  ::SendMessage(hwnd , WM_KEYUP ,VK_LSHIFT  ,   0 );
		if (keyoption & 2)  ::SendMessage(hwnd , WM_KEYUP ,VK_LMENU     ,   0 );
		if (keyoption & 1)  ::SendMessage(hwnd , WM_KEYUP ,VK_CONTROL ,   0 );
	}
	while( (hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT)) !=NULL );

	return true;
}

xreturn::r<bool> ActionImplement::SendMessage(const std::string& targetpc,const std::string& windowname,int message,int wparam,int  lparam)
{
	HWND hwnd = ::GetTopWindow(::GetDesktopWindow());
	do
	{
		char title[ MAX_PATH ]= {0};
		::GetWindowText( hwnd, title, sizeof(title) );

		if ( strstr(title , windowname.c_str() ) == NULL )
		{
			continue;
		}
		
		//keybd_event
		::SendMessage(hwnd , message , wparam,lparam );
	}
	while( (hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT)) !=NULL );

	return true;
}

xreturn::r<bool> ActionImplement::MSleep(const std::string& targetpc,unsigned int mtime )
{
	const unsigned int sleep_poolingtime = 100;
	unsigned int loop = mtime / sleep_poolingtime;
	if (loop <= 0) loop ++;
	//���̎����͂��납�����ǁA�Ƃ肠��������ŁB
	//main thread �ɑ΂����~�� linux�Ή��A�Ȃǂ��l����Ƃ܂��͂���ŗl�q������.
	for(unsigned int t = 0 ; t < loop ; ++t)
	{
//		if (*threadStopFlag)
//		{
//			return false;
//		}
		::Sleep(sleep_poolingtime);
	}
	return true;
}
