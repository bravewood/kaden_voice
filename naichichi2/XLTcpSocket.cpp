// XLTcpSocket.cpp: XLTcpSocket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLTcpSocket.h"



//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLTcpSocket::XLTcpSocket()
{
	this->Connected = false;
}

XLTcpSocket::~XLTcpSocket()
{
	ShutDown();
	Close();
}

XLTcpSocket::XLTcpSocket(SOCKET inSocket) : XLSocket(inSocket)
{
	this->Connected = false;
}

void XLTcpSocket::Create()
{
	XLSocket::Create(SOCK_STREAM , 0);
}




void XLTcpSocket::Connect(const string &inHost , int inPort)	throw(XLIOException)
{
	ASSERT(this->Socket != INVALID_SOCKET);
	ASSERT(this->Connected == false);

	SOCKADDR_IN	sai = {0};
	
	//IP?
	unsigned long connectIP = inet_addr( inHost.c_str() );
	if(connectIP==INADDR_NONE)
	{
		//���O�̂悤�Ȃ̂ŉ������Ă݂�.
		HOSTENT	* hostEnt;
		hostEnt=gethostbyname(inHost.c_str() );
		if(!hostEnt)
		{
			throw XLIOException(ETRACE + "�z�X�g���������ł��܂���" + ErrorToMesage(::WSAGetLastError() ) );
		}
		sai.sin_addr=*((IN_ADDR*)*hostEnt->h_addr_list);
	}
	else
	{
		sai.sin_addr.S_un.S_addr = connectIP;
	}

	sai.sin_port=htons((unsigned short)inPort);
	sai.sin_family=AF_INET;

	this->Connect(&sai) ;
}

void XLTcpSocket::Connect(const SOCKADDR_IN * inSai) throw(XLIOException)
{
	ASSERT(this->Socket != INVALID_SOCKET);
	ASSERT(this->Connected == false);

	//�ڑ������݂�.
	const int ret = connect(Socket,(SOCKADDR*)inSai,sizeof(SOCKADDR_IN));
	if( ret == SOCKET_ERROR )
	{	
		throw XLIOException(ETRACE + "�ڑ��Ɏ��s���܂���" + ErrorToMesage( ::WSAGetLastError() ) );
	}

	this->Connected = true;
}

void XLTcpSocket::Bind(unsigned long inBindIP , int inPort) throw(XLIOException)
{
	ASSERT(this->Socket != INVALID_SOCKET);
	ASSERT(this->Connected == false);

	int	Opt = 1;
	this->SetSockopt(SOL_SOCKET, SO_REUSEADDR, (const char*)&Opt , sizeof(int) );

	SOCKADDR_IN	sai = {0};
	sai.sin_family			= AF_INET;
	sai.sin_addr.s_addr	=   htonl(inBindIP);
	sai.sin_port			= htons((unsigned short)inPort);

	this->Bind( &sai );
}

void XLTcpSocket::Bind(const SOCKADDR_IN * inSai) throw(XLIOException)
{
	ASSERT(this->Socket != INVALID_SOCKET);
	ASSERT(this->Connected == false);

	int ret = ::bind( this->Socket , (struct sockaddr*) inSai , sizeof(sockaddr) );
	if (ret < 0)
	{
		throw XLIOException(ETRACE + "bind �Ɏ��s���܂���" + ErrorToMesage( ::WSAGetLastError() ) );
	}

	this->Connected  = true;
}

void XLTcpSocket::Listen(int inBacklog ) throw(XLIOException)
{
	int ret = ::listen( this->Socket , inBacklog );
	if (ret < 0)
	{
		throw XLIOException(ETRACE + "listen �Ɏ��s���܂���" + ErrorToMesage( ::WSAGetLastError() ) );
	}
}

XLTcpSocket* XLTcpSocket::Accept() throw(XLIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	SOCKET  newSock = ::accept( this->Socket , (struct sockaddr*) NULL , NULL);
	if (newSock <= 0 )
	{
		throw XLIOException(ETRACE + "accept �Ɏ��s���܂���" + ErrorToMesage( ::WSAGetLastError() ) );
	}
	//�V�K�ɍ쐬���ĕԂ�
	return new XLTcpSocket(newSock);
}

void XLTcpSocket::ShutDown()
{
	if ( this->Socket == INVALID_SOCKET) return ;
	if ( this->Connected == false)	return ;

	::shutdown( this->Socket , 1 );

	//���̎��ԂɂȂ��Ă��Ȃ��ł���ꍇ�́A���؂�.
	DWORD TimeOutTime = ::time(NULL) + 10; //10�b�҂�.

	//�ۗ����̂��ׂẴf�[�^���󂯎��.
	int		ret;
	char	buffer[1024];
	while(1)
	{
		ret = ::recv( this->Socket , buffer , 1024 , 0 );
		
		if ( ret <= 0 || ret == SOCKET_ERROR)	break;

		//�^�C���A�E�g�`�F�b�N
		if ( TimeOutTime < ::time(NULL) ) 
		{
			break;
		}
	}
	//�f�[�^������ȏ��M���Ȃ����Ƃ𑊎�ɓ`����.
	::shutdown( this->Socket , 2);

	this->Connected = false;
}

int XLTcpSocket::Send( const char* inBuffer ,int inBufferLen , int inFlags , int inTimeOutS) throw(XLIOException)
{
	Select(SELECT_TYPE_WRITE , inTimeOutS);

	int ret = ::send( this->Socket , inBuffer , inBufferLen , inFlags);
	if (ret < 0)
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			::WSASetLastError(errorCode);
			return ret;
		}
		throw XLIOException(ETRACE + "send �Ɏ��s���܂���" + ErrorToMesage( errorCode ) );
	}
	return ret;
}

int XLTcpSocket::Recv( char* outBuffer ,int inBufferLen , int inFlags , int inTimeOutS) throw(XLIOException)
{
	Select(SELECT_TYPE_READ , inTimeOutS);

	int ret = ::recv( this->Socket , outBuffer , inBufferLen , inFlags);
	if (ret < 0)
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			::WSASetLastError(errorCode);
			return ret;
		}
		throw XLIOException(ETRACE + "recv �Ɏ��s���܂���" + ErrorToMesage( errorCode ) );
	}
	return ret;
}


void XLTcpSocket::Select(SELECT_TYPE inType , long inTimeOutS) throw(XLIOException)
{
	timeval useTimeVal;
	FD_SET	flg;
	//�Z���N�g�̖߂�l
	int i;
	//�Z�����ԂŃ`�F�b�N���܂�.
	for(i = 0 ; i < inTimeOutS ; i++)
	{
		//�b.
		useTimeVal.tv_sec  = 1;
		//�bms
		useTimeVal.tv_usec = 0;
		FD_ZERO(&flg);
		FD_SET(Socket , &flg);

		int	ret = 0;
		switch(inType)
		{
		case SELECT_TYPE_READ:
			ret = select(2 , &flg, NULL , NULL , &useTimeVal);
			break;
		case SELECT_TYPE_WRITE:
			ret = select(2 , NULL, &flg , NULL , &useTimeVal);
			break;
		}
		//��Ԃ��ω�����.
		if (ret != 0) break;	
	}
	//�^�C���A�E�g�ł�.
	if (i == inTimeOutS)
	{
		throw XLIOException(ETRACE + "�^�C���A�E�g���������܂���");
	}

	if ( ! FD_ISSET( this->Socket , &flg ) )
	{
		throw XLIOException(ETRACE + "select ���ɃG���[���������܂���" + ErrorToMesage( ::WSAGetLastError() ) );
	}

}