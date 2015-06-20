#include <stdio.h>
void main() {
    char ch;
    char str[80];
    int a;
    ch = (char)getch();
    gets(str);
    scanf("a=%d", &a);
    putchar(ch);
    puts(str);
    printf("ch=%c, a=%d, str=%s\n", ch, a, str);
}

