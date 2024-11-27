#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#include <pthread.h>
#include <unistd.h>
typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

using namespace std;

#define SIGNTYPE short
#define CHECK bool
/*
	Name: Samine's Defender
	Verson: 2.4.0
	Copyright: (c) Samine 2024-2024
	Author: Samine
	Date: 25-11-24 21:42
	Update1 : 26-11-24 19:30
	Update2 : 26-11-24 21:00 [Add UAC Judge]
	Update3 : 27-11-24 16:30 [Fixed System Crash Problem]
	Update4 : 27-11-24 20:20 [Avoiding being Killed by Taskmgr etc.]
	Update5 : 27-11-24 20:30 [Hiding Main Executable File]
	Description: A defender to protect your computer from CyberClass , LenovoEClass , JiyuClass etc.
*/
void SetColor(unsigned short ForeColor,unsigned short BackGroundColor) {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,(ForeColor%16)|(BackGroundColor%16*16));
}
// method 2: usde winapi, works for x86 and x64
#include <intrin.h>
#define CHAR char
#define INT int
INT cpuInfo[4] = { -1 };
CHAR cpu_manufacture[32] = { 0 };
CHAR cpu_type[32] = { 0 };
CHAR cpu_freq[32] = { 0 };
void getCpuInfo()
{

    __cpuid(cpuInfo, 0x80000002);
    memcpy(cpu_manufacture, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000003);
    memcpy(cpu_type, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000004);
    memcpy(cpu_freq, cpuInfo, sizeof(cpuInfo));

    cout<<"\nCPU型号:"<<cpu_manufacture << cpu_type << cpu_freq << endl;
}

