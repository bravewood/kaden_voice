﻿#pragma once
class XLDebugUtil
{
public:
	//メインスレッドIDを取得する.
	static boost::thread::id GetMainThreadID()
	{
		static auto threadid = boost::this_thread::get_id();
		return threadid;
	}
	//MemoryToFile:		メモリをファイルに
	static void MemoryToFile(const std::string& inFilename ,const void* inMemory , int inSize);
	//MemoryToFile:		メモリをファイルに
	static void MemoryToFile(const std::string& inFilename ,const char* inMemory );
	//MemoryToFile:		メモリをファイルに
	static void MemoryToFile(const std::string& inFilename ,std::string& inMemory );
	//FileCheck:			二つのファイルが同じかどうかチェック.
	static void FileVerify(const std::string& inSrc,const std::string& inDest);
};
#if _DEBUG
//メインスレッドでしか動きません
	#define ASSERT_IS_MAIN_THREAD_RUNNING()			(assert(XLDebugUtil::GetMainThreadID() == boost::this_thread::get_id()))
//メインスレッド以外で動きます
	#define ASSERT___IS_WORKER_THREAD_RUNNING()		(assert(XLDebugUtil::GetMainThreadID() != boost::this_thread::get_id()))
#else
	#define ASSERT_IS_MAIN_THREAD_RUNNING()	
	#define ASSERT___IS_WORKER_THREAD_RUNNING()		
#endif //_DEBUG
