// RNotFoundException.h: RNotFoundException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNOTFOUNDEXCEPTION_H__4ED8E80A_9C48_4984_A8FB_62C8979FCF80__INCLUDED_)
#define AFX_RNOTFOUNDEXCEPTION_H__4ED8E80A_9C48_4984_A8FB_62C8979FCF80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RNotFoundException;
 
#include "comm.h"

#include "RException.h"

//���������Ȃ��Ƃ��ɌĂяo������O
class RNotFoundException : public RException  
{
public:
	RNotFoundException()
	{
	}
	RNotFoundException(std::string inErrorString)
	{
		setError(inErrorString);
	}
	RNotFoundException(char* Format,...)
	{
		setError(Format,(char*)(&Format+1));
	}
	virtual ~RNotFoundException()
	{
	}

};

#endif // !defined(AFX_RNOTFOUNDEXCEPTION_H__4ED8E80A_9C48_4984_A8FB_62C8979FCF80__INCLUDED_)
