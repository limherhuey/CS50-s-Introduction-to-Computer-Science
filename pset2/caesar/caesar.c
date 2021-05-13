#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isdigit(argv[1][i]) == false)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        int key = atoi(argv[1]);
        string plain = get_string("plaintext: ");
        
        printf("ciphertext: ");
        for (int i = 0, n = strlen(plain); i < n; i++)
        {
            if (isalpha(plain[i]))
            {
                if (isupper(plain[i]))
                {
                    printf("%c", (plain[i] - 65 + key) % 26 + 65);
                }
                else
                {
                    printf("%c", (plain[i] - 97 + key) % 26 + 97);
                }
            }
            else
            {
                printf("%c", plain[i]);
            }
        }
        printf("\n");
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}