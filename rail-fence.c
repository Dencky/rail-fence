#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void printUsage(char* argv[]){
    printf("Usage %s -e|-d [-f filepath] [-o filepath] [-r rail count]", argv[0]);
}

int main(int argc, char* argv[])
{
    int opt;
    bool encrypt = false, decrypt = false;
    char* file_in = NULL,* file_out = NULL,* input_text = NULL;
    int rails = 4;
    
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
        long size = ftell(in);
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
    }
    
    return EXIT_SUCCESS;
}