// XLImage.cpp: XLImage �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "XLImage.h"
#include "XLStringUtil.h"
#include <atlbase.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLImage::XLImage()
{
	this->bitmap = NULL;
}

XLImage::~XLImage()
{
	Clear();
}

void XLImage::Clear()
{
	if (this->bitmap)
	{
		delete this->bitmap;
		this->bitmap = NULL;
	}
}

//�摜�ǂݍ���
xreturn::r<bool> XLImage::Load(const std::string & fileName)
{
	_USE_WINDOWS_ENCODING;

	Clear();
	this->bitmap = Gdiplus::Bitmap::FromFile(_A2W(fileName.c_str()) , TRUE);
	if (!this->bitmap)
	{
		return xreturn::error(std::string() + "�t�@�C��" + fileName + "�̓ǂݍ��݂Ɏ��s���܂���");
	}
	return true;
}

//�摜�ǂݍ���
xreturn::r<bool> XLImage::Load(const std::vector<char> & data)
{
	Clear();
	//GlobalFree�� IStream ���s���B
	HGLOBAL memoryHandle = GlobalAlloc(GMEM_MOVEABLE, data.size());
	//�]��
	{
		void * memory = GlobalLock(memoryHandle);
		memcpy(memory , &data[0] , data.size() );
		GlobalUnlock(memory);
	}

	//����������X�g���[���쐬
	CComPtr<IStream> iStream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(memoryHandle, TRUE, &iStream);
	if(FAILED(hr))	 return xreturn::windowsError(hr);
	

	this->bitmap = Gdiplus::Bitmap::FromStream(iStream , TRUE);
	if (!this->bitmap)
	{
		return xreturn::error(std::string() + "����������摜�̓ǂݍ��݂Ɏ��s���܂���");
	}
	return true;
}

//�摜�ǂݍ���
xreturn::r<bool> XLImage::Load(HBITMAP hbitmap)
{
	Clear();
	this->bitmap = Gdiplus::Bitmap::FromHBITMAP(hbitmap,NULL);
	if (!this->bitmap)
	{
		return xreturn::error(std::string() + "HBITMAP����̓ǂݍ��݂Ɏ��s���܂���");
	}
	return true;
}

//�摜�ǂݍ���
xreturn::r<bool> XLImage::Load(HICON hicon)
{
	Clear();
	this->bitmap = Gdiplus::Bitmap::FromHICON(hicon);
	if (!this->bitmap)
	{
		return xreturn::error(std::string() + "HBITMAP����̓ǂݍ��݂Ɏ��s���܂���");
	}
	return true;
}

//�摜�ۑ�
xreturn::r<bool> XLImage::Save(const std::string & fileName,int option) const
{
	_USE_WINDOWS_ENCODING;

	assert(IsEnable()) ;
	std::string ext = XLStringUtil::strtolower(XLStringUtil::baseext(fileName));

	CLSID clsid;
	auto r1 = findEncoder(ext,&clsid);
	if (!r1)
	{
		return xreturn::error(r1.getError());
	}


	Gdiplus::Status status;
	//���������ɕۑ�
	if (ext == ".jpeg" || ext == ".jpg")
	{
		//���k�����w�肷��p�����[�^�[�����
		Gdiplus::EncoderParameters params = {0};
		params.Count						= 1;
		params.Parameter[0].Guid			= Gdiplus::EncoderQuality;
		params.Parameter[0].Type			= Gdiplus::EncoderParameterValueTypeLong;
		params.Parameter[0].NumberOfValues	= 1;
		params.Parameter[0].Value			= &option;

		status = this->bitmap->Save(_A2W(fileName.c_str()) , &clsid ,&params);
	}
	else
	{
		status = this->bitmap->Save(_A2W(fileName.c_str()) , &clsid);
	}
	if (status != Gdiplus::Ok)
	{
		return xreturn::error("�ۑ��Ɏ��s���܂���");
	}
	return true;
}

//�摜�ۑ�
xreturn::r<bool> XLImage::Save(const std::string & ext,std::vector<char> * data,int option) const
{
	assert(IsEnable()) ;

	CLSID clsid;
	auto r1 = findEncoder(ext,&clsid);
	if (!r1)
	{
		return xreturn::error(r1.getError());
	}

//bitmap�ŕێ��������̍ő吔������U��Ƃ��߂��ۂ�
//	const int imagesize = bitmap->GetWidth() * bitmap->GetHeight() * 24;
	const int imagesize = 1;  //�Œ���̐����ɂ���B���蓖�Ă�ƁA��ŃT�C�Y��v���������ɁA���蓖�Ă��T�C�Y��Ԃ��݂����B
	//GlobalFree�� IStream ���s���B
	HGLOBAL memoryHandle = GlobalAlloc(GMEM_MOVEABLE, imagesize);

	//����������X�g���[���쐬
	CComPtr<IStream> iStream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(memoryHandle, TRUE, &iStream);
	if(FAILED(hr))	 return xreturn::windowsError(hr);

	Gdiplus::Status status;
	//���������ɕۑ�
	if (ext == ".jpeg" || ext == ".jpg")
	{
		//���k�����w�肷��p�����[�^�[�����
		Gdiplus::EncoderParameters params = {0};
		params.Count						= 1;
		params.Parameter[0].Guid			= Gdiplus::EncoderQuality;
		params.Parameter[0].Type			= Gdiplus::EncoderParameterValueTypeLong;
		params.Parameter[0].NumberOfValues	= 1;
		params.Parameter[0].Value			= &option;

		status = this->bitmap->Save(iStream , &clsid ,&params);
	}
	else
	{
		status = this->bitmap->Save(iStream , &clsid);
	}
	if (status != Gdiplus::Ok)
	{
		return xreturn::error("�ۑ��Ɏ��s���܂���");
	}
	//�T�C�Y���擾���܂��B
	STATSTG statstg;
	hr = iStream->Stat(&statstg ,0);
	if(FAILED(hr))	 return xreturn::windowsError(hr);
	//�o�b�t�@���m�ۂ��đ�����܂��B
	data->resize(  statstg.cbSize.LowPart);

	const void * memory = GlobalLock(memoryHandle);
	memcpy(&(data->operator[](0)) , memory , statstg.cbSize.LowPart );
	GlobalUnlock(memoryHandle);	

	return true;
}

