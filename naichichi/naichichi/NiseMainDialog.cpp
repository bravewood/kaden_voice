// NiseMainDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "naichichi.h"
#include "NiseMainDialog.h"
#include "afxdialogex.h"

// CNiseMainDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CNiseMainDialog, CDialogEx)

CNiseMainDialog::CNiseMainDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNiseMainDialog::IDD, pParent)
{

}

CNiseMainDialog::~CNiseMainDialog()
{
}

void CNiseMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNiseMainDialog, CDialogEx)
END_MESSAGE_MAP()


// CNiseMainDialog ���b�Z�[�W �n���h���[


BOOL CNiseMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	//�_�C�A���O���őO�ʂ�
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


BOOL CNiseMainDialog::DestroyWindow()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	return CDialogEx::DestroyWindow();
}

