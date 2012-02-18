#pragma once
#include "xreturn.h"

//dll�ǂݍ��݃w���p�[
class LoadLibraryHelper
{
private:
	//DLL �C���X�^���X.
	HMODULE DllInstance;

public:
	LoadLibraryHelper()
	{
		this->DllInstance = NULL;
	}
	virtual ~LoadLibraryHelper()
	{
		if (this->DllInstance != NULL)
		{
			::FreeLibrary(this->DllInstance);
			this->DllInstance = NULL;
		}
	}
	xreturn::r<bool> Load( const std::string& inDLLName )//std�ǂ�ł��Ȃ����Ί��Ƃ��̂��߂� const char* �ō��.
	{
		assert(this->DllInstance == NULL);

		this->DllInstance = ::LoadLibraryA(inDLLName.c_str() );
		if ( this->DllInstance == NULL )
		{
			DWORD lastError = ::GetLastError();
			return xreturn::windowsError(std::string() + "���C�u����" + inDLLName + "��ǂݍ��߂܂���ł���",lastError);
		}
		return true;
	}
	FARPROC GetProcAddress(const char* inProcName)
	{
		assert(this->DllInstance != NULL);
		return ::GetProcAddress(this->DllInstance,inProcName);
	}
};
