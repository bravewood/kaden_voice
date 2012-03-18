#include "common.h"
#include "MainWindow.h"
#include "ScriptRunner.h"
#include "MediaFileAnalize.h"
#include "XLStringUtil.h"
#include "windows_encoding.h"


MediaFileAnalize::MediaFileAnalize()
{
	this->Runner = NULL;
}

MediaFileAnalize::~MediaFileAnalize()
{
	this->PtrShell.Release();

	if (this->Runner)
	{
		delete this->Runner;
		this->Runner = NULL;
	}
}
xreturn::r<bool> MediaFileAnalize::Create(MainWindow* poolMainWindow,const std::string& luaCommand,const std::string& mecabCommand)
{
	this->PoolMainWindow = poolMainWindow;

	this->PtrShell.CreateInstance(__uuidof(Shell32::Shell));
	const std::string luafilename = XLStringUtil::pathcombine( poolMainWindow->Config.GetBaseDirectory() , luaCommand);

	//�X�N���v�g�̃��[�h
	this->Runner = new ScriptRunner(poolMainWindow , false );
	this->Runner->LoadScript(luafilename);

	const std::string mecabdir = XLStringUtil::pathcombine( poolMainWindow->Config.GetBaseDirectory() , mecabCommand);
	this->Mecab.Create( mecabdir );
	return true;

}

xreturn::r<bool> MediaFileAnalize::Analize(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank,  std::string* searchdata )
{
	*part = 0;
	*rank = 0;
	auto r1 = AnalizeLua(dir, filename , title, artist , album ,  alias , part ,rank);
	if ( ! r1  )
	{
		return xreturn::error(r1.getError());
	}
	auto r2 = AnalizeCOM(dir, filename , title, artist , album ,  alias , part ,rank);
	if ( ! r2  )
	{
		return xreturn::error(r2.getError());
	}

	//sqlite�����{�ꕪ���������ɑΉ����Ă��Ȃ��̂ŁA �o�C�O����������Ă�����
	*searchdata = MediaFileAnalize::makesearcableData(filename + " " + *title + " " + *artist + " " + *album + " " + *alias);

	//�^�C�g�����Ȃ��ƕ\���������ɂ�������邢�̂ŁA�^�C�g�����Ȃ��Ȃ�t�@�C���������Ă�����B
	if (makeUserSideSearchableData(*title) == "")
	{
		*title = XLStringUtil::basenameonly(filename);
	}

	return true;
}

xreturn::r<bool> MediaFileAnalize::AnalizeLua(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank)
{
	if ( this->Runner->IsMethodExist("title") )
	{
		*title += this->Runner->callFunction("title",dir,filename);
	}
	if ( this->Runner->IsMethodExist("artist") )
	{
		*artist += this->Runner->callFunction("artist",dir,filename);
	}
	if ( this->Runner->IsMethodExist("album") )
	{
		*album += this->Runner->callFunction("album",dir,filename);
	}
	if ( this->Runner->IsMethodExist("alias") )
	{
		*alias += this->Runner->callFunction("alias",dir,filename);
	}
	return true;
}


xreturn::r<bool> MediaFileAnalize::AnalizeCOM(const std::string& dir,const std::string& filename , std::string* title,std::string* artist ,std::string* album , std::string* alias ,int * part,int * rank)
{
	try
	{
		_variant_t var((short)Shell32::ssfRECENT);

		Shell32::FolderPtr ptrFolder = this->PtrShell->NameSpace( dir.c_str() );
		Shell32::FolderItemPtr ptrItem = ptrFolder->ParseName( filename.c_str() );

		std::string tmpartist;
		tmpartist = ptrFolder->GetDetailsOf( _variant_t((IDispatch *)ptrItem), 13); //�A�[�e�B�X�g
		if (tmpartist == "") tmpartist = ptrFolder->GetDetailsOf( _variant_t((IDispatch *)ptrItem), 20);
		*artist		+= tmpartist;
		*title		+= ptrFolder->GetDetailsOf( _variant_t((IDispatch *)ptrItem), 21);	//�^�C�g��
		*album		+= ptrFolder->GetDetailsOf( _variant_t((IDispatch *)ptrItem), 14);  //�A���o���̃^�C�g��
		*alias		+= ptrFolder->GetDetailsOf( _variant_t((IDispatch *)ptrItem), 16);	//�W������

		ptrItem.Release();
		ptrFolder.Release();
	}
	catch(_com_error& e)
	{
		return xreturn::error(std::string()+"com��OShell32::Shell:" + e.ErrorMessage());
	}
	return true;
}

