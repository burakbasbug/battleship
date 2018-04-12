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

static struct addrinfo *ai = NULL; // stores address information
static int sockfd = -1;            // connection file descriptor

static char *port = DEFAULT_PORT; // the port to bind to
static char *host = DEFAULT_HOST; // the host to bind to
static char *prog_name;
#define OPT_STR "p:h:"

static void usage(void);
static void cleanUp(void);
static void parseArgs(int argc, char *argv[], const char *optstring);
static uint16_t makeGuess(void);
static void sendGuess(uint16_t guess);

int main(int argc, char *argv[])
{
    parseArgs(argc, argv, OPT_STR);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int res = getaddrinfo(host, port, &hints, &ai);
    if (res != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
        return EXIT_FAILURE;
    }

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    printf("CONNECTED TO SERVER!\n");
    if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) < 0)
    {
        fprintf(stderr, "connect: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    while (1)
    {

        //buf[pos] = msg >> (8 * 2);
        //send(sockfd, buf, sizeof(uint16_t), 0);
        sendGuess(makeGuess());
        

        break;
    }

    cleanUp();
    return EXIT_SUCCESS;
}

static void sendGuess(uint16_t guess){
    //uint16_t buf[1]; //1 tane 8bitlik sayi alan buffer (16 bit 2ye bölünecek)
    // Guess su an 16 bit, 8 8 ikiye ayirip önce ilk kismi sonra ikinci kismi.
    uint8_t first8bits = 0x0 | guess; // 0000 0000
    //buf[0] = first8bits;
    
    int i = first8bits;
    uint8_t buf[sizeof(int)];
    int pos;
    for (pos = 0; pos < sizeof(int); pos++)
        buf[pos] = i >> 8 * pos;
    write(sockfd, buf, sizeof(int));
    
    
    // paritybit 0 0 0 x(6 bits) y(6 bits)
    uint16_t parityBit = guess & 1; //reading the first bit.
    int counterFor1s = parityBit;
    for(int i = 1; i<16; i++){
        counterFor1s += ( (guess >> i) & 1);
        parityBit = parityBit ^ ( (guess >> i) & 1);
    }
    //printf("Parity Bit will be: %s, because number of 1s in %d is %d.\n", parityBit%2 == 0 ? "0" : "1", guess, counterFor1s);
    uint8_t second8bitsWithParity = 0xFF & (guess >> 8);
    printf("second part: %d\n", second8bitsWithParity);
    i = second8bitsWithParity;
    for (pos = 0; pos < sizeof(int); pos++)
        buf[pos] = i >> 8 * pos;
    write(sockfd, buf, sizeof(int));
}

static uint16_t makeGuess(void)
{
    //empty bits
    uint16_t guess = 0x0;
    uint16_t x = 4; // B

    //first 6 bits empty, next 6 bits x
    guess = guess | (x << 6);
    uint16_t y = 1;
    guess = guess | y;
    return guess;
}

static void cleanUp(void)
{
    prog_name = NULL;
    port = NULL;
    host = NULL;

    freeaddrinfo(ai);
    if (sockfd >= 0 && close(sockfd) < 0)
    { //her connection icin OS'in sagladigi kaynak
        fprintf(stderr, "close failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static void parseArgs(int argc, char *argv[], const char *optstring)
{
    int localOpt_p = 0, localOpt_h = 0;
    prog_name = argv[0];

    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        switch (opt)
        {
        case 'p':
            localOpt_p++;
            if (localOpt_p > 1)
            {
                usage();
            }
            port = optarg;
            break;
        case 'h':
            localOpt_h++;
            if (localOpt_h > 1)
            {
                usage();
            }
            host = optarg;
            break;
        default:
            usage();
        }
    }
    if ((argc - optind) > 0)
    {
        usage();
    }
    printf("hostname: %s, port: %s\n", host, port);
}

static void usage(void)
{
    fprintf(stderr, "Usage: %s [-h HOSTNAME] [-p PORT]\n", prog_name);
    exit(EXIT_FAILURE);
}
