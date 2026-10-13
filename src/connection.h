    #include <sys/socket.h>
    #include <sys/un.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>

    struct Rspn_StatusLine
    {
        char * protocal;
        char * status_code;
        char * status;
    };

    struct Rspn_HeaderFields
    {
        char * content_type;
        int content_length;
        char * date;
    };


    int CreateListSocket();

    struct Rspn_StatusLine CreateRspnStatus( char * protocal, char * status_code, char * status);

    struct Rspn_HeaderFields CreateRspnHeader(char * content_type, int content_length, char * date);

    void SendAll(long bytes_read, int connfd, char * text);

    void SendHeader(long bytes_read, int connfd, struct Rspn_HeaderFields rspn_headerFields, struct Rspn_StatusLine rspn_statusLine);
    