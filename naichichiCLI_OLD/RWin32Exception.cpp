// RWin32Exception.cpp: RWin32Exception �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RWin32Exception.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
RWin32Exception::RWin32Exception(const string inFormat)
{
	DWORD errorCode = ::GetLastError();

	setErrorCode(errorCode);
	setError( inFormat.c_str() );

	//�G���[�𕶎��񂩂��Đ擪�ɉ����܂��B
	AppendWin32Error(errorCode);
}

RWin32Exception::RWin32Exception(char* Format,...)
{
	DWORD errorCode = ::GetLastError();

	setErrorCode(errorCode);
	setError(Format,(char*)(&Format+1));

	//�G���[�𕶎��񂩂��Đ擪�ɉ����܂��B
	AppendWin32Error(errorCode);
}

RWin32Exception::~RWin32Exception()
{
}

//�G���[�𕶎��񂩂��Đ擪�ɉ����܂��B
void RWin32Exception::AppendWin32Error(DWORD inErrorCode)
{
	LPVOID msgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		inErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
		(LPTSTR) &msgBuf,
		0,
		NULL 
	);

#ifndef _WIN32_WCE
	setError( string( (char*)msgBuf) + "\n" + getMessage() );
#else
	MY_USES_CONVERSION;

	TCHAR* wmsg = (TCHAR*)msgBuf;
	setError( string( _W2A( wmsg ) ) + "\n" + getMessage() );
#endif


	setError( string( (char*)msgBuf) + "\n" + getMessage() );

	::LocalFree( msgBuf );
}
