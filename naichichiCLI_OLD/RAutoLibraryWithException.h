// RAutoLibraryWithException.h: RAutoLibraryWithException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAUTOLIBRARYWITHEXCEPTION_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
#define AFX_RAUTOLIBRARYWITHEXCEPTION_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RDoNotInitaliseException.h"
#include "RNotSupportException.h"
#include "RWin32Exception.h"

//RAutoLibrary  �ɗ�O�������v���X�������C�u�����B
//�v�������Ă���Ȃ���Exception�����āu�����[��v�Ƌ����ăh�A�ɏR�肢�ꂿ�Ⴄ�](�~��YO!)
class RAutoLibraryWithException
{
public:
	//�\�z
	RAutoLibraryWithException() throw()
	{
		LabraryHandle = NULL;
	}

	//�w�肵�����O�ō\�z
	//���s�� RDoNotInitaliseException �ł��B
	RAutoLibraryWithException(const char* inDLLName) throw(RDoNotInitaliseException)
	{
		Create(inDLLName);
	}

	//�j��
	virtual ~RAutoLibraryWithException()
	{
		Destory();
	}

	//DLL�ݒ�
	//@inDLLName	DLL�̖��O
	//���s�� RDoNotInitaliseException �ł��B
	void Create(const char* inDLLName) throw(RDoNotInitaliseException)
	{
		Destory();

#ifndef _WIN32_WCE
		LabraryHandle = ::LoadLibrary(inDLLName);
#else
		MY_USES_CONVERSION;
		LabraryHandle = ::LoadLibrary(_A2W(inDLLName) );
#endif
		if (!IsReading())
		{
			try
			{
				throw RWin32Exception("RAutoLibraryWithException:%s �̓ǂݍ��݂Ɏ��s���܂���",inDLLName);
			}
			catch(RWin32Exception e)
			{
				throw RDoNotInitaliseException(e.getMessage());
			}
		}
	}
	//DLL�j��
	void Destory()
	{
		if (!IsReading()) return;
		::FreeLibrary((HINSTANCE)LabraryHandle);
	}	

	//DLL���� �֐����擾
	//@inFunctionName		�֐���
	//������������Ă��Ȃ��ꍇ�� RDoNotInitaliseException �ł��B
	//����Ȋ֐��Ȃ��ꍇ�� RNotSupportException �ł��B
	FARPROC GetProcAddress(const char* inFunctionName) throw(RDoNotInitaliseException,RNotSupportException)
	{
		if (!IsReading()) 
		{
			throw RDoNotInitaliseException("RAutoLibraryWithException:%s �֐���ǂݍ��߂Ƃ����܂������A���������炳��Ă܂���",inFunctionName);
		}
#ifndef _WIN32_WCE
		FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, inFunctionName);
#else
		MY_USES_CONVERSION;
		FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, _A2W(inFunctionName) );
#endif
		if (ret == NULL)
		{
			throw RNotSupportException("RAutoLibraryWithException:%s �֐��̓T�|�[�g����Ă��܂���",inFunctionName);
		}
		return ret;
	}
	//DLL�͓ǂݍ��܂�Ă����?
	bool IsReading()
	{
		return LabraryHandle != NULL;
	}

private:
	HANDLE	LabraryHandle;
};

#endif // !defined(AFX_RAUTOLIBRARYWITHEXCEPTION_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
