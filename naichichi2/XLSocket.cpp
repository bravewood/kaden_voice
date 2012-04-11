// XLSocket.cpp: XLSocket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "common.h"
#include "XLSocket.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLSocket::XLSocket()
{
	this->Socket = INVALID_SOCKET;
}

XLSocket::~XLSocket()
{

}

XLSocket::XLSocket(SOCKET inSocket)
{
	this->Socket = inSocket;
}


void XLSocket::Create( int type , int protocol ) throw (XLResourceException)
{
	Close();

	//�\�P�b�g���J��
	this->Socket = ::socket(AF_INET,type,protocol);
	if (this->Socket == INVALID_SOCKET)
	{
		throw XLException( ErrorToMesage( ::WSAGetLastError() ) );
	}
}


void XLSocket::Close()
{
	if (this->Socket == INVALID_SOCKET)
	{
		return ;
	}

	::closesocket(this->Socket);
	this->Socket = INVALID_SOCKET;
}

int XLSocket::IoctlSocket(long inCmd , unsigned long * ioArgp) throw(XLParamException)
{
	ASSERT(this->Socket != INVALID_SOCKET);

	int ret =  ::ioctlsocket(this->Socket , inCmd , ioArgp);
	if (ret == SOCKET_ERROR )
	{
		throw XLParamException( ErrorToMesage( ::WSAGetLastError() ) );
	}
	return ret;
}


int XLSocket::SetSockopt( int level, int optname,  const char * optval,  int optlen )
{
	ASSERT(this->Socket != INVALID_SOCKET);

	return ::setsockopt(this->Socket , level , optname , optval , optlen);
}

int XLSocket::GetSockopt( int level, int optname,  char * optval,  int * optlen )
{
	ASSERT(this->Socket != INVALID_SOCKET);
	
	return ::getsockopt(this->Socket , level , optname , optval , optlen);
}

