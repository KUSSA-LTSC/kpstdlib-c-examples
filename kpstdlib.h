// kpstdlib.h
#ifndef KPSTDLIB_H
#define KPSTDLIB_H

// ===== 类型 =====
typedef unsigned long long ux; // 64位无符号
typedef long long ix;          // 64位有符号
typedef void *px;              // 指针
typedef unsigned short us;     // 16位无符号，宽字符用
typedef char *uu;              // UTF-8 字符串指针

// 别名：kk = void，cc = char 方便打字
#define kk void
#define cc char

// ===== 导出数据 =====
extern char bu[22];    // 数字转字符串的内部缓冲区
extern char dlsbur[8]; // $ 替换缓冲区

// ===== 字符串 =====

// 返回字符串长度，不含结尾 0，失败返回 0
ux kp_strlen_fastcall_win64(px str);
#define kp_strlen kp_strlen_fastcall_win64

// 复制字符串，结尾补 0
// srclen 为负时自动算源长度
// 失败返回 0，成功返回目标末尾 0 的地址
px kp_strcpy_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_strcpy kp_strcpy_fastcall_win64

// 同 strcpy，但结尾是 "$\0"
px kp_strcpy_enddls_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_strcpy$ kp_strcpy_enddls_fastcall_win64

// 把源拼接到目标末尾
// 失败返回 0，成功返回新的末尾 0 的地址
px kp_strend_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_strend kp_strend_fastcall_win64

// 返回指向末尾 0 的指针，失败返回 0
px kp_strled_fastcall_win64(px str);
#define kp_strled kp_strled_fastcall_win64

// 把 dst 里所有 '$' 逐字节替换成 repl 的内容
// repllen 为负时自动算，最多 8 字节
// 失败返回 0
ux kp_replace_single_dollar_symbol_wthcnt_fastcall_win64(
    px repl, ix repllen, px dst, ux dstlen);
#define kp_replace$ kp_replace_single_dollar_symbol_wthcnt_fastcall_win64

// ===== 数字 =====

// 数字转十进制字符串，返回字符串地址
// 【注意】结果放在内部缓冲区 bu，下次调用会覆盖
// 【注意】多线程不安全
px kp_prtnum_frmrcx_fastcall_win64(ux num);
#define kp_itoa kp_prtnum_frmrcx_fastcall_win64

// 二进制转十六进制文本，返回末尾 0 地址
// 失败返回 0
px kp_hex2ascii_fastcall_win64(px src, ux srclen, px dst, ux dstlen);
#define kp_hex2ascii kp_hex2ascii_fastcall_win64

// 十六进制文本转二进制，返回末尾之后地址
// 失败返回 0
px kp_ascii2hex_fastcall_win64(px src, ux srclen, px dst, ux dstlen);
#define kp_ascii2hex kp_ascii2hex_fastcall_win64

// ===== 时间 =====

// FileTime 转年月日时分秒
// y/mo/d/h/mi/s 是 6 个输出指针
// 任意参数为 0 直接返回，不写入
ux kp_filetime_to_realtime_frmrax_ret_fastcall_win64(
    ux ft, px buf, px y, px mo, px d, px h, px mi, px s);
#define kp_ft2time kp_filetime_to_realtime_frmrax_ret_fastcall_win64

// FileTime 格式化
// utc 非 0 使用 UTC，否则使用北京时间
ux kp_timefmt_fastcall_win64(ux ft, px buf, px tm, ux utc);
#define kp_timefmt kp_timefmt_fastcall_win64

// 新版：FileTime 计算为纯数字结构体
// flags: bit0 启用 UTC 偏移，bit1 启用毫秒，bit2 启用微秒
// tm 指向至少 8 个 unsigned long long：
//   年, 月, 日, 时, 分, 秒, 毫秒, 微秒
// 成功返回原 ft，失败返回 0
ux kp_improved_filetime_to_realtime_calc_fastcall_win64(
    ux ft, px tm, ux flags, ix utc_offset_seconds);
#define kp_ft2time_calc kp_improved_filetime_to_realtime_calc_fastcall_win64

// ===== 编码 =====

// UTF-8 转 UTF-16LE（Win32 API 版），srclen 填 -1 自动算
// 返回 UTF-16 字节数
ux kp_win32api_ezutf8t16le_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_utf8to16 kp_win32api_ezutf8t16le_fastcall_win64

// UTF-8 转 UTF-16LE（自研版，不依赖 Win32 API）
// （源，源长，目标，目标长）
// srclen < 0 自动算（要求 src[srclen] 可读）
// dstlen 必须 >= srclen * 2 + 2
// 失败返回 0：空指针 / 目标缓冲区不够 / 非法 UTF-8（含末尾截断）
// 字符级错误（代理码点、超限、解出 0）也会把 dst[0] 写成 0xFFFF
ux kp_text_utf8t16le_main_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_utf8to16_own kp_text_utf8t16le_main_fastcall_win64

// 宽字符转 UTF-8
// srclen 是宽字符数，-1 自动算含结尾
// dstlen 是目标字节数
// 返回写入的 UTF-8 字节数，失败返回 0
ux kp_win32api_ezutf16le2utf8_fastcall_win64(px src, ix srclen, px dst, ux dstlen);
#define kp_utf16to8 kp_win32api_ezutf16le2utf8_fastcall_win64

// ===== SIMD strlen =====

// 通用 SIMD 版（带页边界检查，最安全）
ux kp_simd_strlen_fastcall_win64(px str);
#define kp_simd_strlen kp_simd_strlen_fastcall_win64

