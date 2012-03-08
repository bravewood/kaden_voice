#include "common.h"
#include "MecabControl.h"

MecabControl::MecabControl(void)
{
	this->Tagger = NULL;
}


MecabControl::~MecabControl(void)
{
}

xreturn::r<bool> MecabControl::Create(MainWindow* poolMainWindow,const std::string& dicpath)
{
	std::string option = std::string("-d ") + dicpath;
	this->Tagger = MeCab::createTagger(option.c_str());
	if (this->Tagger == NULL)
	{
		return xreturn::error("mecab �̃C���X�^���X�����܂���ł���");
	}
	return true;
}

void MecabControl::Parse(const std::string& str,const std::function< void(const MeCab::Node* node) >& callbackNode)
{
	//���b�N����̂ŁA callbackNode �̒��łƂĂ��Ȃ��d�����Ƃ��Ȃ��łˁB
	{
		boost::unique_lock<boost::mutex> al(this->lock);

		const MeCab::Node* node = this->Tagger->parseToNode( str.c_str() );
		if (node == NULL)
		{
			return ;
		}
		for (; node; node = node->next) 
		{
			callbackNode(node);
		}
	}

}
