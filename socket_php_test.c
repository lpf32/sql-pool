/*
 * gcc server.c -o server
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFSIZE 1024
#define MSGSIZE 100
#define SOCK_PATH "/var/run/mysocket.sock"
#define PERM "0666"
#define WAIT 0
#define LISTEN_BACKLOG 50
#define C_MSG "Server got this message: "
#define handle_error(function) { perror(function); exit(EXIT_FAILURE); }

int main()
{
    int server_socket, accepted_socket, n;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_size;
    char recvd_msg[BUFSIZE], out_str[BUFSIZE];
    mode_t mode = strtol(PERM, 0, 8);

    /* Creating a Unix domain socket */
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket  == -1)
            handle_error("socket");

    /* Binding the socket to a file */
    memset(&my_addr, 0, sizeof(struct sockaddr_un)); /* Clear structure */
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, SOCK_PATH, sizeof(my_addr.sun_path) - 1);
    unlink(my_addr.sun_path);
    if (bind(server_socket, (struct sockaddr *) &my_addr, 
                    sizeof(struct sockaddr_un)) == -1)
            handle_error("bind");

    /* Instructing the socket to listen for incoming connections */
    if (listen(server_socket, LISTEN_BACKLOG) == -1) 
            handle_error("listen");

    /* Changing permission of socket the file */
    if (chmod(SOCK_PATH, mode) == -1)
            handle_error("chmod");

    for(;;) 
    {
            /* Accepting incoming connections one at a time */
            printf("Waiting for a connection...\n");
            peer_addr_size = sizeof(struct sockaddr_un);
            accepted_socket = accept(server_socket, 
                    (struct sockaddr *) &peer_addr, &peer_addr_size);
            if (accepted_socket == -1)
                    handle_error("accept");

            printf("Connected.\n");

            /* Receiving data */
            n = recv(accepted_socket, recvd_msg, MSGSIZE, 0);
            if (n < 0) 
                    handle_error("recv");

            /* Printing received data */
            snprintf(out_str, strlen(C_MSG)+1, "%s", C_MSG);
            strncat(out_str,recvd_msg, n);
            printf("%s\n",out_str);
            fflush (stdout);

            /* Waiting just for fun */
            sleep(WAIT);

            /* Sending data */
            if (send(accepted_socket, out_str, strlen(out_str), 0) < 0)
                    handle_error("recv");

            /* Cleaning up */
            close(accepted_socket);
    }

    return 0;
}
