#pragma once

//atl�g���Ȃ��l�̂��߂́A���P�@�\�� CComPtr
template<typename _INTERFACE>class RSimpleComPtr
{
public:
	RSimpleComPtr()
	{
		this->Object = NULL;
	}
	virtual ~RSimpleComPtr()
	{
		if (this->Object)
		{
			Object->Release();
			this->Object = NULL;
		}
	}

	//�\�z
	HRESULT CoCreateInstance(REFCLSID clsid,unsigned long type = CLSCTX_ALL)
	{
		assert(this->Object == NULL);
		return ::CoCreateInstance(clsid, NULL, type, __uuidof(_INTERFACE) , (void**)&this->Object);
	}
	//�\�z by ������
	HRESULT CoCreateInstance(wchar_t* progid,unsigned long type = CLSCTX_ALL)
	{
		assert(this->Object == NULL);

		CLSID clsid;
		HRESULT hr;
		hr = CLSIDFromProgID(progid, &clsid);
		if (hr != S_OK) return hr;

		return ::CoCreateInstance(clsid, NULL, type, __uuidof(clsid) , (void**)&this->Object);
	}

	//���b�v���Ă���I�u�W�F�N�g���Q��
	_INTERFACE* operator->() const
	{
		assert(this->Object != NULL);
		return this->Object;
	}
	//�I�u�W�F�N�g�̏�������
	_INTERFACE** operator&() 
	{
		assert(this->Object == NULL);	//����ƃ_�����Ă��ƂŁB
		return &this->Object;
	}
	//�C���^�[�t�F�[�X�����o���ꍇ
	operator _INTERFACE*() const
	{
		return this->Object;
	}

	//QueryInterface�Ƃ������̃L���X�g.
	template<typename _CASTINTERFACE> HRESULT QueryInterface(_CASTINTERFACE** target)
	{
		assert(this->Object != NULL);
		return this->Object->QueryInterface(__uuidof(_CASTINTERFACE),(void**)target );
	}
	

private:
	_INTERFACE* Object;
};
