#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


uint32_t R[32], PC;
uint8_t M[1024] = {
    0x13, 0x05, 0x00, 0x00, 0x93, 0x05, 0x10, 0x04, 0x73, 0x00, 0x10, 0x00, 0x13, 0x05,
    0x10, 0x00, 0x93, 0x05, 0x00, 0x00, 0x73, 0x00, 0x10, 0x00, 0x6f, 0x00, 0x00, 0x00,
}; //64-Byte memory


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

int main()
{
    PC = 0;
    R[0] = 0;
    while (!halt)
    {
        inst_cycle();
    }
    return 0;
}
