//test

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    unsigned long prime = 3037000493;
    unsigned long a = 54;
    unsigned long b = 481;
    unsigned int m = 4;

    unsigned int result;

    result = ((a*atoi(argv[1]) + b) % prime) % m;

    printf("%u\n", result);

    return 0;
}