//�������̑������ׂĂЂ炪�Ȃɒ����܂�
std::string MediaFileAnalize::KanjiAndKanakanaToHiragana(const std::string& str)
{
	std::string yomi;
	this->Mecab.Parse(str , [&](const MeCab::Node* node){
		std::vector<std::string> kammalist = XLStringUtil::split_vector(",",node->feature);
		if (kammalist.size() > 7 && kammalist[7] != "*")
		{
			yomi += kammalist[7];
		}
		else
		{
			yomi += std::string(node->surface, 0,node->length);
		}
	});
	//mecab���ƃJ�^�J�i�ǂ݂������Ȃ��̂ŁA�����I�ɂЂ炪�Ȃɒ����܂��B
	return XLStringUtil::mb_convert_kana(yomi,"cHsa");
}

//�����\�f�[�^�����
std::string MediaFileAnalize::makesearcableData(const std::string& str ) 
{
	std::string s = XLStringUtil::chop(str);
	{
		if ( this->Runner->IsMethodExist("makeplain") )
		{
			s = this->Runner->callFunction("makeplain",s);
		}
	}
	std::string cleaupData,yomi,yomiCleaupData,romaji,kana;

	//�L�����������f�[�^�����܂��B
	cleaupData = this->makeUserSideSearchableData(s);
	if (s == cleaupData) cleaupData = "";

	//�N���[���A�b�v�����ǂ݂����܂��B
	yomiCleaupData =  KanjiAndKanakanaToHiragana(cleaupData);
	if (cleaupData == yomiCleaupData) yomiCleaupData = "";

	//���������݂��Ȃ����܂��B
	yomi = KanjiAndKanakanaToHiragana(s);
	if (yomi == yomiCleaupData) yomi = "";

	//���[�}���̂܂܂œ��ꂽ�ꍇ�̕␳
	romaji = XLStringUtil::mb_convert_typo(yomiCleaupData,"r");
	if (romaji == cleaupData) romaji = "";

	//���ȓ��͂ŕϊ�����Ȃ��œ��͂����ꍇ�̕␳
	kana = XLStringUtil::mb_convert_typo(yomiCleaupData,"k");
	if (kana == cleaupData) kana = "";

	//���̃f�[�^ + �L���������f�[�^ + �L���������ǂ� + �ǂ� + ���[�}�� + �J�i
	//�o�C�O�����̌`��
	return XLStringUtil::join(" ", XLStringUtil::unique(	XLStringUtil::makebigram(   makeUserSideSearchableData(
		str + " " + cleaupData + " " + yomiCleaupData + " " + yomi + " " + romaji + " " + kana
		) ) ) );
}

std::string MediaFileAnalize::makeListToBigram(const std::list<std::string>& list ) const
{
	return XLStringUtil::join(" ", 
			XLStringUtil::unique( 
				XLStringUtil::array_map(list , [](const std::string& _) -> std::string 
					{
						return XLStringUtil::join(" ", XLStringUtil::unique(XLStringUtil::makebigram(_)));
					} 
				) 
		)
	);
}

//���X�g�̒��g�𐳋K�����܂��B
std::list<std::string> MediaFileAnalize::makeListToCleanup(const std::list<std::string>& list ) const
{
	return 
		XLStringUtil::array_map(list , [this](const std::string& _) -> std::string 
			{
				return this->makeUserSideSearchableData(_);
			} 
		) ;
		
}
//���[�U�T�C�h�������Ɏg���f�[�^�����
std::string MediaFileAnalize::makeUserSideSearchableData(const std::string& str ) const
{
	std::string s = XLStringUtil::chop(str);

	//����Ȃ��L�����N�^�[
	const char* removeCharTable[] = {
		"!","\"","#","$","%","&","'","(",")","=","-","~","^","|","\\","[","]","{","}","+",";","*",":","?",",",".","<",">","/"
		,"�B","�A","�","�","��","��","��","�E"
		,NULL //�I�[
	};
	//�ǂݕ��̐��K��
	s = XLStringUtil::mb_convert_kana(s,"cHsa");
	//�s�v�ȋL���̏���
	s = XLStringUtil::remove( s , removeCharTable );
	return s;
}
