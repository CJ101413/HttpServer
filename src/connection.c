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