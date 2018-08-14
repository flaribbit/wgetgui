#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "utils.h"
#include "resource.h"

HINSTANCE hInst;
char edit_exe[MAXLEN];
char edit_txt[MAXLEN];
char str_folder[MAXLEN];

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		case WM_INITDIALOG:
		{
			FILE *fp;
			char buf[MAXLEN];
			fp=fopen("conf.dat","r+");
			if(fp){//如果数据存在
				fgets(buf,MAXLEN,fp);//开始读取
				if(*buf!=';'){
					MessageBox(hwndDlg,"数据文件包含错误！\n请删掉conf.dat文件！","错误",MB_OK|MB_ICONSTOP);
					EndDialog(hwndDlg, 0);
				}
				fgets(buf,MAXLEN,fp);//[FolderList]
				if(buf[4]!='d'){
					MessageBox(hwndDlg,"数据文件包含错误！\n请删掉conf.dat文件！","错误",MB_OK|MB_ICONSTOP);
					EndDialog(hwndDlg, 0);
				}
				while(fgets(buf,MAXLEN,fp)){
					if(*buf=='[')break;//[EXE]
					buf[strlen(buf)-1]=0;
					SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_ADDSTRING,0,(LPARAM)buf);
				}
				fgets(edit_exe,MAXLEN,fp);edit_exe[strlen(edit_exe)-1]=0;
				fgets(buf,MAXLEN,fp);//[Cmds]
				fgets(buf,MAXLEN,fp);buf[strlen(buf)-1]=0;
				fclose(fp);
				SetDlgItemText(hwndDlg,IDC_EDIT_CMDS,buf);
			}else{
				strcpy(edit_exe,"wget");
				*str_folder=0;
			}
			SetDlgItemText(hwndDlg,IDC_EDIT_EXE,edit_exe);
		}
		return TRUE;

		case WM_CLOSE:
		{
			FILE *fp;
			int i,n;
			char buf[MAXLEN];
			fp=fopen("conf.dat","w");
			fprintf(fp,";wgetgui config\n");
			fprintf(fp,"[FolderList]\n");
			n=SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETCOUNT,0,0);
			for(i=0;i<n;i++){
				SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETTEXT,i,(LPARAM)buf);
				fprintf(fp,"%s\n",buf);
			}
			fprintf(fp,"[EXE]\n");
			GetDlgItemText(hwndDlg,IDC_EDIT_EXE,buf,MAXLEN);
			fprintf(fp,"%s\n",buf);
			fprintf(fp,"[Cmds]\n");
			GetDlgItemText(hwndDlg,IDC_EDIT_CMDS,buf,MAXLEN);
			fprintf(fp,"%s\n",buf);
			fclose(fp);
			
			EndDialog(hwndDlg, 0);
		}
		return TRUE;
	
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_FOLDERLIST:
					switch(HIWORD(wParam)){
						case LBN_DBLCLK:{//修改
							int sel=SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETCURSEL,0,0);
							SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETTEXT,sel,(LPARAM)str_folder);
							if(SelectFolder(hwndDlg,str_folder)){//如果选择了文件夹
								SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_DELETESTRING,sel,0);
								SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_ADDSTRING,0,(LPARAM)str_folder);
							}
							break;
						}
					}
					break;
				case IDC_ADD://添加
					*str_folder=0;
					if(SelectFolder(hwndDlg,str_folder))
						SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_ADDSTRING,0,(LPARAM)str_folder);
					break;
				case IDC_DEL:{//删除
					int sel=SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETCURSEL,0,0);
					SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_DELETESTRING,sel,0);
					break;
				}
				case IDC_CLEAR:{
					int i,n=SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_GETCOUNT,0,0);
					for(i=0;i<n;i++)
						SendDlgItemMessage(hwndDlg,IDC_FOLDERLIST,LB_DELETESTRING,0,0);
					break;
				}
				case IDC_SELECTEXE:
					if(SelectEXE(hwndDlg,edit_exe))
						SetDlgItemText(hwndDlg,IDC_EDIT_EXE,edit_exe);
					break;
				case IDC_BUTTON_SEL_INPUT:
					if(SelectTXT(hwndDlg,edit_txt))
						SetDlgItemText(hwndDlg,IDC_EDIT_INPUT,edit_txt);
					break;
				case IDC_CHECKBOX1:{
					int sta=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX1);
					EnableWindow(GetDlgItem(hwndDlg,IDC_EDIT_INPUT),sta);
					EnableWindow(GetDlgItem(hwndDlg,IDC_BUTTON_SEL_INPUT),sta);
					break;
				}
				case IDM_EXIT:
					SendMessage(hwndDlg,WM_CLOSE,0,0);
					break;
				case IDC_RUN:
					MessageBox(hwndDlg,"还没写呢 你来打我呀 _(:з」∠)_","提示",MB_OK|MB_ICONINFORMATION);
			}
		}
		return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
