#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    int letter_count(string t);
    int word_count(string t);
    int sentence_count(string t);

    string text = get_string("Text: ");

    int lettercount = letter_count(text), wordcount = word_count(text), sencount = sentence_count(text);

    float L = (float) lettercount / (float) wordcount * 100;
    float S = (float) sencount / (float) wordcount * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);
    
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    if (grade < 16 && grade >= 1)
    {
        printf("Grade %i\n", grade);
    }
}

int letter_count(string t)
{
    int lettercount = 0;
    for (int i = 0; i < strlen(t); i++)
    {
        if (isalpha(t[i]))
        {
            lettercount++;
        }
    }
    return lettercount;
}

int word_count(string t)
{
    int wordcount = 1;
    for (int i = 0; i < strlen(t); i++)
    {
        if (isspace(t[i]))
        {
            wordcount++;
        }
    }
    return wordcount;
}

int sentence_count(string t)
{
    int sencount = 0;
    for (int i = 0; i < strlen(t); i++)
    {
        if (t[i] == '.' || t[i] == '?' || t[i] == '!')
        {
            sencount++;
        }
    }
    return sencount;
}