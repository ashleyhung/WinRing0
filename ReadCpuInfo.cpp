#include "stdafx.h"
#include "windows.h"
#include <tchar.h> 
#include "OlsApiInit.h"
#include "ReadCpuInfo.h"
//#include "OlsDef.h"
//#include "OlsApiInit.h"
#include <string.h>

using namespace std;

ReadCpuInfo::ReadCpuInfo()
{
    ReadCpuInfoInit();      //Initial dll
    strcpy_s(fileName, "CpuInfoRecord.txt");
}

ReadCpuInfo::~ReadCpuInfo()
{
    ReadCpuInfoExit();
}

void ReadCpuInfo::ReadCpuTemp()      
{
    for (size_t i = 0; i < coreNumber; i++) 
    {
        int mask =  0x01 << i;         
        SetProcessAffinityMask(GetCurrentProcess(), mask);
        DWORD eax, ebx, ecx, edx;
        Rdmsr(0x19c, &eax, &edx);    //Read temp（eax&0x7f0000）
        cpuTemp[i] = Tjmax - ((eax & 0x7f0000) >> 16);//Real temp= Tjmax - value
    }

}

void ReadCpuInfo::ReadCpuInfoInit()      
{
//    if (true == CheckAndPrint("Init DLL function", InitializeOls()))                   
//    {
//       if (true == CheckAndPrint("IsCpuid function", IsCpuid()))                     
//        {
//          if (true == CheckAndPrint("Ismsr function", IsMsr()))                       
//          {
                HMODULE m_hOpenLibSys;
                DWORD TjMax;
                DWORD IAcore;
                DWORD PKGsts;
                int Cputemp;
 
                m_hOpenLibSys = NULL;
                if (InitOpenLibSys(&m_hOpenLibSys) != TRUE)
                {
                    printf("DLL Load Error!");
                    return ;
                }
                DWORD eax, ebx, ecx, edx;
                Rdmsr(0x1A2, &eax, &edx);  
                Tjmax = (eax & 0xff0000) >> 16; //Get Tjmax
//                printf("%d\n", Tjmax);                          //show max temperature
                SYSTEM_INFO sysInfo;
                GetSystemInfo(&sysInfo);
                coreNumber = sysInfo.dwNumberOfProcessors;  
                SetProcessAffinityMask(GetCurrentProcess(), 1);  //Get first cpu
    //      }
      // }
//    }
}

void ReadCpuInfo::ReadCpuInfoExit()
{
    DeinitializeOls();
}
int ReadCpuInfo::CheckAndPrint(char message[], BOOL flag)
{
    if (flag)
    {
        printf("%s is checked Enable!\n", message);
        return 1;
    }
    else
    {
        printf("%s is checked unEnabled!\n", message);
        return 0;
    }
}

void ReadCpuInfo::CpuTempDisplay()
{
    for (size_t i = 0; i < coreNumber; i++)
    {
        char buf[1024] = "";
#if (CONSOLE_PRINT)
        printf("Core #%d: %dC\n", i, cpuTemp[i]);
#else
#endif
        sprintf_s(buf, "Core #%d: %dC\n", i, cpuTemp[i]);
        RecordCpuInfo(buf);
    }
}
float ReadCpuInfo::GetCoreTemp(int core_index)
{
    for (size_t i = 0; i < coreNumber; i++)
    {
#if (CONSOLE_PRINT)
        printf("Core #%d: %dC\n", i, cpuTemp[i]);
#else
#endif
        if(core_index==i)
        {
            return cpuTemp[i];
        }
    }
    return 0;
}
void ReadCpuInfo::SysTimeDisplay()
{
    clearRecordFile();
    SYSTEMTIME start; //windows.h中   
    GetLocalTime(&start);
    //printf("%d\n", start.wHour);
#if (CONSOLE_PRINT)
    printf("date: %02d/%02d/%02d\n",
        start.wYear, start.wMonth, start.wDay);
    printf("time: %02d:%02d:%02d\n",
        start.wHour, start.wMinute, start.wSecond);
#else
#endif
    char buf[1024] = "";
    sprintf_s(buf, "date: %02d/%02d/%02d\n", start.wYear, start.wMonth, start.wDay);
    RecordCpuInfo(buf);
    memset(buf, 0, sizeof(buf));

    sprintf_s(buf, "time: %02d:%02d:%02d\n",start.wHour, start.wMinute, start.wSecond);
    RecordCpuInfo(buf);

}

void ReadCpuInfo::RecordCpuInfo(char *buf)
{
    FILE *fp;
    fopen_s(&fp,fileName, "a+");
    if (fp == NULL)
    {
        printf("file open error!\n");
        return;
    }
    else
    {
        fwrite(buf, strlen(buf), 1, fp);
    }
    fclose(fp);

}

void ReadCpuInfo:: clearRecordFile()
{
    FILE *fp;
    fopen_s(&fp,fileName, "w+");
    if (fp == NULL)
    {
        printf("file open error!\n");
        return ;
    }
    fclose(fp);
}