string XLSocket::ErrorToMesage(DWORD inErrorCode)
{
	switch(inErrorCode)
	{
	case WSAEINTR: return string(" �֐��Ăяo�������f���ꂽ�B\nWinSock 1.1 �̃u���b�L���O�֐��Ăяo�����AWSACancelBlockingCall() �ɂ���Ē��f���ꂽ�B");
	case WSAEBADF: return string("�����ȃt�@�C���n���h���B\n�\�P�b�g�֐��ɓn���ꂽ�t�@�C���n���h���i�\�P�b�g�n���h���j���s���ł���B�iWSAENOTSOCK�Ɠ��`�ŗp������j");
	case WSAEACCES: return string("�A�N�Z�X�����ۂ��ꂽ�B\nconnect()�F�u���[�h�L���X�g�A�h���X�Őڑ����悤�Ƃ����\�P�b�g�� SO_BROADCAST�I�v�V���� ���L���ɂȂ��Ă��Ȃ��B\nsendto()�F�u���[�h�L���X�g�A�h���X�ɑ��M���悤�Ƃ����\�P�b�g�� SO_BROADCAST�I�v�V�������L���ɂȂ��Ă��Ȃ��B\naccept()�F��t���悤�Ƃ����ڑ��v�������łɃ^�C���A�E�g�ɂȂ��Ă���A�������͎�������Ă���B");
	case WSAEFAULT: return string("�����ȃo�b�t�@�A�h���X\n�֐��ɓn���ꂽ�o�b�t�@���w���|�C���^�������ȃA�h���X��Ԃ��w���Ă���B�܂��̓o�b�t�@�̃T�C�Y������������B");
	case WSAEINVAL: return string("�����Ȉ������n���ꂽ�B\nlisten()���Ăяo������accept()���Ăяo�����B\n�o�C���h�ς݂̃\�P�b�g�ɑ΂���bind()���悤�Ƃ����B\nlisten()�őҋ@���̃\�P�b�g��connect()���悤�Ƃ����B\n�֐��ɓn���ꂽ�p�����[�^�ɖ����Ȓl���܂܂�Ă���B");
	case WSAEMFILE: return string("�g�p���̃\�P�b�g�̐�����������B\n�����ɗ��p�ł���\�P�b�g���̍ő�ɒB���Ă���A�V���Ƀ\�P�b�g���쐬�ł��Ȃ��B");
//	case WSAEWOULDBLOCK: return string("����̓u���b�L���O�����B\n�m���u���b�L���O���[�h�Ń\�P�b�g�֐����Ăяo���ꂽ�߁A�֐��͑����Ƀ��^�[���������v�����ꂽ����͊������Ă��Ȃ��B");
	case WSAEINPROGRESS: return string("���łɃu���b�L���O�葱�������s����Ă���B\n�u���b�L���O�֐������s���ł��邩�A�T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������ł���BWinSock�ł͂P�̃v���Z�X�œ����ɕ����̃u���b�L���O��������邱�Ƃ͂ł��Ȃ����߁A�O��Ăяo�������삪���S�ɏI������܂Ŏ��̑�����J�n���邱�Ƃ͂ł��Ȃ��B");
	case WSAEALREADY: return string("�v�����ꂽ����͊��Ɏ��s���A�܂��͎��s�ς݁B\n�L�����Z�����悤�Ƃ����񓯊����삪���ɃL�����Z������Ă���B\nconnect()���Ăяo�������A���ɑO��̌Ăяo���ɂ���Đڑ��������ł���B");
	case WSAENOTSOCK: return string("�w�肳�ꂽ�\�P�b�g�������ł���B\n�\�P�b�g�n���h���Ƃ��Ė����Ȓl���\�P�b�g�֐��ɓn���ꂽ�B");
	case WSAEMSGSIZE: return string("���b�Z�[�W�T�C�Y���傫������B\n���M�A�܂��͎�M���悤�Ƃ������b�Z�[�W���A��ՂƂȂ�v���g�R���̃T�|�[�g���Ă���ő僁�b�Z�[�W�T�C�Y�𒴂��Ă���Bioctlsocket()�Ńo�b�t�@�T�C�Y�𑝂₷���Ƃŉ���\�Ȃ��Ƃ�����B");
	case WSAEPROTOTYPE: return string("�\�P�b�g�͗v�����ꂽ�v���g�R���ɓK�����Ă��Ȃ��B\nsocket()���Ăяo���Ƃ��Ɏw�肵���v���g�R���^�C�v���\�P�b�g�^�C�v�ƓK�����Ă��Ȃ��B���b�Z�[�W�^�v���g�R���ŃX�g���[���\�P�b�g���쐬���悤�Ƃ����ꍇ�ȂǁB");
	case WSAENOPROTOOPT: return string("�s���ȃv���g�R���I�v�V�����B\n�w�肵���\�P�b�g�I�v�V�����܂���ioctlsocket()�ł̑��삪�A���̃v���g�R���ł̓T�|�[�g����ĂȂ��B\n�X�g���[���\�P�b�g�� SO_BROADCAST ���w�肵�悤�Ƃ����B");
	case WSAEPROTONOSUPPORT: return string("�v���g�R�����T�|�[�g����Ă��Ȃ��B\n�T�|�[�g����Ă��Ȃ��v���g�R�����w�肵���A�܂��̓v���g�R���̃T�|�[�g���Ă��Ȃ���������s���悤�Ƃ����B");
	case WSAESOCKTNOSUPPORT: return string("�w�肳�ꂽ�\�P�b�g�^�C�v�̓T�|�[�g����Ă��Ȃ��B");
	case WSAEOPNOTSUPP: return string("�v�����ꂽ����̓T�|�[�g����Ă��Ȃ��B\n�o���h�O�f�[�^�𑗐M/��M���悤�Ƃ����\�P�b�g�ł��ꂪ�T�|�[�g����Ă��Ȃ��B\n��ڑ��^(UDP�Ȃ�)�\�P�b�g��accept()���Ăяo�����B");
	case WSAEPFNOSUPPORT: return string("�v���g�R���t�@�~�����T�|�[�g����Ă��Ȃ��B");
	case WSAEAFNOSUPPORT: return string("�A�h���X�t�@�~�����T�|�[�g����Ă��Ȃ��B");
	case WSAEADDRINUSE: return string("�A�h���X�͊��Ɏg�p���ł���B\nbind()���悤�Ƃ����A�h���X�́A���ɂق��̃\�P�b�g�Ŏg���Ă���B�������[�J���A�h���X�𕡐��̃\�P�b�g�Ŏg�����߂ɂ́ASO_REUSEADDR�\�P�b�g�I�v�V������TRUE�ɂ���K�v������B");
	case WSAEADDRNOTAVAIL: return string("�����ȃl�b�g���[�N�A�h���X�B\n�\�P�b�g�֐��ɓn���ꂽ�l�b�g���[�N�A�h���X�ɖ����ȕ���������B");
	case WSAENETDOWN: return string("�l�b�g���[�N���_�E�����Ă���B\n�l�b�g���[�N�V�X�e�������炩�̏�Q���N�����A�@�\��~���Ă��邽�ߗv�����ꂽ���삪���s�ł��Ȃ��B");
	case WSAENETUNREACH: return string("�w�肳�ꂽ�l�b�g���[�N�z�X�g�ɓ��B�ł��Ȃ��B\nWSAEHOSTUNREACH�Ɠ��`�Ŏg���Ă���B");
	case WSAENETRESET: return string("�l�b�g���[�N�ڑ����j�����ꂽ�B\n�L�[�v�A���C�u���s���Ă���ڑ��ŁA���炩�̏�Q�̂��߃^�C���A�E�g�����o����ڑ�������ꂽ�B");
	case WSAECONNABORTED: return string("�l�b�g���[�N�ڑ����j�����ꂽ�B");
	case WSAECONNRESET: return string("�l�b�g���[�N�ڑ�������ɂ���Ĕj�����ꂽ�B");
	case WSAENOBUFS: return string("�o�b�t�@���s�����Ă���B\n�������s���̂���WinSock������ɕK�v�ȃo�b�t�@���m�ۂ��邱�Ƃ��ł��Ȃ��B�܂��́A�o�b�t�@��K�v�Ƃ���֐��Ăяo���ŁA����������o�b�t�@���n���ꂽ�B");
	case WSAEISCONN: return string("�\�P�b�g�͊��ɐڑ�����Ă���B\n�ڑ����̃\�P�b�g�ɑ΂���connect()�Alisten()�Abind()���̑�����s�����Ƃ����B\n�ڑ����̃\�P�b�g��sendto()�����s���悤�Ƃ����B");
	case WSAENOTCONN: return string("�\�P�b�g�͐ڑ�����Ă��Ȃ��B\n�ڑ�����Ă��Ȃ��\�P�b�g��send()�Agetpeername()���̐ڑ����ꂽ�\�P�b�g��K�v�Ƃ��鑀������s���悤�Ƃ����B");
	case WSAESHUTDOWN: return string("�\�P�b�g�̓V���b�g�_�E������Ă���B\n���ɃV���b�g�_�E������đ��M/��M����~����Ă���\�P�b�g�ŁA���M�܂��͎�M�̑�������s���悤�Ƃ����B");
	case WSAETOOMANYREFS: return string("�Q�Ƃ̐�����������B");
	case WSAETIMEDOUT: return string("�ڑ��v�����^�C���A�E�g�����B\nconnect()���Ăяo���Đڑ������݂����A�i����̉������Ȃ����Łj�������^�C���A�E�g�ɂȂ�ڑ��v�����L�����Z�����ꂽ�B");
	case WSAECONNREFUSED: return string("�ڑ������ۂ��ꂽ�B");
	case WSAELOOP: return string("���[�v�B");
	case WSAENAMETOOLONG: return string("���O����������B");
	case WSAEHOSTDOWN: return string("�z�X�g���_�E�����Ă���B\nWSAETIMEDOUT�Ɠ��`�B");
	case WSAEHOSTUNREACH: return string("�z�X�g�ւ̌o�H���Ȃ��B\n�l�b�g���[�N�̍\���㓞�B�ł��Ȃ��z�X�g�ɑ΂��đ�������s���悤�Ƃ����B�܂��̓A�h���X���s���ł���B");
	case WSAENOTEMPTY: return string("�f�B���N�g������ł͂Ȃ��B");
	case WSAEPROCLIM: return string("�v���Z�X�̐�����������B\nWSAStartup()�F����WinSock���Ǘ��ł���ő�v���Z�X���ɒB���Ă��菈�������s�ł��Ȃ��B");
	case WSAEUSERS: return string("���[�U�[�̐�����������B");
	case WSAEDQUOT: return string("�f�B�X�N�N�H�[�^�B");
	case WSAESTALE: return string("���s���悤�Ƃ�������͔p�~����Ă���B");
	case WSAEREMOTE: return string("�����[�g�B");
	case WSASYSNOTREADY: return string("�l�b�g���[�N�T�u�V�X�e�������p�ł��Ȃ��B\nWSAStartup()�F�l�b�g���[�N�T�u�V�X�e�������p�ł��Ȃ��A�܂��͐������Z�b�g�A�b�v����Ă��Ȃ����ߋ@�\���Ă��Ȃ��B");
	case WSAVERNOTSUPPORTED: return string("Winsock.dll�̃o�[�W�������͈͊O�ł���B\nWSAStartup()�F�v������WinSock�o�[�W�����́A���݂̎����ł̓T�|�[�g����Ă��Ȃ��B");
	case WSANOTINITIALISED: return string("WinSock�V�X�e��������������Ă��Ȃ��B\nWinSock�֐������s���悤�Ƃ������AWSAStartup()������Ɏ��s����Ă��Ȃ����ߋ@�\���Ȃ��B");
	case WSAEDISCON: return string("�V���b�g�_�E���������B\n�ڑ�������̏����ɂ���Đؒf����悤�Ƃ��Ă���B");
	case WSAHOST_NOT_FOUND: return string("�z�X�g��������Ȃ��B\nDNS�Ȃǂ̖��O�����T�[�r�X���w�肳�ꂽ�z�X�g���������Ȃ������B�v���g�R����T�[�r�X�̃N�G���[����ɂ����Ă��Ԃ����B");
	case WSATRY_AGAIN: return string("�w�肳�ꂽ�z�X�g��������Ȃ��A�܂��̓T�[�r�X�ُ̈�B");
	case WSANO_RECOVERY: return string("�񕜕s�\�ȃG���[�����������B\n�f�[�^�x�[�X�����̏ꍇ�́A���O�����T�[�r�X���g�p�ł��Ȃ����Ƃ��Ӗ�����B");
	case WSANO_DATA: return string("�v�����ꂽ�^�C�v�̃f�[�^���R�[�h��������Ȃ��B");
	}

	return "";
}
