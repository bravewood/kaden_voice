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
void RSpeechRecognition::Create(const std::string & inToWave,const std::string & inGrammarXML , HWND inWindow , UINT inCallbackMesage )
{
	USES_CONVERSION;

	HRESULT hr;

	// �F���G���W���I�u�W�F�N�g�̍쐬
	//	CLSID_SpSharedRecognizer		���L�I�u�W�F�N�g
	//	CLSID_SpInprocRecognizer		�A�v��������
	
	if ( inToWave.empty() )
	{
//		hr = this->Engine.CoCreateInstance(CLSID_SpSharedRecognizer);
//		if(FAILED(hr))	 RComException(hr , "CLSID_SpSharedRecognizer �\�z �Ɏ��s");
		hr = this->Engine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		CComPtr<ISpAudio> cpAudio;
		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�F���G���W���̃G���W���̃f�B�t�H���g�ɐݒ肷��B
		hr = this->Engine->SetInput(cpAudio, TRUE);
		if(FAILED(hr))	 AfxThrowOleException(hr);

//		hr = this->Engine->SetRecoState( SPRST_ACTIVE );
//		if(FAILED(hr))	 AfxThrowOleException(hr);
	}
	else
	{
		CComPtr<ISpStream> cpStream;

		hr = this->Engine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = cpStream.CoCreateInstance(CLSID_SpStream);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = cpStream->BindToFile( A2W( inToWave.c_str() ) , SPFM_OPEN_READONLY , NULL , NULL,  SPFEI_ALL_EVENTS);  
		if(FAILED(hr))	 AfxThrowOleException(hr);

		hr = this->Engine->SetInput( cpStream, TRUE);  
		if(FAILED(hr))	 AfxThrowOleException(hr);
	}

	// �F���R���e�N�X�g�I�u�W�F�N�g�̍쐬
	hr = this->Engine->CreateRecoContext(&this->RecoCtxt);
	if(FAILED(hr))	 AfxThrowOleException(hr);

//	hr = this->RecoCtxt->SetNotifyWin32Event();	//�C�x���g�̏ꍇ.. �������̕����D���Ȃ񂾂���thread������...
//	if(FAILED(hr))	 AfxThrowOleException(hr);
	hr = this->RecoCtxt->SetNotifyWindowMessage(inWindow,inCallbackMesage,0,0);	//window���b�Z�[�W�̏ꍇ... 
	if(FAILED(hr))	 AfxThrowOleException(hr);

	hr = this->RecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
	if(FAILED(hr))	 AfxThrowOleException(hr);

	hr = this->RecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL);
	if(FAILED(hr))	 AfxThrowOleException(hr);


	//���C���ƂȂ镶�@�̍쐬
	hr = this->RecoCtxt->CreateGrammar(0, &this->DictationGrammar);
	if(FAILED(hr))	 AfxThrowOleException(hr);

	hr = this->DictationGrammar->LoadDictation(NULL, SPLO_STATIC);
	if(FAILED(hr))	 AfxThrowOleException(hr);

	if ( inGrammarXML.empty() )
	{
		//�^���J�n
		hr = this->DictationGrammar->SetDictationState( SPRS_ACTIVE );
		if(FAILED(hr))	 AfxThrowOleException(hr);
	}
	else
	{
		//���[�U�w��t�@�C������̃��[�h
		hr = this->DictationGrammar->LoadCmdFromFile( A2W( inGrammarXML.c_str() ) ,SPLO_STATIC);
		if(FAILED(hr))	 AfxThrowOleException(hr);

		//�^���J�n
		hr = this->DictationGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE );
		if(FAILED(hr))	 AfxThrowOleException(hr);
	}
}

//�F���J�n
void RSpeechRecognition::Listen()
{
	USES_CONVERSION;
	HRESULT hr;

	CSpEvent event;

//�����ɂ��Ă���Ƃ��́A�^�����I�����Ă��鎞��!
//	//�^�����I���܂ő�ҋ@
//	hr = this->RecoCtxt->WaitForNotifyEvent(INFINITE);
//	if ( FAILED(hr) )	AfxThrowOleException(hr);

	hr = event.GetFrom( this->RecoCtxt );
	if ( FAILED(hr) )	AfxThrowOleException(hr);

	//�F����������
	ISpRecoResult* result;
	result = event.RecoResult();

	//�F������������̎擾
	CSpDynamicString dstrText;
	hr = result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
	if ( FAILED(hr) )	AfxThrowOleException(hr);
	this->ResultString = W2A(dstrText);

	//�F���� XML���g�p�����ꍇ�A������ꂽ���ʂ𓾂�.
	SPPHRASE *pPhrase;
	event.RecoResult()->GetPhrase(&pPhrase);
	const SPPHRASEPROPERTY *pProp;
	for (pProp = pPhrase->pProperties; pProp; pProp = pProp->pNextSibling)
	{
		std::string a = W2A(pProp->pszName);
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
		if(FAILED(hr))	AfxThrowOleException(hr);

		hr = event.RecoResult()->GetAudio( 0, 0, &ResultStream );
		if ( FAILED(hr) )	AfxThrowOleException(hr);
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
