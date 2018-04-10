#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// Assertions, errors, signals:
#include <assert.h>
#include <errno.h>
#include <signal.h>

// Time:
#include <time.h>

// Sockets, TCP, ... :
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

// stuff shared by client and server:
#include "common.h"

// Static variables for things you might want to access from several functions:
static const char *port = DEFAULT_PORT; // the port to bind to

// Static variables for resources that should be freed before exiting:
static struct addrinfo *ai = NULL;      // stores address information
static int sockfd = -1;                 // socket file descriptor
static int connfd = -1;                 // connection file descriptor

static char* serverName = NULL;
static const char* OPTSTRING = "p:";
static int gameRound = 0;
#define MAX_GAME_ROUND 80;

void parseArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    /* TODO
     * Add code to parse the command line arguments, maybe as a separate
     * function.
     * 
     * server [-p PORT] SHIP1 ...
     */
    parseArgs(argc, argv);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, port, &hints, &ai);
    /* TODO
     * check for errors
     */

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    /* TODO
     * check for errors
     */

    int val = 1;
    res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val);
    /* TODO
     * check for errors
     */

    res = bind(sockfd, ai->ai_addr, ai->ai_addrlen);
    /* TODO
     * check for errors
     */

    res = listen(sockfd, 1);
    /* TODO
     * check for errors
     */

    connfd = accept(sockfd, NULL, NULL);
    /* TODO
     * check for errors
     */

    /* TODO
     * Here you might want to add variables to keep track of the game status,
     * for instance the number of rounds that have been played.
     *

    while (...) {
        * TODO
         * add code to:
         *  - wait for a request from the client
         *  - check for a parity error or invalid coordinates in the request
         *  - check whether a ship was hit and determine the status to return
         *  - send an according response to the client
         */
        return EXIT_SUCCESS;
    }

    /* TODO
     * cleanup
     */

    void parseArgs(int argc, char *argv[])
    {
        int opt_p = 0;
        char *localPort = NULL;
        for (int i = 1; i < argc; i++)
        {
            int opt;
            while ((opt = getopt(argc, argv, OPTSTRING)) != -1)
            {
                switch (opt)
                {
                case 'p':
                    opt_p++;
                    if(opt_p > 1){
                        //usage();
                        return EXIT_FAILURE;
                    }
                    size_t s = strlen(argv[i]);
                    localPort = malloc(s * sizeof(char));
                    strncpy(localPort, argv[i], s);
                    break;
                default:
                    //usage();
                    return EXIT_FAILURE;
                }
            }

            if (opt == -1 && p[0] != '-')
            {

                printf("Arg%d: %s\n", i, argv[i]);
                if ((i + 1) < argc)
                {
                    optind = i + 1;
                }
            }
        }    
    }


}

