#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NAME "/tmp/test"

int main (int argc, char *argv[]) {
    int sock, msgsock, rval;
    int a=0, b=0;
    struct sockaddr_un server;
    char buf[1024] = {0};
    char ret[1024] = {0};

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror ("opening stream socket");
        return 1;
    }

    server.sun_family = AF_UNIX;
    strcpy (server.sun_path, NAME);
    if (bind (sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        perror ("binding stream socket");
        return 1;
    }

    printf ("socket has name %s\n", server.sun_path);
    listen (sock, 5);
    while (1) {
        msgsock = accept (sock, 0, 0);
        if (msgsock == -1) 
            perror ("accept");
        else do {
            if ((rval = read (msgsock, buf, 1024)) < 0) {
                perror ("reading stream messages");
            } else if (rval == 0) {
                //printf ("ending connection\n");
            } else {
                //printf ("->%s\n", buf);
                sscanf (buf, "%d %d", &a, &b);

                sprintf (ret, "%d", a + b);
                //printf (ret, "%d", a + b);
                write (msgsock, ret, sizeof(ret));
            }
        } while (rval > 0);
        close (msgsock);
    }
    close (sock);
    unlink (NAME);
}