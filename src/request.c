#include "request.h"

void CreateRqstStatus(char ** tokens, struct Rqst_StatusLine * rqst)
{
    char * words[3] = { NULL };
    char * lines = tokens[0];
    const char * p1 = &lines[0];
    const char * p2 = strchr(p1, ' ');

    for (size_t i = 0; i < 3; i++)
    {
        int wordSize = (int)(p2 - p1) + 1;

        words[i] = malloc(wordSize);
        memcpy(words[i], p1, wordSize);
        words[i][wordSize - 1] = '\0';

        if(i == 0)
            rqst->httpMethod = words[0];
        else if(i == 1)
            rqst->url = words[1];
        else if(i == 2)
            rqst->protocal = words[2];
        else
            continue;

        if(i < 2)
        {
            p1 = p2 + 1;
            p2 = strchr(p1, ' ');

            if(p2 == NULL){
                p2 = strchr(p1, '\0');
            }
        }
    }
}

void CreateRqstHeader(char ** lines, int numLines, struct Rqst_HeaderFields * rqst)
{
    char * properties[numLines];
    for (size_t j = 0; j < numLines; j++)
    {   
        properties[j] = NULL;
    }

    rqst->accept = NULL;
    rqst->host = NULL;
    rqst->userAgent = NULL;

    for (size_t i = 1; i < numLines; i++)
    {
        char * firstChar = strstr(lines[i], ":");
        int propSize = firstChar - lines[i];
        
        properties[i] = malloc(propSize + 1);
        memcpy(properties[i], lines[i], propSize);

        if(!firstChar)
            perror("Invalid HTTP request");

        if(*(firstChar += 1) == ' ')
            firstChar += 1;

        char * pNullTerm = lines[i] + strlen(lines[i]);
        int valueSize = pNullTerm - firstChar;

        for (size_t j = 0; j < propSize; j++)
        {
            properties[i][j] = tolower(properties[i][j]);
        }
        
        if(strcmp(properties[i], "host") == 0){
            rqst->host = malloc(valueSize + 1);
            memcpy(rqst->host, firstChar, valueSize + 1);
        }
        if(strcmp(properties[i], "user-agent") == 0){
            rqst->userAgent = malloc(valueSize + 1);
            memcpy(rqst->userAgent, firstChar, valueSize + 1);

        }
        if(strcmp(properties[i], "accept") == 0){
            rqst->accept = malloc(valueSize + 1);
            memcpy(rqst->accept, firstChar, valueSize + 1);
        }
    }
    
}

void DestroyRqstStatus(struct Rqst_StatusLine * rqst)
{
    if (rqst == NULL)
        return;

    free(rqst->httpMethod);
    free(rqst->protocal);
    free(rqst->url);

    rqst->httpMethod = NULL;
    rqst->protocal = NULL;
    rqst->url = NULL;
}

void DestroyRqstHeader(struct Rqst_HeaderFields *rqst)
{
    if (rqst == NULL)
        return;

    free(rqst->accept);
    free(rqst->host);
    free(rqst->userAgent);

    rqst->accept = NULL;
    rqst->host = NULL;
    rqst->userAgent = NULL;
}