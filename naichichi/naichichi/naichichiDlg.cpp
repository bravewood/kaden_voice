
// naichichiDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "naichichi.h"
#include "naichichiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�^�X�N�g���C���N���b�N���ꂽ�Ƃ�
const int WM_USER_TASKTRAY_CLICK = WM_USER+100;
const int WM_USER_SPEECH_RECOGNITIONED = WM_USER+101;
const int WM_TASKBAR_CREATED = RegisterWindowMessage(_T("TaskbarCreated"));

// CnaichichiDlg �_�C�A���O
CnaichichiDlg::CnaichichiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CnaichichiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnaichichiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CnaichichiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_TASKTRAY_CLICK, &CnaichichiDlg::OnTaskTrayClick)
	ON_MESSAGE(WM_USER_SPEECH_RECOGNITIONED, &CnaichichiDlg::OnSpeechRecognitioned)
	ON_REGISTERED_MESSAGE(WM_TASKBAR_CREATED, OnTaskbarCreated)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_COMMAND(IDM_QUIT, OnMenuCommandExit)
END_MESSAGE_MAP()


//���ɓ����Ă��Ȃ����`�F�b�N����.
//(static ���\�b�h)
bool CnaichichiDlg::checkAlreadyRunning() 
{
	//mutex�ł��������ǁA�����ł�message�𑗐M�������̂�
	//main window�����邩�ǂ�������������.
	HWND hwnd = ::GetTopWindow(::GetDesktopWindow());
	do
	{
		TCHAR title[ MAX_PATH ]= {0};
		::GetWindowText( hwnd, title, sizeof(title) );

		if ( lstrcmp(title , "Niaichichi_Hidden_window" ) != 0 )
		{
			continue;
		}
		//������!
		::PostMessage(hwnd , WM_COMMAND, (WPARAM)IDM_FRONT, 0); 
		//���ɓ����Ă܂�!
		return true;
	}
	while( (hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT)) !=NULL );

	//�܂������Ă��܂���!
	return false;
}

// CnaichichiDlg ���b�Z�[�W �n���h���[

BOOL CnaichichiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	//�ǂ����Ă��_�C�A���O�������Ȃ������̂ŁA�A����œ����Ă݂�B
	MoveWindow(1,1,1,1,TRUE);

	//�ʒm�̈�ɃA�C�R���Ȃǂ�ǉ�.
	bool ret = this->AddNotifyIcon();
	if (!ret)
	{
		return FALSE;
	}

	//�����F���G���W���X�^�[�g!
	this->SpeechRecognition.Create("","_vicecommand.xml",this->m_hWnd ,WM_USER_SPEECH_RECOGNITIONED );

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}


// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CnaichichiDlg::OnPaint()
{
	//��\��
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CnaichichiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CnaichichiDlg::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//�A�C�R��������.
	this->DelNotifyIcon();

	CDialogEx::PostNcDestroy();
}




//�ʒm�̈�ɒǉ�
bool CnaichichiDlg::AddNotifyIcon()
{
	//�^�X�N�g���C�ɃA�C�R�����o��
	this->NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	this->NotifyIcon.uID = 0;
	this->NotifyIcon.hWnd = m_hWnd;
	this->NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	this->NotifyIcon.hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	this->NotifyIcon.uCallbackMessage = WM_USER_TASKTRAY_CLICK;
	lstrcpy( this->NotifyIcon.szTip, "�Ȃ������{�C�X�R�}���_�[ by rti\r\n�R�}���h�[�B�J��Ԃ��܂��B�R�}���h�[�B" );

	// �A�C�R����o�^�ł��邩�G���[�ɂȂ�܂ŌJ��Ԃ�
	while (true)
	{
		if (Shell_NotifyIcon(NIM_ADD, &this->NotifyIcon))
		{
			// �o�^�ł�����I���
			break;
		}
		else
		{
			// �^�C���A�E�g���ǂ������ׂ�
			if (::GetLastError() != ERROR_TIMEOUT)
			{
				// �A�C�R���o�^�G���[
				return false;
			}

			// �o�^�ł��Ă��Ȃ����Ƃ��m�F����
			if (Shell_NotifyIcon(NIM_MODIFY, &this->NotifyIcon))
			{
				// �o�^�ł��Ă���
				break;
			}
			else
			{
				// �o�^�ł��Ă��Ȃ�����
				::Sleep(1000);
			}
		}
	}
	return true;
}

//�^�X�N�g���C����A�C�R��������
bool CnaichichiDlg::DelNotifyIcon()
{
	::Shell_NotifyIcon( NIM_DELETE, &this->NotifyIcon );
	return true;
}

//�^�X�N�g���C�̃A�C�R�����N���b�N���ꂽ�Ƃ�
afx_msg LRESULT CnaichichiDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam)
{
	// �J�[�\���̌��݈ʒu���擾
	POINT point;
	::GetCursorPos(&point);

	switch(lParam) 
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetForegroundWindow();	
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		PostMessage(WM_NULL);
		break;
	}
	return 0;
}

//�V�F�������񂾎��ɔ��ł��郁�b�Z�[�W
afx_msg LRESULT CnaichichiDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	this->AddNotifyIcon();
	return 0;
}



//�I��
void CnaichichiDlg::OnMenuCommandExit() 
{
	// �I�����I�΂ꂽ�烁�C���E�B���h�E����ăv���O�������I������B
	PostMessage(WM_CLOSE);
}


void CnaichichiDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}

//�����F�����ꂽ�Ƃ�
afx_msg LRESULT CnaichichiDlg::OnSpeechRecognitioned(WPARAM wParam, LPARAM lParam)
{
	//���ʂ̎擾
	if ( ! this->SpeechRecognition.Listen() )
	{
		return 0;
	}

	//���ʂ����Ă���.
	std::string text = this->SpeechRecognition.getResultString();
	std::string mtext = this->SpeechRecognition.getResultMap("NAME");

	//�R�}���h��F���ł����炵��.
	if (mtext != "")
	{
		std::string title = "�����F���u" + text + "�v ���܂��B";
		std::string message = "�u" + mtext + "�v�����s���܂��B";

		//�o���[�����o��.
		NOTIFYICONDATA	sData;
		ZeroMemory(&sData, sizeof(NOTIFYICONDATA));
		sData.cbSize		= sizeof(NOTIFYICONDATA);
		sData.uFlags		= NIF_INFO;
		sData.hWnd			= this->m_hWnd;
		sData.dwInfoFlags	= NIIF_INFO;
		sData.uTimeout		= 3000;

		lstrcpy(sData.szInfoTitle,title.c_str() );
		lstrcpy(sData.szInfo,  message.c_str() );

		Shell_NotifyIcon(NIM_MODIFY,&sData);


		//�R�}���h�̎��s
		::ShellExecute(NULL,NULL,mtext.c_str(),NULL,NULL,0);

	}

	return 0;
}

