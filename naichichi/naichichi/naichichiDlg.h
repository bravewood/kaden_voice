
// naichichiDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "RSpeechRecognition.h"


// CnaichichiDlg �_�C�A���O
class CnaichichiDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CnaichichiDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_NAICHICHI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

	public:
	//���ɓ����Ă��Ȃ����`�F�b�N����.
	//(static ���\�b�h)
	static bool CnaichichiDlg::checkAlreadyRunning() ;

// ����
protected:
	HICON m_hIcon;
	//�ʒm
	NOTIFYICONDATA NotifyIcon ;
	//�����F��
	RSpeechRecognition SpeechRecognition;

	//�ʒm�̈�ɒǉ�
	bool CnaichichiDlg::AddNotifyIcon();
	//�^�X�N�g���C����A�C�R��������
	bool CnaichichiDlg::DelNotifyIcon();
	//�^�X�N�g���C���N���b�N���ꂽ�ꍇ
	afx_msg LRESULT CnaichichiDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam);
	//�����F�����ꂽ�Ƃ�
	afx_msg LRESULT CnaichichiDlg::OnSpeechRecognitioned(WPARAM wParam, LPARAM lParam);
	//�V�F�������񂾎��ɔ��ł��郁�b�Z�[�W
	afx_msg LRESULT CnaichichiDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
	//�I��
	void CnaichichiDlg::OnMenuCommandExit() ;
	BOOL CnaichichiDlg::Create();

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
