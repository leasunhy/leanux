#include <stdio.h>
#include <leanux/sys.h>

int main() {
    while (1) {
        for (unsigned i = 0; i < 1000000000; ++i) if (i == 50000)
            printf("I'm a normal C program!!!!\n");
    }
}

