// RNoComplateException.h: RNoComplateException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNOCOMPLATEEXCEPTION_H__8850A3C9_9CA4_4AEF_89B8_14C0F8C2E472__INCLUDED_)
#define AFX_RNOCOMPLATEEXCEPTION_H__8850A3C9_9CA4_4AEF_89B8_14C0F8C2E472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RException.h"

//�����������ł��Ȃ������Ƃ��̗�O
class RNoComplateException : public RException  
{
public:
	RNoComplateException()
	{
	}
	RNoComplateException(std::string inErrorString)
	{
		setError(inErrorString);
	}
	RNoComplateException(char* Format,...)
	{
		setError(Format,(char*)(&Format+1));
	}
	virtual ~RNoComplateException()
	{
	}

};

#endif // !defined(AFX_RNOCOMPLATEEXCEPTION_H__8850A3C9_9CA4_4AEF_89B8_14C0F8C2E472__INCLUDED_)
