#include "utils.h"

long ReturnFileSize(char filepath[])
{
        FILE * b_htmlFile = fopen(filepath, "rb");

        if(b_htmlFile == NULL)
        {
            perror("Error Opening File");
            return 1;
        }

        fseek(b_htmlFile, 0, SEEK_END);
        long file_size = ftell(b_htmlFile);
        rewind(b_htmlFile);
        fclose(b_htmlFile);

        return file_size;
}

void ReturnBuffer(char * filepath, long fileSize, char * buffer, long * bytes_read)
{
    FILE * htmlFile = fopen(filepath, "r");
    *bytes_read = fread(buffer, sizeof(char), fileSize, htmlFile);
    fclose(htmlFile);

    buffer[*bytes_read] = '\0';
}

int LineCount(const char * tokens)
{
    const char * splitBody = strstr(tokens, "\r\n\r\n");
    if(splitBody == NULL){
        printf("stsrt() returned Null\n");
    }

    int numLines = 1;
    for (int i = 0; &tokens[i] != splitBody; i++)
    {
        if(tokens[i] == '\n')
            numLines++;
    }

    return numLines;
}

void ParseResponse(const char * tokens, char ** lines, int numLines)
{
    for (size_t i = 0; i < numLines; i++)
    {
        lines[i] = NULL;
    }

    int index = 0;
    for (size_t i = 0; i < numLines; i++)
    {
        const char * firstChar = &tokens[index];
        int lineSize = 0;
        
        while (tokens[index] != '\n')
        {
            index++;
            lineSize++;
        }
        index++;

        lines[i] = malloc(lineSize);
        memcpy(lines[i], firstChar, lineSize);
        lines[i][lineSize - 1] = '\0';
    }
}