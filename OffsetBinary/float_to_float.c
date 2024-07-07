#include <stdio.h>
#include <stdlib.h>

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
