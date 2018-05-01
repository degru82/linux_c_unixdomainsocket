#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NAME "/tmp/test"

int main (int argc, char *argv[]) {
    int sock;
    struct sockaddr_un server;
    char buf[1024] = {0};
    char res[1024] = {0};

    if (argc != 3) {
        printf ("usage:%s <integer 1> <integer 2>\n", argv[0]);
        return 1;
    }
    sprintf (buf, "%s %s", argv[1], argv[2]);

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror ("opening stream socket");
        return 1;
    }

    server.sun_family = AF_UNIX;
    strcpy (server.sun_path, NAME);

    if (connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0) {
        close (sock);
        perror ("connecting stream socket");
        return 1;
    }
    if (write (sock, buf, sizeof(buf)) < 0) {
        perror ("writing on stream socket");
    }

    read (sock, res, 1024);
    printf ("%s + %s = %s", argv[1], argv[2], res);

    close (sock);
}