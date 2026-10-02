#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long ReturnFileSize(char filepath[]);

void ReturnBuffer(char * filepath, long fileSize, char * buffer, long * bytes_read);

int LineCount(const char * tokens);

void ParseResponse(const char * tokens, char ** lines, int numLines);