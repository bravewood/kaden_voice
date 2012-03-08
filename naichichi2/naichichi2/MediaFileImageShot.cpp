#include "common.h"
#include "MainWindow.h"
//#include "ScriptRunner.h"
#include "MediaFileImageShot.h"
#include "XLStringUtil.h"
#include "windows_encoding.h"
#include <atlbase.h>
#include <shlwapi.h>
#include <thumbcache.h> //for vista
#include "XLImage.h"

MediaFileImageShot::MediaFileImageShot()
{
}

MediaFileImageShot::~MediaFileImageShot()
{
}
xreturn::r<bool> MediaFileImageShot::Create(MainWindow* poolMainWindow)
{
	return true;
}

//�t�@�C������T���l�C����bitmap�`���Ŏ擾����.
xreturn::r<bool> MediaFileImageShot::Shot(const std::string& dir,const std::string& filename ,std::vector<char> * image )
{
	HRESULT hr;
	_USE_WINDOWS_ENCODING;

	//�Q�l http://eternalwindows.jp/shell/shellname/shellname05.html
	//     http://social.msdn.microsoft.com/forums/ja-JP/vcgeneralja/thread/37a6b7e5-5cfd-4487-980f-06ed568e8cb4
	const LONG size = 255;

	//�V�F�����ł̃������m�ۊJ���͂���������Ȃ��ƃ_������
	CComPtr<IMalloc> imalloc;
	hr = SHGetMalloc(&imalloc);
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	//�f�B�X�N�g�b�v�t�H���_ root�ɂ�����炵��
	CComPtr<IShellFolder> desktopFolder;
	hr = ::SHGetDesktopFolder( &desktopFolder );
	if(FAILED(hr))	 return xreturn::windowsError(hr);

//	ULONG         chEaten;	//������̃T�C�Y���󂯎��܂��B
	ULONG         dwAttributes;	//�������󂯎��܂��B
	//�t�@�C���ł͂Ȃ��A�e�t�H���_�� LPITEMIDLIST ���擾���܂��B
	LPITEMIDLIST pciItemListDirectory;
	std::string dir2 = dir + "\\" ;
	hr = desktopFolder->ParseDisplayName( NULL, NULL, _A2W(dir2.c_str() ), NULL , &pciItemListDirectory, &dwAttributes);
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	//�t�H���_�� LPITEMIDLIST ���� IShellFolder �ɕό`�����܂��B
	CComPtr<IShellFolder> shellFolderDirectory;
	hr = desktopFolder->BindToObject(pciItemListDirectory, NULL, IID_IShellFolder, (void**)&shellFolderDirectory);
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	//�t�H���_��IShellFolder ����t�@�C������ LPITEMIDLIST �ɕό`�����܂��B
	LPITEMIDLIST pciItemListFilename;
	std::string fullpath = filename;
	hr = shellFolderDirectory->ParseDisplayName( NULL, NULL, _A2W(fullpath.c_str() ), NULL , &pciItemListFilename, &dwAttributes);
	if(FAILED(hr))	 return xreturn::windowsError(hr);


	CComPtr<IExtractImage> extractImage;
//	CComPtr<IExtractImage2> extractImage2;
	CComPtr<IThumbnailProvider> thumbnailProvider;
	CComPtr<IExtractIcon> extractIcon;

	hr = shellFolderDirectory->GetUIObjectOf(NULL, 1,(LPCITEMIDLIST*) &pciItemListFilename, IID_IThumbnailProvider, NULL, (void **)&thumbnailProvider);
	if(SUCCEEDED(hr))
	{
		HBITMAP bitmap;
		WTS_ALPHATYPE alphaType;
		hr = thumbnailProvider->GetThumbnail(size, &bitmap, &alphaType);
		if(SUCCEEDED(hr))
		{
//			imalloc->Free(pciItemListFilename);
//			imalloc->Free(shellFolderDirectory);
			auto r = this->ConvertHBITMAPtoJPEGByte(bitmap , image);
			if (!r)
			{
				return xreturn::error( r.getError() );
			}
			//DeleteObject(bitmap); //����̂��ȁH
			return true;
		}
	}
	//IID_IExtractImage ���������Ă��܂����H
	hr = shellFolderDirectory->GetUIObjectOf(NULL, 1,(LPCITEMIDLIST*) &pciItemListFilename, IID_IExtractImage, NULL, (void **)&extractImage);
	if(SUCCEEDED(hr))
	{
		WCHAR pathBufferW[MAX_PATH];
		HBITMAP bitmap;
		SIZE structSize;
		DWORD dwFlags;
		
		structSize.cx = size;
		structSize.cy = size;
//		dwFlags = IEIFLAG_ASPECT | IEIFLAG_SCREEN|IEIFLAG_CACHE;
		dwFlags = IEIFLAG_SCREEN|IEIFLAG_CACHE;
		hr = extractImage->GetLocation(pathBufferW, MAX_PATH, NULL, &structSize, 32, &dwFlags);
		if(SUCCEEDED(hr))
		{
			hr = extractImage->Extract(&bitmap);
			if(SUCCEEDED(hr))
			{
//				imalloc->Free(shellFolderDirectory);
//				imalloc->Free(pciItemListFilename);
				auto r = this->ConvertHBITMAPtoJPEGByte(bitmap , image);
				if (!r)
				{
					return xreturn::error( r.getError() );
				}
				//DeleteObject(bitmap); //����̂��ȁH
				return true;
			}
		}
	}
/*
	//�A�C�R��
	hr = shellFolderDirectory->GetUIObjectOf(NULL, 1,(LPCITEMIDLIST*) &pciItemListFilename, IID_IExtractIcon, NULL, (void **)&extractIcon);
	if(SUCCEEDED(hr))
	{
		char pathBufferA[MAX_PATH];
		UINT uFlags;
		int  nIndex;
		HICON phicon;
		hr = extractIcon->GetIconLocation(GIL_FORSHELL, pathBufferA, MAX_PATH, &nIndex, &uFlags);
		if(SUCCEEDED(hr))
		{
			hr = extractIcon->Extract(pathBufferA, nIndex, &phicon, NULL, MAKELONG(size, 16));
			if(SUCCEEDED(hr))
			{
//				imalloc->Free(shellFolderDirectory);
//				imalloc->Free(pciItemListFilename);
				auto r = this->ConvertHICONtoBytes(phicon , image);
				if (!r)
				{
					return xreturn::error( r.getError() );
				}
				return true;
			}
			else
			{
				imalloc->Free(shellFolderDirectory);
				imalloc->Free(pciItemListFilename);
				WORD wIndex = 0;

				phicon = ExtractAssociatedIconA(NULL, pathBufferA, &wIndex);
				if (phicon != NULL)
				{
					auto r = this->ConvertHICONtoBytes(phicon , image);
					if (!r)
					{
						return xreturn::error( r.getError() );
					}
					return true;
				}
			}
		}
	}
*/
	//db�e�ʂ��팸���邽�߁A�A�C�R���̓V���b�g���B��Ȃ����Ƃɂ����B
	image->resize(0);
	return false;
}

