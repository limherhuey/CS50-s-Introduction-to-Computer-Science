#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters!\n");
        return 1;
    }
    
    // static initialises all elements in the array to be 0
    static char repeat[26];
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(argv[1][i]))
        {
            if (repeat[tolower(argv[1][i]) - 97])
            {
                printf("Please do not repeat characters.\n");
                return 1;
            }
            repeat[tolower(argv[1][i]) - 97] = tolower(argv[1][i]);
        }
        else
        {
            printf("Key must all be alphabetical characters!\n");
            return 1;
        }
    }
    
    string plain = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        if (isupper(plain[i]))
        {
            printf("%c", toupper(argv[1][plain[i] - 65]));
        }
        if (islower(plain[i]))
        {
            printf("%c", tolower(argv[1][plain[i] - 97]));
        }
        if (!isalpha(plain[i]))
        {
            printf("%c", plain[i]);
        }
    }
    printf("\n");
    return 0;
}