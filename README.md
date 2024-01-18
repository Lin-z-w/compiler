# README

该阶段代码基于以下框架实现

[BryanHeBY/cmmc_optimizer: 南京大学2022年秋季学期编译原理课程实验五代码框架 (github.com)](https://github.com/BryanHeBY/cmmc_optimizer)

同时也借鉴了软件分析课程是实验指南

[Static Program Analysis | Tai-e (pascal-lab.net)](https://tai-e.pascal-lab.net/lectures.html)

## 平台信息

1. GNU Linux Release: Ubuntu 22.04, 5.10.16.3-microsoft-standard-WSL2
2. GCC version: 11.4.0
3. GNU Flex version: 2.6.4
4. GNU Bison version: 3.8.2

## 实现功能

在原本框架代码的基础上，实现了以下的机器无关代码优化

- 可用表达式
- 常量传播
- 常量折叠
- 复制传播
- 活跃变量分析
- 死代码消除

## 编译

在 Code 目录下直接执行 make 即可编译得到可执行文件 parser

## 运行

读取 in.ir 优化后得到 out.ir

```bash
./parser in.ir out.ir
```
