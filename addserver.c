#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

// #define NAME "/tmp/test"


void doprocessing (int sockfd);

int main (int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    unsigned int clilen;
    // struct sockaddr_un server;
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

    // sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("opening stream socket");
        return 1;
    }

    printf("SOCKET CREATED FOR SERVER\n");

    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    // serv_addr.sun_family = AF_UNIX;
    // strcpy (serv_addr.sun_path, NAME);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_un))) {
    if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror ("binding stream socket");
        return 1;
    }

    printf("PORT HAS BEEN BINDED TO LISTEN\n");

    listen (sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("LISTENING FOR 5 CLIENTS...\n");

    while (1) {
        // newsockfd = accept (sockfd, 0, 0);
        newsockfd = accept (sockfd, (struct sockaddr *)&cli_addr, &clilen);

        printf ("CONNECTION FROM CLIENT HAS BEEN ACCEPTED...\n");

        if (newsockfd == -1) {
            perror ("accept");
            return 1;
        } 
        
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // Client Process
            printf ("CHILD PROCESS HAS BEEN CREATED...\n");

            close (sockfd);
            doprocessing (newsockfd);
            return 0;
        } else {
            // Parent Process
            close (newsockfd);
        }

    }
    close (sockfd);
    // unlink (NAME);
}

void doprocessing (int sockfd) {
    int n;
    int augend, addend; // AUGEND + ADDEND = SUM
    char in_buffer [256];
    char out_buffer [256];

    bzero (in_buffer, 256);
    bzero (out_buffer, 256);

    n = read (sockfd, in_buffer, 255);
    if (n < 0) {
        perror ("Reading from Socket");
        return;
    }
    sscanf (in_buffer, "%d %d", &augend, &addend);
    sprintf (out_buffer, "%d", augend + addend);

    write(sockfd, out_buffer, sizeof(out_buffer));
}