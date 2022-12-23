// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t *file_header = malloc(sizeof(uint8_t) * HEADER_SIZE);

    fread(file_header, HEADER_SIZE, 1, input);
    fwrite(file_header, HEADER_SIZE, 1, output);

    free(file_header);

    // TODO: Read samples from input file and write updated data to output file

    int16_t buffer;

    while (1)
    {
        int has_no_bytes = !fread(&buffer, sizeof(int16_t), 1, input);

        if (has_no_bytes)
        {
            break;
        }

        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}

