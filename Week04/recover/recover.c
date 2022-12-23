#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int check_fourth_byte(uint8_t byte);

int main(int argc, char *argv[])
{
    if (argc > 2 || argc < 2)
    {
        printf("Wrong number of arguments; It should be: './recover *file_name*'\n");
        return 1;
    }

    FILE *raw_stream = fopen(argv[1], "r");
    uint8_t *file_buffer = malloc(512);
    uint16_t file_number = 0;
    int has_header = 0;

    char *current_file_name = malloc(3 * sizeof(int));
    FILE *output_image;

    while (fread(file_buffer, sizeof(uint8_t), 512, raw_stream))
    {
        if (file_buffer[0] == 0xff && file_buffer[1] == 0xd8 && file_buffer[2] == 0xff && check_fourth_byte(file_buffer[3]))
        {
            if (file_number > 0)
            {
                fclose(output_image);
            }

            sprintf(current_file_name, "%03i.jpg", file_number);

            output_image = fopen(current_file_name, "w");

            fwrite(file_buffer, sizeof(uint8_t), 512, output_image);

            file_number += 1;
            has_header = 1;
            continue;
        }
        if (has_header)
        {
            fwrite(file_buffer, sizeof(uint8_t), 512, output_image);
        }
    }

    free(file_buffer);
    free(current_file_name);
    fclose(raw_stream);
    fclose(output_image);
    return 0;
}

int check_fourth_byte(uint8_t byte)
{
    uint8_t hex_values[16] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef};

    for (int i = 0; i < 16; i++)
    {
        if (byte == hex_values[i])
        {
            return 1;
        }
    }
    return 0;
}
