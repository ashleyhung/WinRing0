#include "stdafx.h"
#include "ReadCpuInfo.h"
//#include "OlsApi.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
    ReadCpuInfo myCpu;      
    //cout << InitializeOls() << endl;;
    //cout << sysInfo.dwNumberOfProcessors << endl;
    //cout << sysInfo.dwActiveProcessorMask << endl;
    while (1)
    {
        myCpu.ReadCpuTemp();    
        cout << myCpu.cpuTemp[0] << endl;
        myCpu.SysTimeDisplay();
        myCpu.CpuTempDisplay();  
        float ret=myCpu.GetCoreTemp(2);
        printf("%f\n",ret);
        Sleep(1000);
        system("cls");
    }
    return 0;
}
