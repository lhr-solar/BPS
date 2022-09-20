/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * This file creates an interface for the simulator to create a trace for running tests
 */

#include <time.h> // for creating unique logfile names
// for open() syscall
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// for write()
#include <unistd.h>
// for sprintf
#include <stdio.h>
// for signal handler
#include <signal.h>
// for boolean
#include <stdbool.h>

// file descriptor of simulator log file
int simulatorLog;

// handler for Ctrl-C to safely shut down the simulator
void CtrlCHandler(int n) {
    write(simulatorLog, "simulator received SIGINT! Shutting down...\n", 44);
    close(simulatorLog);
}

// intialize the simulator
void Simulator_init(void) {


    // generate unique name for log file
    time_t startTime = time(NULL);
    char filename[30];
    sprintf(filename, "bps-sim-%d.log", startTime);

    // create the log file
    simulatorLog = open(filename, O_CREAT, O_RDWR);
    write(simulatorLog, "simulator started...\n", 21);

    // register the Ctrl-C handler
    sigset_t s;
    sigemptyset(&s);
    const struct sigaction act = {.sa_handler = CtrlCHandler, .sa_mask = s, .sa_flags = 0};
    sigaction(SIGINT, &act, NULL);
}

// log something to the simualtor's log file
void Simulator_log(char *str, int len) {
    write(simulatorLog, str, len);
}

