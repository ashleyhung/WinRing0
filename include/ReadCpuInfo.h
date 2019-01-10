#pragma once
#include<windows.h>
//#include "OlsApi.h"       //头文件
#pragma comment (lib,"WinRing0.lib")   //加载lib
#define CONSOLE_PRINT 1
class ReadCpuInfo
{
private:
    char fileName[30];          
public:
    int cpuTemp[8];
    int coreNumber;              //获得的线程数
    int Tjmax;                  
public:
    ReadCpuInfo();            
    virtual ~ReadCpuInfo();
public:
    //void ReadCpuTempInit();
    void ReadCpuTemp();        //函数2
    void CpuTempDisplay();
    void SysTimeDisplay();
    void clearRecordFile();
    float GetCoreTemp(int core_index);
private:
    void ReadCpuInfoInit();   //函数1
    void ReadCpuInfoExit();
    void RecordCpuInfo(char *buf);
    int CheckAndPrint(char message[], BOOL flag);
};