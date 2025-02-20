#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MSIZE 1024


#define Assert(cond, format, ...)                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            fprintf(stderr, format "\n", ##__VA_ARGS__);                                                               \
            assert(cond);                                                                                              \
        }                                                                                                              \
    } while (0)


#define Perror(cond, format, ...) Assert(cond, format ": %s", ##__VA_ARGS__, strerror(errno))


uint32_t R[32], PC;
uint8_t M[MSIZE]; //1024-Byte memory


bool halt = false;

void inst_cycle()
{
    uint32_t inst = *(uint32_t *)&M[PC];
    if (((inst & 0x7f) == 0x13) && ((inst >> 12) & 0x7) == 0)
    { // addi
        if (((inst >> 7) & 0x1f) != 0)
        {
            R[(inst >> 7) & 0x1f] =
                R[(inst >> 15) & 0x1f] + (((inst >> 20) & 0x7ff) - ((inst & 0x80000000) ? 4096 : 0));
        }
    }
    else if (inst == 0x00100073)
    { // ebreak
        if (R[10] == 0)
        {
            putchar(R[11] & 0xff);
        }
        else if (R[10] == 1)
        {
            halt = true;
        }
        else
        {
            printf("unsupport ebreak command\n");
        }
    }
    else
    {
        printf("unsupported instruction\n");
    }
    PC += 4;
}


int main(int argc, char *argv[])
{
    PC = 0;
    R[0] = 0;
    Assert(argc >= 2, "Program is not given"); // 要求至少包含一个参数
    FILE *fp = fopen(argv[1], "r");
    Perror(fp != NULL, "Fail to open %s", argv[1]); // 要求argv[1]是一个可以成功打开的文件
    int ret = fseek(fp, 0, SEEK_END);
    Perror(ret != -1, "Fail to seek the end of the file"); // 要求fseek()成功
    long fsize = ftell(fp);
    Perror(fsize != -1, "Fail to return the file position"); // 要求ftell()成功
    rewind(fp);
    Assert(fsize < MSIZE, "Program size exceeds 1024 Bytes"); // 要求程序大小不超过1024字节
    ret = fread(M, 1, MSIZE, fp);
    Assert(ret == fsize, "Fail to load the whole program"); // 要求完全读出程序的内容
    fclose(fp);
    while (!halt)
    {
        inst_cycle();
    }
    return 0;
}