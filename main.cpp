#include "stdafx.h"
#include "ReadCpuInfo.h"
//#include "OlsApi.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
    ReadCpuInfo myCpu;      //生成对象
    //cout << InitializeOls() << endl;;
    //cout << sysInfo.dwNumberOfProcessors << endl;
    //cout << sysInfo.dwActiveProcessorMask << endl;
    while (1)
    {
        myCpu.ReadCpuTemp();    //调用对象函数，读取cpu温度
        cout << myCpu.cpuTemp[0] << endl;
        myCpu.SysTimeDisplay(); //显示系统时间
        myCpu.CpuTempDisplay();  //显示cpu温度
        float ret=myCpu.GetCoreTemp(2);
        printf("%f\n",ret);
        Sleep(1000);
        system("cls");
    }
    return 0;
}