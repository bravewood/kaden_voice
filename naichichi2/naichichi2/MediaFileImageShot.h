#pragma once

#include <string>
#include <shlobj.h>

//�t�@�C�����̉��.
class MediaFileImageShot
{
public:
	MediaFileImageShot::MediaFileImageShot();
	virtual MediaFileImageShot::~MediaFileImageShot();
	xreturn::r<bool> MediaFileImageShot::Create(MainWindow* poolMainWindow);

	xreturn::r<bool> MediaFileImageShot::Shot(const std::string& dir,const std::string& filename ,std::vector<char> * image );


private:
	//�t�@�C��������LPITEMIDLIST�����Ƃ����ċ��߂܂��B
	xreturn::r<bool> MediaFileImageShot::ConvertFileNameToPIDL(const std::string& fullpath,LPITEMIDLIST* lpcItemList) const;
	//HBITMAP�� *.jpeg�`���Ń������ɕۑ�
	xreturn::r<bool> MediaFileImageShot::ConvertHBITMAPtoJPEGByte(HBITMAP hbitmap,std::vector<char> * image) const;
	//HICON��jpeg�ŕۑ����܂�
	xreturn::r<bool> MediaFileImageShot::ConvertHICONtoBytes(HICON hicon,std::vector<char> * image) const;

};
