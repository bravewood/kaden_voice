﻿// Recognition_SAPI.h: Recognition_SAPI クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Recognition_SAPI_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_)
#define AFX_Recognition_SAPI_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <atlbase.h>
#include "RSimpleComPtr.h"
#include <sapi.h>
#include <sphelper.h>

class Recognition_SAPI  : public ISpeechRecognitionInterface
{
public:
	Recognition_SAPI();
	virtual ~Recognition_SAPI();
	//音声認識のためのオブジェクトの構築.
	virtual xreturn::r<bool> Recognition_SAPI::Create(MainWindow* poolMainWindow);

	//呼びかけを設定します。
	//設定したあと、 CommitRule() てしてね。
	virtual xreturn::r<bool> Recognition_SAPI::SetYobikake(const std::list<std::string> & yobikakeList);
	//認識結果で不感染なものを捨てる基準値を設定します。
	virtual xreturn::r<bool> Recognition_SAPI::SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter,bool useDictationFilter);
	//コマンドに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> Recognition_SAPI::AddCommandRegexp(CallbackDataStruct & callback,const std::string & str);
	//テンポラリルールに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> Recognition_SAPI::AddTemporaryRegexp(CallbackDataStruct & callback,const std::string & str);
	//テンポラリルールをすべてクリアします
	virtual xreturn::r<bool> Recognition_SAPI::ClearTemporary() ;
	//構築したルールを音声認識エンジンにコミットします。
	virtual xreturn::r<bool> Recognition_SAPI::CommitRule();

private:
	//認識した結果の解析
	xreturn::r<bool> Recognition_SAPI::CallbackReco();
	void Recognition_SAPI::Callback(WPARAM wParam, LPARAM lParam);
	//マッチしたphraseから正規表現キャプチャ () による文字列の抽出.
	bool Recognition_SAPI::PhraseToRegexpCapture(SPPHRASE *pPhrase,std::map<std::string , std::string>* capture, double * SREngineConfidenceAvg) const;
	//ディクテーションフィルターで呼びかけ文字列が入っているか確認する.
	bool Recognition_SAPI::checkDictation(const std::string & dictationString) const;
	//ルールベースで認識した結果の音声部分をもう一度 ディクテーションにかけて認識した結果を取得します。
	xreturn::r<std::string> Recognition_SAPI::convertDictation(ISpRecoResult* result);
	//音声認識ルールを構築します。 正規表現にも対応しています。
	xreturn::r<bool> Recognition_SAPI::AddRegexp(unsigned int id,const std::string & str ,SPSTATEHANDLE stateHandle ) ;
	//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
	xreturn::r<bool> Recognition_SAPI::AddRegexpImpl(const SPPROPERTYINFO* prop,const std::wstring & str, SPSTATEHANDLE stateHandle);

private:
	//ルールベース
	RSimpleComPtr<ISpRecognizer>		RuleEngine;
	RSimpleComPtr<ISpRecoContext>		RuleRecoCtxt;
	RSimpleComPtr<ISpRecoGrammar>		RuleGrammar;

	//Dictation
	RSimpleComPtr<ISpRecognizer>		DictationEngine;
	RSimpleComPtr<ISpRecoContext>		DictationRecoCtxt;
	RSimpleComPtr<ISpRecoGrammar>		DictationGrammar;
	UINT								GlobalRuleNodeCount ;
	UINT								LocalCaptureRuleNodeCount ;
	SPSTATEHANDLE FilterRuleHandleHandle;

	SPSTATEHANDLE BasicRuleHandleHandle;
	SPSTATEHANDLE TemporaryRuleHandle;
	SPSTATEHANDLE YobikakeRuleHandle;
	SPSTATEHANDLE CommandRuleHandle;

	int TemporaryRuleCount ;
	bool IsNeedUpdateRule;

	bool   UseDictationFilter;
	double TemporaryRuleConfidenceFilter;
	double YobikakeRuleConfidenceFilter;
	double BasicRuleConfidenceFilter;
	std::list<std::string> YobikakeListArray;

	MainWindow* PoolMainWindow;

	std::vector<CallbackDataStruct> CallbackDictionary;
};

#endif // !defined(AFX_Recognition_SAPI_H__1477FE93_D7A8_4F29_A369_60E33C71B2B7__INCLUDED_)
