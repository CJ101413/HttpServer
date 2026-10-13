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
