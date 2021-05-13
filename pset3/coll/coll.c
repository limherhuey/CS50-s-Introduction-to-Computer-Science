#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//week 3 shorts simple recursion exercise 
int main (void)
{
    int collatz(int n);
    int x;
    
    do {
        x = get_int("int: ");
    } while (x < 0);

    printf("%i times\n", collatz(x));
}

int collatz(int n)
{
	if (n == 1)
	{
		return 0;
	}
	else if (n % 2 == 0)
	{
		return 1 + collatz(n/2);
	}
	else
	{
		return 1 + collatz(3*n + 1);
	}
}
