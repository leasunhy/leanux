/*
 *Leanux shared_fork test
 */
#include <stdio.h>
#include <string.h>
#include <leanux/sys.h>

int CountLetter(const char *str) {
    return strlen(str);
}

char str[80] = "129djwqhdsajd128dw9i39ie93i8494urjoiew98kdkd";
int LetterNr = -1;
int main() {
    int pid;
    pid = shared_fork();
    __asm__ __volatile__("sti");
    if (pid == -1)
        printf("error in fork!\n");
    if (pid) {
        wait();
        printf("Parent: LetterNr=%d\n", LetterNr);
    } else {
        LetterNr = CountLetter(str);
        printf("Child: LetterNr=%d\n", LetterNr);
    }
    return 0;
}

