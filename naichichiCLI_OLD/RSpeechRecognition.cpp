// RSpeechRecognition.cpp: RSpeechRecognition �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RSpeechRecognition.h"
#include "RStdioFile.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSpeechRecognition::RSpeechRecognition()
{

}

RSpeechRecognition::~RSpeechRecognition()
{

}

void RSpeechRecognition::Create(const string & inToWave) throw(RComException)
{
	string dummy = "";
	this->Create(inToWave,dummy);
}

//�����F���̂��߂̃I�u�W�F�N�g�̍\�z.
void RSpeechRecognition::Create(const string & inToWave,const string & inGrammarXML) throw(RComException)
{
	USES_CONVERSION;

	HRESULT hr;

	// �F���G���W���I�u�W�F�N�g�̍쐬
	//	CLSID_SpSharedRecognizer		���L�I�u�W�F�N�g
	//	CLSID_SpInprocRecognizer		�A�v��������
	
	if ( inToWave.empty() )
	{
//		hr = this->Engine.CoCreateInstance(CLSID_SpSharedRecognizer);
//		if(FAILED(hr))	throw RComException(hr , "CLSID_SpSharedRecognizer �\�z �Ɏ��s");
		hr = this->Engine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	throw RComException(hr , "CLSID_SpInprocRecognizer �\�z �Ɏ��s");


		CComPtr<ISpAudio> cpAudio;
		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
		if(FAILED(hr))	throw RComException(hr , "SpCreateDefaultObjectFromCategoryId �Ɏ��s");

		//�F���G���W���̃G���W���̃f�B�t�H���g�ɐݒ肷��B
		hr = this->Engine->SetInput(cpAudio, TRUE);
		if(FAILED(hr))	throw RComException(hr , "SetInput �Ɏ��s");

//		hr = this->Engine->SetRecoState( SPRST_ACTIVE );
//		if(FAILED(hr))	throw RComException(hr , "SetRecoState �Ɏ��s");
	}
	else
	{
		CComPtr<ISpStream> cpStream;

		hr = this->Engine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	throw RComException(hr , "CLSID_SpInprocRecognizer �\�z �Ɏ��s");

		hr = cpStream.CoCreateInstance(CLSID_SpStream);
		if(FAILED(hr))	throw RComException(hr , "CoCreateInstance  CLSID_SpStream �Ɏ��s");

		hr = cpStream->BindToFile( A2W( inToWave.c_str() ) , SPFM_OPEN_READONLY , NULL , NULL,  SPFEI_ALL_EVENTS);  
	    if(FAILED(hr))	throw RComException(hr , "BindToFile �Ɏ��s");

		hr = this->Engine->SetInput( cpStream, TRUE);  
	    if(FAILED(hr))	throw RComException( this->Engine , CLSID_SpSharedRecognizer , hr , "SetInput �Ɏ��s");
	}

	// �F���R���e�N�X�g�I�u�W�F�N�g�̍쐬
	hr = this->Engine->CreateRecoContext(&this->RecoCtxt);
	if(FAILED(hr))	throw RComException(hr , "CreateRecoContext �Ɏ��s");

	hr = this->RecoCtxt->SetNotifyWin32Event();
	if ( FAILED(hr) )	throw RComException(hr , "SetNotifyWin32Event �Ɏ��s");

	hr = this->RecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
	if ( FAILED(hr) )	throw RComException(hr , "SetInterest �Ɏ��s");

	hr = this->RecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL);
	if ( FAILED(hr) )	throw RComException(hr , "SetAudioOptions �Ɏ��s");


	//���C���ƂȂ镶�@�̍쐬
	hr = this->RecoCtxt->CreateGrammar(0, &this->DictationGrammar);
	if ( FAILED(hr) )	throw RComException(hr , "CreateGrammar �Ɏ��s");

	hr = this->DictationGrammar->LoadDictation(NULL, SPLO_STATIC);
	if ( FAILED(hr) )	throw RComException(hr , "LoadDictation �Ɏ��s");

	if ( inGrammarXML.empty() )
	{
		//�^���J�n
		hr = this->DictationGrammar->SetDictationState( SPRS_ACTIVE );
		if ( FAILED(hr) )	throw RComException(hr , "SetDictationState �Ɏ��s");
	}
	else
	{
		//���[�U�w��t�@�C������̃��[�h
		hr = this->DictationGrammar->LoadCmdFromFile( A2W( inGrammarXML.c_str() ) ,SPLO_STATIC);
		if ( FAILED(hr) )	throw RComException(hr , "LoadCmdFromFile �Ɏ��s");

		//�^���J�n
		hr = this->DictationGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE );
		if ( FAILED(hr) )	throw RComException(hr , "SetRuleState �Ɏ��s");
	}
}

//�F���J�n
void RSpeechRecognition::Listen() throw(RComException)
{
	USES_CONVERSION;
	HRESULT hr;

	CSpEvent event;

	//�^�����I���܂ő�ҋ@
	hr = this->RecoCtxt->WaitForNotifyEvent(INFINITE);
	if ( FAILED(hr) )	throw RComException(hr , "WaitForNotifyEvent �Ɏ��s");

	hr = event.GetFrom( this->RecoCtxt );
	if ( FAILED(hr) )	throw RComException(hr , "GetFrom �Ɏ��s");

	//�F����������
	ISpRecoResult* result;
	result = event.RecoResult();

	//�F������������̎擾
	CSpDynamicString dstrText;
	hr = result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
	if ( FAILED(hr) )	throw RComException(hr , "�^�������e�L�X�g�̎擾�Ɏ��s���܂���");
	this->ResultString = W2A(dstrText);

	//�F���� XML���g�p�����ꍇ�A������ꂽ���ʂ𓾂�.
	SPPHRASE *pPhrase;
	event.RecoResult()->GetPhrase(&pPhrase);
	const SPPHRASEPROPERTY *pProp;
	for (pProp = pPhrase->pProperties; pProp; pProp = pProp->pNextSibling)
	{
		string a = W2A(pProp->pszName);
		this->ResultMap[ W2A(pProp->pszName) ] = W2A(pProp->pszValue);
	}
	CoTaskMemFree(pPhrase);

/*
//�f�o�b�O�̂��߁A�ǂݎ����������wave�t�@�C���ɕۑ����Ă݂�B
	//�t�@�C���ɕۑ�. save
	{
		CComPtr<ISpStreamFormat>	ResultStream;

		CComPtr<ISpVoice> voice;
		hr = this->RecoCtxt->GetVoice(&voice);
		if(FAILED(hr))	throw RComException(hr , "GetVoice �Ɏ��s");

		hr = event.RecoResult()->GetAudio( 0, 0, &ResultStream );
		if ( FAILED(hr) )	throw RComException(hr , "GetAudio �Ɏ��s���܂���");
		{
			CComPtr<ISpStream> cpWavStream; 
			CComPtr<ISpStreamFormat> cpOldStream; 
			CSpStreamFormat OriginalFmt; 
			voice->GetOutputStream( &cpOldStream ); 
			OriginalFmt.AssignFormat(cpOldStream); 
			hr = SPBindToFile( L"C:\\Users\\rti\\Desktop\\naichichi\\test\\output.wav",SPFM_CREATE_ALWAYS, 
				&cpWavStream,&OriginalFmt.FormatId(), 
				OriginalFmt.WaveFormatExPtr() 	); 
			voice->SetOutput(cpWavStream,TRUE); 
		}
	}
*/
}