// SSE2 版，最推荐
ux kp_sse2_strlen_fastcall_win64(px str);
#define kp_sse2_strlen kp_sse2_strlen_fastcall_win64

// SSE 版
ux kp_sse_strlen_fastcall_win64(px str);
#define kp_sse_strlen kp_sse_strlen_fastcall_win64

// AVX2 版（需要 Haswell 2013+）
ux kp_avx2_strlen_fastcall_win64(px str);
#define kp_avx2_strlen kp_avx2_strlen_fastcall_win64

// ===== 文本格式化 =====
// 按固定字节数分组，组间用空格，行末用 \r\n
// （源，源长，目标，目标长，每组字节数，每行组数）
// srclen          为负 → 自动 strlen
// dstlen          为负 → 禁用长度检查
// groups_per_line 为负 → 禁用换行
// 失败返回 0，成功返回目标末尾 '\0' 的指针
px kp_text_format_divide_fastcall_win64(
    px src, ix srclen, px dst, ix dstlen,
    ix group_size, ix groups_per_line);
#define kp_fmt kp_text_format_divide_fastcall_win64

// ===== 内存 =====

// ERMSB 版 memcpy（建议在支持 ERMSB 的 CPU 上用）
// 失败返回 0
px kp_ermsb_fastcall_win64(px src, ux srclen, px dst, ux dstlen);
#define kp_ermsb kp_ermsb_fastcall_win64

// ===== Win32 API 包装 =====
// 统一约定：失败返回 0

// 打开/创建文件
// access:   0x40000000=写, 0x80000000=读
// share:    1=允许读, 2=允许写, 4=允许删
// creation: 2=CREATE_ALWAYS, 3=OPEN_EXISTING, 4=OPEN_ALWAYS
ux kp_win32api_createfile_w_fastcall_win64(px name, ux access, ux share, ux creation);
#define kp_CreateFileW kp_win32api_createfile_w_fastcall_win64

// 读文件，read 接收实际读到的字节数指针
ux kp_win32api_read_file_fastcall_win64(ux hFile, px buf, ux to_read, px read);
#define kp_ReadFile kp_win32api_read_file_fastcall_win64

// 写文件，written 接收实际写入的字节数指针
ux kp_win32api_write_file_fastcall_win64(ux hFile, px buf, ux to_write, px written);
#define kp_WriteFile kp_win32api_write_file_fastcall_win64

// 取文件大小，size 接收结果（64位）
ux kp_win32api_get_file_size_ex_fastcall_win64(ux hFile, px size);
#define kp_GetFileSizeEx kp_win32api_get_file_size_ex_fastcall_win64

// 设文件指针
// dist 是相对 method 的偏移量（64位有符号）
// newpos 接收新位置指针（可为 NULL）
// method: 0=FILE_BEGIN, 1=FILE_CURRENT, 2=FILE_END
ux kp_win32api_set_file_pointer_ex_fastcall_win64(ux hFile, ix dist, px newpos, ux method);
#define kp_SetFilePointerEx kp_win32api_set_file_pointer_ex_fastcall_win64

// 取当前文件指针，out 接收结果（64位）
ux kp_win32api_get_file_pointer_ex_fastcall_win64(ux hFile, px out);
#define kp_GetFilePointerEx kp_win32api_get_file_pointer_ex_fastcall_win64

// 关句柄
ux kp_win32api_close_handle_fastcall_win64(ux h);
#define kp_CloseHandle kp_win32api_close_handle_fastcall_win64

// 弹宽字符窗（UTF-16LE）
// text/caption 必须是 UTF-16LE 字符串
ux kp_win32api_msgbox_w_fastcall_win64(px hWnd, px text, px caption, ux type);
#define kp_MessageBoxW kp_win32api_msgbox_w_fastcall_win64

// 虚拟内存
px kp_win32api_virtual_alloc_fastcall_win64(px addr, ux size, ux type, ux protect);
#define kp_VirtualAlloc kp_win32api_virtual_alloc_fastcall_win64

ux kp_win32api_virtual_free_fastcall_win64(px addr, ux size, ux type);
#define kp_VirtualFree kp_win32api_virtual_free_fastcall_win64

// ===== 裸 Win32 =====

ux MessageBoxA(px hWnd, px text, px caption, ux type);
void ExitProcess(ux code);
ux WriteFile(ux hFile, px buf, ux to_write, px written, px overlapped);
ux CloseHandle(ux h);
ux GetLastError(void);
px VirtualAlloc(px addr, ux size, ux type, ux protect);
ux VirtualFree(px addr, ux size, ux type);

// ===== 常用常量 =====
#define NULL 0

// 文件访问
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000

// 共享模式
#define FILE_SHARE_READ 1
#define FILE_SHARE_WRITE 2
#define FILE_SHARE_DELETE 4

// 创建方式
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5

// 文件属性
#define FILE_ATTRIBUTE_NORMAL 0x80

// 文件指针起始
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2

// 无效句柄
#define INVALID_HANDLE_VALUE ((ux)-1)

// 内存
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define MEM_DECOMMIT 0x4000
#define MEM_RELEASE 0x8000
#define PAGE_NOACCESS 0x01
#define PAGE_READWRITE 0x04

// 消息框按钮
#define MB_OK 0x00000000
#define MB_OKCANCEL 0x00000001
#define MB_YESNO 0x00000004
#define MB_ICONERROR 0x00000010
#define MB_ICONQUESTION 0x00000020
#define MB_ICONWARNING 0x00000030
#define MB_ICONINFORMATION 0x00000040

// 消息框返回值
#define IDOK 1
#define IDCANCEL 2
#define IDYES 6
#define IDNO 7

#endif