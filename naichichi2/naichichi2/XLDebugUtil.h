#pragma once
class XLDebugUtil
{
public:
	//���C���X���b�hID���擾����.
	static DWORD GetMainThreadID()
	{
		static DWORD threadid = ::GetCurrentThreadId();
		return threadid;
	}
	//MemoryToFile:		���������t�@�C����
	static void XLDebugUtil::MemoryToFile(const std::string& inFilename ,const void* inMemory , int inSize);
	//MemoryToFile:		���������t�@�C����
	static void XLDebugUtil::MemoryToFile(const std::string& inFilename ,const char* inMemory );
	//MemoryToFile:		���������t�@�C����
	static void XLDebugUtil::MemoryToFile(const std::string& inFilename ,std::string& inMemory );
	//FileCheck:			��̃t�@�C�����������ǂ����`�F�b�N.
	static void XLDebugUtil::FileVerify(const std::string& inSrc,const std::string& inDest);
};
#if _DEBUG
//���C���X���b�h�ł��������܂���
	#define ASSERT_IS_MAIN_THREAD_RUNNING()			(assert(XLDebugUtil::GetMainThreadID() == ::GetCurrentThreadId()))
//���C���X���b�h�ȊO�œ����܂�
	#define ASSERT___IS_WORKER_THREAD_RUNNING()		(assert(XLDebugUtil::GetMainThreadID() != ::GetCurrentThreadId()))
#else
	#define ASSERT_IS_MAIN_THREAD_RUNNING()	
	#define ASSERT___IS_WORKER_THREAD_RUNNING()		
#endif //_DEBUG
