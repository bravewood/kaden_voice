// RSpeechRecognition.cpp: RSpeechRecognition �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RSpeechRecognition.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSpeechRecognition::RSpeechRecognition()
{

}

RSpeechRecognition::~RSpeechRecognition()
{

}

//�����F���̂��߂̃I�u�W�F�N�g�̍\�z.
void RSpeechRecognition::Create(const std::string & inDicticationFilterWord , const std::string & inGrammarXML , HWND inWindow , UINT inCallbackMesage )
{
	USES_CONVERSION;

	HRESULT hr;

	this->DicticationFilterWord = inDicticationFilterWord;
	this->DictationReady = false;
	this->RuleReady = false;
	this->CallbackWindowHandle = inWindow;
	this->CallbackWindowMesage = inCallbackMesage;

	//Dictation
	{
		CComPtr<ISpAudio> cpAudio;
		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�I�[�f�B�I����ǂݍ���ł�
		hr = this->DictationEngine->SetInput( cpAudio, TRUE);  
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationEngine->CreateRecoContext(&this->DictationRecoCtxt);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�F����n��
		hr = this->DictationRecoCtxt->CreateGrammar(0, &this->DictationGrammar);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationGrammar->LoadDictation(NULL, SPLO_STATIC);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationRecoCtxt->SetNotifyCallbackFunction(__callbackDictation , (WPARAM)this , 0);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->DictationGrammar->SetDictationState(SPRS_ACTIVE );
		if(FAILED(hr))	 AfxThrowOleException(hr);
	}
	//���[���x�[�X�̃G���W�������.
	{
		CComPtr<ISpAudio> cpAudio;
		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�I�[�f�B�I����ǂݍ���ł�
		hr = this->RuleEngine->SetInput( cpAudio, TRUE);  
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleEngine->CreateRecoContext(&this->RuleRecoCtxt);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�F����n��
		hr = this->RuleRecoCtxt->CreateGrammar(0, &this->RuleGrammar);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleGrammar->LoadDictation(NULL, SPLO_STATIC);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleGrammar->LoadCmdFromFile( A2W( inGrammarXML.c_str() ) ,SPLO_STATIC);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->RuleRecoCtxt->SetNotifyCallbackFunction(__callbackRule , (WPARAM)this , 0);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�^���J�n
		hr = this->RuleGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE );
		if(FAILED(hr))	 AfxThrowOleException(hr);
	}
}

void RSpeechRecognition::CallbackDictation()
{
	USES_CONVERSION;
	HRESULT hr;
	CSpEvent event;

	hr = event.GetFrom( this->DictationRecoCtxt );
	if ( FAILED(hr) )	return ;

	//�F����������
	ISpRecoResult* result;
	result = event.RecoResult();

	//�F������������̎擾
	CSpDynamicString dstrText;
	hr = result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
	if ( FAILED(hr) )	return ;
	this->DictationString = W2A(dstrText);

	if ( ! this->RuleReady )
	{
		//�����͊����������A���[���x�[�X���܂��B�����͊��������t���O�𗧂Ă�B
		this->DictationReady = true;
	}
	else
	{
		//�f�B�N�e�[�V�����t�B���^�[�ōi��
		this->RuleReady = false;
		if ( this->DictationString.find(this->DicticationFilterWord) == std::string::npos )
		{
			//�f�B�N�e�[�V�����t�B���^�[�ŗ��Ƃ�
			return ;
		}
		//�R�}���h�F��
		SendMessage(this->CallbackWindowHandle , this->CallbackWindowMesage , 0 , 0);
	}

}

void RSpeechRecognition::CallbackRule()
{
	USES_CONVERSION;
	HRESULT hr;
	std::string dictationString;

	CSpEvent ruleEvent;
	hr = ruleEvent.GetFrom( this->RuleRecoCtxt );
	if ( FAILED(hr) )	return ;

	//�F����������
	ISpRecoResult* result;
	result = ruleEvent.RecoResult();

	//�F������������̎擾
	CSpDynamicString dstrText;
	hr = result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
	if ( FAILED(hr) )	return ;
	this->ResultString = W2A(dstrText);

	//�F���� XML���g�p�����ꍇ�A������ꂽ���ʂ𓾂�.
	SPPHRASE *pPhrase;
	hr = ruleEvent.RecoResult()->GetPhrase(&pPhrase);
	if ( FAILED(hr) )	return ;

	this->ResultMap.clear();
	const SPPHRASEPROPERTY *pProp;
	for (pProp = pPhrase->pProperties; pProp; pProp = pProp->pNextSibling)
	{
		std::string a = W2A(pProp->pszName);
		this->ResultMap[ W2A(pProp->pszName) ] = W2A(pProp->pszValue);
	}
	CoTaskMemFree(pPhrase);

	if ( ! this->DictationReady )
	{
		//�f�B�N�e�[�V�������������Ă��Ȃ��B ���[���͊������Ă���t���O�����𗧂Ă�B
		this->RuleReady = true;
	}
	else
	{
		//�f�B�N�e�[�V�����t�B���^�[�ōi��
		this->DictationReady = false;
		if ( this->DictationString.find(this->DicticationFilterWord) == std::string::npos )
		{
			//�t�B���^�[�ɂ�苑��
			return ;
		}
		//�R�}���h�F��
		SendMessage(this->CallbackWindowHandle , this->CallbackWindowMesage , 0 , 0);
	}
}

