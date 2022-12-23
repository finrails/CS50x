#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height_number;
    do
    {
        height_number = get_int("Height: ");
    }
    while (height_number < 1 || height_number > 8);

    int tracker = height_number;

    for(int i = 0; i < height_number; i++)
    {
        tracker -= 1;
        for (int k = 0; k < tracker; k++)
        {
            printf(" ");
        }

        for(int j = 0; j < i+1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
    return 0;
}
