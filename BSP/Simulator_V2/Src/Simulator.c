/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * This file creates an interface for the simulator to create a trace for running tests
 */

#include "Simulator.h"
#include <time.h> // for creating unique logfile names
// for open() syscall
#include <sys/types.h>  // for stat() and struct stat as well
#include <sys/stat.h>
#include <fcntl.h>
// for write() and read()
#include <unistd.h>
// for sprintf
#include <stdio.h>
// for signal handler
#include <signal.h>
// for boolean
#include <stdbool.h>
#include "cJSON.h" // for json parser

// file descriptor of simulator log file
int simulatorLog;

// simulator stat
// static struct simulator_state *states = NULL;
// static int numStates = 0;

// It knows what the input file is because the Makefile should make a #define for the file path called SIMULATOR_JSON_PATH
#ifndef SIMULATOR_JSON_PATH
#define JSON_PATH "Error: make sure you define the simulator JSON path in the Makefile!"
#else
// #define QUOTE_MARK "
#define STR(X) # X
#define STRINGIFY(X) STR(X)
#define JSON_PATH STRINGIFY(SIMULATOR_JSON_PATH) 
#endif

// read the input json file and parse it
static void readInputFile(void) {
    // get the length of the input file
    struct stat inFileStats;
    if (stat(JSON_PATH, &inFileStats) < 0) {
        printf("error getting stats for file %s\n", JSON_PATH);
        exit(-1);
    }
    off_t length = inFileStats.st_size;

    // read in the input file
    char *inFile = (char *) malloc(length);
    int fd = open(JSON_PATH, O_RDONLY);
    if (fd < 0) {
        printf("error opening file %s\n", JSON_PATH);
        exit(-1);
    }
    if (read(fd, inFile, length) < 0) {
        printf("error reading file %s", JSON_PATH);
        exit(-1);
    }

    // parses the input file as a json
    cJSON *json = cJSON_ParseWithLength(inFile, length);

    // parse the json structure into an array of states
    while(json);
}



// handler for Ctrl-C to safely shut down the simulator
void CtrlCHandler(int n) {
    write(simulatorLog, "simulator received SIGINT! Shutting down...\n", 44);
    close(simulatorLog);
    exit(-1);
}

// intialize the simulator
void Simulator_init(void) {
    // generate unique name for log file
    time_t startTime = time(NULL);
    char filename[30];
    sprintf(filename, "bps-sim-%ld.log", startTime);

    // create the log file
    simulatorLog = open(filename, O_CREAT | O_WRONLY, 0664);
    if (simulatorLog < 0) {
        printf("error opening file %s\n", JSON_PATH);
        exit(-1);
    }
    if (write(simulatorLog, "simulator started...\n", 21) < 0) {
        printf("error writing file %s\n", JSON_PATH);
        exit(-1);
    }

    // register the Ctrl-C handler
    sigset_t s;
    sigemptyset(&s);
    const struct sigaction act = {.sa_handler = CtrlCHandler, .sa_mask = s, .sa_flags = 0};
    sigaction(SIGINT, &act, NULL);

    // initialize the fake inputs
    readInputFile();
}

// log something to the simualtor's log file
void Simulator_log(char *str) {
    write(simulatorLog, str, strlen(str));
}
