#pragma once

#include <vector>

class XLHttpRequerst
{
public:
	
	xreturn::r<bool> Download(const std::string & url , const std::map<std::string,std::string> & header,const std::map<std::string,std::string> & option);
	xreturn::r<std::string> GetContents(const std::string & url , const std::map<std::string,std::string> & header,const std::map<std::string,std::string> & option)
	{
		auto r = this->Download(url ,  header, option);
		if (!r)
		{
			return xreturn::error(r.getError());
		}
		return this->getDataString();
	}

	///////////////////////////////////////////////////////////////////
	//���ʂ��ǂ����邩�̏���
	///////////////////////////////////////////////////////////////////

	const unsigned char* getData() const
	{
		return &this->OutData[0];
	}
	const unsigned int getSize() const
	{
		return this->OutData.size();
	}
	const std::string getDataString() const
	{
		return std::string((char*) (&this->OutData[0]));
	}
	xreturn::r<bool> XLHttpRequerst::saveFile(const std::string & filename) const;
	
private:
	//�_�E�����[�h�����f�[�^
	std::vector<unsigned char> OutData;
	//steam�񂩂Ďg���Â炢����匙�����A�΁[���B
	//std::basic_stringstream<unsigned char> OutData;
};