//�`��
void XLImage::Draw(XLImage* image,int x,int y) const
{
	assert(IsEnable()) ;
	assert(image->IsEnable()) ;

	Gdiplus::Graphics graphics(this->bitmap);
	graphics.DrawImage(this->bitmap ,x ,y);
}

//�`��
void XLImage::Draw(XLImage* image,int x,int y,int width,int height) const
{
	assert(IsEnable()) ;
	assert(image->IsEnable()) ;

	Gdiplus::Graphics graphics(this->bitmap);
	graphics.DrawImage(this->bitmap ,x ,y,width,height);
}


//�`��
void XLImage::Draw(HDC dc,int x,int y) const
{
	assert(IsEnable()) ;

	Gdiplus::Graphics graphics(dc);
	graphics.DrawImage(this->bitmap ,x ,y);
}
//�`��
void XLImage::Draw(HDC dc,int x,int y,int width,int height) const
{
	Gdiplus::Graphics graphics(dc);
	graphics.DrawImage(this->bitmap ,x ,y,width,height);
}

xreturn::r<bool> XLImage::findEncoder(const std::string & ext,CLSID* clsid) const
{
	const wchar_t * format;
	if (ext == ".jpeg" || ext == ".jpg")
	{
		format = L"image/jpeg";
	}
	else if (ext == ".png")
	{
		format = L"image/png";
	}
	else if (ext == ".gif")
	{
		format = L"image/gif";
	}
	else if (ext == ".bmp")
	{
		format = L"image/bmp";
	}
	else
	{
		return xreturn::error(std::string() + "�w�肳�ꂽ�g���q" + ext +"�Ɋ֘A�t����ꂽ�G���R�[�_�[�͂���܂���");
	}
	
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return xreturn::error("Gdiplus::GetImageEncodersSize�Ɏ��s");
	}

	Gdiplus::ImageCodecInfo* pImageCodecInfo =
				(Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
	{
		return xreturn::error("Gdiplus::ImageCodecInfo �̃������m�ۂɎ��s");
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	UINT j = 0;
	for(; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*clsid = pImageCodecInfo[j].Clsid;
			return true;
		}    
	}

	free(pImageCodecInfo);
	return xreturn::error("�G���R�[�_��������܂���");
}


/*
//HBITMAP�� *.bmp�`���Ń������ɕۑ�
xreturn::r<bool> XLImage::ConvertHBITMAPtoBytes(HBITMAP hbitmap,std::vector<char> * image) const
{
	BITMAP bmp; 
	// Retrieve the bitmap color format, width, and height. 
	if (!GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bmp)) 
	{
		return xreturn::error("HBITMAP ���� BITMAP�ɕϊ��ł��܂���");
	}
 
	// Convert the color format to a count of bits. 
	WORD cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
	if (cClrBits == 1)			cClrBits = 1; 
	else if (cClrBits <= 4)		cClrBits = 4; 
	else if (cClrBits <= 8)		cClrBits = 8; 
	else if (cClrBits <= 16)	cClrBits = 16; 
	else if (cClrBits <= 24)	cClrBits = 24; 
	else						cClrBits = 32; 
 
	BITMAPINFOHEADER bitmapInfoHeader ={0};
	PBITMAPINFO pbmi = (PBITMAPINFO)&bitmapInfoHeader;

	// Initialize the fields in the BITMAPINFO structure. 
 	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
	if (cClrBits < 24) pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 
 
	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB; 
 
	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	// For Windows NT, the width must be DWORD aligned unless 
	// the bitmap is RLE compressed. This example shows this. 
	// For Windows 95/98/Me, the width must be WORD aligned unless the 
	// bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) / 8 * pbmi->bmiHeader.biHeight; 

	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0; 

	//�o�͐惁�����̊m��
	const int totalsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmi->bmiHeader.biSizeImage;
	image->resize(totalsize);
	char * writeMemory = &(image->operator[](0));

	PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER) pbmi; 


	BITMAPFILEHEADER* hdr = (BITMAPFILEHEADER*)writeMemory;       // bitmap file-header 
	hdr->bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
	// Compute the size of the entire file. 
	hdr->bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage); 
	hdr->bfReserved1 = 0; 
	hdr->bfReserved2 = 0; 
 
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	memcpy( writeMemory+sizeof(BITMAPFILEHEADER) , pbih , sizeof(BITMAPINFOHEADER) );

	// Copy the array of color indices into the .BMP file.
	hdr->bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 
 
	// Retrieve the color table (RGBQUAD array) and the bits 
	// (array of palette indices) from the DIB. 
	HDC dc = GetDC(NULL);
	LPBYTE lpBits = (LPBYTE) writeMemory + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (!GetDIBits(dc, hbitmap, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) 
	{
		auto error = ::GetLastError();
		ReleaseDC(NULL,dc);
		return xreturn::windowsError(error);
	}
	ReleaseDC(NULL,dc);

	return true;
}
*/
