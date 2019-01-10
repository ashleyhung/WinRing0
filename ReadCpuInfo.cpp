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
    ReadCpuInfoInit();      //构造函数初始化dll
    strcpy_s(fileName, "CpuInfoRecord.txt");
}

ReadCpuInfo::~ReadCpuInfo()
{
    ReadCpuInfoExit();
}

void ReadCpuInfo::ReadCpuTemp()      //主要函数
{
    for (size_t i = 0; i < coreNumber; i++) //遍历CPU
    {
        int mask =  0x01 << i;         
        SetProcessAffinityMask(GetCurrentProcess(), mask);//设置当前使用线程(CPU)
        DWORD eax, ebx, ecx, edx;
        Rdmsr(0x19c, &eax, &edx);    //读取温度寄存器（eax&0x7f0000可以获得温度数据）
        cpuTemp[i] = Tjmax - ((eax & 0x7f0000) >> 16);//实际温度= Tjmax - 温度数据
    }

}

void ReadCpuInfo::ReadCpuInfoInit()      //初始化
{
//    if (true == CheckAndPrint("Init DLL function", InitializeOls()))                   //dll的初始化
//    {
//       if (true == CheckAndPrint("IsCpuid function", IsCpuid()))                      //判断是否支持Cpuid
//        {
//          if (true == CheckAndPrint("Ismsr function", IsMsr()))                       //是否支持Rdmsr
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
                Rdmsr(0x1A2, &eax, &edx);   //读取寄存器
                Tjmax = (eax & 0xff0000) >> 16; //获得Tjmax
//                printf("%d\n", Tjmax);                          //show max temperature
                SYSTEM_INFO sysInfo;
                GetSystemInfo(&sysInfo);
                coreNumber = sysInfo.dwNumberOfProcessors;  //获得线程数
                SetProcessAffinityMask(GetCurrentProcess(), 1);    //切换到第一个cpu;
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
    GetLocalTime(&start);//time.h的tm结构体一样的效果   
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