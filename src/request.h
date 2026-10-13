#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Rqst_StatusLine
{
    char * httpMethod;
    char * url;
    char * protocal;
};

struct Rqst_HeaderFields
{
    char * host;
    char * userAgent;
    char * accept;
};

void CreateRqstStatus(char ** tokens, struct Rqst_StatusLine * rqst);

void CreateRqstHeader(char ** lines, int numLines, struct Rqst_HeaderFields * rqst);

void DestroyRqstStatus(struct Rqst_StatusLine * rqst);

void DestroyRqstHeader(struct Rqst_HeaderFields * rqst);