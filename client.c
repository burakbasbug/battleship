/**
 * @file client.c
 * @author Burak Basbug
 * @date 2018-04-04
 *
 * @brief Client for OSUE exercise 1B `Battleship'.
 */

// IO, C standard library, POSIX API, data types:
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

#define OPT_STR "p:h:"
static char *port = DEFAULT_PORT; // the port to bind to
static char *host = DEFAULT_HOST; // the host to bind to
static char* prog_name;
void usage(void);
void cleanUp(void);
void parseArgs(int argc, char* argv[], const char* optstring);

int main(int argc, char *argv[])
{
    parseArgs(argc, argv, OPT_STR);
    cleanUp();
    return EXIT_SUCCESS;
}

void cleanUp(void)
{
    prog_name = NULL;
    port = NULL;
    host = NULL;
}

void parseArgs(int argc, char* argv[], const char* optstring){
    int localOpt_p = 0, localOpt_h = 0;
    prog_name = argv[0];

    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        switch (opt)
        {
        case 'p':
            localOpt_p++;
            if(localOpt_p > 1){
                usage();
            }
            port = optarg;
            break;
        case 'h':
            localOpt_h++;
            if(localOpt_h > 1){
                usage();
            }
            host = optarg;
            break;
        default:
            usage();
        }
    }
    if((argc - optind) > 0)
    {
        usage();
    }
    printf("CLIENT hostname: %s, port: %s\n", host, port);
}

void usage(void)
{
    fprintf(stderr, "Usage: %s [-h HOSTNAME] [-p PORT]\n", prog_name);
    exit(EXIT_FAILURE);
}
