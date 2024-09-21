#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "H_extraStuff.h"
#include "H_arithmetic.h"


int main(int argc, char* argv[]) {

    // int s1 = isValidRoman("IXCCIX");
    char* s1 = romanToDecimal("CXVI");


    printf("%s\n", s1);


    return 0;
}