#include "response.h"

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
