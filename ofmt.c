// ofmt.c
// 测试 kp_text_format_divide_fastcall_win64
//
// 编译：
//   nasm -f win64 launcher.asm -o launcher.obj
//   nasm -f win64 kpstdlib.asm -o kpstdlib.obj
//   cl /c ofmt.c /Fo:ofmt.obj /utf-8 /GS- /Gs9999999 /W4
//   link /OUT:ofmt.exe launcher.obj ofmt.obj kpstdlib.obj kernel32.lib user32.lib /SUBSYSTEM:WINDOWS /ENTRY:duck /NODEFAULTLIB
//
// 前置：kpstdlib.asm 里已有 global kp_text_format_divide_fastcall_win64
//       kpstdlib.h 里已加好声明

#include "kpstdlib.h"

// ===== 测试数据 =====
// 17 字节：最后一组不完整（2 字节一组 → 9 组，最后一组 1 字节）
static char src[] = "HelloWorld1234567";
static char dst[256];
static unsigned short wbuf[512];

ux luck(void)
{
    px ret;

    // 2 字节一组，一行 4 组
    // 预期输出：
    //   "He ll oW or\r\n"
    //   "ld 12 34 56\r\n"
    //   "7"
    ret = kp_fmt(src, 17, dst, 256, 2, 4);

    if (ret == 0) {
        MessageBoxW(NULL, L"kp_fmt 返回 0（失败）",
                    L"ofmt", MB_OK | MB_ICONERROR);
        return 1;
    }

    // UTF-8 → UTF-16 显示
    if (kp_utf8to16(dst, -1, wbuf, sizeof(wbuf)) == 0) {
        MessageBoxW(NULL, L"UTF-8 → UTF-16 转换失败",
                    L"ofmt", MB_OK | MB_ICONERROR);
        return 2;
    }

    MessageBoxW(NULL, wbuf, L"kp_text_format_divide 结果", MB_OK);
    return 0;
}