//�t�@�C��������LPITEMIDLIST�����Ƃ����ċ��߂܂��B
xreturn::r<bool> MediaFileImageShot::ConvertFileNameToPIDL(const std::string& fullpath,LPITEMIDLIST* lpcItemList) const
{
	HRESULT hr;
	_USE_WINDOWS_ENCODING;

	CComPtr<IShellFolder> desktopFolder;
	hr = ::SHGetDesktopFolder( &desktopFolder );
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	ULONG         chEaten;	//������̃T�C�Y���󂯎��܂��B
	ULONG         dwAttributes;	//�������󂯎��܂��B

	//�@���ۂ�ITEMIDLIST���擾���܂��B
	hr = desktopFolder->ParseDisplayName( NULL, NULL, _A2W(fullpath.c_str() ), &chEaten, lpcItemList, &dwAttributes);
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	return true;
}

//HBITMAP�� *.jpeg�`���Ń������ɕۑ�
//http://hpcgi1.nifty.com/MADIA/Vcbbs/wwwlng.cgi?print+200906/09060004.txt
//http://katahiromz.web.fc2.com/win32/savepic.html
xreturn::r<bool> MediaFileImageShot::ConvertHBITMAPtoJPEGByte(HBITMAP hbitmap,std::vector<char> * image) const
{
	XLImage xlimage;
	auto r1 = xlimage.Load(hbitmap);
	if (!r1)
	{
		return xreturn::error(r1.getError());
	}
	auto r2 = xlimage.Save(".jpeg",image,70);
	if (!r2)
	{
		return xreturn::error(r1.getError());
	}
	return true;
}

//HICON��jpeg�ŕۑ����܂�
xreturn::r<bool> MediaFileImageShot::ConvertHICONtoBytes(HICON hicon,std::vector<char> * image) const
{
	XLImage xlimage;
	auto r1 = xlimage.Load(hicon);
	if (!r1)
	{
		return xreturn::error(r1.getError());
	}
	auto r2 = xlimage.Save(".jpeg",image,70);
	if (!r2)
	{
		return xreturn::error(r1.getError());
	}
	return true;
}
