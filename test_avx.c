/*
 * test_avx.c - 单独测 AVX2 strlen
 *
 * 编译：
 *   nasm -f win64 kpstdlib.asm -o kpstdlib.obj
 *   gcc -o test_avx.exe test_avx.c kpstdlib.obj -nostdlib -lkernel32 -luser32 -mwindows ^
 *       -e main -ffreestanding -fno-stack-protector -fno-asynchronous-unwind-tables -O2
 *
 * 期望值用 __builtin_strlen 让 GCC 编译期算，不再手数。
 */

typedef unsigned long long u64;

__declspec(dllimport) int MessageBoxA(void*, const char*, const char*, unsigned);
__declspec(dllimport) int wsprintfA(char*, const char*, ...);

extern u64 kp_avx2_strlen_fastcall_win64(const char* s);
extern u64 kp_sse2_strlen_fastcall_win64(const char* s);
extern u64 kp_strlen_fastcall_win64(const char* s);

void __main(void) {}

__attribute__((naked)) void ___chkstk_ms(void) {
    __asm__ volatile(
        "push %rcx\n\tpush %rax\n\t"
        "cmp $0x1000, %rax\n\tlea 0x18(%rsp), %rcx\n\tjb 1f\n\t"
        "2:\n\tsub $0x1000, %rcx\n\torl $0, (%rcx)\n\t"
        "sub $0x1000, %rax\n\tcmp $0x1000, %rax\n\tja 2b\n\t"
        "1:\n\tsub %rax, %rcx\n\torl $0, (%rcx)\n\t"
        "pop %rax\n\tpop %rcx\n\tret\n\t"
    );
}

/* 用宏：名字 + 字符串 + GCC 编译期算长度 */
#define CASE(s) { s, __builtin_strlen(s) }

int main(void) {
    static char result[8192];
    char *p = result;

    /* === 各种长度的字符串，期望值由 GCC 算 === */
    static struct { const char *s; u64 expect; } cases[] = {
        CASE(""),
        CASE("a"),
        CASE("ab"),
        CASE("abc"),
        CASE("0123456789"),
        CASE("0123456789abcde"),          /* 15 */
        CASE("0123456789abcdef"),         /* 16 - 正好 SSE 块 */
        CASE("0123456789abcdefg"),        /* 17 - 跨 16 */
        CASE("0123456789abcdefghijklmnopqrstuv"),       /* 31 */
        CASE("0123456789abcdefghijklmnopqrstuvw"),      /* 32 - 正好 AVX 块 */
        CASE("0123456789abcdefghijklmnopqrstuvwxy"),    /* 33 - 跨 32 */
        CASE("0123456789abcdefghijklmnopqrstuvwxyz0123456789ABCDEF"),   /* 62 */
        CASE("0123456789abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG"),  /* 63 */
        CASE("0123456789abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGH"), /* 64 - 正好 64 */
        CASE("0123456789abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHI"),/* 65 - 跨 64 */
    };
    int n = sizeof(cases)/sizeof(cases[0]);

    int fail = 0;
    for (int i = 0; i < n; i++) {
        u64 a = kp_avx2_strlen_fastcall_win64(cases[i].s);
        u64 s = kp_sse2_strlen_fastcall_win64(cases[i].s);
        u64 r = kp_strlen_fastcall_win64(cases[i].s);
        int ok = (a == cases[i].expect && s == cases[i].expect && r == cases[i].expect);
        if (!ok) fail++;
        p += wsprintfA(p,
            "case %2d (expect %I64u): avx=%I64u sse=%I64u repne=%I64u  %s\r\n",
            i, cases[i].expect, a, s, r, ok ? "OK" : "FAIL");
    }

    /* === 4KB 长字符串 === */
    {
        static char big[4096];
        for (int i = 0; i < 4095; i++) big[i] = 'A';
        big[4095] = 0;
        u64 a = kp_avx2_strlen_fastcall_win64(big);
        u64 s = kp_sse2_strlen_fastcall_win64(big);
        u64 r = kp_strlen_fastcall_win64(big);
        int ok = (a == 4095 && s == 4095 && r == 4095);
        if (!ok) fail++;
        p += wsprintfA(p,
            "big 4095: avx=%I64u sse=%I64u repne=%I64u  %s\r\n",
            a, s, r, ok ? "OK" : "FAIL");
    }

    /* === 所有对齐偏移：把长度为 100 的字符串放在 big+off 处 === */
    {
        static char buf[256];
        int align_fail = 0;
        for (int off = 0; off < 64; off++) {
            char *s = buf + off;
            for (int j = 0; j < 100; j++) s[j] = (char)('A' + (j % 26));
            s[100] = 0;
            u64 a = kp_avx2_strlen_fastcall_win64(s);
            u64 ss = kp_sse2_strlen_fastcall_win64(s);
            u64 r = kp_strlen_fastcall_win64(s);
            if (a != 100 || ss != 100 || r != 100) {
                align_fail++;
                p += wsprintfA(p,
                    "align off=%d: avx=%I64u sse=%I64u repne=%I64u  FAIL\r\n",
                    off, a, ss, r);
            }
        }
        if (!align_fail) {
            p += wsprintfA(p, "align offsets 0..63: all OK\r\n");
        } else {
            fail += align_fail;
        }
    }

    p += wsprintfA(p, "\r\nTOTAL: %s\r\n", fail ? "FAIL" : "ALL OK");

    MessageBoxA(0, result, "AVX2 strlen Test", 0);
    return 0;
}