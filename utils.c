#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

char* dynamic_file_reader(char* file_name, int chunk_size)
{
    FILE* file_ptr;

    int chk = chunk_size;

    char* buffer = (char*)malloc(chk);
    
    file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL) {
        perror("Opening file");
        exit(1);
    }
    char c;
    int counter = 0;
    // chars get added in 'chunk_size' num of bytes chunks to buffer - recommended 1024 bytes chunk_size
    while ((c = fgetc(file_ptr)) != EOF) {
        if (counter % chk == 0 && counter != 0) {
            char* tmp_buffer = (char*)realloc(buffer, sizeof(buffer));
            buffer = tmp_buffer;
        }             
        buffer[counter] = c;
        counter++;
    }
    return buffer;
}

