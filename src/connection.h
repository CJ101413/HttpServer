    #include <sys/socket.h>
    #include <sys/un.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
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

    void CreateRqstStatus(char ** tokens, struct Rqst_StatusLine * rqst);

    void CreateRqstHeader(char ** lines, int numLines, struct Rqst_HeaderFields * rqst);

    void DestroyRqstStatus(struct Rqst_StatusLine * rqst);

    void DestroyRqstHeader(struct Rqst_HeaderFields * rqst);

    struct Rspn_StatusLine CreateRspnStatus( char * protocal, char * status_code, char * status);

    struct Rspn_HeaderFields CreateRspnHeader(char * content_type, int content_length, char * date);

    void SendAll(long bytes_read, int connfd, char * text);

    void SendHeader(long bytes_read, int connfd, struct Rspn_HeaderFields rspn_headerFields, struct Rspn_StatusLine rspn_statusLine);
    