#include <stdio.h>

int main(void) {
    FILE* p = popen("sort ./1.c", "r");
    char buff[51];
    if (fread(buff, sizeof(char), 50, p) < 0)
        perror("fread");
    buff[50] = '\0';
    printf("%s\n", buff);
    if (pclose(p) < 0)
        perror("pclose");
    return 0;
}