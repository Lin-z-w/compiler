# README

## 平台信息

1. GNU Linux Release: Ubuntu 22.04, 5.10.16.3-microsoft-standard-WSL2
2. GCC version: 11.4.0
3. GNU Flex version: 2.6.4
4. GNU Bison version: 3.8.2

## 实现功能

对一个给定的C--程序进行基本的词法、语法分析，能够对相应的词法、语法错误进行报错。

对于一个无语法语义错误的程序生成对应的中间代码，并在此基础之上实现对应的指令选择、寄存器选择和栈管理，将C--源代码翻译为MIPS32指令序列（包含伪指令），并能够在SPIM Simulator上运行。

## 编译

在 Code 目录下直接执行 make 即可编译得到可执行文件 parser

## 运行

读取 in.cmm 进行语法检查后翻译得到MIPS32指令序列文件 out.s

```shell
./parser in.cmm out.s
```
