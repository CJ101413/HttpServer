#include "connection.h"



int CreateListSocket()
{
    int _sfd = 0;
    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    _sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sfd < 0){
        perror("socket creation failed\n");
        return 1;
    }
    else
        printf("Socket Creation Success!\n");

    if(bind(_sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("binding failed\n");
        return 1;
    }
    else
        printf("Binding Success!\n");

     if(listen(_sfd, 10) < 0){
        perror("Listening Failed!\n");
        return 1;
     }
    else
        printf("Server is Listening!\n");

    return _sfd;
}

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

struct Rspn_StatusLine CreateRspnStatus( char * protocal, char * status_code, char * status)
{
    struct Rspn_StatusLine rspn_statusLine =
    {
        .protocal = protocal,
        .status_code = status_code,
        .status = status
    };

    return rspn_statusLine;
}

struct Rspn_HeaderFields CreateRspnHeader(char * content_type, int content_length, char * date)
{
    struct Rspn_HeaderFields response_header =
    {
        .content_length = content_length,
        .content_type = content_type,
        .date = date
    };

    return response_header;
}

void SendAll(long bytes_read, int connfd, char * text)
{
    int totalBytes = 0;

    while(totalBytes < strlen(text)){
        int bytes_sent = send(connfd, text + totalBytes, strlen(text) - totalBytes, 0);

        if(bytes_sent < 0){
            perror("Header Sent Failure");
            break;
        }
        totalBytes += bytes_sent;
    }
}

void SendHeader(long bytes_read, int connfd, struct Rspn_HeaderFields rspn_headerFields, struct Rspn_StatusLine rspn_statusLine)
{
    char header[256];
    if(strcmp(rspn_statusLine.status_code, "200") == 0)
    {
        snprintf(header, sizeof(header),
        "HTTP/1.1 %s %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        rspn_statusLine.status_code,
        rspn_statusLine.status,
        rspn_headerFields.content_type,
        rspn_headerFields.content_length);
    }

    SendAll(bytes_read, connfd, header);
}
