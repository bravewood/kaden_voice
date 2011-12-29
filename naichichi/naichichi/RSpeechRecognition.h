// RSpeechRecognition.h: RSpeechRecognition �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPEECHRECOGNITION_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_)
#define AFX_RSPEECHRECOGNITION_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <sapi.h>
#include <sphelper.h>

class RSpeechRecognition  
{
public:
	RSpeechRecognition();
	virtual ~RSpeechRecognition();
	//�����F���̂��߂̃I�u�W�F�N�g�̍\�z.
	void RSpeechRecognition::Create(const std::string & inDicticationFilterWord,const std::string & inGrammarXML , HWND inWindow , UINT inCallbackMesage );
	//�F���J�n
	bool RSpeechRecognition::Listen() ;

	const std::string & getResultString() const
	{
		return ResultString;
	}
	const std::string  getResultMap(const std::string & inKey) const
	{
		auto i = ResultMap.find(inKey);
		if (i == ResultMap.end() ) return "";

		return (*i).second;
	}
private:
	static void __stdcall __callbackDictation(WPARAM wParam, LPARAM lParam)
	{
		((RSpeechRecognition*)wParam)->CallbackDictation();
	}

	static void __stdcall __callbackRule(WPARAM wParam, LPARAM lParam)
	{
		((RSpeechRecognition*)wParam)->CallbackRule();
	}

private:
	void CallbackRule();
	void CallbackDictation();

private:
	//���[���x�[�X
	CComPtr<ISpRecognizer>		RuleEngine;
	CComPtr<ISpRecoContext>		RuleRecoCtxt;
	CComPtr<ISpRecoGrammar>     RuleGrammar;

	//Dictation
	CComPtr<ISpRecognizer>		DictationEngine;
	CComPtr<ISpRecoContext>		DictationRecoCtxt;
	CComPtr<ISpRecoGrammar>     DictationGrammar;
	std::string                 DicticationFilterWord;
	HWND						CallbackWindowHandle;
	UINT						CallbackWindowMesage;

	//�F������
	std::string			ResultString;
	std::map<std::string,std::string>		ResultMap;
	bool				RuleReady;

	std::string			DictationString;
	bool				DictationReady;
};

#endif // !defined(AFX_RSPEECHRECOGNITION_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_)
