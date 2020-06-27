#ifndef SEND_H
#define SEND_H

void send_init(void);
void send_status(unsigned char whichArray);
void send_SOC(void);
void send_voltages(unsigned char whichArray);
void send_temps(unsigned char whichArray);
void send_balancing(void);
void send_next(void);
void send_start(void);
void send_bootstrap(unsigned char whichSet);

#endif
