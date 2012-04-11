#pragma once
#ifdef _WINDOWS
//windows���� COM �� IID_IFELanguage  ���g���܂��B
//http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=9739 ����_�E�����[�h���Ă��Ă�
//�_�E�����[�h������ C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include �Ƃ��ɂł��Ԃ������
const IID IID_IFELanguage = { 0x019f7152, 0xe6db, 0x11d0, { 0x83, 0xc3, 0x00, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e }};
#include <msime.h>
#else
//linux���� mecab��ǂݒ��o�����Ɏg���x���I�� -llibmecab �ł��
#include <mecab.h>
#endif

class MediaFileTextToYomi
{
public:
	MediaFileTextToYomi();
	virtual ~MediaFileTextToYomi();
	xreturn::r<bool> Create();
	xreturn::r<std::string> MediaFileTextToYomi::Parse(const std::string& str);
private:
#ifdef _WINDOWS
	IFELanguage* FELanguage;
#else
	MeCab::Tagger *Tagger;
#endif
};
