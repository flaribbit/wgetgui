#define MAXLEN 1024

int SelectEXE(HWND hwndDlg,char *filename);
int SelectFolder(HWND hwndDlg,char *filename);
int A2U(char *a,WCHAR *u);
static int CALLBACK _SelectFolderCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
