#pragma once

#include <string>
#import <SHELL32.dll> rename("ShellExecute","SHELL32ShellExecute")

//�t�@�C�����̉��.
class MediaFileAnalize
{
public:
	MediaFileAnalize::MediaFileAnalize();
	virtual MediaFileAnalize::~MediaFileAnalize();
	xreturn::r<bool> MediaFileAnalize::Create(MainWindow* poolMainWindow,const std::string& luaCommand);

	bool MediaFileAnalize::Analize(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank, std::string* searchdata);

	//�����\�ȃf�[�^�����
	std::string makesearcableData(const std::string& str );
	//���X�g����o�C�O�����𐶐����܂��B
	std::string MediaFileAnalize::makeListToBigram(const std::list<std::string>& list ) const;
	//���[�U�T�C�h�������Ɏg���f�[�^�����
	std::string MediaFileAnalize::makeUserSideSearchableData(const std::string& str ) const;
	//���X�g�̒��g�𐳋K�����܂��B
	std::list<std::string> MediaFileAnalize::makeListToCleanup(const std::list<std::string>& list ) const;

private:
	bool MediaFileAnalize::AnalizeLua(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank);
	bool MediaFileAnalize::AnalizeCOM(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank);

	Shell32::IShellDispatchPtr PtrShell;
	ScriptRunner* Runner;
	MainWindow* PoolMainWindow;
};
