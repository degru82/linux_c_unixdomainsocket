// #include <sys/types.h>
#include <sys/socket.h>
// #include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>



// #define NAME "/tmp/test"

int main (int argc, char *argv[]) {
    int sockfd, portno, n;
    // struct sockaddr_un server;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char out_buffer [256] = {0};
    char in_buffer [256] = {0};

    char hostname [1024] = {0};
    

    if (argc != 3) {
        printf ("usage:%s <integer 1> <integer 2>\n", argv[0]);
        return 0;
    }
    sprintf (out_buffer, "%s %s", argv[1], argv[2]);

    // sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("opening stream socket");
        return 1;
    }

    printf ("SOCKET OPENED\n");

    gethostname(hostname, 1023);
    server = gethostbyname(hostname);
    if (server == NULL) {
        perror ("no such host");
        return 0;
    }

    printf ("GOT HOSTNAME AND INFO: %s\n", hostname);

    // serv_addr.sun_family = AF_UNIX;
    // strcpy (serv_addr.sun_path, NAME);
    portno = 5001;
    bzero ((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, 
            (char*) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);

    printf ("READY TO CONNECT\n");

    // if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0) {
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close (sockfd);
        perror ("connecting stream socket");
        return 1;
    }

    printf ("CONNECTED... READY TO WRITE\n");

    if (write (sockfd, out_buffer, sizeof(out_buffer)) < 0) {
        perror ("writing on stream socket");
    }

    printf ("WROTE NUMBERS TO SERVER\n");

    read (sockfd, in_buffer, 256);

    printf ("%s + %s = %s", argv[1], argv[2], in_buffer);

    close (sockfd);
}