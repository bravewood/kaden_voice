// RNotSupportException.h: RNotSupportException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNOTSUPPORTEXCEPTION_H__6659B25E_536A_4477_A91C_DB122095DF33__INCLUDED_)
#define AFX_RNOTSUPPORTEXCEPTION_H__6659B25E_536A_4477_A91C_DB122095DF33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RNotSupportException;

#include "comm.h"

#include "RException.h"


//�T�|�[�g����Ă��Ȃ��@�\���Ăяo���ꂽ�Ƃ��ɕԂ���O.
class RNotSupportException : public RException  
{
public:
	RNotSupportException()
	{
	}
	RNotSupportException(std::string inErrorString)
	{
		setError(inErrorString);
	}
	RNotSupportException(char* Format,...)
	{
		setError(Format,(char*)(&Format+1));
	}
	virtual ~RNotSupportException()
	{
	}

};

#endif // !defined(AFX_RNOTSUPPORTEXCEPTION_H__6659B25E_536A_4477_A91C_DB122095DF33__INCLUDED_)
