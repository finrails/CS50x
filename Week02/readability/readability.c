#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string user_text = get_string("Word: ");
    int string_size = strlen(user_text);
    int sentences = 0;
    int words = 0;
    int letters = 0;

    for (int i = 0; i < string_size; i++)
    {
        if (user_text[i] == '.' || user_text[i] == '!' || user_text[i] == '?')
        {
            sentences++;
        }

        if (user_text[i] == ' ' || user_text[i+1] == '\0')
        {
            words++;
        }

        if(user_text[i] != '!' && user_text[i] != ',' && user_text[i] != '.' && user_text[i] != ' ' && user_text[i] != '\'' && user_text[i] != '?')
        {u
            letters++;
        }
    }

    float avg_l = (float)letters / words * 100;
    float avg_s = (float)sentences / words * 100;
    int index = round(0.0588 * avg_l - 0.296 * avg_s - 15.8);
    if (index > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }

    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    printf("Grade %i\n", index);
    return 0;
}
