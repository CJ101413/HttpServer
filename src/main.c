#include <stdio.h>
#include <string.h>
#include "connection.h"
#include "utils.h"

//Server

int main()
{
    int sfd, connfd;
    struct sockaddr_in client = {0};
    socklen_t len = sizeof(client);

    sfd = CreateListSocket();

    while (1)
    {
        char filepath[] = "/home/joshua/dev/HttpServer";

        connfd = accept(sfd, (struct sockaddr *)&client, &len);
        if(connfd < 0){
            perror("Server failed to accept the request..\n");
            return 1;
        }
        else
            printf("Server accepted the request!\n");
        
        char receiving_buffer[6000];
        int received_bytes = recv(connfd, receiving_buffer, sizeof(receiving_buffer), 0);
        if(received_bytes > 0)
            receiving_buffer[received_bytes] = '\0';

        
        int numLines = LineCount(receiving_buffer);
        char * lines[numLines];
        ParseResponse(receiving_buffer, lines, numLines);

        struct Rqst_StatusLine  rqst_status = {0}; 
        struct Rqst_HeaderFields  rqst_headers = {0};
        struct Rspn_StatusLine rspn_status = {0};
        struct Rspn_HeaderFields rspn_headers = {0};
        CreateRqstStatus(lines, &rqst_status);
        CreateRqstHeader(lines, numLines, &rqst_headers);

        long bytes_read = 0;
        
        const char *path = rqst_status.url;
        if (strcmp(path, "/") == 0)
        {
            path = "/index.html";
        }
        strcat(filepath, path);

        long file_size = ReturnFileSize(filepath);
        char * buffer = malloc(file_size + 1);
        ReturnBuffer(filepath, file_size, buffer, &bytes_read);

        if(strcmp(rqst_status.httpMethod, "GET") == 0)
        {
            rspn_status.protocal = "HTTP/1.1";
            rspn_status.status_code = "200";
            rspn_status.status = "OK";

            rspn_headers.content_length = bytes_read;
            rspn_headers.content_type = "text/html; charset=utf-8";
            rspn_headers.date = "Random Date go brrrrrr";
        }
        else
        {
            printf("Status Code: 405\n");
            printf("Method Not Allowed\n");
        }

        SendHeader(bytes_read, connfd, rspn_headers, rspn_status);
        int body_sent = send(connfd, buffer, bytes_read, 0);

        close(connfd);
        free(buffer);
        DestroyRqstHeader(&rqst_headers);
        DestroyRqstStatus(&rqst_status);
    }
}