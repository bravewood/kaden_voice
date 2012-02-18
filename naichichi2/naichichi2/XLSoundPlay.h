#pragma once

typedef void (*XLSoundPlay_CALLBACK)(int _this, int event);

class IXLSoundPlay_Interface
{
public:
	virtual xreturn::r<bool> play(const std::string & filename) = 0;
};

//�Ȃ�Ƃ�����mp3���Đ�����.
class XLSoundPlay
{
public:
	XLSoundPlay();
	virtual ~XLSoundPlay();

	xreturn::r<bool> play(const std::string & filename) ;
private:
	IXLSoundPlay_Interface* Object;
};
