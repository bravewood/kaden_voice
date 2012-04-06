// XLSocket.h: XLSocket �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSOCKET_H__137F6EB5_32A1_46CD_9CA7_EC6E9C6A6E6A__INCLUDED_)
#define AFX_XLSOCKET_H__137F6EB5_32A1_46CD_9CA7_EC6E9C6A6E6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XLResourceException.h"
#include "XLParamException.h"


class XLSocket  
{
public:
	/**
	 * XLSocket:	�\�P�b�g�쐬
	 *
	 * @return  
	 */
	XLSocket();
	virtual ~XLSocket();

	/**
	 * XLSocket:		�쐬�ς݃\�P�b�g�̊��蓖��.
	 *
	 * @param inSocket 
	 * @return  
	 */
	XLSocket(SOCKET inSocket);

	/**
	 * Create:	�\�P�b�g�̍쐬
	 *
	 * @param type	�\�P�b�g�̎��	SOCK_STREAM : TCP
	 *								SOCK_DGRAM : UDP
	 * @return void 
	 */
	void Create( int type , int protocol ) throw (XLResourceException);

	/**
	 * Close:	�\�P�b�g�����
	 *
	 * @return void 
	 */
	void Close();

	/**
	 * ioctlsocket:
	 *
	 * @param inCmd 
	 * @param ioArgp 
	 * @return  int	
	 */
	int IoctlSocket(long inCmd , unsigned long * ioArgp) throw(XLParamException);

	int SetSockopt( int level, int optname,  const char * optval,  int optlen );

	int GetSockopt( int level, int optname,  char * optval,  int * optlen );

protected:

	/**
	 * ErrorToMesage:		�\�P�b�g�G���[�𕶎���
	 *
	 * @param inErrorCode	
	 * @return string 
	 */
	string ErrorToMesage(DWORD inErrorCode);

	/**
	 * Socket:	�\�P�b�g�n���h��
	 */
	SOCKET	Socket;
};

#endif // !defined(AFX_XLSOCKET_H__137F6EB5_32A1_46CD_9CA7_EC6E9C6A6E6A__INCLUDED_)
