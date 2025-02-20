# 程序的执行和模拟器

# 简介

本文主要介绍如何实现一个可以执行指令的模拟器以及一个可以执行C代码的模拟器。

# 编译到freestanding环境

我们之前接触到的程序都是在宿主机（linux）上运行的,我们写一个简单的hello,world程序，然后使用objdump查看.out文件的内容，我们可以发现，简单一个helloword程序的内容也非常复杂。为了使我们更好的学习，我们先从freestanding环境开始。

<p>

___我发现我的机器无法运行rv32cc, 所以所以这里我使用rv64cc和64位qemu riscv virt环境___

在qemu_riscv_magic_program.c中写入如下内容**根据我的测试，64位也可以使用下面的方法退出**
```c
#include <stdint.h>
void _start(){
    volatile uint8_t *p = (uint8_t *)(uintptr_t)0x10000000;
    *p = 'A';
    volatile uint32_t *exit = (uint32_t *)(uintptr_t)0x100000;
    *exit = 0x5555; // magic number
    _start();
}
```
在shell中编译上面的代码
```shell
riscv64-unknown-elf-gcc -ffreestanding -nostdlib -Wl,-Ttext=0x80000000 -O2 qemu_riscv_magic_program.c -o a.out
# 使用qemu运行
qemu-system-riscv64 -nographic -M virt -bios none -kernel a.out
```
我们发现程序在输出字符‘A’后自动退出了。


# 自制freestanding运行时环境

我们约定如下：

- 程序从地址0开始运行
- 只支持两条指令：addi(加法)和ebreak(特殊的一些处理)
    - 寄存器a0=0时，输出寄存器a1低八位的字符
    - 寄存器a0=1时，结束运行

## 写一个只包含addi和ebreak的程序
在riscv_program_with_addi_ebreak.c中加入如下的内容（代码来自一生一芯ppt）
```c
static void ebreak(long arg0, long arg1)
{
    asm volatile("addi a0, x0, %0;"
                 "addi a1, x0, %1;"
                 "ebreak"
                 :
                 : "i"(arg0), "i"(arg1));
}

static void putch(char ch)
{
    ebreak(0, ch);
}

static void halt(int code)
{
    ebreak(1, code);
    while (1)
        ;
}

void _start(){
    putch('A');
    halt(0);
}
```
我们使用如下命令编译并查看程序

![addi and ebreak](https://feng-arch.cn/wp-content/uploads/2025/02/1740036459-addi_and_ebreak.png)

可以发现我们使用64位gcc编译出的结果和一生一芯教程中的结果不一样，我们的部分addi指令变成了c.li指令。查询相关资料我们可以知道，这是因为编译器使用了压缩指令集，**c.li**指令用于将一个数立即加载到指定的寄存器中。使用如下的编译选项来告诉编译器使用通用的64位riscv架构
```shell
riscv64-unknown-elf-gcc -ffreestanding -nostdlib -static -Wl,-Ttext=0 -O2 -march=rv64g -mabi=lp64 -o prog.out riscv_program_with_addi_and_ebreak.c
```
![](https://feng-arch.cn/wp-content/uploads/2025/02/1740036944-addi_and_ebreak2.png)


## YEMU模拟器
我们上面写的程序的指令addi和ebreak是我们自己约定的，所以我们的程序不能再qemu上运行，所以我们需要一个自己的指令集模拟器。
### 指令周期
我们的模拟器会按照下面的流程执行一条指令
- 取指：从PC指向的位置读取指令
- 译指：按照约定（手册）解析操作码和操作数
- 执行：按照解析出的操作码，对操作数进行处理
- 更新PC

简化代码表述如下：
```c
#include <stdbool.h>
bool halt = false;

while (!halt)
{
    inst_cycle();
}
```
