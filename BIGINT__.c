#include<stdio.h>
#include <stdbool.h>
#include <string.h>

#define size 35
#define maximum_digits 309

typedef enum{ SMALL = -1 ,EQUAL ,LARGE} cmp;

typedef struct 
{
    long int input_num[size];
    char sign;
} BigInt;

BigInt InitialiseBigInt(char sign)
{
    BigInt num;
    for(int i = 0; i < size; i++)
    {
        num.input_num[i] = 0;
    }
    num.sign = sign;
    return num;
}

void InputBigInt(char *str, BigInt *num)
{
    int length = 0;
    // Calculate string length
    while(str[length] != '\0')
    {
        length++;
    }
    char sign = str[0];
    
    *num = InitialiseBigInt(sign);
    
    // Start from rightmost digit
    int pos = length - 1;
    int curr_block = size - 1;
    
    // Process 9 digits at a time from right to left
    while(pos >= 1 && curr_block >= 0)
    {
        int value = 0;
        int power = 1;
        
        // Take up to 9 digits for current block
        for(int i = 0; i < 9 && pos >= 1; i++)
        {
            value += (str[pos] - '0') * power;
            power *= 10;
            pos--;
        }
        printf("\n");
        
        num->input_num[curr_block] = value;
        curr_block--;
    }
}

void PrintBigInt(BigInt num)
{
    int i = 0;
    
    // Find first non-zero block
    while(i < size && num.input_num[i] == 0)
    {
        i++;
    }
    
    // If all blocks are zero
    if(i == size)
    {
        printf("0\n");
        return;
    }
    
    //sign of the 1024 bit number
    printf("%c", num.sign);
    // Print first block without leading zeros
    printf("%d", num.input_num[i]);
    i++;
    
    // Print remaining blocks with leading zeros
    for(; i < size; i++)
    {
        printf("%09d", num.input_num[i]);
    }
    printf("\n");
}

cmp CompareMagnitude(char *num1, char *num2)
{
    cmp cm;

    int len1 = strlen(num1);
    int len2 = strlen(num2);

    if (len1 > len2) {
        cm = LARGE;
    } else if (len1 < len2) {
        cm = SMALL;
    } else {
        int i = 1;
        while (i < len1 && num1[i] == num2[i]) {
            i++;
        }
        if (i == len1) {
            cm = EQUAL;
        } else if (num1[i] < num2[i]) {
            cm = SMALL;
        } else {
            cm = LARGE;
        }
    }
    return cm;
}


BigInt AddHelper(BigInt num1, BigInt num2, char sign)
{
    BigInt result;
    result = InitialiseBigInt(sign);
    int carry = 0;
    
    for(int i = size - 1; i >= 0; i--)
    {
        long long sum = (long long)num1.input_num[i] + num2.input_num[i] + carry;
        carry = sum / 1000000000;
        result.input_num[i] = sum % 1000000000;
    }
    
    return result;
}

BigInt SubHelper(BigInt num1, BigInt num2, char sign)
{
    BigInt result;
    result = InitialiseBigInt(sign);
    int borrow = 0;
    
    for(int i = size - 1; i >= 0; i--)
    {
        int diff = num1.input_num[i] - num2.input_num[i] - borrow;
        if(diff < 0)
        {
            diff += 1000000000;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result.input_num[i] = diff;
    }
    
    return result;
}

BigInt MultHelper(BigInt num1, BigInt num2, char sign)
{
    BigInt result;
    result = InitialiseBigInt(sign);
    for(int i = size - 1; i >= 0; i--)
    {
        for(int j = size - 1; j >= 0; j--)
        {
            if(i + j - (size - 1) < 0) break;
            
            long long product = (long long)num1.input_num[i] * num2.input_num[j];
            int pos = i + j - (size - 1);
            
            // Add to current position
            long long current = result.input_num[pos];
            current += product;
            
            // Handle carry
            if(current >= 1000000000)
            {
                result.input_num[pos - 1] += current / 1000000000;
                current %= 1000000000;
            }
            result.input_num[pos] = current;
        }
    }
    
    return result;
}

BigInt AddBigInt(BigInt num1 , BigInt num2, cmp cm)
{
    BigInt result;
    if(num1.sign == num2.sign)
    {
        result = AddHelper(num1, num2, num1.sign);
    }
    else
    {
        if(num1.sign == '+' && num2.sign == '-')
        {
            if(cm == LARGE || cm == EQUAL)
            {
                result = SubHelper(num1, num2, '+');
            }
            else
            {
                result = SubHelper(num2, num1, '-');
            }
        }
        else
        {
            if(cm == LARGE || cm == EQUAL)
            {
                result = SubHelper(num1, num2, '-');
            }
            else
            {
                result = SubHelper(num2, num1, '+');
            } 
        }
    }
    return result;
}

BigInt SubBigInt(BigInt b1, BigInt b2, cmp cm) {
    BigInt result;
    char sign;
    if (b1.sign == '+' && b2.sign == '+') {
        if (cm == LARGE || cm == EQUAL) {
            sign = '+';
            result = SubHelper(b1,b2,sign);
        }
        else
        {
            sign = '-';
            result = SubHelper(b2,b1,sign);
        }
    }
    else if(b1.sign == '+' && b2.sign == '-')
    {
        sign = '+';
        result = AddHelper(b1,b2,sign);
    }
    else if(b1.sign == '-' && b2.sign == '+')
    {
        sign = '-';
        result = AddHelper(b1,b2,sign);
    }
    else
    {
        if (cm == LARGE || cm == EQUAL) {
            sign = '-';
            result = SubHelper(b1,b2,sign);
        }
        else
        {
            sign = '+';
            result = SubHelper(b2,b1,sign);
        }
    }
    return result;
}

BigInt MultBigInt(BigInt b1, BigInt b2, cmp cm) 
{
    BigInt result;
    char sign;
    if(b1.sign == b2.sign)
    {
        sign = '+';
    }
    else
    {
        sign = '-';
    }
    result = MultHelper(b1,b2,sign);
    return result;
}

int main()
{
    while(1)
    {
        BigInt num1_1024bit, num2_1024bit, result_1024bit;
        char input_str1[maximum_digits + 2], input_str2[maximum_digits + 2];
        // Take inputs
        printf("Enter the first number: ");
        scanf("%310s", input_str1);
        int c;
        while((c = getchar()) != '\n' && c != EOF);
        printf("Enter the second number: ");
        scanf("%310s", input_str2);
        while((c = getchar()) != '\n' && c != EOF);

        cmp cm = CompareMagnitude(input_str1, input_str2);
        
        InputBigInt(input_str1, &num1_1024bit);
        InputBigInt(input_str2, &num2_1024bit);
        PrintBigInt(num1_1024bit);
        PrintBigInt(num2_1024bit);

        //Sum
        result_1024bit = AddBigInt(num1_1024bit, num2_1024bit, cm);
        printf("\nSum: ");
        PrintBigInt(result_1024bit);
        
        // Subtraction
        result_1024bit = SubBigInt(num1_1024bit, num2_1024bit, cm);
        printf("\nSubtraction: ");
        PrintBigInt(result_1024bit);

        // Multiplication
        result_1024bit = MultBigInt(num1_1024bit, num2_1024bit, cm);
        printf("\nProduct: ");
        PrintBigInt(result_1024bit);
        
        // Ask if the user wants to continue
        char cont;
        printf("\nWould you like to perform another calculation? (y/n): ");
        scanf(" %c", &cont);
        
        if(cont == 'n' || cont == 'N')
        {
            printf("\nExiting...\n");
            break;
        }
    }
    
    return 0;
}