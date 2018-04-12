/**
 * @file server.c
 * @author OSUE Team <osue-team@cps.tuwien.ac.at>
 * @date 2018-03-02
 *
 * @brief Server for OSUE exercise 1B `Battleship'.
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

// Static variables for things you might want to access from several functions:
static char *port = DEFAULT_PORT; // the port to bind to

// Static variables for resources that should be freed before exiting:
static struct addrinfo *ai = NULL;      // stores address information
static int sockfd = -1;                 // socket file descriptor
static int connfd = -1;                 // connection file descriptor

/*
 * TODO You should also have some kind of a list which saves information
 * about your ships. For this purpose you might want to define a struct. Bear
 * in mind that your server must keep record about which ships have been hit
 * (and also where they have been hit) in order to know when a ship was sunk.
 * 
 * You might also find it convenient to add a few functions, for instance: - a
 * function which cleans up all resources and exits the program - a function
 * which parses the arguments from the command line - a function which adds a
 * new ship to your list of ships - a function which checks whether a
 * client's shot hit any of your ships
 */

// Prototypes
static void usage(void);
static void invalidShip(char *givenShip);
static void cleanUp(void);
static void parseArgsAndCreateShips(int argc, char *argv[], const char *optstring);
static int getIntBetweenAandJ(char c);
static int digitCharToInt(char c);
static int prepareSocketAndGetConnfd(void);
static void sighandler(int signum);

#define OPT_STR "p:"
static char *prog_name;

int main(int argc, char *argv[])
{
	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

    parseArgsAndCreateShips(argc, argv, OPT_STR);
    uint8_t map[MAP_SIZE][MAP_SIZE];
    memset(&map, SQUARE_UNKNOWN, sizeof(map));
    print_map(map);

	connfd = prepareSocketAndGetConnfd();
	if(connfd < 0){
		return EXIT_FAILURE;
	}
	printf("GAME IS STARTING...\n");

    for (int roundNr = 1; roundNr <= MAX_ROUNDS; roundNr++)
    {
		
        /*
		 * TODO add code to: 
		 * - wait for a request from the client -
		 * check for a parity error or invalid coordinates in the
		 * request 
		 * - check whether a ship was hit and determine the
		 * status to return 
		 * - send an according response to the
		 * client
		 */
		//fixed width types: https://os.mbed.com/handbook/C-Data-Types
		



		break;
    }

	printf("Game LOST.\n");
    cleanUp();
    return EXIT_SUCCESS;
}

static void cleanUp(void)
{
	freeaddrinfo(ai);
	if(connfd >= 0 && close(connfd) < 0){ //Her bir connection icin olusturulan fd
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(sockfd >= 0 && close(sockfd) < 0){ //her connection icin OS'in sagladigi kaynak
		fprintf(stderr, "close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

    prog_name = NULL;
    port = NULL;
    //empty MAP;
}

static void sighandler(int signum) {
   printf("Caught signal %d!\n", signum);
   cleanUp();
   exit(EXIT_SUCCESS);
}

static void parseArgsAndCreateShips(int argc, char *argv[], const char *optstring)
{
    int localOpt_p = 0;
    prog_name = argv[0];
    if (argc == 1)
    {
        usage();
    }
    for (int argmntIndex = 1; argmntIndex < argc; argmntIndex++)
    {
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
            default:
                usage();
            }
        }

        if (optind == argmntIndex)
        {
            char *ship = argv[argmntIndex];
            printf("Ship: %s\n", ship);

            size_t arglen = strnlen(ship, 5);
            if (arglen != 4)
            {
                fprintf(stderr, "ERROR: wrong syntax for ship coordinates: %s\n", ship);
				cleanUp();
				exit(EXIT_FAILURE);
            }

            //Bow(front)
            int x1 = getIntBetweenAandJ(ship[0]);
			int y1 = digitCharToInt(ship[1]);
			//Stern(back)
            int x2 = getIntBetweenAandJ(ship[2]);
			int y2 = digitCharToInt(ship[3]);
			if( (x1<0) || (y1<0) || (x2<0) || (y2<0)  )
			{
				fprintf(stderr, "ERROR: coordinates outside of map: %s\n", ship);
				cleanUp();
				exit(EXIT_FAILURE);
			}
            optind++;
        }
    }
    printf("SERVER port: %s\n", port);
}

static void usage(void)
{
    fprintf(stderr, "Usage: %s [-p PORT] SHIP1 ...\n", prog_name);
    exit(EXIT_FAILURE);
}

static void invalidShip(char *givenShip)
{
    fprintf(stderr, "Invalid ship: %s\n", givenShip);
    usage();
}

//char A - MAP_SIZE arasindaysa sayisi, diger her sey icin -1
static int getIntBetweenAandJ(char c)
{
    int x = -1;
    for (x = 0; x < MAP_SIZE; x++)
    {
        if ((x + 'A') == c)
        {
            return x;
        }
    }
	return -1;
}

//char 0-MAP_SIZE arasindaysa sayisi, diger her sey icin -1
static int digitCharToInt(char c)
{
    int x = -1;
    for (x = 0; x < MAP_SIZE; x++)
    {
        if ((x + '0') == c)
        {
            return x;
        }
    }
    return -1;
}

struct shipdata
{
    int x;
    int y;
    int status;
};

struct shipsType2
{
    struct shipdata d[SHIP_CNT_LEN2];
};

struct shipsType3
{
    struct shipdata d[SHIP_CNT_LEN3];
};

struct shipsType4
{
    struct shipdata d[SHIP_CNT_LEN4];
};

static int prepareSocketAndGetConnfd(void) {

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, port, &hints, &ai);
	if(res != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
		return -1;
	}

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(sockfd < 0){
		fprintf(stderr, "socket: %s\n", strerror(errno));
		return -1;
	}

    int val = 1;
    res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val);
    if(res < 0){
		fprintf(stderr, "setsockopt: %s\n", strerror(errno));
		return -1;
	}

    res = bind(sockfd, ai->ai_addr, ai->ai_addrlen);
    if(res < 0){
		fprintf(stderr, "bind: %s\n", strerror(errno));
		return -1;
	}

    res = listen(sockfd, 1);
    if(res < 0){
		fprintf(stderr, "listen: %s\n", strerror(errno));
		return -1;
	}

	printf("SERVER IS WAITING FOR CLIENT.\n");
    int localConnfd = accept(sockfd, NULL, NULL);
    if(localConnfd < 0){
		fprintf(stderr, "accept: %s\n", strerror(errno));
		return -1;
	}
	return localConnfd;
}

/*
	 * Notes: - NULL for static pointers? -
	 * 
	 * Next: - Tüm assignment süper detayli okunacak. Sonra karar-> -
	 * initMap() Acaba struct ile map yapmak yerine map[][] mi yapmali?
	 * Cünkü printing function öyle calisiyor. Eger öyle ise "For
	 * this purpose you might want to define a struct." ne icin denmis? -
	 * Acaba gemileri mi diyor? => Her bir gemi tipi/HER GEMI icin bir
	 * struct yapip icine (tanimlandiysa) koordinatlarini atmaca mesela?
	 * - Eger map[][] yapilacaksa init de memset ile yapilabilir
	 */
