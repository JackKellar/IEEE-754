#include <stdio.h>
#include <stdlib.h>

//IEEE 754 uses offset notation for the exponent part in each of its various formats of precision. 
//Unusually however, instead of using "excess 2^n−1" it uses "excess 2^(n−1) − 1" (i.e. excess-15, excess-127, excess-1023, excess-16383) 
//This means that inverting the leading (high-order) bit of the exponent will not convert the exponent to correct two's complement notation.

//This program takes any float number and prints it as it's binary string without converting anything.
//It also shows the values of the mantissa and exponent components.

int main(){
float number;

puts("any float: ");

scanf("%f", &number);

void* ptr = (void*)malloc(sizeof(unsigned int));
*(float*)ptr = number;

unsigned int k = *(unsigned int*)ptr;
printf("%u", (1<<31 & k) != 0);
printf(" ");

for(unsigned int i = 1<<30; i>1<<22; i>>=1){
    printf("%u", (i & k) != 0);
}
printf(" ");

for(unsigned int i = 1<<22; i>0<<0; i>>=1){
    printf("%u", (i & k) != 0);
}
puts("");
unsigned int exp = 0xff;
printf("Exponent value: %d", exp & k>>23);
puts("");

unsigned int mask = 0x7fffff;
float mantissa = 1.0;
int FractionBits = mask & k;

for(int i=22; i>0; i--){
    unsigned int FractionMask = 1<<i;
    int check = FractionMask & FractionBits;
    if(check){
        mantissa += 0.5/(1<<(22-i));
    }
}
printf("Mantissa value: %f", mantissa);

return 0;
}