void Gotoxy(int x, int y)
{
    COORD pos = { (short)x,(short)y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}
bool IfUAC() 
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
 
    BOOL  Op = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);
 
    if (Op)
    {
        CheckTokenMembership(NULL, AdministratorsGroup, &Op);
        FreeSid(AdministratorsGroup);
    }
 
    return  Op == TRUE ;
}
void HideCursor() { 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
 	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
}
inline void Init() {
	system("@taskkill /F /T /IM ebClnt.exe");
	system("@taskkill /F /T /IM ExamCore.exe");
	system("@taskkill /F /T /IM StudentMain.exe");
	cout.tie(0);
	system("@echo off");
	HideCursor();
	SetConsoleTitle("Samine Defender Main Executable");
	system("@mode con cols=100 lines=25");
	system("@shutdown -a");
	system("color f0");
	system("@cls");
}
inline CHECK KillProgram(LPCSTR   ExeName)  {  
 	LPCSTR   File;  
 	HANDLE   hProcessSnap;  
	PROCESSENTRY32   pe32;  
 	if(!ExeName||!ExeName[0])  
  		return false;  
 	File = strrchr(ExeName, '\\');  
 	if (File!=0)
  		ExeName = File+1;  
 	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
 	if(hProcessSnap==(HANDLE)-1) 
  		return false;
 	memset(&pe32,0,sizeof(pe32));
 	pe32.dwSize=sizeof(PROCESSENTRY32);
 	if(Process32First(hProcessSnap,&pe32)) {  
  	do {  
   		File = strrchr(pe32.szExeFile, '\\');  
   		File = File ? File+1 : pe32.szExeFile;  
	    if(strcmpi(File,ExeName)==0) {  
    		TerminateProcess(OpenProcess(
     		PROCESS_ALL_ACCESS,0,pe32  
     		.th32ProcessID), 0); 
     		return true;
   		}  
  	}  
  	while(Process32Next(hProcessSnap,&pe32));} 
	return false; 
	CloseHandle(hProcessSnap);
}
struct CYBER{
	CHECK CYBERFIRST=false;
	CHECK CYBERSECOND=false;
};
struct LENOVOCLASs{
	CHECK LENOVOCLASSFIRST=false;
	CHECK LENOVOCLASSSECOND=false;
	CHECK LENOVOCLASSTHIRD=false;
};
struct CTClass{
	CHECK CTFIRST=false;
	CHECK CTSECOND=false;
	CHECK CTTHIRD=false;
};
struct Class{
	CHECK ShutDownJiyuRun=false;
	CHECK ShutLenovoCloud=false;
	CYBER Cyber;
	LENOVOCLASs LENOVOCLASS;
	CTClass CTCLASS;
};
struct STATUS{
	CHECK ShutDownRun=false;
	Class CLASS;
};
UINT DefendShutdown(){
	SetColor(15,15);
	int BackValue = system("shutdown -a");
	if(BackValue == 128) return 0;
	if(BackValue == 1) return -1;//错误：拒绝访问 
	if(BackValue == 0) return 1;
	SetColor(0,15);
}
STATUS CheckShutClass;
void NormalKillProcess() {
	KillProgram("LenovoLockScreen.exe");
}
void ShutClass() {
	if(KillProgram("ebClnt.exe")) CheckShutClass.CLASS.ShutLenovoCloud = true;
	if(KillProgram("tvnserver32.exe")) CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSFIRST = true;
	if(KillProgram("DesktopCheck.exe")) CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSSECOND = true;
	if(KillProgram("DeploymentAgent.exe")) CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSTHIRD = true;
	if(KillProgram("CT.exe")) CheckShutClass.CLASS.CTCLASS.CTFIRST = true;
	if(KillProgram("ctfmon.exe")) CheckShutClass.CLASS.CTCLASS.CTSECOND = true;
	if(KillProgram("CTDaemon.exe")) CheckShutClass.CLASS.CTCLASS.CTTHIRD = true;
	if(KillProgram("secprocess123.exe")) CheckShutClass.CLASS.Cyber.CYBERFIRST = true;
	if(KillProgram("Student.exe")) CheckShutClass.CLASS.Cyber.CYBERSECOND = true;
}
#define OK 1
#define NAOK 0
#define UNOK -1
void OS(CHECK check) {
	if(check) {
		SetColor(10,15);
		cout<<"TRUE";
		SetColor(0,15);
	}
	else {
		SetColor(4,15);
		cout<<"FALSE";
		SetColor(0,15);		
	}
}
UINT DetailedInformation(bool OutputInformation) {
	if(!OutputInformation) {
		if(CheckShutClass.CLASS.ShutDownJiyuRun &&
		   CheckShutClass.CLASS.ShutLenovoCloud &&
		   CheckShutClass.CLASS.CTCLASS.CTFIRST && CheckShutClass.CLASS.CTCLASS.CTSECOND && CheckShutClass.CLASS.CTCLASS.CTTHIRD &&
		   CheckShutClass.CLASS.Cyber.CYBERFIRST && CheckShutClass.CLASS.Cyber.CYBERSECOND &&
		   CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSFIRST && CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSSECOND && CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSTHIRD) {
				return OK;
		   }
		else if(CheckShutClass.CLASS.ShutDownJiyuRun ||
		   CheckShutClass.CLASS.ShutLenovoCloud ||
		   CheckShutClass.CLASS.CTCLASS.CTFIRST || CheckShutClass.CLASS.CTCLASS.CTSECOND || CheckShutClass.CLASS.CTCLASS.CTTHIRD ||
		   CheckShutClass.CLASS.Cyber.CYBERFIRST || CheckShutClass.CLASS.Cyber.CYBERSECOND ||
		   CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSFIRST || CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSSECOND || CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSTHIRD) {
				return NAOK;
		   }
		else {
			return UNOK;
		}
	}
	cout<<"CheckShutClass.CLASS.ShutDownJiyuRun = ";OS(CheckShutClass.CLASS.ShutDownJiyuRun);cout<<endl;
	cout<<"CheckShutClass.CLASS.ShutLenovoCloud = ";OS(CheckShutClass.CLASS.ShutLenovoCloud);cout<<endl;
	cout<<"CheckShutClass.CLASS.CTCLASS.CTFIRST = ";OS(CheckShutClass.CLASS.CTCLASS.CTFIRST);cout<<endl;
	cout<<"CheckShutClass.CLASS.CTCLASS.CTSECOND = ";OS(CheckShutClass.CLASS.CTCLASS.CTSECOND);cout<<endl;
	cout<<"CheckShutClass.CLASS.CTCLASS.CTTHIRD = ";OS(CheckShutClass.CLASS.CTCLASS.CTTHIRD);cout<<endl;
	cout<<"CheckShutClass.CLASS.Cyber.CYBERFIRST = ";OS(CheckShutClass.CLASS.Cyber.CYBERFIRST);cout<<endl;
	cout<<"CheckShutClass.CLASS.Cyber.CYBERSECOND = ";OS(CheckShutClass.CLASS.Cyber.CYBERSECOND);cout<<endl;
	cout<<"CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSFIRST = ";OS(CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSFIRST);cout<<endl;
	cout<<"CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSSECOND = ";OS(CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSSECOND);cout<<endl;
	cout<<"CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSTHIRD = ";OS(CheckShutClass.CLASS.LENOVOCLASS.LENOVOCLASSTHIRD);cout<<endl;
}

