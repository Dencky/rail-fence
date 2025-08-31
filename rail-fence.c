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
    char* file_in = NULL,* file_out = NULL;
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
    
    return 0;
}