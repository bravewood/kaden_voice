#include "comm.h"
#include "RSpeechRecognition.h"

void main()
{
	//COM�̏�����.
	::CoInitialize( NULL );

	try
	{
		RSpeechRecognition sr;
		sr.Create("","_vicecommand.xml");
		while(1)
		{
			puts("go!");

			sr.Listen();
//			sr.Listen();

			string text = sr.getResultString();
			string mtext = sr.getResultMap("NAME");
			puts(text.c_str() );
			puts(mtext.c_str() );

			if (mtext == "QUIT")
			{
				puts("���ځ[��");
				break;
			}
			if (mtext != "")
			{
				::ShellExecute(NULL,NULL,mtext.c_str(),NULL,NULL,0);
			}
		}
	}
	catch(RException e)
	{
		puts( e.getMessage() );
		ASSERT(0);
		return ;
	}

	//COM�̊J��
	::CoUninitialize();

}