DWORD start = GetTickCount();
BOOL FindProcessHandleAndKill(HWND hWnd)
{
	DWORD   ProcessID;
	HANDLE   hProcess;
	if (hWnd == NULL)
	{
		return   FALSE;
	}
	else
	{
		if (GetWindowThreadProcessId(hWnd, &ProcessID) == 0)//&#65418;&#65383;&#65392;&#65436;  
		{
			return FALSE;
		}
		else
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
			if (hProcess == NULL)
			{
				return FALSE;
			}
			else
			{
				if (TerminateProcess(hProcess, 0))
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
}
void LockKiller() {
	HWND tker;
//	tker = FindWindow(NULL, "任务管理器");
//	FindProcessHandleAndKill(tker);
//	tker = FindWindow(NULL, "运行");
//	FindProcessHandleAndKill(tker);
//	tker = FindWindow(NULL, "C:\\Windows\\system32\\cmd.exe");
//	FindProcessHandleAndKill(tker);
//	tker = FindWindow(NULL, "Windows Powershell");
//	FindProcessHandleAndKill(tker);
//	tker = FindWindow(NULL, "管理员: Windows Powershell");
//	FindProcessHandleAndKill(tker);
	tker = FindWindow(NULL, "Samine Defender Main Executable");
	ShowWindow(tker, SW_HIDE);
}
void Defend() {
	UINT ShutdownRun = DefendShutdown();
	//=====MODULE:DEFENDSHUTDOWN=====
	cout<<"防瞬关：";
	if(ShutdownRun==1) {
		SetColor(10,15);
		cout<<"正常";
		SetColor(0,15);
	}
	else if(ShutdownRun == 0) {
		MessageBox(NULL,"错误：请使用管理员权限访问本软件。","Samine's Defender 2.4.0",MB_OK|MB_ICONWARNING);
		SetColor(4,15);
		cout<<"异常";
		SetColor(0,15);
	}
	else {
		SetColor(4,15);
		cout<<"异常";
		SetColor(0,15);
	}
	///======MODULE:KILLPROGRAMS
	ShutClass();
	UINT ShutClassRun = DetailedInformation(false);
	cout<<"\n脱控模块：";
	if(ShutdownRun == 1) {
		SetColor(10,15);
		cout<<"正常";
		SetColor(0,15);
	}
	else if(ShutdownRun == 0) {
		SetColor(14,15);
		cout<<"部分正常";
		SetColor(0,15);
	}
	else {
		SetColor(4,15);
		cout<<"异常";
		SetColor(0,15);
	}
	cout<<"\n启动预防：";
	LockKiller();
	SetColor(10,15);
	cout<<"正常";
	SetColor(0,15);
}
unsigned long long ticks = 200000;

bool LookAtInformation(){
	system("cls");
	Init();
	while(1){
		Gotoxy(0,0);
		HWND jiyu = FindWindow(NULL, "屏幕广播");
		ShowWindow(jiyu, SW_HIDE);
		time_t now = time(0);
		char *dt = ctime(&now);
		NormalKillProcess();
		DWORD nowtime = GetTickCount();
		cout<<"程序运行正常！ "<<dt<<endl;
		cout<<fixed<<setprecision(3)<<"程序已运行时间:"<<(double)(nowtime-start)/1000*1.0<<"sec.   设定的转换时间:"<<ticks/1000<<"秒"<<endl;
		if(nowtime - start >= ticks) {
			system("start Defender.exe");
			return 0;
		}
		Defend();
		getCpuInfo();
		cout<<endl<<endl<<"具体信息：\n";
		DetailedInformation(true);
		cout<<"\n返回主界面请点[B]";
		if(kbhit()){
			char ch = getch();
			if(ch == 'B' || ch == 'b') return true;
		}
		Gotoxy(0,0);
	}
	return true;
} 
int mian() {
	//!!!!!!!!更改版本号后一定要更改Killer里的程序名称，不然无法成功隐蔽 
	if(!IfUAC()) {
		SetConsoleTitle("Samine Defender 2.4.0 | 未使用管理员权限运行");
		system("@mode con cols=100 lines=25");
		system("color f0");
		MessageBox(NULL,"请右键程序，点击“以管理员身份运行”。","Samine's Defender 2.4.0",MB_OK|MB_ICONWARNING);
		return -1;
	}
	Init();
	while(1) {
		Gotoxy(0,0);
		HWND jiyu = FindWindow(NULL, "屏幕广播");
		ShowWindow(jiyu, SW_HIDE);
		time_t now = time(0);
		char *dt = ctime(&now);
		NormalKillProcess();
		DWORD nowtime = GetTickCount();
		cout<<"程序运行正常！ "<<dt<<endl;
		cout<<fixed<<setprecision(3)<<"程序已运行时间:"<<(double)(nowtime-start)/1000*1.0<<"sec.   设定的转换时间:"<<ticks/1000<<"秒"<<endl;
		if(nowtime - start >= ticks) {
			system("start Defender.exe");
			return 0;
		}
		Defend();
		getCpuInfo();
		cout<<"\n查看具体信息请点[I]";
		if(kbhit()){
			char ch = getch();
			if(ch == 'I' || ch == 'i') if(LookAtInformation() == false) return 0;
			Init();
			system("cls");
		}
		Gotoxy(0,0);
	}
	return 0;
}
int main() {
	//究极简化版！！！
	if(!IfUAC()) {
		SetConsoleTitle("Samine Defender 2.4.0 | 未使用管理员权限运行");
		system("@mode con cols=100 lines=25");
		system("color f0");
		MessageBox(NULL,"请右键程序，点击“以管理员身份运行”。","Samine's Defender 2.4.0",MB_OK|MB_ICONWARNING);
		return -1;
	}
	const TCHAR szOperation[] = ("open");
	const TCHAR szAddress[] = ("https://github.com/Samine233/Samine-Defender/tree/main#readme");
		HINSTANCE hRslt = ShellExecute(NULL, szOperation,
			szAddress, NULL, NULL, SW_SHOWNORMAL);
		assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
	Init();
	if(1 < 0) mian(); 
	while(1) {
		HWND jiyu = FindWindow(NULL, "屏幕广播");
		ShowWindow(jiyu, SW_HIDE);
		NormalKillProcess();
		time_t now = time(0);
		DWORD nowtime = GetTickCount();
		if(nowtime - start >= ticks) {
			system("start Defender.exe");
			return 0;
		}
		Defend();
	}
}
