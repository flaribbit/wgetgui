#define WINVER 0x0600

#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

#include "utils.h"
#include "resource.h"

int SelectEXE(HWND hwndDlg,char *filename){
	OPENFILENAME ofn;
	int result;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	ofn.lpstrFilter = "应用程序(*.exe)\0*.exe\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAXLEN;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	return GetOpenFileName(&ofn);
}

int SelectTXT(HWND hwndDlg,char *filename){
	OPENFILENAME ofn;
	int result;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0所有文件\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAXLEN;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	return GetOpenFileName(&ofn);
}

int SelectFolder(HWND hwndDlg,char *filename){
	BROWSEINFO bi;
	LPITEMIDLIST lpItemList;
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner=hwndDlg;
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_NEWDIALOGSTYLE;
	bi.lpszTitle="选择目标文件夹";
	bi.lpfn=_SelectFolderCallbackProc;
	bi.lParam=(LPARAM)filename;
	lpItemList=SHBrowseForFolder(&bi);
	if(lpItemList){
		SHGetPathFromIDList(lpItemList,filename);
		return 1;
	}
	return 0;
}

int A2U(char *a,WCHAR *u){
	int length = MultiByteToWideChar(CP_SUPPORTED,0,a,-1,NULL,0);
	MultiByteToWideChar(CP_SUPPORTED,0,a,-1,u,length);
	return length;
}

static int CALLBACK _SelectFolderCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hwnd,BFFM_SETSELECTION,MAXLEN,lpData);
			break;
		default:break;
	}
	return 0;
}
