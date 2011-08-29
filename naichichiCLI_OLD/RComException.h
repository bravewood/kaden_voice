// RComException.h: RComException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMEXCEPTION_H__0C26593B_A172_4C5B_9753_908214334FF8__INCLUDED_)
#define AFX_RCOMEXCEPTION_H__0C26593B_A172_4C5B_9753_908214334FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RConferException.h"

class RComException : public RConferException  
{
private:
	//�G���[�����Ȃ��̂Ŕ񐄏�
	RComException(const string inFormat)
	{
	}
	RComException(char* Format,...)
	{
	}
public:
	RComException(IUnknown * inUnkown , REFIID inRIID , unsigned long inErrorCode,std::string inErrorString)
		: RConferException(inErrorCode,inErrorString)
	{
		setErrorCode(inErrorCode);
		setError(inErrorString);
		setError(AppendComErrorEx(inUnkown,inRIID) + "\n" + 
			AppendComError((HRESULT)inErrorCode)  + "\n" + 
			AfxGetFullScodeString((HRESULT)inErrorCode) + "\n" + 
			getMessage() );
	}
	RComException(unsigned long inErrorCode,std::string inErrorString)
	{
		setErrorCode(inErrorCode);
		setError(inErrorString);
		setError(AppendComError((HRESULT)inErrorCode)  + "\n" + getMessage() );
	}
	RComException(unsigned long inErrorCode,char* Format,...)
	{
		setErrorCode(inErrorCode);
		setError(Format,(char*)(&Format+1));
		setError(AppendComError((HRESULT)inErrorCode)  + "\n" + getMessage() );
	}
	virtual ~RComException()
	{
	}
private:
	//MFC���q�؂�����O��߂܂��郋�[�`��.
	LPCTSTR RComException::AfxGetScodeString(SCODE sc);
	LPCTSTR RComException::AfxGetScodeRangeString(SCODE sc);
	LPCTSTR RComException::AfxGetSeverityString(SCODE sc);
	LPCTSTR RComException::AfxGetFacilityString(SCODE sc);
	string RComException::AfxGetFullScodeString(SCODE sc);

private:
	//�G���[�𕶎��񂩂��Đ擪�ɉ����܂��B
	string AppendComError(HRESULT inErrorCode);
	//COM�̗�O��߂܂��Ă݂�.
	string AppendComErrorEx(IUnknown * inUnkown , REFIID inRIID);



};

#endif // !defined(AFX_RCOMEXCEPTION_H__0C26593B_A172_4C5B_9753_908214334FF8__INCLUDED_)
