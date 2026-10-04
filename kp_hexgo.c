// kp_hexgo.c

// ; Copyright (c) 2026-8086 KUSSA (KUSSA_LTSC)
// ; All rights reserved.
// ;
// ; SPDX-License-Identifier: LicenseRef-scancode-kudos-sal-2.5

//  nasm -f win64 launcher.asm -o launcher.obj
//  nasm -f win64 kpstdlib.asm -o kpstdlib.obj
//  cl /c kp_hexgo.c /Fo:kp_hexgo.obj /utf-8 /GS- /W4
//  link /OUT:convert.exe launcher.obj kp_hexgo.obj kpstdlib.obj kernel32.lib user32.lib /SUBSYSTEM:WINDOWS /ENTRY:duck /NODEFAULTLIB

#include "kpstdlib.h"

cc filezone[1024] = {0};
us info16[256] = {0};
cc info[] = "成功打开了文件，句柄：";
cc buffer[256] = {0};
cc asciizone[2048];
us widezone[2048];

ix luck(kk)
{

    ux hd = kp_CreateFileW(
        L"input.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING);
    if (hd == 0)
    {
        kp_MessageBoxW(
            NULL,
            L"打开文件时候出错",
            L"出错了",
            MB_ICONERROR | MB_OK);
        return -1;
    }
    kp_strcpy(
        info,
        -1,
        buffer,
        256);
    kp_strend(kp_itoa(hd), -1, buffer, 256);
    kp_utf8to16(buffer, -1, info16, 512);
    kp_MessageBoxW(
        NULL,
        info16,
        L"这是一个可以编辑的标题",
        MB_OK);
    ux fsize;
    ux loadsign = kp_GetFileSizeEx(hd, &fsize);
    if (loadsign == 0)
    {
        kp_MessageBoxW(NULL, L"获取文件大小失败", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -4;
    }
    if (fsize >= 1024)
    {
        kp_MessageBoxW(
            NULL,
            L"缓冲区不够大",
            L"出错了",
            MB_ICONERROR | MB_OK);
        kp_CloseHandle(hd);
        return -2;
    }
    if (fsize == 0)
    {
        kp_MessageBoxW(NULL, L"空文件", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -3;
    }

    ux iread;
    if (kp_ReadFile(hd, filezone, fsize, &iread) == 0)
    {
        kp_MessageBoxW(NULL, L"读取文件失败", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -8;
    }

    if (iread != fsize)
    {
        kp_MessageBoxW(NULL, L"读取文件大小不匹配", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -5;
    }
    ux ckhx = kp_hex2ascii(filezone, fsize, asciizone, 2048);
    if (ckhx == 0)
    {
        kp_MessageBoxW(NULL, L"hex 转换失败", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -6;
    }

    if (kp_utf8to16(asciizone, -1, widezone, 4096) == 0)
    {
        kp_MessageBoxW(NULL, L"utf16le转换失败", L"错误", MB_OK);
        kp_CloseHandle(hd);
        return -7;
    }
    kp_MessageBoxW(0, widezone, L"内容：", 0);
    kp_CloseHandle(hd);

    return 0;
}