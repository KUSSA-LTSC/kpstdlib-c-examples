// utfc.c
//  nasm -f win64 launcher.asm -o launcher.obj
//  nasm -f win64 kpstdlib.asm -o kpstdlib.obj
//  cl /c utfc.c /Fo:utfc.obj /utf-8 /GS- /W4
//  link /OUT:utfc.exe launcher.obj utfc.obj kpstdlib.obj kernel32.lib user32.lib /SUBSYSTEM:WINDOWS /ENTRY:duck /NODEFAULTLIB

#include "kpstdlib.h"

cc filezone[1024] = {0};
us widezone[2048] = {0};

ix luck(kk)
{
    kp_MessageBoxW(NULL, L"1. 进入 luck", L"调试", MB_OK);

    ux hd = kp_CreateFileW(
        L"input.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING);
    if (hd == 0)
    {
        kp_MessageBoxW(NULL, L"2. 打不开 input.txt", L"调试", MB_OK);
        return -1;
    }

    ux fsize = 0;
    kp_GetFileSizeEx(hd, &fsize);

    ux iread = 0;
    kp_ReadFile(hd, filezone, fsize, &iread);
    kp_CloseHandle(hd);

    filezone[fsize] = 0;

    kp_MessageBoxW(NULL, L"3. 文件读完了", L"调试", MB_OK);

    ux wbytes = kp_utf8to16_own(filezone, -1, widezone, 4096);

    kp_MessageBoxW(NULL, L"4. 转换函数返回了", L"调试", MB_OK);

    if (wbytes == 0)
    {
        kp_MessageBoxW(NULL, L"5. 返回 0，转换失败", L"调试", MB_OK);
        return -3;
    }

    kp_MessageBoxW(0, widezone, L"6. 转换结果", MB_OK);

    return 0;
}