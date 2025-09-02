#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printUsage(char* argv[]){
    printf("Usage %s -e|-d [-f filepath] [-o filepath] [-r rail count]", argv[0]);
}

void encryption(char* input_text, char* output_text, int rails, int size){
    //If rails is either too small or too large for the message we just copy it
    if (rails <= 1 || rails >= size)
    {
        memcpy(output_text, input_text, size);
        return;
    }
    
    int period = 2*rails - 2;
    int out_pos = 0;

    for (int row = 0; row < rails; row++)
    {
        for (int pos = row; pos < size; pos += period)
        {
            //First element in period
            output_text[out_pos++] = input_text[pos];
            
            //Mirror element in period
            int mirror = pos + period - 2 * row;
            if (row > 0 && row < rails - 1 && mirror < size)
            {
                output_text[out_pos++] = input_text[mirror];
            }
            
        }
    }
}

void decryption(const char* input_text, char* output_text, int rails, int size) {
    
    if (rails <= 1 || rails >= size) {
        memcpy(output_text, input_text, size);
        return;
    }

    //Counting the number of elements for each rail
    int *rowCount = calloc(rails, sizeof(int));
    int row = 0, dir = 1;
    for (int i = 0; i < size; i++) {
        rowCount[row]++;
        row += dir;
        if (row == 0 || row == rails - 1) dir = -dir;
    }

    //Choping the input string based on rails
    char **rows = malloc(rails * sizeof(char*));
    int *rowPos = calloc(rails, sizeof(int));
    int next = 0;
    for (int i = 0; i < rails; i++) {
        rows[i] = malloc(rowCount[i]);
        memcpy(rows[i], input_text + next, rowCount[i]);
        next += rowCount[i];
    }

    //Constructing the output
    row = 0; dir = 1;
    for (int i = 0; i < size; i++) {
        output_text[i] = rows[row][rowPos[row]++];
        row += dir;
        if (row == 0 || row == rails - 1) dir = -dir;
    }

    for (int r = 0; r < rails; r++) free(rows[r]);
    free(rows);
    free(rowCount);
    free(rowPos);
}

void freeInAndOut(char* input_text, char* output_text, char* file_in){
    if (file_in)
    {
        free(input_text);
    }
    free(output_text);
}

int main(int argc, char* argv[])
{
    int opt;
    bool encrypt = false, decrypt = false;
    char* file_in = NULL,* file_out = NULL,* input_text = NULL,* output_text = NULL;
    int rails = 4, size = 0;
    
    while ((opt = getopt(argc, argv, "def:o:r:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            encrypt = true;
            break;
        
        case 'd':
            decrypt = true;
            break;

        case 'f':
            file_in = optarg;
            break;
        
        case 'o':
            file_out = optarg;
            break;
        
        case 'r':
            rails = atoi(optarg);
            break;
        
        default:
            printUsage(argv);
            return EXIT_FAILURE;
        }
    }

    if ((encrypt && decrypt) || (!encrypt && !decrypt)){
        printUsage(argv);
        return EXIT_FAILURE;
    }

    if (file_in)
    {
        FILE* in = fopen(file_in, "r");
        if (!in)
        {
            perror("fopen");
            return EXIT_FAILURE;
        }

        fseek(in, 0, SEEK_END);
        size = ftell(in);
        rewind(in);

        input_text = (char*) calloc(size + 1, sizeof(char));
        if (!input_text)
        {
            perror("calloc");
            fclose(in);
            return EXIT_FAILURE;
        }

        fread(input_text, sizeof(char), size, in);
        fclose(in);
        
    }else{
        if (optind >= argc)
        {
            printUsage(argv);
            return EXIT_FAILURE;
        }
        input_text = argv[optind];

        while (input_text[size] != '\0')
        {
            size++;
        }
    }

    output_text = (char*) calloc(size + 1, sizeof(char));
    if (!output_text)
    {
        perror("calloc");
        free(input_text);
        return EXIT_FAILURE;
    }

    if (encrypt)
    {
        encryption(input_text, output_text, rails, size);
    } else {
        decryption(input_text, output_text, rails, size);
    }

    if (file_out)
    {
        FILE* out = fopen(file_out, "w");
        if (!out)
        {
            perror("fopen");
            freeInAndOut(input_text, output_text, file_in);
            return EXIT_FAILURE;
        }

        fwrite(output_text, sizeof(char), size, out);
        fclose(out);
        
    }else{
        printf("%s", output_text);
    }

    freeInAndOut(input_text, output_text, file_in);
    
    return EXIT_SUCCESS;
}