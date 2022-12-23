    #include <cs50.h>
    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>

    int get_character_index(char current_character);
    string reconstruct_word(string original_word);
    int duplicate_sanitizer(string sentence);

    int main(int argc, string argv[])
    {

        if (argc > 2)
        {
            printf("Invalid number of arguments! It should be 'substitution [argument]'.\n");
            return 1;
        }

        if (argc < 2)
        {
            printf("Invalid number of arguments! It should be 'substitution [argument]'.\n");
            return 1;
        }

        int arg_len = strlen(argv[1]);

        if (arg_len < 26)
        {
            printf("Invalid key! It must contain 26 characters, but you passed %i instead!\n", arg_len);
            return 1;
        }

        for (int i = 0; i < arg_len; i++)
        {
            if(ispunct(argv[1][i]))
            {
                printf("Invalid key! It must not contain any punctuation symbol in.\n");
                return 1;
            }
        }

        if (duplicate_sanitizer(argv[1]))
        {
            printf("Invalid argument. The key must not have the same character twice!\n");
            return 1;
        }

        string argument = argv[1];
        string user_input = get_string("plaintext:  ");
        int user_input_len = strlen(user_input);

        for (int i = 0; i < arg_len; i++)
        {
            if(islower(argument[i]))
            {
                argument[i] = (char)toupper(argument[i]);
            }
        }

        printf("ciphertext: ");
        for (int i = 0; i < user_input_len; i++)
        {
            int shifted_pos = get_character_index(user_input[i]);
            if (shifted_pos >= 0)
            {
                if (isupper(user_input[i]))
                {
                    printf("%c", argument[shifted_pos]);
                }
                else
                {
                    printf("%c", tolower(argument[shifted_pos]));
                }
            }
            else
            {
                printf("%c", user_input[i]);
            }
        }
        printf("\n");
        return 0;
    }

    int duplicate_sanitizer(string sentence)
    {
        int sentence_size = strlen(sentence);
        int duplicates = -1;
        for (int i = 0; i < sentence_size; i++)
        {
            for (int j = 0; j < sentence_size; j++)
            {
                if (sentence[i] == sentence[j])
                {
                    duplicates++;
                }
            }
            if(duplicates)
            {
                return 1;
            }
            duplicates = -1;
        }
        return 0;
    }

    int get_character_index(char current_character)
    {
        string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        int alphabet_len = strlen(alphabet);

        for (int i = 0; i < alphabet_len; i++)
        {
            if (current_character == alphabet[i] || (char)tolower(alphabet[i]) == current_character)
            {
                return i;
            }
        }
        return -1;
    }
