#include "common.h"
#include "XLSoundPlay.h"
#include "XLStringUtil.h"



#pragma comment(lib,"winmm.lib")

//MCI�𗘗p����mp3���Đ�����.
//http://www5b.biglobe.ne.jp/~u-hei/memorandom/aviplaybackfrommemory/step1_mci.html
//http://programeasy.soudesune.net/winapi/35.html
class XLSoundPlay_MCI : public IXLSoundPlay_Interface
{
public:
	XLSoundPlay_MCI()
	{
	}
	virtual ~XLSoundPlay_MCI()
	{
	}

	virtual xreturn::r<bool> play(const std::string & filename)
	{
		std::string ext = XLStringUtil::strtolower(XLStringUtil::baseext(filename));

		MCI_OPEN_PARMS open_parms;
		MCI_PLAY_PARMS play_parms;
		MCI_GENERIC_PARMS parms;
		open_parms.wDeviceID = 0;
		open_parms.lpstrElementName = filename.c_str();
		if (ext == ".mp3") 
		{
			open_parms.lpstrDeviceType = "MPEGVideo";
		}
		else if (ext == ".wav")
		{
			open_parms.lpstrDeviceType = "WaveAudio";
		}
		else
		{
			return xreturn::error("���Ή��̌`��" + ext + "���I������܂����B �t�@�C��:" + filename );
		}

		if(mciSendCommand(0,MCI_OPEN,MCI_WAIT|MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)&open_parms) == 0){
			play_parms.dwFrom = 0;
			mciSendCommand(open_parms.wDeviceID,MCI_PLAY,MCI_WAIT|MCI_FROM,(DWORD)&play_parms);
			mciSendCommand(open_parms.wDeviceID,MCI_STOP,MCI_WAIT,(DWORD)&parms);
		}

		return true;
	}
};


//�Ȃ�Ƃ����ĉ��y(wav/mp3��)���Đ�����.
XLSoundPlay::XLSoundPlay()
{
	this->Object = new XLSoundPlay_MCI;
}

XLSoundPlay::~XLSoundPlay()
{
	delete this->Object;
}

xreturn::r<bool> XLSoundPlay::play(const std::string & filename) 
{
	return this->Object->play(filename);
}

