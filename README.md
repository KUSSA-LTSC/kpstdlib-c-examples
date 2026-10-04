# kpstdlib-c-examples

**kpstdlib** 的 C 语言示例 —— KUSSA's standard library for NASM Win64.

**主仓库：** https://github.com/KUSSA-LTSC/KUDOS-kpstdlib

> 最新的汇编源码请以主仓库为准。

---

## 这是什么

给 [kpstdlib](https://github.com/KUSSA-LTSC/KUDOS-kpstdlib) 写的 C 示例。
包括头文件、示例程序、构建脚本，以及一份汇编源码快照。

---

## 目录结构

```
汇编源码快照
C 头文件 + 示例
构建脚本
```

| 示例 | 用途 |
|---|---|
| `kp_hexgo.c` | 十六进制查看器，读 `input.txt` 弹窗显示 |
| `ofmt.c` | 测试 `kp_text_format_divide` 文本分组 |
| `utfc.c` | 测试自研 UTF-8 → UTF-16 转换 |
| `test_avx.c` | 测试 SIMD strlen 的 AVX2 版（GCC 用） |

---

## 环境要求

- Windows x64
- [NASM](https://www.nasm.us/)
- MSVC（Visual Studio 或者 Build Tools）
- MinGW-w64 GCC（用于 `test_avx.c` ）

**需要修改 `msvc.bat`：**

```bat
call D:\VS\VC\Auxiliary\Build\vcvars64.bat
```

把路径改成你机器上的 `vcvars64.bat`。

---

## 编译

双击对应的 bat：

```
build_kp_hexgo.bat    →  convert.exe
build_ofmt.bat        →  ofmt.exe
build_utfc.bat        →  utfc.exe
```

**`kp_hexgo` / `utfc` 需要 `input.txt`：**

```
在程序同目录建一个 input.txt，随便写点东西，运行 exe。
或者用我提供的示范版本。
```

---

## 许可证

和主仓库一致：

- **KUDOS SOURCE AVAILABLE LICENSE 2.5**
- 源码可见，**不是** OSI 开源
- 禁止商业使用、禁止分发、禁止 AI 训练
- 详细条款见 `LICENSE` / `LICENSE_CN`

---

## 免费链接

https://github.com/KUSSA-LTSC/KUDOS-kpstdlib
