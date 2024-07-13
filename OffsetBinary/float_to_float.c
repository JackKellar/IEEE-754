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
    
    //A void pointer allows us to assign it to the float without the compiler complaining about type
    void* ptr = (void*)malloc(sizeof(unsigned int));
    //Explicit casting a float pointer and dereferencing it
    *(float*)ptr = number;
    
    //Assigning the value of our pointer to this integer and explicit casting it to an integer
    unsigned int k = *(unsigned int*)ptr;
    //This is a 32bit integer with a sign bit (most significant bit), an exponent portion (next 8 bits) and a mantissa portion (next 23 bits).
    //Shifting 1 LEFT 31 bits and bitwise ANDing to k gives us the sign bit.
    printf("%u", (1<<31 & k) != 0);
    printf(" ");
    
    //For the exponent portion, we want the next 8 bits past the sign bit. 31-8 = 23.
    //We shift RIGHT and bitwise AND for the exponent portion until we reach the 23rd bit.
    for(unsigned int i = 1<<30; i>1<<22; i>>=1){
        printf("%u", (i & k) != 0);
    }
    printf(" ");
    
    //The same happens for the mantissa portion. Shifting LEFT 0 times is unnecessary. It is only there for clarity.
    for(unsigned int i = 1<<22; i>0<<0; i>>=1){
        printf("%u", (i & k) != 0);
    }
    puts("");
    
    //This mask allows us to show the value of our exponent portion. F in HEX gives us 4 bits. We need 8 for the exponent portion.
    unsigned int exp = 0xff;
    //Bitwise ANDing the exponent mask to our integer allows us to get the value of it.
    //We shift k RIGHT 23 bits so we don't need to loop at all or shift more than once.
    //Say E are our exponent bits and the dashes (-) are the entire 32bit integer, what we're doing looks like this:
    //-EEEEEEEE------------------------ shift RIGHT 23 bits
    //-------------------------EEEEEEEE 
    //-------------------------11111111 <- our 8bit mask
    printf("Exponent value: %d", exp & k>>23);
    puts("");
    
    //This mask allows us to show the value of our mantissa portion. F in HEX gives us 4 bits. We need 23. 
    unsigned int mask = 0x7fffff;
    //IEEE-754 uses the following formula: (-1)^S x  (1 + mantissa) x 2^(exponent - bias)
    //S = Sign bit
    //Mantissa = Mantissa portion
    //Exponent = Exponent portion
    //Bias = 127
    //The mantissa is stored in bits 1-23. An invisible leading bit (i.e. it is not actually stored) with value 1.0 is placed in front 
    float mantissa = 1.0;
    int FractionBits = mask & k;
    //then bit 23 has a value of 1/2, bit 22 has value 1/4 etc. As a result, the mantissa has a value between 1.0 and 2. 
    //If the exponent reaches -127 (binary 00000000), the leading 1 is no longer used to enable gradual underflow.
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
