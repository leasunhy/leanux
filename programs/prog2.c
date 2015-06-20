#include <stdio.h>

#include <leanux/sys.h>

void main() {
    char *s = "I am Liu Siyuan, 13349073";

    lab_ouch();

    printf("Original string: %s\n", s);

    lab_capitalize(s);
    printf("Capitalized str: %s\n", s);

    lab_lower(s);
    printf(" Lowered string: %s\n", s);

    char *ns = "-123456";
    int num = lab_atoi(ns);
    printf("Returned Number: %d\n", num);

    char ns_new[20];
    lab_itoa(num, ns_new);
    printf("Returned String: %s\n", ns_new);

    /*put string at the 15th row and the 14th column*/
    lab_puts_at(s, 15, 14);
}

