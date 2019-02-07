/*-------------------------------------------------------------------------
 * MPLAB-Cxx  dsPIC30F6012A processor header
 *
 * (c) Copyright 1999-2011 Microchip Technology, All rights reserved
 *-------------------------------------------------------------------------*/

#ifndef __dsPIC30F6012A__
#error "Include file does not match processor setting"
#endif

#ifndef __30F6012A_H
#define __30F6012A_H

extern volatile unsigned int  WREG0 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG1 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG2 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG3 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG4 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG5 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG6 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG7 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG8 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG9 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG10 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG11 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG12 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG13 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG14 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  WREG15 __attribute__((__sfr__,__deprecated__,__unsafe__));
extern volatile unsigned int  SPLIM __attribute__((__sfr__));
typedef struct tagACC {
    unsigned int L;
    unsigned int H;
    unsigned char U;
} ACC;

/* SFR blocks for each ACC module */
extern volatile ACC ACCA __attribute__((__sfr__));

extern volatile unsigned int  ACCAL __attribute__((__sfr__));
extern volatile unsigned int  ACCAH __attribute__((__sfr__));
extern volatile unsigned char ACCAU __attribute__((__sfr__));

/* SFR blocks for each ACC module */
extern volatile ACC ACCB __attribute__((__sfr__));

extern volatile unsigned int  ACCBL __attribute__((__sfr__));
extern volatile unsigned int  ACCBH __attribute__((__sfr__));
extern volatile unsigned char ACCBU __attribute__((__sfr__));
extern volatile unsigned int  PCL __attribute__((__sfr__));
extern volatile unsigned char PCH __attribute__((__sfr__));
extern volatile unsigned char TBLPAG __attribute__((__sfr__));
extern volatile unsigned char PSVPAG __attribute__((__sfr__));
extern volatile unsigned int  RCOUNT __attribute__((__sfr__));
extern volatile unsigned int  DCOUNT __attribute__((__sfr__));
extern volatile unsigned int  DOSTARTL __attribute__((__sfr__));
extern volatile unsigned int  DOSTARTH __attribute__((__sfr__));
extern volatile unsigned int  DOENDL __attribute__((__sfr__));
extern volatile unsigned int  DOENDH __attribute__((__sfr__));
extern volatile unsigned int  SR __attribute__((__sfr__));
__extension__ typedef struct tagSRBITS {
  union {
    struct {
      unsigned C:1;
      unsigned Z:1;
      unsigned OV:1;
      unsigned N:1;
      unsigned RA:1;
      unsigned IPL:3;
      unsigned DC:1;
      unsigned DA:1;
      unsigned SAB:1;
      unsigned OAB:1;
      unsigned SB:1;
      unsigned SA:1;
      unsigned OB:1;
      unsigned OA:1;
    };
    struct {
      unsigned :5;
      unsigned IPL0:1;
      unsigned IPL1:1;
      unsigned IPL2:1;
    };
  };
} SRBITS;
extern volatile SRBITS SRbits __attribute__((__sfr__));

extern volatile unsigned int  CORCON __attribute__((__sfr__));
__extension__ typedef struct tagCORCONBITS {
  union {
    struct {
      unsigned IF:1;
      unsigned RND:1;
      unsigned PSV:1;
      unsigned IPL3:1;
      unsigned ACCSAT:1;
      unsigned SATDW:1;
      unsigned SATB:1;
      unsigned SATA:1;
      unsigned DL:3;
      unsigned EDT:1;
      unsigned US:1;
    };
    struct {
      unsigned :8;
      unsigned DL0:1;
      unsigned DL1:1;
      unsigned DL2:1;
    };
  };
} CORCONBITS;
extern volatile CORCONBITS CORCONbits __attribute__((__sfr__));

extern volatile unsigned int  MODCON __attribute__((__sfr__));
__extension__ typedef struct tagMODCONBITS {
  union {
    struct {
      unsigned XWM:4;
      unsigned YWM:4;
      unsigned BWM:4;
      unsigned :2;
      unsigned YMODEN:1;
      unsigned XMODEN:1;
    };
    struct {
      unsigned XWM0:1;
      unsigned XWM1:1;
      unsigned XWM2:1;
      unsigned XWM3:1;
      unsigned YWM0:1;
      unsigned YWM1:1;
      unsigned YWM2:1;
      unsigned YWM3:1;
      unsigned BWM0:1;
      unsigned BWM1:1;
      unsigned BWM2:1;
      unsigned BWM3:1;
    };
  };
} MODCONBITS;
extern volatile MODCONBITS MODCONbits __attribute__((__sfr__));

extern volatile unsigned int  XMODSRT __attribute__((__sfr__));
extern volatile unsigned int  XMODEND __attribute__((__sfr__));
extern volatile unsigned int  YMODSRT __attribute__((__sfr__));
extern volatile unsigned int  YMODEND __attribute__((__sfr__));
extern volatile unsigned int  XBREV __attribute__((__sfr__));
__extension__ typedef struct tagXBREVBITS {
  union {
    struct {
      unsigned XB:15;
      unsigned BREN:1;
    };
    struct {
      unsigned XB0:1;
      unsigned XB1:1;
      unsigned XB2:1;
      unsigned XB3:1;
      unsigned XB4:1;
      unsigned XB5:1;
      unsigned XB6:1;
      unsigned XB7:1;
      unsigned XB8:1;
      unsigned XB9:1;
      unsigned XB10:1;
      unsigned XB11:1;
      unsigned XB12:1;
      unsigned XB13:1;
      unsigned XB14:1;
    };
  };
} XBREVBITS;
extern volatile XBREVBITS XBREVbits __attribute__((__sfr__));

extern volatile unsigned int  DISICNT __attribute__((__sfr__));
typedef struct tagDISICNTBITS {
  unsigned DISICNT:14;
} DISICNTBITS;
extern volatile DISICNTBITS DISICNTbits __attribute__((__sfr__));

extern volatile unsigned int  INTCON1 __attribute__((__sfr__));
typedef struct tagINTCON1BITS {
  unsigned :1;
  unsigned OSCFAIL:1;
  unsigned STKERR:1;
  unsigned ADDRERR:1;
  unsigned MATHERR:1;
  unsigned :3;
  unsigned COVTE:1;
  unsigned OVBTE:1;
  unsigned OVATE:1;
  unsigned :4;
  unsigned NSTDIS:1;
} INTCON1BITS;
extern volatile INTCON1BITS INTCON1bits __attribute__((__sfr__));

extern volatile unsigned int  INTCON2 __attribute__((__sfr__));
typedef struct tagINTCON2BITS {
  unsigned INT0EP:1;
  unsigned INT1EP:1;
  unsigned INT2EP:1;
  unsigned INT3EP:1;
  unsigned INT4EP:1;
  unsigned :9;
  unsigned DISI:1;
  unsigned ALTIVT:1;
} INTCON2BITS;
extern volatile INTCON2BITS INTCON2bits __attribute__((__sfr__));

extern volatile unsigned int  IFS0 __attribute__((__sfr__));
typedef struct tagIFS0BITS {
  unsigned INT0IF:1;
  unsigned IC1IF:1;
  unsigned OC1IF:1;
  unsigned T1IF:1;
  unsigned IC2IF:1;
  unsigned OC2IF:1;
  unsigned T2IF:1;
  unsigned T3IF:1;
  unsigned SPI1IF:1;
  unsigned U1RXIF:1;
  unsigned U1TXIF:1;
  unsigned ADIF:1;
  unsigned NVMIF:1;
  unsigned SI2CIF:1;
  unsigned MI2CIF:1;
  unsigned CNIF:1;
} IFS0BITS;
extern volatile IFS0BITS IFS0bits __attribute__((__sfr__));

extern volatile unsigned int  IFS1 __attribute__((__sfr__));
typedef struct tagIFS1BITS {
  unsigned INT1IF:1;
  unsigned IC7IF:1;
  unsigned IC8IF:1;
  unsigned OC3IF:1;
  unsigned OC4IF:1;
  unsigned T4IF:1;
  unsigned T5IF:1;
  unsigned INT2IF:1;
  unsigned U2RXIF:1;
  unsigned U2TXIF:1;
  unsigned SPI2IF:1;
  unsigned C1IF:1;
  unsigned IC3IF:1;
  unsigned IC4IF:1;
  unsigned IC5IF:1;
  unsigned IC6IF:1;
} IFS1BITS;
extern volatile IFS1BITS IFS1bits __attribute__((__sfr__));

extern volatile unsigned int  IFS2 __attribute__((__sfr__));
typedef struct tagIFS2BITS {
  unsigned OC5IF:1;
  unsigned OC6IF:1;
  unsigned OC7IF:1;
  unsigned OC8IF:1;
  unsigned INT3IF:1;
  unsigned INT4IF:1;
  unsigned C2IF:1;
  unsigned :2;
  unsigned DCIIF:1;
  unsigned LVDIF:1;
} IFS2BITS;
extern volatile IFS2BITS IFS2bits __attribute__((__sfr__));

extern volatile unsigned int  IEC0 __attribute__((__sfr__));
typedef struct tagIEC0BITS {
  unsigned INT0IE:1;
  unsigned IC1IE:1;
  unsigned OC1IE:1;
  unsigned T1IE:1;
  unsigned IC2IE:1;
  unsigned OC2IE:1;
  unsigned T2IE:1;
  unsigned T3IE:1;
  unsigned SPI1IE:1;
  unsigned U1RXIE:1;
  unsigned U1TXIE:1;
  unsigned ADIE:1;
  unsigned NVMIE:1;
  unsigned SI2CIE:1;
  unsigned MI2CIE:1;
  unsigned CNIE:1;
} IEC0BITS;
extern volatile IEC0BITS IEC0bits __attribute__((__sfr__));

extern volatile unsigned int  IEC1 __attribute__((__sfr__));
typedef struct tagIEC1BITS {
  unsigned INT1IE:1;
  unsigned IC7IE:1;
  unsigned IC8IE:1;
  unsigned OC3IE:1;
  unsigned OC4IE:1;
  unsigned T4IE:1;
  unsigned T5IE:1;
  unsigned INT2IE:1;
  unsigned U2RXIE:1;
  unsigned U2TXIE:1;
  unsigned SPI2IE:1;
  unsigned C1IE:1;
  unsigned IC3IE:1;
  unsigned IC4IE:1;
  unsigned IC5IE:1;
  unsigned IC6IE:1;
} IEC1BITS;
extern volatile IEC1BITS IEC1bits __attribute__((__sfr__));

extern volatile unsigned int  IEC2 __attribute__((__sfr__));
typedef struct tagIEC2BITS {
  unsigned OC5IE:1;
  unsigned OC6IE:1;
  unsigned OC7IE:1;
  unsigned OC8IE:1;
  unsigned INT3IE:1;
  unsigned INT4IE:1;
  unsigned C2IE:1;
  unsigned :2;
  unsigned DCIIE:1;
  unsigned LVDIE:1;
} IEC2BITS;
extern volatile IEC2BITS IEC2bits __attribute__((__sfr__));

extern volatile unsigned int  IPC0 __attribute__((__sfr__));
__extension__ typedef struct tagIPC0BITS {
  union {
    struct {
      unsigned INT0IP:3;
      unsigned :1;
      unsigned IC1IP:3;
      unsigned :1;
      unsigned OC1IP:3;
      unsigned :1;
      unsigned T1IP:3;
    };
    struct {
      unsigned INT0IP0:1;
      unsigned INT0IP1:1;
      unsigned INT0IP2:1;
      unsigned :1;
      unsigned IC1IP0:1;
      unsigned IC1IP1:1;
      unsigned IC1IP2:1;
      unsigned :1;
      unsigned OC1IP0:1;
      unsigned OC1IP1:1;
      unsigned OC1IP2:1;
      unsigned :1;
      unsigned T1IP0:1;
      unsigned T1IP1:1;
      unsigned T1IP2:1;
    };
  };
} IPC0BITS;
extern volatile IPC0BITS IPC0bits __attribute__((__sfr__));

extern volatile unsigned int  IPC1 __attribute__((__sfr__));
__extension__ typedef struct tagIPC1BITS {
  union {
    struct {
      unsigned IC2IP:3;
      unsigned :1;
      unsigned OC2IP:3;
      unsigned :1;
      unsigned T2IP:3;
      unsigned :1;
      unsigned T3IP:3;
    };
    struct {
      unsigned IC2IP0:1;
      unsigned IC2IP1:1;
      unsigned IC2IP2:1;
      unsigned :1;
      unsigned OC2IP0:1;
      unsigned OC2IP1:1;
      unsigned OC2IP2:1;
      unsigned :1;
      unsigned T2IP0:1;
      unsigned T2IP1:1;
      unsigned T2IP2:1;
      unsigned :1;
      unsigned T3IP0:1;
      unsigned T3IP1:1;
      unsigned T3IP2:1;
    };
  };
} IPC1BITS;
extern volatile IPC1BITS IPC1bits __attribute__((__sfr__));

extern volatile unsigned int  IPC2 __attribute__((__sfr__));
__extension__ typedef struct tagIPC2BITS {
  union {
    struct {
      unsigned SPI1IP:3;
      unsigned :1;
      unsigned U1RXIP:3;
      unsigned :1;
      unsigned U1TXIP:3;
      unsigned :1;
      unsigned ADIP:3;
    };
    struct {
      unsigned SPI1IP0:1;
      unsigned SPI1IP1:1;
      unsigned SPI1IP2:1;
      unsigned :1;
      unsigned U1RXIP0:1;
      unsigned U1RXIP1:1;
      unsigned U1RXIP2:1;
      unsigned :1;
      unsigned U1TXIP0:1;
      unsigned U1TXIP1:1;
      unsigned U1TXIP2:1;
      unsigned :1;
      unsigned ADIP0:1;
      unsigned ADIP1:1;
      unsigned ADIP2:1;
    };
  };
} IPC2BITS;
extern volatile IPC2BITS IPC2bits __attribute__((__sfr__));

extern volatile unsigned int  IPC3 __attribute__((__sfr__));
__extension__ typedef struct tagIPC3BITS {
  union {
    struct {
      unsigned NVMIP:3;
      unsigned :1;
      unsigned SI2CIP:3;
      unsigned :1;
      unsigned MI2CIP:3;
      unsigned :1;
      unsigned CNIP:3;
    };
    struct {
      unsigned NVMIP0:1;
      unsigned NVMIP1:1;
      unsigned NVMIP2:1;
      unsigned :1;
      unsigned SI2CIP0:1;
      unsigned SI2CIP1:1;
      unsigned SI2CIP2:1;
      unsigned :1;
      unsigned MI2CIP0:1;
      unsigned MI2CIP1:1;
      unsigned MI2CIP2:1;
      unsigned :1;
      unsigned CNIP0:1;
      unsigned CNIP1:1;
      unsigned CNIP2:1;
    };
  };
} IPC3BITS;
extern volatile IPC3BITS IPC3bits __attribute__((__sfr__));

extern volatile unsigned int  IPC4 __attribute__((__sfr__));
__extension__ typedef struct tagIPC4BITS {
  union {
    struct {
      unsigned INT1IP:3;
      unsigned :1;
      unsigned IC7IP:3;
      unsigned :1;
      unsigned IC8IP:3;
      unsigned :1;
      unsigned OC3IP:3;
    };
    struct {
      unsigned INT1IP0:1;
      unsigned INT1IP1:1;
      unsigned INT1IP2:1;
      unsigned :1;
      unsigned IC7IP0:1;
      unsigned IC7IP1:1;
      unsigned IC7IP2:1;
      unsigned :1;
      unsigned IC8IP0:1;
      unsigned IC8IP1:1;
      unsigned IC8IP2:1;
      unsigned :1;
      unsigned OC3IP0:1;
      unsigned OC3IP1:1;
      unsigned OC3IP2:1;
    };
  };
} IPC4BITS;
extern volatile IPC4BITS IPC4bits __attribute__((__sfr__));

extern volatile unsigned int  IPC5 __attribute__((__sfr__));
__extension__ typedef struct tagIPC5BITS {
  union {
    struct {
      unsigned OC4IP:3;
      unsigned :1;
      unsigned T4IP:3;
      unsigned :1;
      unsigned T5IP:3;
      unsigned :1;
      unsigned INT2IP:3;
    };
    struct {
      unsigned OC4IP0:1;
      unsigned OC4IP1:1;
      unsigned OC4IP2:1;
      unsigned :1;
      unsigned T4IP0:1;
      unsigned T4IP1:1;
      unsigned T4IP2:1;
      unsigned :1;
      unsigned T5IP0:1;
      unsigned T5IP1:1;
      unsigned T5IP2:1;
      unsigned :1;
      unsigned INT2IP0:1;
      unsigned INT2IP1:1;
      unsigned INT2IP2:1;
    };
  };
} IPC5BITS;
extern volatile IPC5BITS IPC5bits __attribute__((__sfr__));

extern volatile unsigned int  IPC6 __attribute__((__sfr__));
__extension__ typedef struct tagIPC6BITS {
  union {
    struct {
      unsigned U2RXIP:3;
      unsigned :1;
      unsigned U2TXIP:3;
      unsigned :1;
      unsigned SPI2IP:3;
      unsigned :1;
      unsigned C1IP:3;
    };
    struct {
      unsigned U2RXIP0:1;
      unsigned U2RXIP1:1;
      unsigned U2RXIP2:1;
      unsigned :1;
      unsigned U2TXIP0:1;
      unsigned U2TXIP1:1;
      unsigned U2TXIP2:1;
      unsigned :1;
      unsigned SPI2IP0:1;
      unsigned SPI2IP1:1;
      unsigned SPI2IP2:1;
      unsigned :1;
      unsigned C1IP0:1;
      unsigned C1IP1:1;
      unsigned C1IP2:1;
    };
  };
} IPC6BITS;
extern volatile IPC6BITS IPC6bits __attribute__((__sfr__));

extern volatile unsigned int  IPC7 __attribute__((__sfr__));
__extension__ typedef struct tagIPC7BITS {
  union {
    struct {
      unsigned IC3IP:3;
      unsigned :1;
      unsigned IC4IP:3;
      unsigned :1;
      unsigned IC5IP:3;
      unsigned :1;
      unsigned IC6IP:3;
    };
    struct {
      unsigned IC3IP0:1;
      unsigned IC3IP1:1;
      unsigned IC3IP2:1;
      unsigned :1;
      unsigned IC4IP0:1;
      unsigned IC4IP1:1;
      unsigned IC4IP2:1;
      unsigned :1;
      unsigned IC5IP0:1;
      unsigned IC5IP1:1;
      unsigned IC5IP2:1;
      unsigned :1;
      unsigned IC6IP0:1;
      unsigned IC6IP1:1;
      unsigned IC6IP2:1;
    };
  };
} IPC7BITS;
extern volatile IPC7BITS IPC7bits __attribute__((__sfr__));

extern volatile unsigned int  IPC8 __attribute__((__sfr__));
__extension__ typedef struct tagIPC8BITS {
  union {
    struct {
      unsigned OC5IP:3;
      unsigned :1;
      unsigned OC6IP:3;
      unsigned :1;
      unsigned OC7IP:3;
      unsigned :1;
      unsigned OC8IP:3;
    };
    struct {
      unsigned OC5IP0:1;
      unsigned OC5IP1:1;
      unsigned OC5IP2:1;
      unsigned :1;
      unsigned OC6IP0:1;
      unsigned OC6IP1:1;
      unsigned OC6IP2:1;
      unsigned :1;
      unsigned OC7IP0:1;
      unsigned OC7IP1:1;
      unsigned OC7IP2:1;
      unsigned :1;
      unsigned OC8IP0:1;
      unsigned OC8IP1:1;
      unsigned OC8IP2:1;
    };
  };
} IPC8BITS;
extern volatile IPC8BITS IPC8bits __attribute__((__sfr__));

extern volatile unsigned int  IPC9 __attribute__((__sfr__));
__extension__ typedef struct tagIPC9BITS {
  union {
    struct {
      unsigned INT3IP:3;
      unsigned :1;
      unsigned INT4IP:3;
      unsigned :1;
      unsigned C2IP:3;
    };
    struct {
      unsigned INT3IP0:1;
      unsigned INT3IP1:1;
      unsigned INT3IP2:1;
      unsigned :1;
      unsigned INT4IP0:1;
      unsigned INT4IP1:1;
      unsigned INT4IP2:1;
      unsigned :1;
      unsigned C2IP0:1;
      unsigned C2IP1:1;
      unsigned C2IP2:1;
    };
  };
} IPC9BITS;
extern volatile IPC9BITS IPC9bits __attribute__((__sfr__));

extern volatile unsigned int  IPC10 __attribute__((__sfr__));
__extension__ typedef struct tagIPC10BITS {
  union {
    struct {
      unsigned :4;
      unsigned DCIIP:3;
      unsigned :1;
      unsigned LVDIP:3;
    };
    struct {
      unsigned :4;
      unsigned DCIIP0:1;
      unsigned DCIIP1:1;
      unsigned DCIIP2:1;
      unsigned :1;
      unsigned LVDIP0:1;
      unsigned LVDIP1:1;
      unsigned LVDIP2:1;
    };
  };
} IPC10BITS;
extern volatile IPC10BITS IPC10bits __attribute__((__sfr__));

extern volatile unsigned int  INTTREG __attribute__((__sfr__));
__extension__ typedef struct tagINTTREGBITS {
  union {
    struct {
      unsigned VECNUM:6;
      unsigned :2;
      unsigned ILR:4;
      unsigned :1;
      unsigned VHOLD:1;
      unsigned TMODE:1;
      unsigned IRQTOCPU:1;
    };
    struct {
      unsigned VECNUM0:1;
      unsigned VECNUM1:1;
      unsigned VECNUM2:1;
      unsigned VECNUM3:1;
      unsigned VECNUM4:1;
      unsigned VECNUM5:1;
      unsigned :2;
      unsigned ILR0:1;
      unsigned ILR1:1;
      unsigned ILR2:1;
      unsigned ILR3:1;
    };
  };
} INTTREGBITS;
extern volatile INTTREGBITS INTTREGbits __attribute__((__sfr__));

extern volatile unsigned int  CNEN1 __attribute__((__sfr__));
typedef struct tagCNEN1BITS {
  unsigned CN0IE:1;
  unsigned CN1IE:1;
  unsigned CN2IE:1;
  unsigned CN3IE:1;
  unsigned CN4IE:1;
  unsigned CN5IE:1;
  unsigned CN6IE:1;
  unsigned CN7IE:1;
  unsigned CN8IE:1;
  unsigned CN9IE:1;
  unsigned CN10IE:1;
  unsigned CN11IE:1;
  unsigned CN12IE:1;
  unsigned CN13IE:1;
  unsigned CN14IE:1;
  unsigned CN15IE:1;
} CNEN1BITS;
extern volatile CNEN1BITS CNEN1bits __attribute__((__sfr__));

extern volatile unsigned int  CNEN2 __attribute__((__sfr__));
typedef struct tagCNEN2BITS {
  unsigned CN16IE:1;
  unsigned CN17IE:1;
  unsigned CN18IE:1;
} CNEN2BITS;
extern volatile CNEN2BITS CNEN2bits __attribute__((__sfr__));

extern volatile unsigned int  CNPU1 __attribute__((__sfr__));
typedef struct tagCNPU1BITS {
  unsigned CN0PUE:1;
  unsigned CN1PUE:1;
  unsigned CN2PUE:1;
  unsigned CN3PUE:1;
  unsigned CN4PUE:1;
  unsigned CN5PUE:1;
  unsigned CN6PUE:1;
  unsigned CN7PUE:1;
  unsigned CN8PUE:1;
  unsigned CN9PUE:1;
  unsigned CN10PUE:1;
  unsigned CN11PUE:1;
  unsigned CN12PUE:1;
  unsigned CN13PUE:1;
  unsigned CN14PUE:1;
  unsigned CN15PUE:1;
} CNPU1BITS;
extern volatile CNPU1BITS CNPU1bits __attribute__((__sfr__));

extern volatile unsigned int  CNPU2 __attribute__((__sfr__));
typedef struct tagCNPU2BITS {
  unsigned CN16PUE:1;
  unsigned CN17PUE:1;
  unsigned CN18PUE:1;
} CNPU2BITS;
extern volatile CNPU2BITS CNPU2bits __attribute__((__sfr__));

extern volatile unsigned int  TMR1 __attribute__((__sfr__));
extern volatile unsigned int  PR1 __attribute__((__sfr__));
extern volatile unsigned int  T1CON __attribute__((__sfr__));
__extension__ typedef struct tagT1CONBITS {
  union {
    struct {
      unsigned :1;
      unsigned TCS:1;
      unsigned TSYNC:1;
      unsigned :1;
      unsigned TCKPS:2;
      unsigned TGATE:1;
      unsigned :6;
      unsigned TSIDL:1;
      unsigned :1;
      unsigned TON:1;
    };
    struct {
      unsigned :4;
      unsigned TCKPS0:1;
      unsigned TCKPS1:1;
    };
  };
} T1CONBITS;
extern volatile T1CONBITS T1CONbits __attribute__((__sfr__));

extern volatile unsigned int  TMR2 __attribute__((__sfr__));
extern volatile unsigned int  TMR3HLD __attribute__((__sfr__));
extern volatile unsigned int  TMR3 __attribute__((__sfr__));
extern volatile unsigned int  PR2 __attribute__((__sfr__));
extern volatile unsigned int  PR3 __attribute__((__sfr__));
extern volatile unsigned int  T2CON __attribute__((__sfr__));
__extension__ typedef struct tagT2CONBITS {
  union {
    struct {
      unsigned :1;
      unsigned TCS:1;
      unsigned :1;
      unsigned T32:1;
      unsigned TCKPS:2;
      unsigned TGATE:1;
      unsigned :6;
      unsigned TSIDL:1;
      unsigned :1;
      unsigned TON:1;
    };
    struct {
      unsigned :4;
      unsigned TCKPS0:1;
      unsigned TCKPS1:1;
    };
  };
} T2CONBITS;
extern volatile T2CONBITS T2CONbits __attribute__((__sfr__));

extern volatile unsigned int  T3CON __attribute__((__sfr__));
__extension__ typedef struct tagT3CONBITS {
  union {
    struct {
      unsigned :1;
      unsigned TCS:1;
      unsigned :2;
      unsigned TCKPS:2;
      unsigned TGATE:1;
      unsigned :6;
      unsigned TSIDL:1;
      unsigned :1;
      unsigned TON:1;
    };
    struct {
      unsigned :4;
      unsigned TCKPS0:1;
      unsigned TCKPS1:1;
    };
  };
} T3CONBITS;
extern volatile T3CONBITS T3CONbits __attribute__((__sfr__));

extern volatile unsigned int  TMR4 __attribute__((__sfr__));
extern volatile unsigned int  TMR5HLD __attribute__((__sfr__));
extern volatile unsigned int  TMR5 __attribute__((__sfr__));
extern volatile unsigned int  PR4 __attribute__((__sfr__));
extern volatile unsigned int  PR5 __attribute__((__sfr__));
extern volatile unsigned int  T4CON __attribute__((__sfr__));
__extension__ typedef struct tagT4CONBITS {
  union {
    struct {
      unsigned :1;
      unsigned TCS:1;
      unsigned :1;
      unsigned T32:1;
      unsigned TCKPS:2;
      unsigned TGATE:1;
      unsigned :6;
      unsigned TSIDL:1;
      unsigned :1;
      unsigned TON:1;
    };
    struct {
      unsigned :4;
      unsigned TCKPS0:1;
      unsigned TCKPS1:1;
    };
  };
} T4CONBITS;
extern volatile T4CONBITS T4CONbits __attribute__((__sfr__));

extern volatile unsigned int  T5CON __attribute__((__sfr__));
__extension__ typedef struct tagT5CONBITS {
  union {
    struct {
      unsigned :1;
      unsigned TCS:1;
      unsigned :2;
      unsigned TCKPS:2;
      unsigned TGATE:1;
      unsigned :6;
      unsigned TSIDL:1;
      unsigned :1;
      unsigned TON:1;
    };
    struct {
      unsigned :4;
      unsigned TCKPS0:1;
      unsigned TCKPS1:1;
    };
  };
} T5CONBITS;
extern volatile T5CONBITS T5CONbits __attribute__((__sfr__));


/* Generic structure of entire SFR area for Input Capture modules */
typedef struct tagIC {
        unsigned int icxbuf;
        unsigned int icxcon;
} IC, *PIC;


/* SFR blocks for Input Capture modules */
extern volatile IC IC1 __attribute__((__sfr__));

extern volatile unsigned int  IC1BUF __attribute__((__sfr__));
extern volatile unsigned int  IC1CON __attribute__((__sfr__));
typedef struct tagIC1CONBITS {
  unsigned ICM:3;
  unsigned ICBNE:1;
  unsigned ICOV:1;
  unsigned ICI:2;
  unsigned ICTMR:1;
  unsigned :5;
  unsigned ICSIDL:1;
} IC1CONBITS;
extern volatile IC1CONBITS IC1CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC2 __attribute__((__sfr__));

extern volatile unsigned int  IC2BUF __attribute__((__sfr__));
extern volatile unsigned int  IC2CON __attribute__((__sfr__));
__extension__ typedef struct tagIC2CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC2CONBITS;
extern volatile IC2CONBITS IC2CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC3 __attribute__((__sfr__));

extern volatile unsigned int  IC3BUF __attribute__((__sfr__));
extern volatile unsigned int  IC3CON __attribute__((__sfr__));
__extension__ typedef struct tagIC3CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC3CONBITS;
extern volatile IC3CONBITS IC3CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC4 __attribute__((__sfr__));

extern volatile unsigned int  IC4BUF __attribute__((__sfr__));
extern volatile unsigned int  IC4CON __attribute__((__sfr__));
__extension__ typedef struct tagIC4CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC4CONBITS;
extern volatile IC4CONBITS IC4CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC5 __attribute__((__sfr__));

extern volatile unsigned int  IC5BUF __attribute__((__sfr__));
extern volatile unsigned int  IC5CON __attribute__((__sfr__));
__extension__ typedef struct tagIC5CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC5CONBITS;
extern volatile IC5CONBITS IC5CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC6 __attribute__((__sfr__));

extern volatile unsigned int  IC6BUF __attribute__((__sfr__));
extern volatile unsigned int  IC6CON __attribute__((__sfr__));
__extension__ typedef struct tagIC6CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC6CONBITS;
extern volatile IC6CONBITS IC6CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC7 __attribute__((__sfr__));

extern volatile unsigned int  IC7BUF __attribute__((__sfr__));
extern volatile unsigned int  IC7CON __attribute__((__sfr__));
__extension__ typedef struct tagIC7CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC7CONBITS;
extern volatile IC7CONBITS IC7CONbits __attribute__((__sfr__));


/* SFR blocks for Input Capture modules */
extern volatile IC IC8 __attribute__((__sfr__));

extern volatile unsigned int  IC8BUF __attribute__((__sfr__));
extern volatile unsigned int  IC8CON __attribute__((__sfr__));
__extension__ typedef struct tagIC8CONBITS {
  union {
    struct {
      unsigned ICM:3;
      unsigned ICBNE:1;
      unsigned ICOV:1;
      unsigned ICI:2;
      unsigned ICTMR:1;
      unsigned :5;
      unsigned ICSIDL:1;
    };
    struct {
      unsigned ICM0:1;
      unsigned ICM1:1;
      unsigned ICM2:1;
      unsigned :2;
      unsigned ICI0:1;
      unsigned ICI1:1;
    };
  };
} IC8CONBITS;
extern volatile IC8CONBITS IC8CONbits __attribute__((__sfr__));


/* Generic structure of entire SFR area for Output Compare modules */
typedef struct tagOC {
        unsigned int ocxrs;
        unsigned int ocxr;
        unsigned int ocxcon;
} OC, *POC;


/* SFR blocks for Output Compare modules */
extern volatile OC OC1 __attribute__((__sfr__));

extern volatile unsigned int  OC1RS __attribute__((__sfr__));
extern volatile unsigned int  OC1R __attribute__((__sfr__));
extern volatile unsigned int  OC1CON __attribute__((__sfr__));
__extension__ typedef struct tagOC1CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC1CONBITS;
extern volatile OC1CONBITS OC1CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC2 __attribute__((__sfr__));

extern volatile unsigned int  OC2RS __attribute__((__sfr__));
extern volatile unsigned int  OC2R __attribute__((__sfr__));
extern volatile unsigned int  OC2CON __attribute__((__sfr__));
__extension__ typedef struct tagOC2CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC2CONBITS;
extern volatile OC2CONBITS OC2CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC3 __attribute__((__sfr__));

extern volatile unsigned int  OC3RS __attribute__((__sfr__));
extern volatile unsigned int  OC3R __attribute__((__sfr__));
extern volatile unsigned int  OC3CON __attribute__((__sfr__));
__extension__ typedef struct tagOC3CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC3CONBITS;
extern volatile OC3CONBITS OC3CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC4 __attribute__((__sfr__));

extern volatile unsigned int  OC4RS __attribute__((__sfr__));
extern volatile unsigned int  OC4R __attribute__((__sfr__));
extern volatile unsigned int  OC4CON __attribute__((__sfr__));
__extension__ typedef struct tagOC4CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC4CONBITS;
extern volatile OC4CONBITS OC4CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC5 __attribute__((__sfr__));

extern volatile unsigned int  OC5RS __attribute__((__sfr__));
extern volatile unsigned int  OC5R __attribute__((__sfr__));
extern volatile unsigned int  OC5CON __attribute__((__sfr__));
__extension__ typedef struct tagOC5CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC5CONBITS;
extern volatile OC5CONBITS OC5CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC6 __attribute__((__sfr__));

extern volatile unsigned int  OC6RS __attribute__((__sfr__));
extern volatile unsigned int  OC6R __attribute__((__sfr__));
extern volatile unsigned int  OC6CON __attribute__((__sfr__));
__extension__ typedef struct tagOC6CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC6CONBITS;
extern volatile OC6CONBITS OC6CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC7 __attribute__((__sfr__));

extern volatile unsigned int  OC7RS __attribute__((__sfr__));
extern volatile unsigned int  OC7R __attribute__((__sfr__));
extern volatile unsigned int  OC7CON __attribute__((__sfr__));
__extension__ typedef struct tagOC7CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC7CONBITS;
extern volatile OC7CONBITS OC7CONbits __attribute__((__sfr__));


/* SFR blocks for Output Compare modules */
extern volatile OC OC8 __attribute__((__sfr__));

extern volatile unsigned int  OC8RS __attribute__((__sfr__));
extern volatile unsigned int  OC8R __attribute__((__sfr__));
extern volatile unsigned int  OC8CON __attribute__((__sfr__));
__extension__ typedef struct tagOC8CONBITS {
  union {
    struct {
      unsigned OCM:3;
      unsigned OCTSEL:1;
      unsigned OCFLT:1;
      unsigned :8;
      unsigned OCSIDL:1;
    };
    struct {
      unsigned OCM0:1;
      unsigned OCM1:1;
      unsigned OCM2:1;
    };
  };
} OC8CONBITS;
extern volatile OC8CONBITS OC8CONbits __attribute__((__sfr__));

extern volatile unsigned int  I2CRCV __attribute__((__sfr__));
__extension__ typedef struct tagI2CRCVBITS {
  union {
    struct {
      unsigned I2CRCV:8;
    };
    struct {
      unsigned I2CRCV0:1;
      unsigned I2CRCV1:1;
      unsigned I2CRCV2:1;
      unsigned I2CRCV3:1;
      unsigned I2CRCV4:1;
      unsigned I2CRCV5:1;
      unsigned I2CRCV6:1;
      unsigned I2CRCV7:1;
    };
  };
} I2CRCVBITS;
extern volatile I2CRCVBITS I2CRCVbits __attribute__((__sfr__));

extern volatile unsigned int  I2CTRN __attribute__((__sfr__));
__extension__ typedef struct tagI2CTRNBITS {
  union {
    struct {
      unsigned I2CTRN:8;
    };
    struct {
      unsigned I2CTRN0:1;
      unsigned I2CTRN1:1;
      unsigned I2CTRN2:1;
      unsigned I2CTRN3:1;
      unsigned I2CTRN4:1;
      unsigned I2CTRN5:1;
      unsigned I2CTRN6:1;
      unsigned I2CTRN7:1;
    };
  };
} I2CTRNBITS;
extern volatile I2CTRNBITS I2CTRNbits __attribute__((__sfr__));

extern volatile unsigned int  I2CBRG __attribute__((__sfr__));
typedef struct tagI2CBRGBITS {
  unsigned I2CBRG:9;
} I2CBRGBITS;
extern volatile I2CBRGBITS I2CBRGbits __attribute__((__sfr__));

extern volatile unsigned int  I2CCON __attribute__((__sfr__));
typedef struct tagI2CCONBITS {
  unsigned SEN:1;
  unsigned RSEN:1;
  unsigned PEN:1;
  unsigned RCEN:1;
  unsigned ACKEN:1;
  unsigned ACKDT:1;
  unsigned STREN:1;
  unsigned GCEN:1;
  unsigned SMEN:1;
  unsigned DISSLW:1;
  unsigned A10M:1;
  unsigned IPMIEN:1;
  unsigned SCLREL:1;
  unsigned I2CSIDL:1;
  unsigned :1;
  unsigned I2CEN:1;
} I2CCONBITS;
extern volatile I2CCONBITS I2CCONbits __attribute__((__sfr__));

extern volatile unsigned int  I2CSTAT __attribute__((__sfr__));
__extension__ typedef struct tagI2CSTATBITS {
  union {
    struct {
      unsigned TBF:1;
      unsigned RBF:1;
      unsigned R_NOT_W:1;
      unsigned S:1;
      unsigned P:1;
      unsigned D_NOT_A:1;
      unsigned I2COV:1;
      unsigned IWCOL:1;
      unsigned ADD10:1;
      unsigned GCSTAT:1;
      unsigned BCL:1;
      unsigned :3;
      unsigned TRSTAT:1;
      unsigned ACKSTAT:1;
    };
    struct {
      unsigned :2;
      unsigned R_W:1;
      unsigned :2;
      unsigned D_A:1;
    };
  };
} I2CSTATBITS;
extern volatile I2CSTATBITS I2CSTATbits __attribute__((__sfr__));

extern volatile unsigned int  I2CADD __attribute__((__sfr__));
typedef struct tagI2CADDBITS {
  unsigned I2CADD:10;
} I2CADDBITS;
extern volatile I2CADDBITS I2CADDbits __attribute__((__sfr__));


/* Generic structure of entire SFR area for each UART module */
typedef struct tagUART {
        unsigned int uxmode;
        unsigned int uxsta;
        unsigned int uxtxreg;
        unsigned int uxrxreg;
        unsigned int uxbrg;
} UART, *PUART;

#define UxMODEBITS U1MODEBITS
#define UxSTABITS U1STABITS
#define UxTXREGBITS U1TXREGBITS
#define UxRXREGBITS U1RXREGBITS
#define UxBRGBITS U1BRGBITS


/* SFR blocks for each UART module */
extern volatile UART UART1 __attribute__((__sfr__));

extern volatile unsigned int  U1MODE __attribute__((__sfr__));
__extension__ typedef struct tagU1MODEBITS {
  union {
    struct {
      unsigned STSEL:1;
      unsigned PDSEL:2;
      unsigned :2;
      unsigned ABAUD:1;
      unsigned LPBACK:1;
      unsigned WAKE:1;
      unsigned :2;
      unsigned ALTIO:1;
      unsigned :2;
      unsigned USIDL:1;
      unsigned :1;
      unsigned UARTEN:1;
    };
    struct {
      unsigned :1;
      unsigned PDSEL0:1;
      unsigned PDSEL1:1;
    };
  };
} U1MODEBITS;
extern volatile U1MODEBITS U1MODEbits __attribute__((__sfr__));

extern volatile unsigned int  U1STA __attribute__((__sfr__));
__extension__ typedef struct tagU1STABITS {
  union {
    struct {
      unsigned URXDA:1;
      unsigned OERR:1;
      unsigned FERR:1;
      unsigned PERR:1;
      unsigned RIDLE:1;
      unsigned ADDEN:1;
      unsigned URXISEL:2;
      unsigned TRMT:1;
      unsigned UTXBF:1;
      unsigned UTXEN:1;
      unsigned UTXBRK:1;
      unsigned :3;
      unsigned UTXISEL:1;
    };
    struct {
      unsigned :6;
      unsigned URXISEL0:1;
      unsigned URXISEL1:1;
    };
  };
} U1STABITS;
extern volatile U1STABITS U1STAbits __attribute__((__sfr__));

extern volatile unsigned int  U1TXREG __attribute__((__sfr__));
__extension__ typedef struct tagU1TXREGBITS {
  union {
    struct {
      unsigned UTXREG0:1;
      unsigned UTXREG1:1;
      unsigned UTXREG2:1;
      unsigned UTXREG3:1;
      unsigned UTXREG4:1;
      unsigned UTXREG5:1;
      unsigned UTXREG6:1;
      unsigned UTXREG7:1;
      unsigned UTXREG8:1;
    };
    struct {
      unsigned :8;
      unsigned UTX8:1;
    };
  };
} U1TXREGBITS;
extern volatile U1TXREGBITS U1TXREGbits __attribute__((__sfr__));

extern volatile unsigned int  U1RXREG __attribute__((__sfr__));
__extension__ typedef struct tagU1RXREGBITS {
  union {
    struct {
      unsigned URXREG0:1;
      unsigned URXREG1:1;
      unsigned URXREG2:1;
      unsigned URXREG3:1;
      unsigned URXREG4:1;
      unsigned URXREG5:1;
      unsigned URXREG6:1;
      unsigned URXREG7:1;
      unsigned URXREG8:1;
    };
    struct {
      unsigned :8;
      unsigned URX8:1;
    };
  };
} U1RXREGBITS;
extern volatile U1RXREGBITS U1RXREGbits __attribute__((__sfr__));

extern volatile unsigned int  U1BRG __attribute__((__sfr__));

/* SFR blocks for each UART module */
extern volatile UART UART2 __attribute__((__sfr__));

extern volatile unsigned int  U2MODE __attribute__((__sfr__));
__extension__ typedef struct tagU2MODEBITS {
  union {
    struct {
      unsigned STSEL:1;
      unsigned PDSEL:2;
      unsigned :2;
      unsigned ABAUD:1;
      unsigned LPBACK:1;
      unsigned WAKE:1;
      unsigned :5;
      unsigned USIDL:1;
      unsigned :1;
      unsigned UARTEN:1;
    };
    struct {
      unsigned :1;
      unsigned PDSEL0:1;
      unsigned PDSEL1:1;
    };
  };
} U2MODEBITS;
extern volatile U2MODEBITS U2MODEbits __attribute__((__sfr__));

extern volatile unsigned int  U2STA __attribute__((__sfr__));
__extension__ typedef struct tagU2STABITS {
  union {
    struct {
      unsigned URXDA:1;
      unsigned OERR:1;
      unsigned FERR:1;
      unsigned PERR:1;
      unsigned RIDLE:1;
      unsigned ADDEN:1;
      unsigned URXISEL:2;
      unsigned TRMT:1;
      unsigned UTXBF:1;
      unsigned UTXEN:1;
      unsigned UTXBRK:1;
      unsigned :3;
      unsigned UTXISEL:1;
    };
    struct {
      unsigned :6;
      unsigned URXISEL0:1;
      unsigned URXISEL1:1;
    };
  };
} U2STABITS;
extern volatile U2STABITS U2STAbits __attribute__((__sfr__));

extern volatile unsigned int  U2TXREG __attribute__((__sfr__));
__extension__ typedef struct tagU2TXREGBITS {
  union {
    struct {
      unsigned UTXREG0:1;
      unsigned UTXREG1:1;
      unsigned UTXREG2:1;
      unsigned UTXREG3:1;
      unsigned UTXREG4:1;
      unsigned UTXREG5:1;
      unsigned UTXREG6:1;
      unsigned UTXREG7:1;
      unsigned UTXREG8:1;
    };
    struct {
      unsigned :8;
      unsigned UTX8:1;
    };
  };
} U2TXREGBITS;
extern volatile U2TXREGBITS U2TXREGbits __attribute__((__sfr__));

extern volatile unsigned int  U2RXREG __attribute__((__sfr__));
__extension__ typedef struct tagU2RXREGBITS {
  union {
    struct {
      unsigned URXREG0:1;
      unsigned URXREG1:1;
      unsigned URXREG2:1;
      unsigned URXREG3:1;
      unsigned URXREG4:1;
      unsigned URXREG5:1;
      unsigned URXREG6:1;
      unsigned URXREG7:1;
      unsigned URXREG8:1;
    };
    struct {
      unsigned :8;
      unsigned URX8:1;
    };
  };
} U2RXREGBITS;
extern volatile U2RXREGBITS U2RXREGbits __attribute__((__sfr__));

extern volatile unsigned int  U2BRG __attribute__((__sfr__));

/* Generic structure of entire SFR area for each SPI module */
typedef struct tagSPI {
        unsigned int spixstat;
        unsigned int spixcon1;
        unsigned int spixcon2;
        unsigned int unused;
        unsigned int spixbuf;
} SPI, *PSPI;

#define SPIxSTATBITS SPI1STATBITS
#define SPIxCONBITS SPI1CONBITS


/* SFR blocks for each SPI module */
extern volatile SPI SPI1 __attribute__((__sfr__));

extern volatile unsigned int  SPI1STAT __attribute__((__sfr__));
typedef struct tagSPI1STATBITS {
  unsigned SPIRBF:1;
  unsigned SPITBF:1;
  unsigned :4;
  unsigned SPIROV:1;
  unsigned :6;
  unsigned SPISIDL:1;
  unsigned :1;
  unsigned SPIEN:1;
} SPI1STATBITS;
extern volatile SPI1STATBITS SPI1STATbits __attribute__((__sfr__));

extern volatile unsigned int  SPI1CON __attribute__((__sfr__));
__extension__ typedef struct tagSPI1CONBITS {
  union {
    struct {
      unsigned PPRE0:1;
      unsigned PPRE1:1;
      unsigned SPRE0:1;
      unsigned SPRE1:1;
      unsigned SPRE2:1;
      unsigned MSTEN:1;
      unsigned CKP:1;
      unsigned SSEN:1;
      unsigned CKE:1;
      unsigned SMP:1;
      unsigned MODE16:1;
      unsigned DISSDO:1;
      unsigned :1;
      unsigned SPIFSD:1;
      unsigned FRMEN:1;
    };
    struct {
      unsigned PPRE:2;
      unsigned SPRE:3;
    };
  };
} SPI1CONBITS;
extern volatile SPI1CONBITS SPI1CONbits __attribute__((__sfr__));

extern volatile unsigned int  SPI1BUF __attribute__((__sfr__));

/* SFR blocks for each SPI module */
extern volatile SPI SPI2 __attribute__((__sfr__));

extern volatile unsigned int  SPI2STAT __attribute__((__sfr__));
typedef struct tagSPI2STATBITS {
  unsigned SPIRBF:1;
  unsigned SPITBF:1;
  unsigned :4;
  unsigned SPIROV:1;
  unsigned :6;
  unsigned SPISIDL:1;
  unsigned :1;
  unsigned SPIEN:1;
} SPI2STATBITS;
extern volatile SPI2STATBITS SPI2STATbits __attribute__((__sfr__));

extern volatile unsigned int  SPI2CON __attribute__((__sfr__));
__extension__ typedef struct tagSPI2CONBITS {
  union {
    struct {
      unsigned PPRE0:1;
      unsigned PPRE1:1;
      unsigned SPRE0:1;
      unsigned SPRE1:1;
      unsigned SPRE2:1;
      unsigned MSTEN:1;
      unsigned CKP:1;
      unsigned SSEN:1;
      unsigned CKE:1;
      unsigned SMP:1;
      unsigned MODE16:1;
      unsigned DISSDO:1;
      unsigned :1;
      unsigned SPIFSD:1;
      unsigned FRMEN:1;
    };
    struct {
      unsigned PPRE:2;
      unsigned SPRE:3;
    };
  };
} SPI2CONBITS;
extern volatile SPI2CONBITS SPI2CONbits __attribute__((__sfr__));

extern volatile unsigned int  SPI2BUF __attribute__((__sfr__));
extern volatile unsigned int  DCICON1 __attribute__((__sfr__));
__extension__ typedef struct tagDCICON1BITS {
  union {
    struct {
      unsigned COFSM:2;
      unsigned :3;
      unsigned DJST:1;
      unsigned CSDOM:1;
      unsigned UNFM:1;
      unsigned COFSD:1;
      unsigned CSCKE:1;
      unsigned CSCKD:1;
      unsigned DLOOP:1;
      unsigned :1;
      unsigned DCISIDL:1;
      unsigned :1;
      unsigned DCIEN:1;
    };
    struct {
      unsigned COFSM0:1;
      unsigned COFSM1:1;
    };
  };
} DCICON1BITS;
extern volatile DCICON1BITS DCICON1bits __attribute__((__sfr__));

extern volatile unsigned int  DCICON2 __attribute__((__sfr__));
__extension__ typedef struct tagDCICON2BITS {
  union {
    struct {
      unsigned WS:4;
      unsigned :1;
      unsigned COFSG:4;
      unsigned :1;
      unsigned BLEN:2;
    };
    struct {
      unsigned WS0:1;
      unsigned WS1:1;
      unsigned WS2:1;
      unsigned WS3:1;
      unsigned :1;
      unsigned COFSG0:1;
      unsigned COFSG1:1;
      unsigned COFSG2:1;
      unsigned COFSG3:1;
      unsigned :1;
      unsigned BLEN0:1;
      unsigned BLEN1:1;
    };
  };
} DCICON2BITS;
extern volatile DCICON2BITS DCICON2bits __attribute__((__sfr__));

extern volatile unsigned int  DCICON3 __attribute__((__sfr__));
__extension__ typedef struct tagDCICON3BITS {
  union {
    struct {
      unsigned BCG:12;
    };
    struct {
      unsigned BCG0:1;
      unsigned BCG1:1;
      unsigned BCG2:1;
      unsigned BCG3:1;
      unsigned BCG4:1;
      unsigned BCG5:1;
      unsigned BCG6:1;
      unsigned BCG7:1;
      unsigned BCG8:1;
      unsigned BCG9:1;
      unsigned BCG10:1;
      unsigned BCG11:1;
    };
  };
} DCICON3BITS;
extern volatile DCICON3BITS DCICON3bits __attribute__((__sfr__));

extern volatile unsigned int  DCISTAT __attribute__((__sfr__));
__extension__ typedef struct tagDCISTATBITS {
  union {
    struct {
      unsigned TMPTY:1;
      unsigned TUNF:1;
      unsigned RFUL:1;
      unsigned ROV:1;
      unsigned :4;
      unsigned SLOT:4;
    };
    struct {
      unsigned :8;
      unsigned SLOT0:1;
      unsigned SLOT1:1;
      unsigned SLOT2:1;
      unsigned SLOT3:1;
    };
  };
} DCISTATBITS;
extern volatile DCISTATBITS DCISTATbits __attribute__((__sfr__));

extern volatile unsigned int  TSCON __attribute__((__sfr__));
typedef struct tagTSCONBITS {
  unsigned TSE0:1;
  unsigned TSE1:1;
  unsigned TSE2:1;
  unsigned TSE3:1;
  unsigned TSE4:1;
  unsigned TSE5:1;
  unsigned TSE6:1;
  unsigned TSE7:1;
  unsigned TSE8:1;
  unsigned TSE9:1;
  unsigned TSE10:1;
  unsigned TSE11:1;
  unsigned TSE12:1;
  unsigned TSE13:1;
  unsigned TSE14:1;
  unsigned TSE15:1;
} TSCONBITS;
extern volatile TSCONBITS TSCONbits __attribute__((__sfr__));

extern volatile unsigned int  RSCON __attribute__((__sfr__));
typedef struct tagRSCONBITS {
  unsigned RSE0:1;
  unsigned RSE1:1;
  unsigned RSE2:1;
  unsigned RSE3:1;
  unsigned RSE4:1;
  unsigned RSE5:1;
  unsigned RSE6:1;
  unsigned RSE7:1;
  unsigned RSE8:1;
  unsigned RSE9:1;
  unsigned RSE10:1;
  unsigned RSE11:1;
  unsigned RSE12:1;
  unsigned RSE13:1;
  unsigned RSE14:1;
  unsigned RSE15:1;
} RSCONBITS;
extern volatile RSCONBITS RSCONbits __attribute__((__sfr__));

extern volatile unsigned int  RXBUF0 __attribute__((__sfr__));
extern volatile unsigned int  RXBUF1 __attribute__((__sfr__));
extern volatile unsigned int  RXBUF2 __attribute__((__sfr__));
extern volatile unsigned int  RXBUF3 __attribute__((__sfr__));
extern volatile unsigned int  TXBUF0 __attribute__((__sfr__));
extern volatile unsigned int  TXBUF1 __attribute__((__sfr__));
extern volatile unsigned int  TXBUF2 __attribute__((__sfr__));
extern volatile unsigned int  TXBUF3 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF0 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF1 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF2 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF3 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF4 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF5 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF6 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF7 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF8 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUF9 __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFA __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFB __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFC __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFD __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFE __attribute__((__sfr__));
extern volatile unsigned int  ADCBUFF __attribute__((__sfr__));
extern volatile unsigned int  ADCON1 __attribute__((__sfr__));
__extension__ typedef struct tagADCON1BITS {
  union {
    struct {
      unsigned DONE:1;
      unsigned SAMP:1;
      unsigned ASAM:1;
      unsigned :2;
      unsigned SSRC:3;
      unsigned FORM:2;
      unsigned :3;
      unsigned ADSIDL:1;
      unsigned :1;
      unsigned ADON:1;
    };
    struct {
      unsigned :5;
      unsigned SSRC0:1;
      unsigned SSRC1:1;
      unsigned SSRC2:1;
      unsigned FORM0:1;
      unsigned FORM1:1;
    };
  };
} ADCON1BITS;
extern volatile ADCON1BITS ADCON1bits __attribute__((__sfr__));

extern volatile unsigned int  ADCON2 __attribute__((__sfr__));
__extension__ typedef struct tagADCON2BITS {
  union {
    struct {
      unsigned ALTS:1;
      unsigned BUFM:1;
      unsigned SMPI:4;
      unsigned :1;
      unsigned BUFS:1;
      unsigned :2;
      unsigned CSCNA:1;
      unsigned :2;
      unsigned VCFG:3;
    };
    struct {
      unsigned :2;
      unsigned SMPI0:1;
      unsigned SMPI1:1;
      unsigned SMPI2:1;
      unsigned SMPI3:1;
      unsigned :7;
      unsigned VCFG0:1;
      unsigned VCFG1:1;
      unsigned VCFG2:1;
    };
  };
} ADCON2BITS;
extern volatile ADCON2BITS ADCON2bits __attribute__((__sfr__));

extern volatile unsigned int  ADCON3 __attribute__((__sfr__));
__extension__ typedef struct tagADCON3BITS {
  union {
    struct {
      unsigned ADCS:6;
      unsigned :1;
      unsigned ADRC:1;
      unsigned SAMC:5;
    };
    struct {
      unsigned ADCS0:1;
      unsigned ADCS1:1;
      unsigned ADCS2:1;
      unsigned ADCS3:1;
      unsigned ADCS4:1;
      unsigned ADCS5:1;
      unsigned :2;
      unsigned SAMC0:1;
      unsigned SAMC1:1;
      unsigned SAMC2:1;
      unsigned SAMC3:1;
      unsigned SAMC4:1;
    };
  };
} ADCON3BITS;
extern volatile ADCON3BITS ADCON3bits __attribute__((__sfr__));

extern volatile unsigned int  ADCHS __attribute__((__sfr__));
__extension__ typedef struct tagADCHSBITS {
  union {
    struct {
      unsigned CH0SA:4;
      unsigned CH0NA:1;
      unsigned :3;
      unsigned CH0SB:4;
      unsigned CH0NB:1;
    };
    struct {
      unsigned CH0SA0:1;
      unsigned CH0SA1:1;
      unsigned CH0SA2:1;
      unsigned CH0SA3:1;
      unsigned :4;
      unsigned CH0SB0:1;
      unsigned CH0SB1:1;
      unsigned CH0SB2:1;
      unsigned CH0SB3:1;
    };
  };
} ADCHSBITS;
extern volatile ADCHSBITS ADCHSbits __attribute__((__sfr__));

extern volatile unsigned int  ADPCFG __attribute__((__sfr__));
typedef struct tagADPCFGBITS {
  unsigned PCFG0:1;
  unsigned PCFG1:1;
  unsigned PCFG2:1;
  unsigned PCFG3:1;
  unsigned PCFG4:1;
  unsigned PCFG5:1;
  unsigned PCFG6:1;
  unsigned PCFG7:1;
  unsigned PCFG8:1;
  unsigned PCFG9:1;
  unsigned PCFG10:1;
  unsigned PCFG11:1;
  unsigned PCFG12:1;
  unsigned PCFG13:1;
  unsigned PCFG14:1;
  unsigned PCFG15:1;
} ADPCFGBITS;
extern volatile ADPCFGBITS ADPCFGbits __attribute__((__sfr__));

extern volatile unsigned int  ADCSSL __attribute__((__sfr__));
typedef struct tagADCSSLBITS {
  unsigned CSSL0:1;
  unsigned CSSL1:1;
  unsigned CSSL2:1;
  unsigned CSSL3:1;
  unsigned CSSL4:1;
  unsigned CSSL5:1;
  unsigned CSSL6:1;
  unsigned CSSL7:1;
  unsigned CSSL8:1;
  unsigned CSSL9:1;
  unsigned CSSL10:1;
  unsigned CSSL11:1;
  unsigned CSSL12:1;
  unsigned CSSL13:1;
  unsigned CSSL14:1;
  unsigned CSSL15:1;
} ADCSSLBITS;
extern volatile ADCSSLBITS ADCSSLbits __attribute__((__sfr__));

extern volatile unsigned int  TRISB __attribute__((__sfr__));
typedef struct tagTRISBBITS {
  unsigned TRISB0:1;
  unsigned TRISB1:1;
  unsigned TRISB2:1;
  unsigned TRISB3:1;
  unsigned TRISB4:1;
  unsigned TRISB5:1;
  unsigned TRISB6:1;
  unsigned TRISB7:1;
  unsigned TRISB8:1;
  unsigned TRISB9:1;
  unsigned TRISB10:1;
  unsigned TRISB11:1;
  unsigned TRISB12:1;
  unsigned TRISB13:1;
  unsigned TRISB14:1;
  unsigned TRISB15:1;
} TRISBBITS;
extern volatile TRISBBITS TRISBbits __attribute__((__sfr__));

extern volatile unsigned int  PORTB __attribute__((__sfr__));
typedef struct tagPORTBBITS {
  unsigned RB0:1;
  unsigned RB1:1;
  unsigned RB2:1;
  unsigned RB3:1;
  unsigned RB4:1;
  unsigned RB5:1;
  unsigned RB6:1;
  unsigned RB7:1;
  unsigned RB8:1;
  unsigned RB9:1;
  unsigned RB10:1;
  unsigned RB11:1;
  unsigned RB12:1;
  unsigned RB13:1;
  unsigned RB14:1;
  unsigned RB15:1;
} PORTBBITS;
extern volatile PORTBBITS PORTBbits __attribute__((__sfr__));

extern volatile unsigned int  LATB __attribute__((__sfr__));
typedef struct tagLATBBITS {
  unsigned LATB0:1;
  unsigned LATB1:1;
  unsigned LATB2:1;
  unsigned LATB3:1;
  unsigned LATB4:1;
  unsigned LATB5:1;
  unsigned LATB6:1;
  unsigned LATB7:1;
  unsigned LATB8:1;
  unsigned LATB9:1;
  unsigned LATB10:1;
  unsigned LATB11:1;
  unsigned LATB12:1;
  unsigned LATB13:1;
  unsigned LATB14:1;
  unsigned LATB15:1;
} LATBBITS;
extern volatile LATBBITS LATBbits __attribute__((__sfr__));

extern volatile unsigned int  TRISC __attribute__((__sfr__));
typedef struct tagTRISCBITS {
  unsigned :1;
  unsigned TRISC1:1;
  unsigned TRISC2:1;
  unsigned :10;
  unsigned TRISC13:1;
  unsigned TRISC14:1;
  unsigned TRISC15:1;
} TRISCBITS;
extern volatile TRISCBITS TRISCbits __attribute__((__sfr__));

extern volatile unsigned int  PORTC __attribute__((__sfr__));
typedef struct tagPORTCBITS {
  unsigned :1;
  unsigned RC1:1;
  unsigned RC2:1;
  unsigned :10;
  unsigned RC13:1;
  unsigned RC14:1;
  unsigned RC15:1;
} PORTCBITS;
extern volatile PORTCBITS PORTCbits __attribute__((__sfr__));

extern volatile unsigned int  LATC __attribute__((__sfr__));
typedef struct tagLATCBITS {
  unsigned :1;
  unsigned LATC1:1;
  unsigned LATC2:1;
  unsigned :10;
  unsigned LATC13:1;
  unsigned LATC14:1;
  unsigned LATC15:1;
} LATCBITS;
extern volatile LATCBITS LATCbits __attribute__((__sfr__));

extern volatile unsigned int  TRISD __attribute__((__sfr__));
typedef struct tagTRISDBITS {
  unsigned TRISD0:1;
  unsigned TRISD1:1;
  unsigned TRISD2:1;
  unsigned TRISD3:1;
  unsigned TRISD4:1;
  unsigned TRISD5:1;
  unsigned TRISD6:1;
  unsigned TRISD7:1;
  unsigned TRISD8:1;
  unsigned TRISD9:1;
  unsigned TRISD10:1;
  unsigned TRISD11:1;
} TRISDBITS;
extern volatile TRISDBITS TRISDbits __attribute__((__sfr__));

extern volatile unsigned int  PORTD __attribute__((__sfr__));
typedef struct tagPORTDBITS {
  unsigned RD0:1;
  unsigned RD1:1;
  unsigned RD2:1;
  unsigned RD3:1;
  unsigned RD4:1;
  unsigned RD5:1;
  unsigned RD6:1;
  unsigned RD7:1;
  unsigned RD8:1;
  unsigned RD9:1;
  unsigned RD10:1;
  unsigned RD11:1;
} PORTDBITS;
extern volatile PORTDBITS PORTDbits __attribute__((__sfr__));

extern volatile unsigned int  LATD __attribute__((__sfr__));
typedef struct tagLATDBITS {
  unsigned LATD0:1;
  unsigned LATD1:1;
  unsigned LATD2:1;
  unsigned LATD3:1;
  unsigned LATD4:1;
  unsigned LATD5:1;
  unsigned LATD6:1;
  unsigned LATD7:1;
  unsigned LATD8:1;
  unsigned LATD9:1;
  unsigned LATD10:1;
  unsigned LATD11:1;
} LATDBITS;
extern volatile LATDBITS LATDbits __attribute__((__sfr__));

extern volatile unsigned int  TRISF __attribute__((__sfr__));
typedef struct tagTRISFBITS {
  unsigned TRISF0:1;
  unsigned TRISF1:1;
  unsigned TRISF2:1;
  unsigned TRISF3:1;
  unsigned TRISF4:1;
  unsigned TRISF5:1;
  unsigned TRISF6:1;
} TRISFBITS;
extern volatile TRISFBITS TRISFbits __attribute__((__sfr__));

extern volatile unsigned int  PORTF __attribute__((__sfr__));
typedef struct tagPORTFBITS {
  unsigned RF0:1;
  unsigned RF1:1;
  unsigned RF2:1;
  unsigned RF3:1;
  unsigned RF4:1;
  unsigned RF5:1;
  unsigned RF6:1;
} PORTFBITS;
extern volatile PORTFBITS PORTFbits __attribute__((__sfr__));

extern volatile unsigned int  LATF __attribute__((__sfr__));
typedef struct tagLATFBITS {
  unsigned LATF0:1;
  unsigned LATF1:1;
  unsigned LATF2:1;
  unsigned LATF3:1;
  unsigned LATF4:1;
  unsigned LATF5:1;
  unsigned LATF6:1;
} LATFBITS;
extern volatile LATFBITS LATFbits __attribute__((__sfr__));

extern volatile unsigned int  TRISG __attribute__((__sfr__));
typedef struct tagTRISGBITS {
  unsigned TRISG0:1;
  unsigned TRISG1:1;
  unsigned TRISG2:1;
  unsigned TRISG3:1;
  unsigned :2;
  unsigned TRISG6:1;
  unsigned TRISG7:1;
  unsigned TRISG8:1;
  unsigned TRISG9:1;
  unsigned :2;
  unsigned TRISG12:1;
  unsigned TRISG13:1;
  unsigned TRISG14:1;
  unsigned TRISG15:1;
} TRISGBITS;
extern volatile TRISGBITS TRISGbits __attribute__((__sfr__));

extern volatile unsigned int  PORTG __attribute__((__sfr__));
typedef struct tagPORTGBITS {
  unsigned RG0:1;
  unsigned RG1:1;
  unsigned RG2:1;
  unsigned RG3:1;
  unsigned :2;
  unsigned RG6:1;
  unsigned RG7:1;
  unsigned RG8:1;
  unsigned RG9:1;
  unsigned :2;
  unsigned RG12:1;
  unsigned RG13:1;
  unsigned RG14:1;
  unsigned RG15:1;
} PORTGBITS;
extern volatile PORTGBITS PORTGbits __attribute__((__sfr__));

extern volatile unsigned int  LATG __attribute__((__sfr__));
typedef struct tagLATGBITS {
  unsigned LATG0:1;
  unsigned LATG1:1;
  unsigned LATG2:1;
  unsigned LATG3:1;
  unsigned :2;
  unsigned LATG6:1;
  unsigned LATG7:1;
  unsigned LATG8:1;
  unsigned LATG9:1;
  unsigned :2;
  unsigned LATG12:1;
  unsigned LATG13:1;
  unsigned LATG14:1;
  unsigned LATG15:1;
} LATGBITS;
extern volatile LATGBITS LATGbits __attribute__((__sfr__));


/* Generic structure of entire SFR area for each CAN module */
typedef struct tagCAN {
        unsigned int cxrxf0sid;
        unsigned int cxrxf0eidh;
        unsigned int cxrxf0eidl;
        unsigned int reserved0;
        unsigned int cxrxf1sid;
        unsigned int cxrxf1eidh;
        unsigned int cxrxf1eidl;
        unsigned int reserved1;
        unsigned int cxrxf2sid;
        unsigned int cxrxf2eidh;
        unsigned int cxrxf2eidl;
        unsigned int reserved2;
        unsigned int cxrxf3sid;
        unsigned int cxrxf3eidh;
        unsigned int cxrxf3eidl;
        unsigned int reserved3;
        unsigned int cxrxf4sid;
        unsigned int cxrxf4eidh;
        unsigned int cxrxf4eidl;
        unsigned int reserved4;
        unsigned int cxrxf5sid;
        unsigned int cxrxf5eidh;
        unsigned int cxrxf5eidl;
        unsigned int reserved5;
        unsigned int cxrxm0sid;
        unsigned int cxrxm0eidh;
        unsigned int cxrxm0eidl;
        unsigned int reserved6;
        unsigned int cxrxm1sid;
        unsigned int cxrxm1eidh;
        unsigned int cxrxm1eidl;
        unsigned int reserved7;
        unsigned int cxtx2sid;
        unsigned int cxtx2eid;
        unsigned int cxtx2dlc;
        unsigned int cxtx2b1;
        unsigned int cxtx2b2;
        unsigned int cxtx2b3;
        unsigned int cxtx2b4;
        unsigned int cxtx2con;
        unsigned int cxtx1sid;
        unsigned int cxtx1eid;
        unsigned int cxtx1dlc;
        unsigned int cxtx1b1;
        unsigned int cxtx1b2;
        unsigned int cxtx1b3;
        unsigned int cxtx1b4;
        unsigned int cxtx1con;
        unsigned int cxtx0sid;
        unsigned int cxtx0eid;
        unsigned int cxtx0dlc;
        unsigned int cxtx0b1;
        unsigned int cxtx0b2;
        unsigned int cxtx0b3;
        unsigned int cxtx0b4;
        unsigned int cxtx0con;
        unsigned int cxrx1sid;
        unsigned int cxrx1eid;
        unsigned int cxrx1dlc;
        unsigned int cxrx1b1;
        unsigned int cxrx1b2;
        unsigned int cxrx1b3;
        unsigned int cxrx1b4;
        unsigned int cxrx1con;
        unsigned int cxrx0sid;
        unsigned int cxrx0eid;
        unsigned int cxrx0dlc;
        unsigned int cxrx0b1;
        unsigned int cxrx0b2;
        unsigned int cxrx0b3;
        unsigned int cxrx0b4;
        unsigned int cxrx0con;
        unsigned int cxctrl;
        unsigned int cxcfg1;
        unsigned int cxcfg2;
        unsigned int cxintf;
        unsigned int cxinte;
        unsigned int cxec;
        unsigned int reserved8[50];
} CAN, *PCAN;

/* SFR blocks for each CAN module */
extern volatile CAN CAN1 __attribute__((__sfr__));

extern volatile unsigned int  C1RXF0SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF0SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF0SIDBITS;
extern volatile C1RXF0SIDBITS C1RXF0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF0EIDH __attribute__((__sfr__));
typedef struct tagC1RXF0EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF0EIDHBITS;
extern volatile C1RXF0EIDHBITS C1RXF0EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF0EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF0EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF0EIDLBITS;
extern volatile C1RXF0EIDLBITS C1RXF0EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF1SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF1SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF1SIDBITS;
extern volatile C1RXF1SIDBITS C1RXF1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF1EIDH __attribute__((__sfr__));
typedef struct tagC1RXF1EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF1EIDHBITS;
extern volatile C1RXF1EIDHBITS C1RXF1EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF1EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF1EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF1EIDLBITS;
extern volatile C1RXF1EIDLBITS C1RXF1EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF2SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF2SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF2SIDBITS;
extern volatile C1RXF2SIDBITS C1RXF2SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF2EIDH __attribute__((__sfr__));
typedef struct tagC1RXF2EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF2EIDHBITS;
extern volatile C1RXF2EIDHBITS C1RXF2EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF2EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF2EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF2EIDLBITS;
extern volatile C1RXF2EIDLBITS C1RXF2EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF3SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF3SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF3SIDBITS;
extern volatile C1RXF3SIDBITS C1RXF3SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF3EIDH __attribute__((__sfr__));
typedef struct tagC1RXF3EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF3EIDHBITS;
extern volatile C1RXF3EIDHBITS C1RXF3EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF3EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF3EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF3EIDLBITS;
extern volatile C1RXF3EIDLBITS C1RXF3EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF4SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF4SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF4SIDBITS;
extern volatile C1RXF4SIDBITS C1RXF4SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF4EIDH __attribute__((__sfr__));
typedef struct tagC1RXF4EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF4EIDHBITS;
extern volatile C1RXF4EIDHBITS C1RXF4EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF4EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF4EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF4EIDLBITS;
extern volatile C1RXF4EIDLBITS C1RXF4EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF5SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF5SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXF5SIDBITS;
extern volatile C1RXF5SIDBITS C1RXF5SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF5EIDH __attribute__((__sfr__));
typedef struct tagC1RXF5EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXF5EIDHBITS;
extern volatile C1RXF5EIDHBITS C1RXF5EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXF5EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXF5EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXF5EIDLBITS;
extern volatile C1RXF5EIDLBITS C1RXF5EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM0SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXM0SIDBITS {
  union {
    struct {
      unsigned MIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXM0SIDBITS;
extern volatile C1RXM0SIDBITS C1RXM0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM0EIDH __attribute__((__sfr__));
typedef struct tagC1RXM0EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXM0EIDHBITS;
extern volatile C1RXM0EIDHBITS C1RXM0EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM0EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXM0EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXM0EIDLBITS;
extern volatile C1RXM0EIDLBITS C1RXM0EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM1SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RXM1SIDBITS {
  union {
    struct {
      unsigned MIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RXM1SIDBITS;
extern volatile C1RXM1SIDBITS C1RXM1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM1EIDH __attribute__((__sfr__));
typedef struct tagC1RXM1EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RXM1EIDHBITS;
extern volatile C1RXM1EIDHBITS C1RXM1EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C1RXM1EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC1RXM1EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1RXM1EIDLBITS;
extern volatile C1RXM1EIDLBITS C1RXM1EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2SID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX2SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C1TX2SIDBITS;
extern volatile C1TX2SIDBITS C1TX2SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2EID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX2EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C1TX2EIDBITS;
extern volatile C1TX2EIDBITS C1TX2EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2DLC __attribute__((__sfr__));
__extension__ typedef struct tagC1TX2DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C1TX2DLCBITS;
extern volatile C1TX2DLCBITS C1TX2DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2B1 __attribute__((__sfr__));
typedef struct tagC1TX2B1BITS {
  unsigned TXB2D00:1;
  unsigned TXB2D01:1;
  unsigned TXB2D02:1;
  unsigned TXB2D03:1;
  unsigned TXB2D04:1;
  unsigned TXB2D05:1;
  unsigned TXB2D06:1;
  unsigned TXB2D07:1;
  unsigned TXB2D10:1;
  unsigned TXB2D11:1;
  unsigned TXB2D12:1;
  unsigned TXB2D13:1;
  unsigned TXB2D14:1;
  unsigned TXB2D15:1;
  unsigned TXB2D16:1;
  unsigned TXB2D17:1;
} C1TX2B1BITS;
extern volatile C1TX2B1BITS C1TX2B1bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2B2 __attribute__((__sfr__));
typedef struct tagC1TX2B2BITS {
  unsigned TXB2D20:1;
  unsigned TXB2D21:1;
  unsigned TXB2D22:1;
  unsigned TXB2D23:1;
  unsigned TXB2D24:1;
  unsigned TXB2D25:1;
  unsigned TXB2D26:1;
  unsigned TXB2D27:1;
  unsigned TXB2D30:1;
  unsigned TXB2D31:1;
  unsigned TXB2D32:1;
  unsigned TXB2D33:1;
  unsigned TXB2D34:1;
  unsigned TXB2D35:1;
  unsigned TXB2D36:1;
  unsigned TXB2D37:1;
} C1TX2B2BITS;
extern volatile C1TX2B2BITS C1TX2B2bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2B3 __attribute__((__sfr__));
typedef struct tagC1TX2B3BITS {
  unsigned TX2B2D40:1;
  unsigned TX2B2D41:1;
  unsigned TX2B2D42:1;
  unsigned TX2B2D43:1;
  unsigned TX2B2D44:1;
  unsigned TX2B2D45:1;
  unsigned TX2B2D46:1;
  unsigned TX2B2D47:1;
  unsigned TXB2D50:1;
  unsigned TXB2D51:1;
  unsigned TXB2D52:1;
  unsigned TXB2D53:1;
  unsigned TXB2D54:1;
  unsigned TXB2D55:1;
  unsigned TXB2D56:1;
  unsigned TXB2D57:1;
} C1TX2B3BITS;
extern volatile C1TX2B3BITS C1TX2B3bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2B4 __attribute__((__sfr__));
typedef struct tagC1TX2B4BITS {
  unsigned TX2B2D60:1;
  unsigned TX2B2D61:1;
  unsigned TX2B2D62:1;
  unsigned TX2B2D63:1;
  unsigned TX2B2D64:1;
  unsigned TX2B2D65:1;
  unsigned TX2B2D66:1;
  unsigned TX2B2D67:1;
  unsigned TX2B2D70:1;
  unsigned TX2B2D71:1;
  unsigned TX2B2D72:1;
  unsigned TX2B2D73:1;
  unsigned TX2B2D74:1;
  unsigned TX2B2D75:1;
  unsigned TX2B2D76:1;
  unsigned TX2B2D77:1;
} C1TX2B4BITS;
extern volatile C1TX2B4BITS C1TX2B4bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX2CON __attribute__((__sfr__));
__extension__ typedef struct tagC1TX2CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C1TX2CONBITS;
extern volatile C1TX2CONBITS C1TX2CONbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1SID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX1SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C1TX1SIDBITS;
extern volatile C1TX1SIDBITS C1TX1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1EID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX1EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C1TX1EIDBITS;
extern volatile C1TX1EIDBITS C1TX1EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1DLC __attribute__((__sfr__));
__extension__ typedef struct tagC1TX1DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned :3;
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C1TX1DLCBITS;
extern volatile C1TX1DLCBITS C1TX1DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1B1 __attribute__((__sfr__));
typedef struct tagC1TX1B1BITS {
  unsigned TXB1D00:1;
  unsigned TXB1D01:1;
  unsigned TXB1D02:1;
  unsigned TXB1D03:1;
  unsigned TXB1D04:1;
  unsigned TXB1D05:1;
  unsigned TXB1D06:1;
  unsigned TXB1D07:1;
  unsigned TXB1D10:1;
  unsigned TXB1D11:1;
  unsigned TXB1D12:1;
  unsigned TXB1D13:1;
  unsigned TXB1D14:1;
  unsigned TXB1D15:1;
  unsigned TXB1D16:1;
  unsigned TXB1D17:1;
} C1TX1B1BITS;
extern volatile C1TX1B1BITS C1TX1B1bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1B2 __attribute__((__sfr__));
typedef struct tagC1TX1B2BITS {
  unsigned TXB1D20:1;
  unsigned TXB1D21:1;
  unsigned TXB1D22:1;
  unsigned TXB1D23:1;
  unsigned TXB1D24:1;
  unsigned TXB1D25:1;
  unsigned TXB1D26:1;
  unsigned TXB1D27:1;
  unsigned TXB1D30:1;
  unsigned TXB1D31:1;
  unsigned TXB1D32:1;
  unsigned TXB1D33:1;
  unsigned TXB1D34:1;
  unsigned TXB1D35:1;
  unsigned TXB1D36:1;
  unsigned TXB1D37:1;
} C1TX1B2BITS;
extern volatile C1TX1B2BITS C1TX1B2bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1B3 __attribute__((__sfr__));
typedef struct tagC1TX1B3BITS {
  unsigned TXB1D40:1;
  unsigned TXB1D41:1;
  unsigned TXB1D42:1;
  unsigned TXB1D43:1;
  unsigned TXB1D44:1;
  unsigned TXB1D45:1;
  unsigned TXB1D46:1;
  unsigned TXB1D47:1;
  unsigned TXB1D50:1;
  unsigned TXB1D51:1;
  unsigned TXB1D52:1;
  unsigned TXB1D53:1;
  unsigned TXB1D54:1;
  unsigned TXB1D55:1;
  unsigned TXB1D56:1;
  unsigned TXB1D57:1;
} C1TX1B3BITS;
extern volatile C1TX1B3BITS C1TX1B3bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1B4 __attribute__((__sfr__));
typedef struct tagC1TX1B4BITS {
  unsigned TXB1D60:1;
  unsigned TXB1D61:1;
  unsigned TXB1D62:1;
  unsigned TXB1D63:1;
  unsigned TXB1D64:1;
  unsigned TXB1D65:1;
  unsigned TXB1D66:1;
  unsigned TXB1D67:1;
  unsigned TXB1D70:1;
  unsigned TXB1D71:1;
  unsigned TXB1D72:1;
  unsigned TXB1D73:1;
  unsigned TXB1D74:1;
  unsigned TXB1D75:1;
  unsigned TXB1D76:1;
  unsigned TXB1D77:1;
} C1TX1B4BITS;
extern volatile C1TX1B4BITS C1TX1B4bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX1CON __attribute__((__sfr__));
__extension__ typedef struct tagC1TX1CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C1TX1CONBITS;
extern volatile C1TX1CONBITS C1TX1CONbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0SID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX0SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C1TX0SIDBITS;
extern volatile C1TX0SIDBITS C1TX0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0EID __attribute__((__sfr__));
__extension__ typedef struct tagC1TX0EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C1TX0EIDBITS;
extern volatile C1TX0EIDBITS C1TX0EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0DLC __attribute__((__sfr__));
__extension__ typedef struct tagC1TX0DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned :3;
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C1TX0DLCBITS;
extern volatile C1TX0DLCBITS C1TX0DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0B1 __attribute__((__sfr__));
typedef struct tagC1TX0B1BITS {
  unsigned TXB0D00:1;
  unsigned TXB0D01:1;
  unsigned TXB0D02:1;
  unsigned TXB0D03:1;
  unsigned TXB0D04:1;
  unsigned TXB0D05:1;
  unsigned TXB0D06:1;
  unsigned TXB0D07:1;
  unsigned TXB0D10:1;
  unsigned TXB0D11:1;
  unsigned TXB0D12:1;
  unsigned TXB0D13:1;
  unsigned TXB0D14:1;
  unsigned TXB0D15:1;
  unsigned TXB0D16:1;
  unsigned TXB0D17:1;
} C1TX0B1BITS;
extern volatile C1TX0B1BITS C1TX0B1bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0B2 __attribute__((__sfr__));
typedef struct tagC1TX0B2BITS {
  unsigned TXB0D20:1;
  unsigned TXB0D21:1;
  unsigned TXB0D22:1;
  unsigned TXB0D23:1;
  unsigned TXB0D24:1;
  unsigned TXB0D25:1;
  unsigned TXB0D26:1;
  unsigned TXB0D27:1;
  unsigned TXB0D30:1;
  unsigned TXB0D31:1;
  unsigned TXB0D32:1;
  unsigned TXB0D33:1;
  unsigned TXB0D34:1;
  unsigned TXB0D35:1;
  unsigned TXB0D36:1;
  unsigned TXB0D37:1;
} C1TX0B2BITS;
extern volatile C1TX0B2BITS C1TX0B2bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0B3 __attribute__((__sfr__));
typedef struct tagC1TX0B3BITS {
  unsigned TXB0D40:1;
  unsigned TXB0D41:1;
  unsigned TXB0D42:1;
  unsigned TXB0D43:1;
  unsigned TXB0D44:1;
  unsigned TXB0D45:1;
  unsigned TXB0D46:1;
  unsigned TXB0D47:1;
  unsigned TXB0D50:1;
  unsigned TXB0D51:1;
  unsigned TXB0D52:1;
  unsigned TXB0D53:1;
  unsigned TXB0D54:1;
  unsigned TXB0D55:1;
  unsigned TXB0D56:1;
  unsigned TXB0D57:1;
} C1TX0B3BITS;
extern volatile C1TX0B3BITS C1TX0B3bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0B4 __attribute__((__sfr__));
typedef struct tagC1TX0B4BITS {
  unsigned TXB0D60:1;
  unsigned TXB0D61:1;
  unsigned TXB0D62:1;
  unsigned TXB0D63:1;
  unsigned TXB0D64:1;
  unsigned TXB0D65:1;
  unsigned TXB0D66:1;
  unsigned TXB0D67:1;
  unsigned TXB0D70:1;
  unsigned TXB0D71:1;
  unsigned TXB0D72:1;
  unsigned TXB0D73:1;
  unsigned TXB0D74:1;
  unsigned TXB0D75:1;
  unsigned TXB0D76:1;
  unsigned TXB0D77:1;
} C1TX0B4BITS;
extern volatile C1TX0B4BITS C1TX0B4bits __attribute__((__sfr__));

extern volatile unsigned int  C1TX0CON __attribute__((__sfr__));
__extension__ typedef struct tagC1TX0CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C1TX0CONBITS;
extern volatile C1TX0CONBITS C1TX0CONbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RX1SIDBITS {
  union {
    struct {
      unsigned RXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RX1SIDBITS;
extern volatile C1RX1SIDBITS C1RX1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1EID __attribute__((__sfr__));
typedef struct tagC1RX1EIDBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RX1EIDBITS;
extern volatile C1RX1EIDBITS C1RX1EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1DLC __attribute__((__sfr__));
__extension__ typedef struct tagC1RX1DLCBITS {
  union {
    struct {
      unsigned DLC:4;
      unsigned RXRB0:1;
      unsigned :3;
      unsigned RXRB1:1;
      unsigned RXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C1RX1DLCBITS;
extern volatile C1RX1DLCBITS C1RX1DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1B1 __attribute__((__sfr__));
typedef struct tagC1RX1B1BITS {
  unsigned TXB1D00:1;
  unsigned TXB1D01:1;
  unsigned TXB1D02:1;
  unsigned TXB1D03:1;
  unsigned TXB1D04:1;
  unsigned TXB1D05:1;
  unsigned TXB1D06:1;
  unsigned TXB1D07:1;
  unsigned TXB1D10:1;
  unsigned TXB1D11:1;
  unsigned TXB1D12:1;
  unsigned TXB1D13:1;
  unsigned TXB1D14:1;
  unsigned TXB1D15:1;
  unsigned TXB1D16:1;
  unsigned TXB1D17:1;
} C1RX1B1BITS;
extern volatile C1RX1B1BITS C1RX1B1bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1B2 __attribute__((__sfr__));
typedef struct tagC1RX1B2BITS {
  unsigned TXB1D20:1;
  unsigned TXB1D21:1;
  unsigned TXB1D22:1;
  unsigned TXB1D23:1;
  unsigned TXB1D24:1;
  unsigned TXB1D25:1;
  unsigned TXB1D26:1;
  unsigned TXB1D27:1;
  unsigned TXB1D30:1;
  unsigned TXB1D31:1;
  unsigned TXB1D32:1;
  unsigned TXB1D33:1;
  unsigned TXB1D34:1;
  unsigned TXB1D35:1;
  unsigned TXB1D36:1;
  unsigned TXB1D37:1;
} C1RX1B2BITS;
extern volatile C1RX1B2BITS C1RX1B2bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1B3 __attribute__((__sfr__));
typedef struct tagC1RX1B3BITS {
  unsigned TXB1D40:1;
  unsigned TXB1D41:1;
  unsigned TXB1D42:1;
  unsigned TXB1D43:1;
  unsigned TXB1D44:1;
  unsigned TXB1D45:1;
  unsigned TXB1D46:1;
  unsigned TXB1D47:1;
  unsigned TXB1D50:1;
  unsigned TXB1D51:1;
  unsigned TXB1D52:1;
  unsigned TXB1D53:1;
  unsigned TXB1D54:1;
  unsigned TXB1D55:1;
  unsigned TXB1D56:1;
  unsigned TXB1D57:1;
} C1RX1B3BITS;
extern volatile C1RX1B3BITS C1RX1B3bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1B4 __attribute__((__sfr__));
typedef struct tagC1RX1B4BITS {
  unsigned TXB1D60:1;
  unsigned TXB1D61:1;
  unsigned TXB1D62:1;
  unsigned TXB1D63:1;
  unsigned TXB1D64:1;
  unsigned TXB1D65:1;
  unsigned TXB1D66:1;
  unsigned TXB1D67:1;
  unsigned TXB1D70:1;
  unsigned TXB1D71:1;
  unsigned TXB1D72:1;
  unsigned TXB1D73:1;
  unsigned TXB1D74:1;
  unsigned TXB1D75:1;
  unsigned TXB1D76:1;
  unsigned TXB1D77:1;
} C1RX1B4BITS;
extern volatile C1RX1B4BITS C1RX1B4bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX1CON __attribute__((__sfr__));
__extension__ typedef struct tagC1RX1CONBITS {
  union {
    struct {
      unsigned FILHIT:3;
      unsigned RXRTRRO:1;
      unsigned :3;
      unsigned RXFUL:1;
    };
    struct {
      unsigned FILHIT0:1;
      unsigned FILHIT1:1;
      unsigned FILHIT2:1;
    };
  };
} C1RX1CONBITS;
extern volatile C1RX1CONBITS C1RX1CONbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0SID __attribute__((__sfr__));
__extension__ typedef struct tagC1RX0SIDBITS {
  union {
    struct {
      unsigned RXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C1RX0SIDBITS;
extern volatile C1RX0SIDBITS C1RX0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0EID __attribute__((__sfr__));
typedef struct tagC1RX0EIDBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C1RX0EIDBITS;
extern volatile C1RX0EIDBITS C1RX0EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0DLC __attribute__((__sfr__));
__extension__ typedef struct tagC1RX0DLCBITS {
  union {
    struct {
      unsigned DLC:4;
      unsigned RXRB0:1;
      unsigned :3;
      unsigned RXRB1:1;
      unsigned RXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C1RX0DLCBITS;
extern volatile C1RX0DLCBITS C1RX0DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0B1 __attribute__((__sfr__));
typedef struct tagC1RX0B1BITS {
  unsigned TXB0D00:1;
  unsigned TXB0D01:1;
  unsigned TXB0D02:1;
  unsigned TXB0D03:1;
  unsigned TXB0D04:1;
  unsigned TXB0D05:1;
  unsigned TXB0D06:1;
  unsigned TXB0D07:1;
  unsigned TXB0D10:1;
  unsigned TXB0D11:1;
  unsigned TXB0D12:1;
  unsigned TXB0D13:1;
  unsigned TXB0D14:1;
  unsigned TXB0D15:1;
  unsigned TXB0D16:1;
  unsigned TXB0D17:1;
} C1RX0B1BITS;
extern volatile C1RX0B1BITS C1RX0B1bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0B2 __attribute__((__sfr__));
typedef struct tagC1RX0B2BITS {
  unsigned TXB0D20:1;
  unsigned TXB0D21:1;
  unsigned TXB0D22:1;
  unsigned TXB0D23:1;
  unsigned TXB0D24:1;
  unsigned TXB0D25:1;
  unsigned TXB0D26:1;
  unsigned TXB0D27:1;
  unsigned TXB0D30:1;
  unsigned TXB0D31:1;
  unsigned TXB0D32:1;
  unsigned TXB0D33:1;
  unsigned TXB0D34:1;
  unsigned TXB0D35:1;
  unsigned TXB0D36:1;
  unsigned TXB0D37:1;
} C1RX0B2BITS;
extern volatile C1RX0B2BITS C1RX0B2bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0B3 __attribute__((__sfr__));
typedef struct tagC1RX0B3BITS {
  unsigned TXB0D40:1;
  unsigned TXB0D41:1;
  unsigned TXB0D42:1;
  unsigned TXB0D43:1;
  unsigned TXB0D44:1;
  unsigned TXB0D45:1;
  unsigned TXB0D46:1;
  unsigned TXB0D47:1;
  unsigned TXB0D50:1;
  unsigned TXB0D51:1;
  unsigned TXB0D52:1;
  unsigned TXB0D53:1;
  unsigned TXB0D54:1;
  unsigned TXB0D55:1;
  unsigned TXB0D56:1;
  unsigned TXB0D57:1;
} C1RX0B3BITS;
extern volatile C1RX0B3BITS C1RX0B3bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0B4 __attribute__((__sfr__));
typedef struct tagC1RX0B4BITS {
  unsigned TXB0D60:1;
  unsigned TXB0D61:1;
  unsigned TXB0D62:1;
  unsigned TXB0D63:1;
  unsigned TXB0D64:1;
  unsigned TXB0D65:1;
  unsigned TXB0D66:1;
  unsigned TXB0D67:1;
  unsigned TXB0D70:1;
  unsigned TXB0D71:1;
  unsigned TXB0D72:1;
  unsigned TXB0D73:1;
  unsigned TXB0D74:1;
  unsigned TXB0D75:1;
  unsigned TXB0D76:1;
  unsigned TXB0D77:1;
} C1RX0B4BITS;
extern volatile C1RX0B4BITS C1RX0B4bits __attribute__((__sfr__));

extern volatile unsigned int  C1RX0CON __attribute__((__sfr__));
__extension__ typedef struct tagC1RX0CONBITS {
  union {
    struct {
      unsigned FILHIT0:1;
      unsigned JTOFF:1;
      unsigned DBEN:1;
      unsigned RXRTRRO:1;
      unsigned :3;
      unsigned RXFUL:1;
    };
    struct {
      unsigned FILHIT:1;
      unsigned :1;
      unsigned RXB0DBEN:1;
    };
  };
} C1RX0CONBITS;
extern volatile C1RX0CONBITS C1RX0CONbits __attribute__((__sfr__));

extern volatile unsigned int  C1CTRL __attribute__((__sfr__));
__extension__ typedef struct tagC1CTRLBITS {
  union {
    struct {
      unsigned :1;
      unsigned ICODE:3;
      unsigned :1;
      unsigned OPMODE:3;
      unsigned REQOP:3;
      unsigned CANCKS:1;
      unsigned ABAT:1;
      unsigned CANSIDL:1;
      unsigned CANFRZ:1;
      unsigned CANCAP:1;
    };
    struct {
      unsigned :1;
      unsigned ICODE0:1;
      unsigned ICODE1:1;
      unsigned ICODE2:1;
      unsigned :1;
      unsigned OPMODE0:1;
      unsigned OPMODE1:1;
      unsigned OPMODE2:1;
      unsigned REQOP0:1;
      unsigned REQOP1:1;
      unsigned REQOP2:1;
      unsigned :2;
      unsigned CSIDL:1;
    };
  };
} C1CTRLBITS;
extern volatile C1CTRLBITS C1CTRLbits __attribute__((__sfr__));

extern volatile unsigned int  C1CFG1 __attribute__((__sfr__));
__extension__ typedef struct tagC1CFG1BITS {
  union {
    struct {
      unsigned BRP:6;
      unsigned SJW:2;
    };
    struct {
      unsigned BRP0:1;
      unsigned BRP1:1;
      unsigned BRP2:1;
      unsigned BRP3:1;
      unsigned BRP4:1;
      unsigned BRP5:1;
      unsigned SJW0:1;
      unsigned SJW1:1;
    };
  };
} C1CFG1BITS;
extern volatile C1CFG1BITS C1CFG1bits __attribute__((__sfr__));

extern volatile unsigned int  C1CFG2 __attribute__((__sfr__));
__extension__ typedef struct tagC1CFG2BITS {
  union {
    struct {
      unsigned PRSEG:3;
      unsigned SEG1PH:3;
      unsigned SAM:1;
      unsigned SEG2PHTS:1;
      unsigned SEG2PH:3;
      unsigned :3;
      unsigned WAKFIL:1;
    };
    struct {
      unsigned PRSEG0:1;
      unsigned PRSEG1:1;
      unsigned PRSEG2:1;
      unsigned SEG1PH0:1;
      unsigned SEG1PH1:1;
      unsigned SEG1PH2:1;
      unsigned :2;
      unsigned SEG2PH0:1;
      unsigned SEG2PH1:1;
      unsigned SEG2PH2:1;
    };
  };
} C1CFG2BITS;
extern volatile C1CFG2BITS C1CFG2bits __attribute__((__sfr__));

extern volatile unsigned int  C1INTF __attribute__((__sfr__));
__extension__ typedef struct tagC1INTFBITS {
  union {
    struct {
      unsigned RXB0IF:1;
      unsigned RXB1IF:1;
      unsigned TXB0IF:1;
      unsigned TXB1IF:1;
      unsigned TXB2IF:1;
      unsigned ERRIF:1;
      unsigned WAKIF:1;
      unsigned IRXIF:1;
      unsigned EWARN:1;
      unsigned RXWARN:1;
      unsigned TXWARN:1;
      unsigned RXBP:1;
      unsigned TXBP:1;
      unsigned TXBO:1;
      unsigned RXB1OVFL:1;
      unsigned RXB0OVFL:1;
    };
    struct {
      unsigned RX0IF:1;
      unsigned RX1IF:1;
      unsigned TX0IF:1;
      unsigned TX1IF:1;
      unsigned TX2IF:1;
      unsigned :2;
      unsigned IVRIF:1;
      unsigned :1;
      unsigned RXWAR:1;
      unsigned TXWAR:1;
      unsigned RXEP:1;
      unsigned TXEP:1;
      unsigned :1;
      unsigned RX1OVR:1;
      unsigned RX0OVR:1;
    };
  };
} C1INTFBITS;
extern volatile C1INTFBITS C1INTFbits __attribute__((__sfr__));

extern volatile unsigned int  C1INTE __attribute__((__sfr__));
__extension__ typedef struct tagC1INTEBITS {
  union {
    struct {
      unsigned RXB0IE:1;
      unsigned RXB1IE:1;
      unsigned TXB0IE:1;
      unsigned TXB1IE:1;
      unsigned TXB2IE:1;
      unsigned ERRIE:1;
      unsigned WAKIE:1;
      unsigned IRXIE:1;
    };
    struct {
      unsigned RX0IE:1;
      unsigned RX1IE:1;
      unsigned TX0IE:1;
      unsigned TX1IE:1;
      unsigned TX2IE:1;
      unsigned :2;
      unsigned IVRIE:1;
    };
  };
} C1INTEBITS;
extern volatile C1INTEBITS C1INTEbits __attribute__((__sfr__));

extern volatile unsigned int  C1EC __attribute__((__sfr__));
typedef struct tagC1ECBITS {
  unsigned REC0:1;
  unsigned REC1:1;
  unsigned REC2:1;
  unsigned REC3:1;
  unsigned REC4:1;
  unsigned REC5:1;
  unsigned REC6:1;
  unsigned REC7:1;
  unsigned TEC0:1;
  unsigned TEC1:1;
  unsigned TEC2:1;
  unsigned TEC3:1;
  unsigned TEC4:1;
  unsigned TEC5:1;
  unsigned TEC6:1;
  unsigned TEC7:1;
} C1ECBITS;
extern volatile C1ECBITS C1ECbits __attribute__((__sfr__));

extern volatile unsigned char C1RERRCNT __attribute__((__sfr__));
extern volatile unsigned char C1TERRCNT __attribute__((__sfr__));

/* SFR blocks for each CAN module */
extern volatile CAN CAN2 __attribute__((__sfr__));

extern volatile unsigned int  C2RXF0SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF0SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF0SIDBITS;
extern volatile C2RXF0SIDBITS C2RXF0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF0EIDH __attribute__((__sfr__));
typedef struct tagC2RXF0EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF0EIDHBITS;
extern volatile C2RXF0EIDHBITS C2RXF0EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF0EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF0EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF0EIDLBITS;
extern volatile C2RXF0EIDLBITS C2RXF0EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF1SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF1SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF1SIDBITS;
extern volatile C2RXF1SIDBITS C2RXF1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF1EIDH __attribute__((__sfr__));
typedef struct tagC2RXF1EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF1EIDHBITS;
extern volatile C2RXF1EIDHBITS C2RXF1EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF1EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF1EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF1EIDLBITS;
extern volatile C2RXF1EIDLBITS C2RXF1EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF2SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF2SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF2SIDBITS;
extern volatile C2RXF2SIDBITS C2RXF2SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF2EIDH __attribute__((__sfr__));
typedef struct tagC2RXF2EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF2EIDHBITS;
extern volatile C2RXF2EIDHBITS C2RXF2EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF2EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF2EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF2EIDLBITS;
extern volatile C2RXF2EIDLBITS C2RXF2EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF3SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF3SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF3SIDBITS;
extern volatile C2RXF3SIDBITS C2RXF3SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF3EIDH __attribute__((__sfr__));
typedef struct tagC2RXF3EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF3EIDHBITS;
extern volatile C2RXF3EIDHBITS C2RXF3EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF3EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF3EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF3EIDLBITS;
extern volatile C2RXF3EIDLBITS C2RXF3EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF4SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF4SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF4SIDBITS;
extern volatile C2RXF4SIDBITS C2RXF4SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF4EIDH __attribute__((__sfr__));
typedef struct tagC2RXF4EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF4EIDHBITS;
extern volatile C2RXF4EIDHBITS C2RXF4EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF4EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF4EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF4EIDLBITS;
extern volatile C2RXF4EIDLBITS C2RXF4EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF5SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF5SIDBITS {
  union {
    struct {
      unsigned EXIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXF5SIDBITS;
extern volatile C2RXF5SIDBITS C2RXF5SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF5EIDH __attribute__((__sfr__));
typedef struct tagC2RXF5EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXF5EIDHBITS;
extern volatile C2RXF5EIDHBITS C2RXF5EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXF5EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXF5EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXF5EIDLBITS;
extern volatile C2RXF5EIDLBITS C2RXF5EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM0SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXM0SIDBITS {
  union {
    struct {
      unsigned MIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXM0SIDBITS;
extern volatile C2RXM0SIDBITS C2RXM0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM0EIDH __attribute__((__sfr__));
typedef struct tagC2RXM0EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXM0EIDHBITS;
extern volatile C2RXM0EIDHBITS C2RXM0EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM0EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXM0EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXM0EIDLBITS;
extern volatile C2RXM0EIDLBITS C2RXM0EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM1SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RXM1SIDBITS {
  union {
    struct {
      unsigned MIDE:1;
      unsigned :1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RXM1SIDBITS;
extern volatile C2RXM1SIDBITS C2RXM1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM1EIDH __attribute__((__sfr__));
typedef struct tagC2RXM1EIDHBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RXM1EIDHBITS;
extern volatile C2RXM1EIDHBITS C2RXM1EIDHbits __attribute__((__sfr__));

extern volatile unsigned int  C2RXM1EIDL __attribute__((__sfr__));
__extension__ typedef struct tagC2RXM1EIDLBITS {
  union {
    struct {
      unsigned :10;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2RXM1EIDLBITS;
extern volatile C2RXM1EIDLBITS C2RXM1EIDLbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2SID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX2SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C2TX2SIDBITS;
extern volatile C2TX2SIDBITS C2TX2SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2EID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX2EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C2TX2EIDBITS;
extern volatile C2TX2EIDBITS C2TX2EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2DLC __attribute__((__sfr__));
__extension__ typedef struct tagC2TX2DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned :3;
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C2TX2DLCBITS;
extern volatile C2TX2DLCBITS C2TX2DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2B1 __attribute__((__sfr__));
typedef struct tagC2TX2B1BITS {
  unsigned TXB2D00:1;
  unsigned TXB2D01:1;
  unsigned TXB2D02:1;
  unsigned TXB2D03:1;
  unsigned TXB2D04:1;
  unsigned TXB2D05:1;
  unsigned TXB2D06:1;
  unsigned TXB2D07:1;
  unsigned TXB2D10:1;
  unsigned TXB2D11:1;
  unsigned TXB2D12:1;
  unsigned TXB2D13:1;
  unsigned TXB2D14:1;
  unsigned TXB2D15:1;
  unsigned TXB2D16:1;
  unsigned TXB2D17:1;
} C2TX2B1BITS;
extern volatile C2TX2B1BITS C2TX2B1bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2B2 __attribute__((__sfr__));
typedef struct tagC2TX2B2BITS {
  unsigned TXB2D20:1;
  unsigned TXB2D21:1;
  unsigned TXB2D22:1;
  unsigned TXB2D23:1;
  unsigned TXB2D24:1;
  unsigned TXB2D25:1;
  unsigned TXB2D26:1;
  unsigned TXB2D27:1;
  unsigned TXB2D30:1;
  unsigned TXB2D31:1;
  unsigned TXB2D32:1;
  unsigned TXB2D33:1;
  unsigned TXB2D34:1;
  unsigned TXB2D35:1;
  unsigned TXB2D36:1;
  unsigned TXB2D37:1;
} C2TX2B2BITS;
extern volatile C2TX2B2BITS C2TX2B2bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2B3 __attribute__((__sfr__));
typedef struct tagC2TX2B3BITS {
  unsigned TXB2D40:1;
  unsigned TXB2D41:1;
  unsigned TXB2D42:1;
  unsigned TXB2D43:1;
  unsigned TXB2D44:1;
  unsigned TXB2D45:1;
  unsigned TXB2D46:1;
  unsigned TXB2D47:1;
  unsigned TXB2D50:1;
  unsigned TXB2D51:1;
  unsigned TXB2D52:1;
  unsigned TXB2D53:1;
  unsigned TXB2D54:1;
  unsigned TXB2D55:1;
  unsigned TXB2D56:1;
  unsigned TXB2D57:1;
} C2TX2B3BITS;
extern volatile C2TX2B3BITS C2TX2B3bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2B4 __attribute__((__sfr__));
typedef struct tagC2TX2B4BITS {
  unsigned TXB2D60:1;
  unsigned TXB2D61:1;
  unsigned TXB2D62:1;
  unsigned TXB2D63:1;
  unsigned TXB2D64:1;
  unsigned TXB2D65:1;
  unsigned TXB2D66:1;
  unsigned TXB2D67:1;
  unsigned TXB2D70:1;
  unsigned TXB2D71:1;
  unsigned TXB2D72:1;
  unsigned TXB2D73:1;
  unsigned TXB2D74:1;
  unsigned TXB2D75:1;
  unsigned TXB2D76:1;
  unsigned TXB2D77:1;
} C2TX2B4BITS;
extern volatile C2TX2B4BITS C2TX2B4bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX2CON __attribute__((__sfr__));
__extension__ typedef struct tagC2TX2CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C2TX2CONBITS;
extern volatile C2TX2CONBITS C2TX2CONbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1SID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX1SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C2TX1SIDBITS;
extern volatile C2TX1SIDBITS C2TX1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1EID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX1EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C2TX1EIDBITS;
extern volatile C2TX1EIDBITS C2TX1EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1DLC __attribute__((__sfr__));
__extension__ typedef struct tagC2TX1DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned :3;
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C2TX1DLCBITS;
extern volatile C2TX1DLCBITS C2TX1DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1B1 __attribute__((__sfr__));
typedef struct tagC2TX1B1BITS {
  unsigned TXB1D00:1;
  unsigned TXB1D01:1;
  unsigned TXB1D02:1;
  unsigned TXB1D03:1;
  unsigned TXB1D04:1;
  unsigned TXB1D05:1;
  unsigned TXB1D06:1;
  unsigned TXB1D07:1;
  unsigned TXB1D10:1;
  unsigned TXB1D11:1;
  unsigned TXB1D12:1;
  unsigned TXB1D13:1;
  unsigned TXB1D14:1;
  unsigned TXB1D15:1;
  unsigned TXB1D16:1;
  unsigned TXB1D17:1;
} C2TX1B1BITS;
extern volatile C2TX1B1BITS C2TX1B1bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1B2 __attribute__((__sfr__));
typedef struct tagC2TX1B2BITS {
  unsigned TXB1D20:1;
  unsigned TXB1D21:1;
  unsigned TXB1D22:1;
  unsigned TXB1D23:1;
  unsigned TXB1D24:1;
  unsigned TXB1D25:1;
  unsigned TXB1D26:1;
  unsigned TXB1D27:1;
  unsigned TXB1D30:1;
  unsigned TXB1D31:1;
  unsigned TXB1D32:1;
  unsigned TXB1D33:1;
  unsigned TXB1D34:1;
  unsigned TXB1D35:1;
  unsigned TXB1D36:1;
  unsigned TXB1D37:1;
} C2TX1B2BITS;
extern volatile C2TX1B2BITS C2TX1B2bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1B3 __attribute__((__sfr__));
typedef struct tagC2TX1B3BITS {
  unsigned TXB1D40:1;
  unsigned TXB1D41:1;
  unsigned TXB1D42:1;
  unsigned TXB1D43:1;
  unsigned TXB1D44:1;
  unsigned TXB1D45:1;
  unsigned TXB1D46:1;
  unsigned TXB1D47:1;
  unsigned TXB1D50:1;
  unsigned TXB1D51:1;
  unsigned TXB1D52:1;
  unsigned TXB1D53:1;
  unsigned TXB1D54:1;
  unsigned TXB1D55:1;
  unsigned TXB1D56:1;
  unsigned TXB1D57:1;
} C2TX1B3BITS;
extern volatile C2TX1B3BITS C2TX1B3bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1B4 __attribute__((__sfr__));
typedef struct tagC2TX1B4BITS {
  unsigned TXB1D60:1;
  unsigned TXB1D61:1;
  unsigned TXB1D62:1;
  unsigned TXB1D63:1;
  unsigned TXB1D64:1;
  unsigned TXB1D65:1;
  unsigned TXB1D66:1;
  unsigned TXB1D67:1;
  unsigned TXB1D70:1;
  unsigned TXB1D71:1;
  unsigned TXB1D72:1;
  unsigned TXB1D73:1;
  unsigned TXB1D74:1;
  unsigned TXB1D75:1;
  unsigned TXB1D76:1;
  unsigned TXB1D77:1;
} C2TX1B4BITS;
extern volatile C2TX1B4BITS C2TX1B4bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX1CON __attribute__((__sfr__));
__extension__ typedef struct tagC2TX1CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C2TX1CONBITS;
extern volatile C2TX1CONBITS C2TX1CONbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0SID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX0SIDBITS {
  union {
    struct {
      unsigned TXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned :3;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID5_0:6;
      unsigned :3;
      unsigned SID10_6:5;
    };
  };
} C2TX0SIDBITS;
extern volatile C2TX0SIDBITS C2TX0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0EID __attribute__((__sfr__));
__extension__ typedef struct tagC2TX0EIDBITS {
  union {
    struct {
      unsigned EID6:1;
      unsigned EID7:1;
      unsigned EID8:1;
      unsigned EID9:1;
      unsigned EID10:1;
      unsigned EID11:1;
      unsigned EID12:1;
      unsigned EID13:1;
      unsigned :4;
      unsigned EID14:1;
      unsigned EID15:1;
      unsigned EID16:1;
      unsigned EID17:1;
    };
    struct {
      unsigned EID13_6:8;
      unsigned :4;
      unsigned EID17_14:4;
    };
  };
} C2TX0EIDBITS;
extern volatile C2TX0EIDBITS C2TX0EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0DLC __attribute__((__sfr__));
__extension__ typedef struct tagC2TX0DLCBITS {
  union {
    struct {
      unsigned :3;
      unsigned DLC:4;
      unsigned TXRB0:1;
      unsigned TXRB1:1;
      unsigned TXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
  };
} C2TX0DLCBITS;
extern volatile C2TX0DLCBITS C2TX0DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0B1 __attribute__((__sfr__));
typedef struct tagC2TX0B1BITS {
  unsigned TXB0D00:1;
  unsigned TXB0D01:1;
  unsigned TXB0D02:1;
  unsigned TXB0D03:1;
  unsigned TXB0D04:1;
  unsigned TXB0D05:1;
  unsigned TXB0D06:1;
  unsigned TXB0D07:1;
  unsigned TXB0D10:1;
  unsigned TXB0D11:1;
  unsigned TXB0D12:1;
  unsigned TXB0D13:1;
  unsigned TXB0D14:1;
  unsigned TXB0D15:1;
  unsigned TXB0D16:1;
  unsigned TXB0D17:1;
} C2TX0B1BITS;
extern volatile C2TX0B1BITS C2TX0B1bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0B2 __attribute__((__sfr__));
typedef struct tagC2TX0B2BITS {
  unsigned TXB0D20:1;
  unsigned TXB0D21:1;
  unsigned TXB0D22:1;
  unsigned TXB0D23:1;
  unsigned TXB0D24:1;
  unsigned TXB0D25:1;
  unsigned TXB0D26:1;
  unsigned TXB0D27:1;
  unsigned TXB0D30:1;
  unsigned TXB0D31:1;
  unsigned TXB0D32:1;
  unsigned TXB0D33:1;
  unsigned TXB0D34:1;
  unsigned TXB0D35:1;
  unsigned TXB0D36:1;
  unsigned TXB0D37:1;
} C2TX0B2BITS;
extern volatile C2TX0B2BITS C2TX0B2bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0B3 __attribute__((__sfr__));
typedef struct tagC2TX0B3BITS {
  unsigned TXB0D40:1;
  unsigned TXB0D41:1;
  unsigned TXB0D42:1;
  unsigned TXB0D43:1;
  unsigned TXB0D44:1;
  unsigned TXB0D45:1;
  unsigned TXB0D46:1;
  unsigned TXB0D47:1;
  unsigned TXB0D50:1;
  unsigned TXB0D51:1;
  unsigned TXB0D52:1;
  unsigned TXB0D53:1;
  unsigned TXB0D54:1;
  unsigned TXB0D55:1;
  unsigned TXB0D56:1;
  unsigned TXB0D57:1;
} C2TX0B3BITS;
extern volatile C2TX0B3BITS C2TX0B3bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0B4 __attribute__((__sfr__));
typedef struct tagC2TX0B4BITS {
  unsigned TXB0D60:1;
  unsigned TXB0D61:1;
  unsigned TXB0D62:1;
  unsigned TXB0D63:1;
  unsigned TXB0D64:1;
  unsigned TXB0D65:1;
  unsigned TXB0D66:1;
  unsigned TXB0D67:1;
  unsigned TXB0D70:1;
  unsigned TXB0D71:1;
  unsigned TXB0D72:1;
  unsigned TXB0D73:1;
  unsigned TXB0D74:1;
  unsigned TXB0D75:1;
  unsigned TXB0D76:1;
  unsigned TXB0D77:1;
} C2TX0B4BITS;
extern volatile C2TX0B4BITS C2TX0B4bits __attribute__((__sfr__));

extern volatile unsigned int  C2TX0CON __attribute__((__sfr__));
__extension__ typedef struct tagC2TX0CONBITS {
  union {
    struct {
      unsigned TXPRI:2;
      unsigned :1;
      unsigned TXREQ:1;
      unsigned TXERR:1;
      unsigned TXLARB:1;
      unsigned TXABT:1;
    };
    struct {
      unsigned TXPRI0:1;
      unsigned TXPRI1:1;
    };
  };
} C2TX0CONBITS;
extern volatile C2TX0CONBITS C2TX0CONbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RX1SIDBITS {
  union {
    struct {
      unsigned RXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RX1SIDBITS;
extern volatile C2RX1SIDBITS C2RX1SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1EID __attribute__((__sfr__));
typedef struct tagC2RX1EIDBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RX1EIDBITS;
extern volatile C2RX1EIDBITS C2RX1EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1DLC __attribute__((__sfr__));
__extension__ typedef struct tagC2RX1DLCBITS {
  union {
    struct {
      unsigned DLC:4;
      unsigned RXRB0:1;
      unsigned :3;
      unsigned RXRB1:1;
      unsigned RXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C2RX1DLCBITS;
extern volatile C2RX1DLCBITS C2RX1DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1B1 __attribute__((__sfr__));
typedef struct tagC2RX1B1BITS {
  unsigned TXB1D00:1;
  unsigned TXB1D01:1;
  unsigned TXB1D02:1;
  unsigned TXB1D03:1;
  unsigned TXB1D04:1;
  unsigned TXB1D05:1;
  unsigned TXB1D06:1;
  unsigned TXB1D07:1;
  unsigned TXB1D10:1;
  unsigned TXB1D11:1;
  unsigned TXB1D12:1;
  unsigned TXB1D13:1;
  unsigned TXB1D14:1;
  unsigned TXB1D15:1;
  unsigned TXB1D16:1;
  unsigned TXB1D17:1;
} C2RX1B1BITS;
extern volatile C2RX1B1BITS C2RX1B1bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1B2 __attribute__((__sfr__));
typedef struct tagC2RX1B2BITS {
  unsigned TXB1D20:1;
  unsigned TXB1D21:1;
  unsigned TXB1D22:1;
  unsigned TXB1D23:1;
  unsigned TXB1D24:1;
  unsigned TXB1D25:1;
  unsigned TXB1D26:1;
  unsigned TXB1D27:1;
  unsigned TXB1D30:1;
  unsigned TXB1D31:1;
  unsigned TXB1D32:1;
  unsigned TXB1D33:1;
  unsigned TXB1D34:1;
  unsigned TXB1D35:1;
  unsigned TXB1D36:1;
  unsigned TXB1D37:1;
} C2RX1B2BITS;
extern volatile C2RX1B2BITS C2RX1B2bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1B3 __attribute__((__sfr__));
typedef struct tagC2RX1B3BITS {
  unsigned TXB1D40:1;
  unsigned TXB1D41:1;
  unsigned TXB1D42:1;
  unsigned TXB1D43:1;
  unsigned TXB1D44:1;
  unsigned TXB1D45:1;
  unsigned TXB1D46:1;
  unsigned TXB1D47:1;
  unsigned TXB1D50:1;
  unsigned TXB1D51:1;
  unsigned TXB1D52:1;
  unsigned TXB1D53:1;
  unsigned TXB1D54:1;
  unsigned TXB1D55:1;
  unsigned TXB1D56:1;
  unsigned TXB1D57:1;
} C2RX1B3BITS;
extern volatile C2RX1B3BITS C2RX1B3bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1B4 __attribute__((__sfr__));
typedef struct tagC2RX1B4BITS {
  unsigned TXB1D60:1;
  unsigned TXB1D61:1;
  unsigned TXB1D62:1;
  unsigned TXB1D63:1;
  unsigned TXB1D64:1;
  unsigned TXB1D65:1;
  unsigned TXB1D66:1;
  unsigned TXB1D67:1;
  unsigned TXB1D70:1;
  unsigned TXB1D71:1;
  unsigned TXB1D72:1;
  unsigned TXB1D73:1;
  unsigned TXB1D74:1;
  unsigned TXB1D75:1;
  unsigned TXB1D76:1;
  unsigned TXB1D77:1;
} C2RX1B4BITS;
extern volatile C2RX1B4BITS C2RX1B4bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX1CON __attribute__((__sfr__));
__extension__ typedef struct tagC2RX1CONBITS {
  union {
    struct {
      unsigned FILHIT:3;
      unsigned RXRTRRO:1;
      unsigned :3;
      unsigned RXFUL:1;
    };
    struct {
      unsigned FILHIT0:1;
      unsigned FILHIT1:1;
      unsigned FILHIT2:1;
    };
  };
} C2RX1CONBITS;
extern volatile C2RX1CONBITS C2RX1CONbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0SID __attribute__((__sfr__));
__extension__ typedef struct tagC2RX0SIDBITS {
  union {
    struct {
      unsigned RXIDE:1;
      unsigned SRR:1;
      unsigned SID0:1;
      unsigned SID1:1;
      unsigned SID2:1;
      unsigned SID3:1;
      unsigned SID4:1;
      unsigned SID5:1;
      unsigned SID6:1;
      unsigned SID7:1;
      unsigned SID8:1;
      unsigned SID9:1;
      unsigned SID10:1;
    };
    struct {
      unsigned :2;
      unsigned SID:11;
    };
  };
} C2RX0SIDBITS;
extern volatile C2RX0SIDBITS C2RX0SIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0EID __attribute__((__sfr__));
typedef struct tagC2RX0EIDBITS {
  unsigned EID6:1;
  unsigned EID7:1;
  unsigned EID8:1;
  unsigned EID9:1;
  unsigned EID10:1;
  unsigned EID11:1;
  unsigned EID12:1;
  unsigned EID13:1;
  unsigned EID14:1;
  unsigned EID15:1;
  unsigned EID16:1;
  unsigned EID17:1;
} C2RX0EIDBITS;
extern volatile C2RX0EIDBITS C2RX0EIDbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0DLC __attribute__((__sfr__));
__extension__ typedef struct tagC2RX0DLCBITS {
  union {
    struct {
      unsigned DLC:4;
      unsigned RXRB0:1;
      unsigned :3;
      unsigned RXRB1:1;
      unsigned RXRTR:1;
      unsigned EID0:1;
      unsigned EID1:1;
      unsigned EID2:1;
      unsigned EID3:1;
      unsigned EID4:1;
      unsigned EID5:1;
    };
    struct {
      unsigned :10;
      unsigned EID5_0:6;
    };
    struct {
      unsigned DLC0:1;
      unsigned DLC1:1;
      unsigned DLC2:1;
      unsigned DLC3:1;
    };
  };
} C2RX0DLCBITS;
extern volatile C2RX0DLCBITS C2RX0DLCbits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0B1 __attribute__((__sfr__));
typedef struct tagC2RX0B1BITS {
  unsigned TXB0D00:1;
  unsigned TXB0D01:1;
  unsigned TXB0D02:1;
  unsigned TXB0D03:1;
  unsigned TXB0D04:1;
  unsigned TXB0D05:1;
  unsigned TXB0D06:1;
  unsigned TXB0D07:1;
  unsigned TXB0D10:1;
  unsigned TXB0D11:1;
  unsigned TXB0D12:1;
  unsigned TXB0D13:1;
  unsigned TXB0D14:1;
  unsigned TXB0D15:1;
  unsigned TXB0D16:1;
  unsigned TXB0D17:1;
} C2RX0B1BITS;
extern volatile C2RX0B1BITS C2RX0B1bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0B2 __attribute__((__sfr__));
typedef struct tagC2RX0B2BITS {
  unsigned TXB0D20:1;
  unsigned TXB0D21:1;
  unsigned TXB0D22:1;
  unsigned TXB0D23:1;
  unsigned TXB0D24:1;
  unsigned TXB0D25:1;
  unsigned TXB0D26:1;
  unsigned TXB0D27:1;
  unsigned TXB0D30:1;
  unsigned TXB0D31:1;
  unsigned TXB0D32:1;
  unsigned TXB0D33:1;
  unsigned TXB0D34:1;
  unsigned TXB0D35:1;
  unsigned TXB0D36:1;
  unsigned TXB0D37:1;
} C2RX0B2BITS;
extern volatile C2RX0B2BITS C2RX0B2bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0B3 __attribute__((__sfr__));
typedef struct tagC2RX0B3BITS {
  unsigned TXB0D40:1;
  unsigned TXB0D41:1;
  unsigned TXB0D42:1;
  unsigned TXB0D43:1;
  unsigned TXB0D44:1;
  unsigned TXB0D45:1;
  unsigned TXB0D46:1;
  unsigned TXB0D47:1;
  unsigned TXB0D50:1;
  unsigned TXB0D51:1;
  unsigned TXB0D52:1;
  unsigned TXB0D53:1;
  unsigned TXB0D54:1;
  unsigned TXB0D55:1;
  unsigned TXB0D56:1;
  unsigned TXB0D57:1;
} C2RX0B3BITS;
extern volatile C2RX0B3BITS C2RX0B3bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0B4 __attribute__((__sfr__));
typedef struct tagC2RX0B4BITS {
  unsigned TXB0D60:1;
  unsigned TXB0D61:1;
  unsigned TXB0D62:1;
  unsigned TXB0D63:1;
  unsigned TXB0D64:1;
  unsigned TXB0D65:1;
  unsigned TXB0D66:1;
  unsigned TXB0D67:1;
  unsigned TXB0D70:1;
  unsigned TXB0D71:1;
  unsigned TXB0D72:1;
  unsigned TXB0D73:1;
  unsigned TXB0D74:1;
  unsigned TXB0D75:1;
  unsigned TXB0D76:1;
  unsigned TXB0D77:1;
} C2RX0B4BITS;
extern volatile C2RX0B4BITS C2RX0B4bits __attribute__((__sfr__));

extern volatile unsigned int  C2RX0CON __attribute__((__sfr__));
__extension__ typedef struct tagC2RX0CONBITS {
  union {
    struct {
      unsigned FILHIT0:1;
      unsigned JTOFF:1;
      unsigned DBEN:1;
      unsigned RXRTRRO:1;
      unsigned :3;
      unsigned RXFUL:1;
    };
    struct {
      unsigned FILHIT:1;
      unsigned :1;
      unsigned RXB0DBEN:1;
    };
  };
} C2RX0CONBITS;
extern volatile C2RX0CONBITS C2RX0CONbits __attribute__((__sfr__));

extern volatile unsigned int  C2CTRL __attribute__((__sfr__));
__extension__ typedef struct tagC2CTRLBITS {
  union {
    struct {
      unsigned :1;
      unsigned ICODE:3;
      unsigned :1;
      unsigned OPMODE:3;
      unsigned REQOP:3;
      unsigned CANCKS:1;
      unsigned ABAT:1;
      unsigned CANSIDL:1;
      unsigned CANFRZ:1;
      unsigned CANCAP:1;
    };
    struct {
      unsigned :1;
      unsigned ICODE0:1;
      unsigned ICODE1:1;
      unsigned ICODE2:1;
      unsigned :1;
      unsigned OPMODE0:1;
      unsigned OPMODE1:1;
      unsigned OPMODE2:1;
      unsigned REQOP0:1;
      unsigned REQOP1:1;
      unsigned REQOP2:1;
      unsigned :2;
      unsigned CSIDL:1;
    };
  };
} C2CTRLBITS;
extern volatile C2CTRLBITS C2CTRLbits __attribute__((__sfr__));

extern volatile unsigned int  C2CFG1 __attribute__((__sfr__));
__extension__ typedef struct tagC2CFG1BITS {
  union {
    struct {
      unsigned BRP:6;
      unsigned SJW:2;
    };
    struct {
      unsigned BRP0:1;
      unsigned BRP1:1;
      unsigned BRP2:1;
      unsigned BRP3:1;
      unsigned BRP4:1;
      unsigned BRP5:1;
      unsigned SJW0:1;
      unsigned SJW1:1;
    };
  };
} C2CFG1BITS;
extern volatile C2CFG1BITS C2CFG1bits __attribute__((__sfr__));

extern volatile unsigned int  C2CFG2 __attribute__((__sfr__));
__extension__ typedef struct tagC2CFG2BITS {
  union {
    struct {
      unsigned PRSEG:3;
      unsigned SEG1PH:3;
      unsigned SAM:1;
      unsigned SEG2PHTS:1;
      unsigned SEG2PH:3;
      unsigned :3;
      unsigned WAKFIL:1;
    };
    struct {
      unsigned PRSEG0:1;
      unsigned PRSEG1:1;
      unsigned PRSEG2:1;
      unsigned SEG1PH0:1;
      unsigned SEG1PH1:1;
      unsigned SEG1PH2:1;
      unsigned :2;
      unsigned SEG2PH0:1;
      unsigned SEG2PH1:1;
      unsigned SEG2PH2:1;
    };
  };
} C2CFG2BITS;
extern volatile C2CFG2BITS C2CFG2bits __attribute__((__sfr__));

extern volatile unsigned int  C2INTF __attribute__((__sfr__));
__extension__ typedef struct tagC2INTFBITS {
  union {
    struct {
      unsigned RXB0IF:1;
      unsigned RXB1IF:1;
      unsigned TXB0IF:1;
      unsigned TXB1IF:1;
      unsigned TXB2IF:1;
      unsigned ERRIF:1;
      unsigned WAKIF:1;
      unsigned IRXIF:1;
      unsigned EWARN:1;
      unsigned RXWARN:1;
      unsigned TXWARN:1;
      unsigned RXBP:1;
      unsigned TXBP:1;
      unsigned TXBO:1;
      unsigned RXB1OVFL:1;
      unsigned RXB0OVFL:1;
    };
    struct {
      unsigned RX0IF:1;
      unsigned RX1IF:1;
      unsigned TX0IF:1;
      unsigned TX1IF:1;
      unsigned TX2IF:1;
      unsigned :2;
      unsigned IVRIF:1;
      unsigned :1;
      unsigned RXWAR:1;
      unsigned TXWAR:1;
      unsigned RXEP:1;
      unsigned TXEP:1;
      unsigned :1;
      unsigned RX1OVR:1;
      unsigned RX0OVR:1;
    };
  };
} C2INTFBITS;
extern volatile C2INTFBITS C2INTFbits __attribute__((__sfr__));

extern volatile unsigned int  C2INTE __attribute__((__sfr__));
__extension__ typedef struct tagC2INTEBITS {
  union {
    struct {
      unsigned RXB0IE:1;
      unsigned RXB1IE:1;
      unsigned TXB0IE:1;
      unsigned TXB1IE:1;
      unsigned TXB2IE:1;
      unsigned ERRIE:1;
      unsigned WAKIE:1;
      unsigned IRXIE:1;
    };
    struct {
      unsigned RX0IE:1;
      unsigned RX1IE:1;
      unsigned TX0IE:1;
      unsigned TX1IE:1;
      unsigned TX2IE:1;
      unsigned :2;
      unsigned IVRIE:1;
    };
  };
} C2INTEBITS;
extern volatile C2INTEBITS C2INTEbits __attribute__((__sfr__));

extern volatile unsigned int  C2EC __attribute__((__sfr__));
typedef struct tagC2ECBITS {
  unsigned REC0:1;
  unsigned REC1:1;
  unsigned REC2:1;
  unsigned REC3:1;
  unsigned REC4:1;
  unsigned REC5:1;
  unsigned REC6:1;
  unsigned REC7:1;
  unsigned TEC0:1;
  unsigned TEC1:1;
  unsigned TEC2:1;
  unsigned TEC3:1;
  unsigned TEC4:1;
  unsigned TEC5:1;
  unsigned TEC6:1;
  unsigned TEC7:1;
} C2ECBITS;
extern volatile C2ECBITS C2ECbits __attribute__((__sfr__));

extern volatile unsigned char C2RERRCNT __attribute__((__sfr__));
extern volatile unsigned char C2TERRCNT __attribute__((__sfr__));
extern volatile unsigned int  RCON __attribute__((__sfr__));
__extension__ typedef struct tagRCONBITS {
  union {
    struct {
      unsigned POR:1;
      unsigned BOR:1;
      unsigned IDLE:1;
      unsigned SLEEP:1;
      unsigned WDTO:1;
      unsigned SWDTEN:1;
      unsigned SWR:1;
      unsigned EXTR:1;
      unsigned LVDL:4;
      unsigned LVDEN:1;
      unsigned BGST:1;
      unsigned IOPUWR:1;
      unsigned TRAPR:1;
    };
    struct {
      unsigned :8;
      unsigned LVDL0:1;
      unsigned LVDL1:1;
      unsigned LVDL2:1;
      unsigned LVDL3:1;
    };
  };
} RCONBITS;
extern volatile RCONBITS RCONbits __attribute__((__sfr__));

extern volatile unsigned int  OSCCON __attribute__((__sfr__));
__extension__ typedef struct tagOSCCONBITS {
  union {
    struct {
      unsigned OSWEN:1;
      unsigned LPOSCEN:1;
      unsigned :1;
      unsigned CF:1;
      unsigned :1;
      unsigned LOCK:1;
      unsigned POST:2;
      unsigned NOSC:3;
      unsigned :1;
      unsigned COSC:3;
    };
    struct {
      unsigned :6;
      unsigned POST0:1;
      unsigned POST1:1;
      unsigned NOSC0:1;
      unsigned NOSC1:1;
      unsigned NOSC2:1;
      unsigned :1;
      unsigned COSC0:1;
      unsigned COSC1:1;
      unsigned COSC2:1;
    };
  };
} OSCCONBITS;
extern volatile OSCCONBITS OSCCONbits __attribute__((__sfr__));

extern volatile unsigned char OSCCONL __attribute__((__sfr__));
extern volatile unsigned char OSCCONH __attribute__((__sfr__));
extern volatile unsigned int  OSCTUN __attribute__((__sfr__));
typedef struct tagOSCTUNBITS {
  unsigned TUN0:1;
  unsigned TUN1:1;
  unsigned TUN2:1;
  unsigned TUN3:1;
} OSCTUNBITS;
extern volatile OSCTUNBITS OSCTUNbits __attribute__((__sfr__));

extern volatile unsigned int  BSRAM __attribute__((__sfr__));
typedef struct tagBSRAMBITS {
  unsigned RL_BSR:1;
  unsigned IR_BSR:1;
  unsigned IW_BSR:1;
} BSRAMBITS;
extern volatile BSRAMBITS BSRAMbits __attribute__((__sfr__));

extern volatile unsigned int  SSRAM __attribute__((__sfr__));
typedef struct tagSSRAMBITS {
  unsigned RL_SSR:1;
  unsigned IR_SSR:1;
  unsigned IW_SSR:1;
} SSRAMBITS;
extern volatile SSRAMBITS SSRAMbits __attribute__((__sfr__));

extern volatile unsigned int  NVMCON __attribute__((__sfr__));
__extension__ typedef struct tagNVMCONBITS {
  union {
    struct {
      unsigned PROGOP:7;
      unsigned :1;
      unsigned TWRI:1;
      unsigned :4;
      unsigned WRERR:1;
      unsigned WREN:1;
      unsigned WR:1;
    };
    struct {
      unsigned PROGOP0:1;
      unsigned PROGOP1:1;
      unsigned PROGOP2:1;
      unsigned PROGOP3:1;
      unsigned PROGOP4:1;
      unsigned PROGOP5:1;
      unsigned PROGOP6:1;
    };
  };
} NVMCONBITS;
extern volatile NVMCONBITS NVMCONbits __attribute__((__sfr__));

extern volatile unsigned int  NVMADR __attribute__((__sfr__));
extern volatile unsigned int  NVMADRU __attribute__((__sfr__));
typedef struct tagNVMADRUBITS {
  unsigned NVMADR:8;
} NVMADRUBITS;
extern volatile NVMADRUBITS NVMADRUbits __attribute__((__sfr__));

extern volatile unsigned int  NVMKEY __attribute__((__sfr__));
extern volatile unsigned int  PMD1 __attribute__((__sfr__));
typedef struct tagPMD1BITS {
  unsigned ADCMD:1;
  unsigned C1MD:1;
  unsigned C2MD:1;
  unsigned SPI1MD:1;
  unsigned SPI2MD:1;
  unsigned U1MD:1;
  unsigned U2MD:1;
  unsigned I2CMD:1;
  unsigned DCIMD:1;
  unsigned :2;
  unsigned T1MD:1;
  unsigned T2MD:1;
  unsigned T3MD:1;
  unsigned T4MD:1;
  unsigned T5MD:1;
} PMD1BITS;
extern volatile PMD1BITS PMD1bits __attribute__((__sfr__));

extern volatile unsigned int  PMD2 __attribute__((__sfr__));
typedef struct tagPMD2BITS {
  unsigned OC1MD:1;
  unsigned OC2MD:1;
  unsigned OC3MD:1;
  unsigned OC4MD:1;
  unsigned OC5MD:1;
  unsigned OC6MD:1;
  unsigned OC7MD:1;
  unsigned OC8MD:1;
  unsigned IC1MD:1;
  unsigned IC2MD:1;
  unsigned IC3MD:1;
  unsigned IC4MD:1;
  unsigned IC5MD:1;
  unsigned IC6MD:1;
  unsigned IC7MD:1;
  unsigned IC8MD:1;
} PMD2BITS;
extern volatile PMD2BITS PMD2bits __attribute__((__sfr__));



/* ---------------------------------------------------------- */
/* Defines for unique SFR bit names                           */
/* ---------------------------------------------------------- */

/* SR */
#define _C SRbits.C
#define _Z SRbits.Z
#define _OV SRbits.OV
#define _N SRbits.N
#define _RA SRbits.RA
#define _IPL SRbits.IPL
#define _DC SRbits.DC
#define _DA SRbits.DA
#define _SAB SRbits.SAB
#define _OAB SRbits.OAB
#define _SB SRbits.SB
#define _SA SRbits.SA
#define _OB SRbits.OB
#define _OA SRbits.OA
#define _IPL0 SRbits.IPL0
#define _IPL1 SRbits.IPL1
#define _IPL2 SRbits.IPL2

/* CORCON */
#define _IF CORCONbits.IF
#define _RND CORCONbits.RND
#define _PSV CORCONbits.PSV
#define _IPL3 CORCONbits.IPL3
#define _ACCSAT CORCONbits.ACCSAT
#define _SATDW CORCONbits.SATDW
#define _SATB CORCONbits.SATB
#define _SATA CORCONbits.SATA
#define _DL CORCONbits.DL
#define _EDT CORCONbits.EDT
#define _US CORCONbits.US
#define _DL0 CORCONbits.DL0
#define _DL1 CORCONbits.DL1
#define _DL2 CORCONbits.DL2

/* MODCON */
#define _XWM MODCONbits.XWM
#define _YWM MODCONbits.YWM
#define _BWM MODCONbits.BWM
#define _YMODEN MODCONbits.YMODEN
#define _XMODEN MODCONbits.XMODEN
#define _XWM0 MODCONbits.XWM0
#define _XWM1 MODCONbits.XWM1
#define _XWM2 MODCONbits.XWM2
#define _XWM3 MODCONbits.XWM3
#define _YWM0 MODCONbits.YWM0
#define _YWM1 MODCONbits.YWM1
#define _YWM2 MODCONbits.YWM2
#define _YWM3 MODCONbits.YWM3
#define _BWM0 MODCONbits.BWM0
#define _BWM1 MODCONbits.BWM1
#define _BWM2 MODCONbits.BWM2
#define _BWM3 MODCONbits.BWM3

/* XBREV */
#define _XB XBREVbits.XB
#define _BREN XBREVbits.BREN
#define _XB0 XBREVbits.XB0
#define _XB1 XBREVbits.XB1
#define _XB2 XBREVbits.XB2
#define _XB3 XBREVbits.XB3
#define _XB4 XBREVbits.XB4
#define _XB5 XBREVbits.XB5
#define _XB6 XBREVbits.XB6
#define _XB7 XBREVbits.XB7
#define _XB8 XBREVbits.XB8
#define _XB9 XBREVbits.XB9
#define _XB10 XBREVbits.XB10
#define _XB11 XBREVbits.XB11
#define _XB12 XBREVbits.XB12
#define _XB13 XBREVbits.XB13
#define _XB14 XBREVbits.XB14

/* DISICNT */
#define _DISICNT DISICNTbits.DISICNT

/* INTCON1 */
#define _OSCFAIL INTCON1bits.OSCFAIL
#define _STKERR INTCON1bits.STKERR
#define _ADDRERR INTCON1bits.ADDRERR
#define _MATHERR INTCON1bits.MATHERR
#define _COVTE INTCON1bits.COVTE
#define _OVBTE INTCON1bits.OVBTE
#define _OVATE INTCON1bits.OVATE
#define _NSTDIS INTCON1bits.NSTDIS

/* INTCON2 */
#define _INT0EP INTCON2bits.INT0EP
#define _INT1EP INTCON2bits.INT1EP
#define _INT2EP INTCON2bits.INT2EP
#define _INT3EP INTCON2bits.INT3EP
#define _INT4EP INTCON2bits.INT4EP
#define _DISI INTCON2bits.DISI
#define _ALTIVT INTCON2bits.ALTIVT

/* IFS0 */
#define _INT0IF IFS0bits.INT0IF
#define _IC1IF IFS0bits.IC1IF
#define _OC1IF IFS0bits.OC1IF
#define _T1IF IFS0bits.T1IF
#define _IC2IF IFS0bits.IC2IF
#define _OC2IF IFS0bits.OC2IF
#define _T2IF IFS0bits.T2IF
#define _T3IF IFS0bits.T3IF
#define _SPI1IF IFS0bits.SPI1IF
#define _U1RXIF IFS0bits.U1RXIF
#define _U1TXIF IFS0bits.U1TXIF
#define _ADIF IFS0bits.ADIF
#define _NVMIF IFS0bits.NVMIF
#define _SI2CIF IFS0bits.SI2CIF
#define _MI2CIF IFS0bits.MI2CIF
#define _CNIF IFS0bits.CNIF

/* IFS1 */
#define _INT1IF IFS1bits.INT1IF
#define _IC7IF IFS1bits.IC7IF
#define _IC8IF IFS1bits.IC8IF
#define _OC3IF IFS1bits.OC3IF
#define _OC4IF IFS1bits.OC4IF
#define _T4IF IFS1bits.T4IF
#define _T5IF IFS1bits.T5IF
#define _INT2IF IFS1bits.INT2IF
#define _U2RXIF IFS1bits.U2RXIF
#define _U2TXIF IFS1bits.U2TXIF
#define _SPI2IF IFS1bits.SPI2IF
#define _C1IF IFS1bits.C1IF
#define _IC3IF IFS1bits.IC3IF
#define _IC4IF IFS1bits.IC4IF
#define _IC5IF IFS1bits.IC5IF
#define _IC6IF IFS1bits.IC6IF

/* IFS2 */
#define _OC5IF IFS2bits.OC5IF
#define _OC6IF IFS2bits.OC6IF
#define _OC7IF IFS2bits.OC7IF
#define _OC8IF IFS2bits.OC8IF
#define _INT3IF IFS2bits.INT3IF
#define _INT4IF IFS2bits.INT4IF
#define _C2IF IFS2bits.C2IF
#define _DCIIF IFS2bits.DCIIF
#define _LVDIF IFS2bits.LVDIF

/* IEC0 */
#define _INT0IE IEC0bits.INT0IE
#define _IC1IE IEC0bits.IC1IE
#define _OC1IE IEC0bits.OC1IE
#define _T1IE IEC0bits.T1IE
#define _IC2IE IEC0bits.IC2IE
#define _OC2IE IEC0bits.OC2IE
#define _T2IE IEC0bits.T2IE
#define _T3IE IEC0bits.T3IE
#define _SPI1IE IEC0bits.SPI1IE
#define _U1RXIE IEC0bits.U1RXIE
#define _U1TXIE IEC0bits.U1TXIE
#define _ADIE IEC0bits.ADIE
#define _NVMIE IEC0bits.NVMIE
#define _SI2CIE IEC0bits.SI2CIE
#define _MI2CIE IEC0bits.MI2CIE
#define _CNIE IEC0bits.CNIE

/* IEC1 */
#define _INT1IE IEC1bits.INT1IE
#define _IC7IE IEC1bits.IC7IE
#define _IC8IE IEC1bits.IC8IE
#define _OC3IE IEC1bits.OC3IE
#define _OC4IE IEC1bits.OC4IE
#define _T4IE IEC1bits.T4IE
#define _T5IE IEC1bits.T5IE
#define _INT2IE IEC1bits.INT2IE
#define _U2RXIE IEC1bits.U2RXIE
#define _U2TXIE IEC1bits.U2TXIE
#define _SPI2IE IEC1bits.SPI2IE
#define _C1IE IEC1bits.C1IE
#define _IC3IE IEC1bits.IC3IE
#define _IC4IE IEC1bits.IC4IE
#define _IC5IE IEC1bits.IC5IE
#define _IC6IE IEC1bits.IC6IE

/* IEC2 */
#define _OC5IE IEC2bits.OC5IE
#define _OC6IE IEC2bits.OC6IE
#define _OC7IE IEC2bits.OC7IE
#define _OC8IE IEC2bits.OC8IE
#define _INT3IE IEC2bits.INT3IE
#define _INT4IE IEC2bits.INT4IE
#define _C2IE IEC2bits.C2IE
#define _DCIIE IEC2bits.DCIIE
#define _LVDIE IEC2bits.LVDIE

/* IPC0 */
#define _INT0IP IPC0bits.INT0IP
#define _IC1IP IPC0bits.IC1IP
#define _OC1IP IPC0bits.OC1IP
#define _T1IP IPC0bits.T1IP
#define _INT0IP0 IPC0bits.INT0IP0
#define _INT0IP1 IPC0bits.INT0IP1
#define _INT0IP2 IPC0bits.INT0IP2
#define _IC1IP0 IPC0bits.IC1IP0
#define _IC1IP1 IPC0bits.IC1IP1
#define _IC1IP2 IPC0bits.IC1IP2
#define _OC1IP0 IPC0bits.OC1IP0
#define _OC1IP1 IPC0bits.OC1IP1
#define _OC1IP2 IPC0bits.OC1IP2
#define _T1IP0 IPC0bits.T1IP0
#define _T1IP1 IPC0bits.T1IP1
#define _T1IP2 IPC0bits.T1IP2

/* IPC1 */
#define _IC2IP IPC1bits.IC2IP
#define _OC2IP IPC1bits.OC2IP
#define _T2IP IPC1bits.T2IP
#define _T3IP IPC1bits.T3IP
#define _IC2IP0 IPC1bits.IC2IP0
#define _IC2IP1 IPC1bits.IC2IP1
#define _IC2IP2 IPC1bits.IC2IP2
#define _OC2IP0 IPC1bits.OC2IP0
#define _OC2IP1 IPC1bits.OC2IP1
#define _OC2IP2 IPC1bits.OC2IP2
#define _T2IP0 IPC1bits.T2IP0
#define _T2IP1 IPC1bits.T2IP1
#define _T2IP2 IPC1bits.T2IP2
#define _T3IP0 IPC1bits.T3IP0
#define _T3IP1 IPC1bits.T3IP1
#define _T3IP2 IPC1bits.T3IP2

/* IPC2 */
#define _SPI1IP IPC2bits.SPI1IP
#define _U1RXIP IPC2bits.U1RXIP
#define _U1TXIP IPC2bits.U1TXIP
#define _ADIP IPC2bits.ADIP
#define _SPI1IP0 IPC2bits.SPI1IP0
#define _SPI1IP1 IPC2bits.SPI1IP1
#define _SPI1IP2 IPC2bits.SPI1IP2
#define _U1RXIP0 IPC2bits.U1RXIP0
#define _U1RXIP1 IPC2bits.U1RXIP1
#define _U1RXIP2 IPC2bits.U1RXIP2
#define _U1TXIP0 IPC2bits.U1TXIP0
#define _U1TXIP1 IPC2bits.U1TXIP1
#define _U1TXIP2 IPC2bits.U1TXIP2
#define _ADIP0 IPC2bits.ADIP0
#define _ADIP1 IPC2bits.ADIP1
#define _ADIP2 IPC2bits.ADIP2

/* IPC3 */
#define _NVMIP IPC3bits.NVMIP
#define _SI2CIP IPC3bits.SI2CIP
#define _MI2CIP IPC3bits.MI2CIP
#define _CNIP IPC3bits.CNIP
#define _NVMIP0 IPC3bits.NVMIP0
#define _NVMIP1 IPC3bits.NVMIP1
#define _NVMIP2 IPC3bits.NVMIP2
#define _SI2CIP0 IPC3bits.SI2CIP0
#define _SI2CIP1 IPC3bits.SI2CIP1
#define _SI2CIP2 IPC3bits.SI2CIP2
#define _MI2CIP0 IPC3bits.MI2CIP0
#define _MI2CIP1 IPC3bits.MI2CIP1
#define _MI2CIP2 IPC3bits.MI2CIP2
#define _CNIP0 IPC3bits.CNIP0
#define _CNIP1 IPC3bits.CNIP1
#define _CNIP2 IPC3bits.CNIP2

/* IPC4 */
#define _INT1IP IPC4bits.INT1IP
#define _IC7IP IPC4bits.IC7IP
#define _IC8IP IPC4bits.IC8IP
#define _OC3IP IPC4bits.OC3IP
#define _INT1IP0 IPC4bits.INT1IP0
#define _INT1IP1 IPC4bits.INT1IP1
#define _INT1IP2 IPC4bits.INT1IP2
#define _IC7IP0 IPC4bits.IC7IP0
#define _IC7IP1 IPC4bits.IC7IP1
#define _IC7IP2 IPC4bits.IC7IP2
#define _IC8IP0 IPC4bits.IC8IP0
#define _IC8IP1 IPC4bits.IC8IP1
#define _IC8IP2 IPC4bits.IC8IP2
#define _OC3IP0 IPC4bits.OC3IP0
#define _OC3IP1 IPC4bits.OC3IP1
#define _OC3IP2 IPC4bits.OC3IP2

/* IPC5 */
#define _OC4IP IPC5bits.OC4IP
#define _T4IP IPC5bits.T4IP
#define _T5IP IPC5bits.T5IP
#define _INT2IP IPC5bits.INT2IP
#define _OC4IP0 IPC5bits.OC4IP0
#define _OC4IP1 IPC5bits.OC4IP1
#define _OC4IP2 IPC5bits.OC4IP2
#define _T4IP0 IPC5bits.T4IP0
#define _T4IP1 IPC5bits.T4IP1
#define _T4IP2 IPC5bits.T4IP2
#define _T5IP0 IPC5bits.T5IP0
#define _T5IP1 IPC5bits.T5IP1
#define _T5IP2 IPC5bits.T5IP2
#define _INT2IP0 IPC5bits.INT2IP0
#define _INT2IP1 IPC5bits.INT2IP1
#define _INT2IP2 IPC5bits.INT2IP2

/* IPC6 */
#define _U2RXIP IPC6bits.U2RXIP
#define _U2TXIP IPC6bits.U2TXIP
#define _SPI2IP IPC6bits.SPI2IP
#define _C1IP IPC6bits.C1IP
#define _U2RXIP0 IPC6bits.U2RXIP0
#define _U2RXIP1 IPC6bits.U2RXIP1
#define _U2RXIP2 IPC6bits.U2RXIP2
#define _U2TXIP0 IPC6bits.U2TXIP0
#define _U2TXIP1 IPC6bits.U2TXIP1
#define _U2TXIP2 IPC6bits.U2TXIP2
#define _SPI2IP0 IPC6bits.SPI2IP0
#define _SPI2IP1 IPC6bits.SPI2IP1
#define _SPI2IP2 IPC6bits.SPI2IP2
#define _C1IP0 IPC6bits.C1IP0
#define _C1IP1 IPC6bits.C1IP1
#define _C1IP2 IPC6bits.C1IP2

/* IPC7 */
#define _IC3IP IPC7bits.IC3IP
#define _IC4IP IPC7bits.IC4IP
#define _IC5IP IPC7bits.IC5IP
#define _IC6IP IPC7bits.IC6IP
#define _IC3IP0 IPC7bits.IC3IP0
#define _IC3IP1 IPC7bits.IC3IP1
#define _IC3IP2 IPC7bits.IC3IP2
#define _IC4IP0 IPC7bits.IC4IP0
#define _IC4IP1 IPC7bits.IC4IP1
#define _IC4IP2 IPC7bits.IC4IP2
#define _IC5IP0 IPC7bits.IC5IP0
#define _IC5IP1 IPC7bits.IC5IP1
#define _IC5IP2 IPC7bits.IC5IP2
#define _IC6IP0 IPC7bits.IC6IP0
#define _IC6IP1 IPC7bits.IC6IP1
#define _IC6IP2 IPC7bits.IC6IP2

/* IPC8 */
#define _OC5IP IPC8bits.OC5IP
#define _OC6IP IPC8bits.OC6IP
#define _OC7IP IPC8bits.OC7IP
#define _OC8IP IPC8bits.OC8IP
#define _OC5IP0 IPC8bits.OC5IP0
#define _OC5IP1 IPC8bits.OC5IP1
#define _OC5IP2 IPC8bits.OC5IP2
#define _OC6IP0 IPC8bits.OC6IP0
#define _OC6IP1 IPC8bits.OC6IP1
#define _OC6IP2 IPC8bits.OC6IP2
#define _OC7IP0 IPC8bits.OC7IP0
#define _OC7IP1 IPC8bits.OC7IP1
#define _OC7IP2 IPC8bits.OC7IP2
#define _OC8IP0 IPC8bits.OC8IP0
#define _OC8IP1 IPC8bits.OC8IP1
#define _OC8IP2 IPC8bits.OC8IP2

/* IPC9 */
#define _INT3IP IPC9bits.INT3IP
#define _INT4IP IPC9bits.INT4IP
#define _C2IP IPC9bits.C2IP
#define _INT3IP0 IPC9bits.INT3IP0
#define _INT3IP1 IPC9bits.INT3IP1
#define _INT3IP2 IPC9bits.INT3IP2
#define _INT4IP0 IPC9bits.INT4IP0
#define _INT4IP1 IPC9bits.INT4IP1
#define _INT4IP2 IPC9bits.INT4IP2
#define _C2IP0 IPC9bits.C2IP0
#define _C2IP1 IPC9bits.C2IP1
#define _C2IP2 IPC9bits.C2IP2

/* IPC10 */
#define _DCIIP IPC10bits.DCIIP
#define _LVDIP IPC10bits.LVDIP
#define _DCIIP0 IPC10bits.DCIIP0
#define _DCIIP1 IPC10bits.DCIIP1
#define _DCIIP2 IPC10bits.DCIIP2
#define _LVDIP0 IPC10bits.LVDIP0
#define _LVDIP1 IPC10bits.LVDIP1
#define _LVDIP2 IPC10bits.LVDIP2

/* INTTREG */
#define _VECNUM INTTREGbits.VECNUM
#define _ILR INTTREGbits.ILR
#define _VHOLD INTTREGbits.VHOLD
#define _TMODE INTTREGbits.TMODE
#define _IRQTOCPU INTTREGbits.IRQTOCPU
#define _VECNUM0 INTTREGbits.VECNUM0
#define _VECNUM1 INTTREGbits.VECNUM1
#define _VECNUM2 INTTREGbits.VECNUM2
#define _VECNUM3 INTTREGbits.VECNUM3
#define _VECNUM4 INTTREGbits.VECNUM4
#define _VECNUM5 INTTREGbits.VECNUM5
#define _ILR0 INTTREGbits.ILR0
#define _ILR1 INTTREGbits.ILR1
#define _ILR2 INTTREGbits.ILR2
#define _ILR3 INTTREGbits.ILR3

/* CNEN1 */
#define _CN0IE CNEN1bits.CN0IE
#define _CN1IE CNEN1bits.CN1IE
#define _CN2IE CNEN1bits.CN2IE
#define _CN3IE CNEN1bits.CN3IE
#define _CN4IE CNEN1bits.CN4IE
#define _CN5IE CNEN1bits.CN5IE
#define _CN6IE CNEN1bits.CN6IE
#define _CN7IE CNEN1bits.CN7IE
#define _CN8IE CNEN1bits.CN8IE
#define _CN9IE CNEN1bits.CN9IE
#define _CN10IE CNEN1bits.CN10IE
#define _CN11IE CNEN1bits.CN11IE
#define _CN12IE CNEN1bits.CN12IE
#define _CN13IE CNEN1bits.CN13IE
#define _CN14IE CNEN1bits.CN14IE
#define _CN15IE CNEN1bits.CN15IE

/* CNEN2 */
#define _CN16IE CNEN2bits.CN16IE
#define _CN17IE CNEN2bits.CN17IE
#define _CN18IE CNEN2bits.CN18IE

/* CNPU1 */
#define _CN0PUE CNPU1bits.CN0PUE
#define _CN1PUE CNPU1bits.CN1PUE
#define _CN2PUE CNPU1bits.CN2PUE
#define _CN3PUE CNPU1bits.CN3PUE
#define _CN4PUE CNPU1bits.CN4PUE
#define _CN5PUE CNPU1bits.CN5PUE
#define _CN6PUE CNPU1bits.CN6PUE
#define _CN7PUE CNPU1bits.CN7PUE
#define _CN8PUE CNPU1bits.CN8PUE
#define _CN9PUE CNPU1bits.CN9PUE
#define _CN10PUE CNPU1bits.CN10PUE
#define _CN11PUE CNPU1bits.CN11PUE
#define _CN12PUE CNPU1bits.CN12PUE
#define _CN13PUE CNPU1bits.CN13PUE
#define _CN14PUE CNPU1bits.CN14PUE
#define _CN15PUE CNPU1bits.CN15PUE

/* CNPU2 */
#define _CN16PUE CNPU2bits.CN16PUE
#define _CN17PUE CNPU2bits.CN17PUE
#define _CN18PUE CNPU2bits.CN18PUE

/* T1CON */
#define _TCS T1CONbits.TCS
#define _TSYNC T1CONbits.TSYNC
#define _TCKPS T1CONbits.TCKPS
#define _TGATE T1CONbits.TGATE
#define _TSIDL T1CONbits.TSIDL
#define _TON T1CONbits.TON
#define _TCKPS0 T1CONbits.TCKPS0
#define _TCKPS1 T1CONbits.TCKPS1

/* T2CON */
/* Bitname _TCS cannot be defined because it is used by more than one SFR */
/* Bitname _T32 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS cannot be defined because it is used by more than one SFR */
/* Bitname _TGATE cannot be defined because it is used by more than one SFR */
/* Bitname _TSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _TON cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS0 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS1 cannot be defined because it is used by more than one SFR */

/* T3CON */
/* Bitname _TCS cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS cannot be defined because it is used by more than one SFR */
/* Bitname _TGATE cannot be defined because it is used by more than one SFR */
/* Bitname _TSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _TON cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS0 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS1 cannot be defined because it is used by more than one SFR */

/* T4CON */
/* Bitname _TCS cannot be defined because it is used by more than one SFR */
/* Bitname _T32 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS cannot be defined because it is used by more than one SFR */
/* Bitname _TGATE cannot be defined because it is used by more than one SFR */
/* Bitname _TSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _TON cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS0 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS1 cannot be defined because it is used by more than one SFR */

/* T5CON */
/* Bitname _TCS cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS cannot be defined because it is used by more than one SFR */
/* Bitname _TGATE cannot be defined because it is used by more than one SFR */
/* Bitname _TSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _TON cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS0 cannot be defined because it is used by more than one SFR */
/* Bitname _TCKPS1 cannot be defined because it is used by more than one SFR */

/* IC1CON */
#define _ICM IC1CONbits.ICM
#define _ICBNE IC1CONbits.ICBNE
#define _ICOV IC1CONbits.ICOV
#define _ICI IC1CONbits.ICI
#define _ICTMR IC1CONbits.ICTMR
#define _ICSIDL IC1CONbits.ICSIDL

/* IC2CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC3CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC4CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC5CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC6CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC7CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* IC8CON */
/* Bitname _ICM cannot be defined because it is used by more than one SFR */
/* Bitname _ICBNE cannot be defined because it is used by more than one SFR */
/* Bitname _ICOV cannot be defined because it is used by more than one SFR */
/* Bitname _ICI cannot be defined because it is used by more than one SFR */
/* Bitname _ICTMR cannot be defined because it is used by more than one SFR */
/* Bitname _ICSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _ICM0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICM2 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICI1 cannot be defined because it is used by more than one SFR */

/* OC1CON */
#define _OCM OC1CONbits.OCM
#define _OCTSEL OC1CONbits.OCTSEL
#define _OCFLT OC1CONbits.OCFLT
#define _OCSIDL OC1CONbits.OCSIDL
#define _OCM0 OC1CONbits.OCM0
#define _OCM1 OC1CONbits.OCM1
#define _OCM2 OC1CONbits.OCM2

/* OC2CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC3CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC4CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC5CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC6CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC7CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* OC8CON */
/* Bitname _OCM cannot be defined because it is used by more than one SFR */
/* Bitname _OCTSEL cannot be defined because it is used by more than one SFR */
/* Bitname _OCFLT cannot be defined because it is used by more than one SFR */
/* Bitname _OCSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _OCM0 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM1 cannot be defined because it is used by more than one SFR */
/* Bitname _OCM2 cannot be defined because it is used by more than one SFR */

/* I2CRCV */
#define _I2CRCV I2CRCVbits.I2CRCV
#define _I2CRCV0 I2CRCVbits.I2CRCV0
#define _I2CRCV1 I2CRCVbits.I2CRCV1
#define _I2CRCV2 I2CRCVbits.I2CRCV2
#define _I2CRCV3 I2CRCVbits.I2CRCV3
#define _I2CRCV4 I2CRCVbits.I2CRCV4
#define _I2CRCV5 I2CRCVbits.I2CRCV5
#define _I2CRCV6 I2CRCVbits.I2CRCV6
#define _I2CRCV7 I2CRCVbits.I2CRCV7

/* I2CTRN */
#define _I2CTRN I2CTRNbits.I2CTRN
#define _I2CTRN0 I2CTRNbits.I2CTRN0
#define _I2CTRN1 I2CTRNbits.I2CTRN1
#define _I2CTRN2 I2CTRNbits.I2CTRN2
#define _I2CTRN3 I2CTRNbits.I2CTRN3
#define _I2CTRN4 I2CTRNbits.I2CTRN4
#define _I2CTRN5 I2CTRNbits.I2CTRN5
#define _I2CTRN6 I2CTRNbits.I2CTRN6
#define _I2CTRN7 I2CTRNbits.I2CTRN7

/* I2CBRG */
#define _I2CBRG I2CBRGbits.I2CBRG

/* I2CCON */
#define _SEN I2CCONbits.SEN
#define _RSEN I2CCONbits.RSEN
#define _PEN I2CCONbits.PEN
#define _RCEN I2CCONbits.RCEN
#define _ACKEN I2CCONbits.ACKEN
#define _ACKDT I2CCONbits.ACKDT
#define _STREN I2CCONbits.STREN
#define _GCEN I2CCONbits.GCEN
#define _SMEN I2CCONbits.SMEN
#define _DISSLW I2CCONbits.DISSLW
#define _A10M I2CCONbits.A10M
#define _IPMIEN I2CCONbits.IPMIEN
#define _SCLREL I2CCONbits.SCLREL
#define _I2CSIDL I2CCONbits.I2CSIDL
#define _I2CEN I2CCONbits.I2CEN

/* I2CSTAT */
#define _TBF I2CSTATbits.TBF
#define _RBF I2CSTATbits.RBF
#define _R_NOT_W I2CSTATbits.R_NOT_W
#define _S I2CSTATbits.S
#define _P I2CSTATbits.P
#define _D_NOT_A I2CSTATbits.D_NOT_A
#define _I2COV I2CSTATbits.I2COV
#define _IWCOL I2CSTATbits.IWCOL
#define _ADD10 I2CSTATbits.ADD10
#define _GCSTAT I2CSTATbits.GCSTAT
#define _BCL I2CSTATbits.BCL
#define _TRSTAT I2CSTATbits.TRSTAT
#define _ACKSTAT I2CSTATbits.ACKSTAT
#define _R_W I2CSTATbits.R_W
#define _D_A I2CSTATbits.D_A

/* I2CADD */
#define _I2CADD I2CADDbits.I2CADD

/* U1MODE */
#define _STSEL U1MODEbits.STSEL
#define _PDSEL U1MODEbits.PDSEL
#define _ABAUD U1MODEbits.ABAUD
#define _LPBACK U1MODEbits.LPBACK
#define _WAKE U1MODEbits.WAKE
#define _ALTIO U1MODEbits.ALTIO
#define _USIDL U1MODEbits.USIDL
#define _UARTEN U1MODEbits.UARTEN
#define _PDSEL0 U1MODEbits.PDSEL0
#define _PDSEL1 U1MODEbits.PDSEL1

/* U1STA */
#define _URXDA U1STAbits.URXDA
#define _OERR U1STAbits.OERR
#define _FERR U1STAbits.FERR
#define _PERR U1STAbits.PERR
#define _RIDLE U1STAbits.RIDLE
#define _ADDEN U1STAbits.ADDEN
#define _URXISEL U1STAbits.URXISEL
#define _TRMT U1STAbits.TRMT
#define _UTXBF U1STAbits.UTXBF
#define _UTXEN U1STAbits.UTXEN
#define _UTXBRK U1STAbits.UTXBRK
#define _UTXISEL U1STAbits.UTXISEL
#define _URXISEL0 U1STAbits.URXISEL0
#define _URXISEL1 U1STAbits.URXISEL1

/* U1TXREG */
#define _UTXREG0 U1TXREGbits.UTXREG0
#define _UTXREG1 U1TXREGbits.UTXREG1
#define _UTXREG2 U1TXREGbits.UTXREG2
#define _UTXREG3 U1TXREGbits.UTXREG3
#define _UTXREG4 U1TXREGbits.UTXREG4
#define _UTXREG5 U1TXREGbits.UTXREG5
#define _UTXREG6 U1TXREGbits.UTXREG6
#define _UTXREG7 U1TXREGbits.UTXREG7
#define _UTXREG8 U1TXREGbits.UTXREG8
#define _UTX8 U1TXREGbits.UTX8

/* U1RXREG */
#define _URXREG0 U1RXREGbits.URXREG0
#define _URXREG1 U1RXREGbits.URXREG1
#define _URXREG2 U1RXREGbits.URXREG2
#define _URXREG3 U1RXREGbits.URXREG3
#define _URXREG4 U1RXREGbits.URXREG4
#define _URXREG5 U1RXREGbits.URXREG5
#define _URXREG6 U1RXREGbits.URXREG6
#define _URXREG7 U1RXREGbits.URXREG7
#define _URXREG8 U1RXREGbits.URXREG8
#define _URX8 U1RXREGbits.URX8

/* U2MODE */
/* Bitname _STSEL cannot be defined because it is used by more than one SFR */
/* Bitname _PDSEL cannot be defined because it is used by more than one SFR */
/* Bitname _ABAUD cannot be defined because it is used by more than one SFR */
/* Bitname _LPBACK cannot be defined because it is used by more than one SFR */
/* Bitname _WAKE cannot be defined because it is used by more than one SFR */
/* Bitname _USIDL cannot be defined because it is used by more than one SFR */
/* Bitname _UARTEN cannot be defined because it is used by more than one SFR */
/* Bitname _PDSEL0 cannot be defined because it is used by more than one SFR */
/* Bitname _PDSEL1 cannot be defined because it is used by more than one SFR */

/* U2STA */
/* Bitname _URXDA cannot be defined because it is used by more than one SFR */
/* Bitname _OERR cannot be defined because it is used by more than one SFR */
/* Bitname _FERR cannot be defined because it is used by more than one SFR */
/* Bitname _PERR cannot be defined because it is used by more than one SFR */
/* Bitname _RIDLE cannot be defined because it is used by more than one SFR */
/* Bitname _ADDEN cannot be defined because it is used by more than one SFR */
/* Bitname _URXISEL cannot be defined because it is used by more than one SFR */
/* Bitname _TRMT cannot be defined because it is used by more than one SFR */
/* Bitname _UTXBF cannot be defined because it is used by more than one SFR */
/* Bitname _UTXEN cannot be defined because it is used by more than one SFR */
/* Bitname _UTXBRK cannot be defined because it is used by more than one SFR */
/* Bitname _UTXISEL cannot be defined because it is used by more than one SFR */
/* Bitname _URXISEL0 cannot be defined because it is used by more than one SFR */
/* Bitname _URXISEL1 cannot be defined because it is used by more than one SFR */

/* U2TXREG */
/* Bitname _UTXREG0 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG1 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG2 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG3 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG4 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG5 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG6 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG7 cannot be defined because it is used by more than one SFR */
/* Bitname _UTXREG8 cannot be defined because it is used by more than one SFR */
/* Bitname _UTX8 cannot be defined because it is used by more than one SFR */

/* U2RXREG */
/* Bitname _URXREG0 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG1 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG2 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG3 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG4 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG5 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG6 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG7 cannot be defined because it is used by more than one SFR */
/* Bitname _URXREG8 cannot be defined because it is used by more than one SFR */
/* Bitname _URX8 cannot be defined because it is used by more than one SFR */

/* SPI1STAT */
#define _SPIRBF SPI1STATbits.SPIRBF
#define _SPITBF SPI1STATbits.SPITBF
#define _SPIROV SPI1STATbits.SPIROV
#define _SPISIDL SPI1STATbits.SPISIDL
#define _SPIEN SPI1STATbits.SPIEN

/* SPI1CON */
/* Bitname _PPRE0 cannot be defined because it is used by more than one SFR */
/* Bitname _PPRE1 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE0 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE1 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE2 cannot be defined because it is used by more than one SFR */
/* Bitname _MSTEN cannot be defined because it is used by more than one SFR */
/* Bitname _CKP cannot be defined because it is used by more than one SFR */
/* Bitname _SSEN cannot be defined because it is used by more than one SFR */
/* Bitname _CKE cannot be defined because it is used by more than one SFR */
/* Bitname _SMP cannot be defined because it is used by more than one SFR */
/* Bitname _MODE16 cannot be defined because it is used by more than one SFR */
/* Bitname _DISSDO cannot be defined because it is used by more than one SFR */
/* Bitname _SPIFSD cannot be defined because it is used by more than one SFR */
/* Bitname _FRMEN cannot be defined because it is used by more than one SFR */
/* Bitname _PPRE cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE cannot be defined because it is used by more than one SFR */

/* SPI2STAT */
/* Bitname _SPIRBF cannot be defined because it is used by more than one SFR */
/* Bitname _SPITBF cannot be defined because it is used by more than one SFR */
/* Bitname _SPIROV cannot be defined because it is used by more than one SFR */
/* Bitname _SPISIDL cannot be defined because it is used by more than one SFR */
/* Bitname _SPIEN cannot be defined because it is used by more than one SFR */

/* SPI2CON */
/* Bitname _PPRE0 cannot be defined because it is used by more than one SFR */
/* Bitname _PPRE1 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE0 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE1 cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE2 cannot be defined because it is used by more than one SFR */
/* Bitname _MSTEN cannot be defined because it is used by more than one SFR */
/* Bitname _CKP cannot be defined because it is used by more than one SFR */
/* Bitname _SSEN cannot be defined because it is used by more than one SFR */
/* Bitname _CKE cannot be defined because it is used by more than one SFR */
/* Bitname _SMP cannot be defined because it is used by more than one SFR */
/* Bitname _MODE16 cannot be defined because it is used by more than one SFR */
/* Bitname _DISSDO cannot be defined because it is used by more than one SFR */
/* Bitname _SPIFSD cannot be defined because it is used by more than one SFR */
/* Bitname _FRMEN cannot be defined because it is used by more than one SFR */
/* Bitname _PPRE cannot be defined because it is used by more than one SFR */
/* Bitname _SPRE cannot be defined because it is used by more than one SFR */

/* DCICON1 */
#define _COFSM DCICON1bits.COFSM
#define _DJST DCICON1bits.DJST
#define _CSDOM DCICON1bits.CSDOM
#define _UNFM DCICON1bits.UNFM
#define _COFSD DCICON1bits.COFSD
#define _CSCKE DCICON1bits.CSCKE
#define _CSCKD DCICON1bits.CSCKD
#define _DLOOP DCICON1bits.DLOOP
#define _DCISIDL DCICON1bits.DCISIDL
#define _DCIEN DCICON1bits.DCIEN
#define _COFSM0 DCICON1bits.COFSM0
#define _COFSM1 DCICON1bits.COFSM1

/* DCICON2 */
#define _WS DCICON2bits.WS
#define _COFSG DCICON2bits.COFSG
#define _BLEN DCICON2bits.BLEN
#define _WS0 DCICON2bits.WS0
#define _WS1 DCICON2bits.WS1
#define _WS2 DCICON2bits.WS2
#define _WS3 DCICON2bits.WS3
#define _COFSG0 DCICON2bits.COFSG0
#define _COFSG1 DCICON2bits.COFSG1
#define _COFSG2 DCICON2bits.COFSG2
#define _COFSG3 DCICON2bits.COFSG3
#define _BLEN0 DCICON2bits.BLEN0
#define _BLEN1 DCICON2bits.BLEN1

/* DCICON3 */
#define _BCG DCICON3bits.BCG
#define _BCG0 DCICON3bits.BCG0
#define _BCG1 DCICON3bits.BCG1
#define _BCG2 DCICON3bits.BCG2
#define _BCG3 DCICON3bits.BCG3
#define _BCG4 DCICON3bits.BCG4
#define _BCG5 DCICON3bits.BCG5
#define _BCG6 DCICON3bits.BCG6
#define _BCG7 DCICON3bits.BCG7
#define _BCG8 DCICON3bits.BCG8
#define _BCG9 DCICON3bits.BCG9
#define _BCG10 DCICON3bits.BCG10
#define _BCG11 DCICON3bits.BCG11

/* DCISTAT */
#define _TMPTY DCISTATbits.TMPTY
#define _TUNF DCISTATbits.TUNF
#define _RFUL DCISTATbits.RFUL
#define _ROV DCISTATbits.ROV
#define _SLOT DCISTATbits.SLOT
#define _SLOT0 DCISTATbits.SLOT0
#define _SLOT1 DCISTATbits.SLOT1
#define _SLOT2 DCISTATbits.SLOT2
#define _SLOT3 DCISTATbits.SLOT3

/* TSCON */
#define _TSE0 TSCONbits.TSE0
#define _TSE1 TSCONbits.TSE1
#define _TSE2 TSCONbits.TSE2
#define _TSE3 TSCONbits.TSE3
#define _TSE4 TSCONbits.TSE4
#define _TSE5 TSCONbits.TSE5
#define _TSE6 TSCONbits.TSE6
#define _TSE7 TSCONbits.TSE7
#define _TSE8 TSCONbits.TSE8
#define _TSE9 TSCONbits.TSE9
#define _TSE10 TSCONbits.TSE10
#define _TSE11 TSCONbits.TSE11
#define _TSE12 TSCONbits.TSE12
#define _TSE13 TSCONbits.TSE13
#define _TSE14 TSCONbits.TSE14
#define _TSE15 TSCONbits.TSE15

/* RSCON */
#define _RSE0 RSCONbits.RSE0
#define _RSE1 RSCONbits.RSE1
#define _RSE2 RSCONbits.RSE2
#define _RSE3 RSCONbits.RSE3
#define _RSE4 RSCONbits.RSE4
#define _RSE5 RSCONbits.RSE5
#define _RSE6 RSCONbits.RSE6
#define _RSE7 RSCONbits.RSE7
#define _RSE8 RSCONbits.RSE8
#define _RSE9 RSCONbits.RSE9
#define _RSE10 RSCONbits.RSE10
#define _RSE11 RSCONbits.RSE11
#define _RSE12 RSCONbits.RSE12
#define _RSE13 RSCONbits.RSE13
#define _RSE14 RSCONbits.RSE14
#define _RSE15 RSCONbits.RSE15

/* ADCON1 */
#define _DONE ADCON1bits.DONE
#define _SAMP ADCON1bits.SAMP
#define _ASAM ADCON1bits.ASAM
#define _SSRC ADCON1bits.SSRC
#define _FORM ADCON1bits.FORM
#define _ADSIDL ADCON1bits.ADSIDL
#define _ADON ADCON1bits.ADON
#define _SSRC0 ADCON1bits.SSRC0
#define _SSRC1 ADCON1bits.SSRC1
#define _SSRC2 ADCON1bits.SSRC2
#define _FORM0 ADCON1bits.FORM0
#define _FORM1 ADCON1bits.FORM1

/* ADCON2 */
#define _ALTS ADCON2bits.ALTS
#define _BUFM ADCON2bits.BUFM
#define _SMPI ADCON2bits.SMPI
#define _BUFS ADCON2bits.BUFS
#define _CSCNA ADCON2bits.CSCNA
#define _VCFG ADCON2bits.VCFG
#define _SMPI0 ADCON2bits.SMPI0
#define _SMPI1 ADCON2bits.SMPI1
#define _SMPI2 ADCON2bits.SMPI2
#define _SMPI3 ADCON2bits.SMPI3
#define _VCFG0 ADCON2bits.VCFG0
#define _VCFG1 ADCON2bits.VCFG1
#define _VCFG2 ADCON2bits.VCFG2

/* ADCON3 */
#define _ADCS ADCON3bits.ADCS
#define _ADRC ADCON3bits.ADRC
#define _SAMC ADCON3bits.SAMC
#define _ADCS0 ADCON3bits.ADCS0
#define _ADCS1 ADCON3bits.ADCS1
#define _ADCS2 ADCON3bits.ADCS2
#define _ADCS3 ADCON3bits.ADCS3
#define _ADCS4 ADCON3bits.ADCS4
#define _ADCS5 ADCON3bits.ADCS5
#define _SAMC0 ADCON3bits.SAMC0
#define _SAMC1 ADCON3bits.SAMC1
#define _SAMC2 ADCON3bits.SAMC2
#define _SAMC3 ADCON3bits.SAMC3
#define _SAMC4 ADCON3bits.SAMC4

/* ADCHS */
#define _CH0SA ADCHSbits.CH0SA
#define _CH0NA ADCHSbits.CH0NA
#define _CH0SB ADCHSbits.CH0SB
#define _CH0NB ADCHSbits.CH0NB
#define _CH0SA0 ADCHSbits.CH0SA0
#define _CH0SA1 ADCHSbits.CH0SA1
#define _CH0SA2 ADCHSbits.CH0SA2
#define _CH0SA3 ADCHSbits.CH0SA3
#define _CH0SB0 ADCHSbits.CH0SB0
#define _CH0SB1 ADCHSbits.CH0SB1
#define _CH0SB2 ADCHSbits.CH0SB2
#define _CH0SB3 ADCHSbits.CH0SB3

/* ADPCFG */
#define _PCFG0 ADPCFGbits.PCFG0
#define _PCFG1 ADPCFGbits.PCFG1
#define _PCFG2 ADPCFGbits.PCFG2
#define _PCFG3 ADPCFGbits.PCFG3
#define _PCFG4 ADPCFGbits.PCFG4
#define _PCFG5 ADPCFGbits.PCFG5
#define _PCFG6 ADPCFGbits.PCFG6
#define _PCFG7 ADPCFGbits.PCFG7
#define _PCFG8 ADPCFGbits.PCFG8
#define _PCFG9 ADPCFGbits.PCFG9
#define _PCFG10 ADPCFGbits.PCFG10
#define _PCFG11 ADPCFGbits.PCFG11
#define _PCFG12 ADPCFGbits.PCFG12
#define _PCFG13 ADPCFGbits.PCFG13
#define _PCFG14 ADPCFGbits.PCFG14
#define _PCFG15 ADPCFGbits.PCFG15

/* ADCSSL */
#define _CSSL0 ADCSSLbits.CSSL0
#define _CSSL1 ADCSSLbits.CSSL1
#define _CSSL2 ADCSSLbits.CSSL2
#define _CSSL3 ADCSSLbits.CSSL3
#define _CSSL4 ADCSSLbits.CSSL4
#define _CSSL5 ADCSSLbits.CSSL5
#define _CSSL6 ADCSSLbits.CSSL6
#define _CSSL7 ADCSSLbits.CSSL7
#define _CSSL8 ADCSSLbits.CSSL8
#define _CSSL9 ADCSSLbits.CSSL9
#define _CSSL10 ADCSSLbits.CSSL10
#define _CSSL11 ADCSSLbits.CSSL11
#define _CSSL12 ADCSSLbits.CSSL12
#define _CSSL13 ADCSSLbits.CSSL13
#define _CSSL14 ADCSSLbits.CSSL14
#define _CSSL15 ADCSSLbits.CSSL15

/* TRISB */
#define _TRISB0 TRISBbits.TRISB0
#define _TRISB1 TRISBbits.TRISB1
#define _TRISB2 TRISBbits.TRISB2
#define _TRISB3 TRISBbits.TRISB3
#define _TRISB4 TRISBbits.TRISB4
#define _TRISB5 TRISBbits.TRISB5
#define _TRISB6 TRISBbits.TRISB6
#define _TRISB7 TRISBbits.TRISB7
#define _TRISB8 TRISBbits.TRISB8
#define _TRISB9 TRISBbits.TRISB9
#define _TRISB10 TRISBbits.TRISB10
#define _TRISB11 TRISBbits.TRISB11
#define _TRISB12 TRISBbits.TRISB12
#define _TRISB13 TRISBbits.TRISB13
#define _TRISB14 TRISBbits.TRISB14
#define _TRISB15 TRISBbits.TRISB15

/* PORTB */
#define _RB0 PORTBbits.RB0
#define _RB1 PORTBbits.RB1
#define _RB2 PORTBbits.RB2
#define _RB3 PORTBbits.RB3
#define _RB4 PORTBbits.RB4
#define _RB5 PORTBbits.RB5
#define _RB6 PORTBbits.RB6
#define _RB7 PORTBbits.RB7
#define _RB8 PORTBbits.RB8
#define _RB9 PORTBbits.RB9
#define _RB10 PORTBbits.RB10
#define _RB11 PORTBbits.RB11
#define _RB12 PORTBbits.RB12
#define _RB13 PORTBbits.RB13
#define _RB14 PORTBbits.RB14
#define _RB15 PORTBbits.RB15

/* LATB */
#define _LATB0 LATBbits.LATB0
#define _LATB1 LATBbits.LATB1
#define _LATB2 LATBbits.LATB2
#define _LATB3 LATBbits.LATB3
#define _LATB4 LATBbits.LATB4
#define _LATB5 LATBbits.LATB5
#define _LATB6 LATBbits.LATB6
#define _LATB7 LATBbits.LATB7
#define _LATB8 LATBbits.LATB8
#define _LATB9 LATBbits.LATB9
#define _LATB10 LATBbits.LATB10
#define _LATB11 LATBbits.LATB11
#define _LATB12 LATBbits.LATB12
#define _LATB13 LATBbits.LATB13
#define _LATB14 LATBbits.LATB14
#define _LATB15 LATBbits.LATB15

/* TRISC */
#define _TRISC1 TRISCbits.TRISC1
#define _TRISC2 TRISCbits.TRISC2
#define _TRISC13 TRISCbits.TRISC13
#define _TRISC14 TRISCbits.TRISC14
#define _TRISC15 TRISCbits.TRISC15

/* PORTC */
#define _RC1 PORTCbits.RC1
#define _RC2 PORTCbits.RC2
#define _RC13 PORTCbits.RC13
#define _RC14 PORTCbits.RC14
#define _RC15 PORTCbits.RC15

/* LATC */
#define _LATC1 LATCbits.LATC1
#define _LATC2 LATCbits.LATC2
#define _LATC13 LATCbits.LATC13
#define _LATC14 LATCbits.LATC14
#define _LATC15 LATCbits.LATC15

/* TRISD */
#define _TRISD0 TRISDbits.TRISD0
#define _TRISD1 TRISDbits.TRISD1
#define _TRISD2 TRISDbits.TRISD2
#define _TRISD3 TRISDbits.TRISD3
#define _TRISD4 TRISDbits.TRISD4
#define _TRISD5 TRISDbits.TRISD5
#define _TRISD6 TRISDbits.TRISD6
#define _TRISD7 TRISDbits.TRISD7
#define _TRISD8 TRISDbits.TRISD8
#define _TRISD9 TRISDbits.TRISD9
#define _TRISD10 TRISDbits.TRISD10
#define _TRISD11 TRISDbits.TRISD11

/* PORTD */
#define _RD0 PORTDbits.RD0
#define _RD1 PORTDbits.RD1
#define _RD2 PORTDbits.RD2
#define _RD3 PORTDbits.RD3
#define _RD4 PORTDbits.RD4
#define _RD5 PORTDbits.RD5
#define _RD6 PORTDbits.RD6
#define _RD7 PORTDbits.RD7
#define _RD8 PORTDbits.RD8
#define _RD9 PORTDbits.RD9
#define _RD10 PORTDbits.RD10
#define _RD11 PORTDbits.RD11

/* LATD */
#define _LATD0 LATDbits.LATD0
#define _LATD1 LATDbits.LATD1
#define _LATD2 LATDbits.LATD2
#define _LATD3 LATDbits.LATD3
#define _LATD4 LATDbits.LATD4
#define _LATD5 LATDbits.LATD5
#define _LATD6 LATDbits.LATD6
#define _LATD7 LATDbits.LATD7
#define _LATD8 LATDbits.LATD8
#define _LATD9 LATDbits.LATD9
#define _LATD10 LATDbits.LATD10
#define _LATD11 LATDbits.LATD11

/* TRISF */
#define _TRISF0 TRISFbits.TRISF0
#define _TRISF1 TRISFbits.TRISF1
#define _TRISF2 TRISFbits.TRISF2
#define _TRISF3 TRISFbits.TRISF3
#define _TRISF4 TRISFbits.TRISF4
#define _TRISF5 TRISFbits.TRISF5
#define _TRISF6 TRISFbits.TRISF6

/* PORTF */
#define _RF0 PORTFbits.RF0
#define _RF1 PORTFbits.RF1
#define _RF2 PORTFbits.RF2
#define _RF3 PORTFbits.RF3
#define _RF4 PORTFbits.RF4
#define _RF5 PORTFbits.RF5
#define _RF6 PORTFbits.RF6

/* LATF */
#define _LATF0 LATFbits.LATF0
#define _LATF1 LATFbits.LATF1
#define _LATF2 LATFbits.LATF2
#define _LATF3 LATFbits.LATF3
#define _LATF4 LATFbits.LATF4
#define _LATF5 LATFbits.LATF5
#define _LATF6 LATFbits.LATF6

/* TRISG */
#define _TRISG0 TRISGbits.TRISG0
#define _TRISG1 TRISGbits.TRISG1
#define _TRISG2 TRISGbits.TRISG2
#define _TRISG3 TRISGbits.TRISG3
#define _TRISG6 TRISGbits.TRISG6
#define _TRISG7 TRISGbits.TRISG7
#define _TRISG8 TRISGbits.TRISG8
#define _TRISG9 TRISGbits.TRISG9
#define _TRISG12 TRISGbits.TRISG12
#define _TRISG13 TRISGbits.TRISG13
#define _TRISG14 TRISGbits.TRISG14
#define _TRISG15 TRISGbits.TRISG15

/* PORTG */
#define _RG0 PORTGbits.RG0
#define _RG1 PORTGbits.RG1
#define _RG2 PORTGbits.RG2
#define _RG3 PORTGbits.RG3
#define _RG6 PORTGbits.RG6
#define _RG7 PORTGbits.RG7
#define _RG8 PORTGbits.RG8
#define _RG9 PORTGbits.RG9
#define _RG12 PORTGbits.RG12
#define _RG13 PORTGbits.RG13
#define _RG14 PORTGbits.RG14
#define _RG15 PORTGbits.RG15

/* LATG */
#define _LATG0 LATGbits.LATG0
#define _LATG1 LATGbits.LATG1
#define _LATG2 LATGbits.LATG2
#define _LATG3 LATGbits.LATG3
#define _LATG6 LATGbits.LATG6
#define _LATG7 LATGbits.LATG7
#define _LATG8 LATGbits.LATG8
#define _LATG9 LATGbits.LATG9
#define _LATG12 LATGbits.LATG12
#define _LATG13 LATGbits.LATG13
#define _LATG14 LATGbits.LATG14
#define _LATG15 LATGbits.LATG15

/* C1RXF0SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF0EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF0EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXF1SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF1EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF1EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXF2SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF2EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF2EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXF3SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF3EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF3EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXF4SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF4EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF4EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXF5SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXF5EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXF5EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXM0SID */
/* Bitname _MIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXM0EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXM0EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1RXM1SID */
/* Bitname _MIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RXM1EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RXM1EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1TX2SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C1TX2EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C1TX2DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C1TX2B1 */
/* Bitname _TXB2D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D17 cannot be defined because it is used by more than one SFR */

/* C1TX2B2 */
/* Bitname _TXB2D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D37 cannot be defined because it is used by more than one SFR */

/* C1TX2B3 */
#define _TX2B2D40 C1TX2B3bits.TX2B2D40
#define _TX2B2D41 C1TX2B3bits.TX2B2D41
#define _TX2B2D42 C1TX2B3bits.TX2B2D42
#define _TX2B2D43 C1TX2B3bits.TX2B2D43
#define _TX2B2D44 C1TX2B3bits.TX2B2D44
#define _TX2B2D45 C1TX2B3bits.TX2B2D45
#define _TX2B2D46 C1TX2B3bits.TX2B2D46
#define _TX2B2D47 C1TX2B3bits.TX2B2D47
/* Bitname _TXB2D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D57 cannot be defined because it is used by more than one SFR */

/* C1TX2B4 */
#define _TX2B2D60 C1TX2B4bits.TX2B2D60
#define _TX2B2D61 C1TX2B4bits.TX2B2D61
#define _TX2B2D62 C1TX2B4bits.TX2B2D62
#define _TX2B2D63 C1TX2B4bits.TX2B2D63
#define _TX2B2D64 C1TX2B4bits.TX2B2D64
#define _TX2B2D65 C1TX2B4bits.TX2B2D65
#define _TX2B2D66 C1TX2B4bits.TX2B2D66
#define _TX2B2D67 C1TX2B4bits.TX2B2D67
#define _TX2B2D70 C1TX2B4bits.TX2B2D70
#define _TX2B2D71 C1TX2B4bits.TX2B2D71
#define _TX2B2D72 C1TX2B4bits.TX2B2D72
#define _TX2B2D73 C1TX2B4bits.TX2B2D73
#define _TX2B2D74 C1TX2B4bits.TX2B2D74
#define _TX2B2D75 C1TX2B4bits.TX2B2D75
#define _TX2B2D76 C1TX2B4bits.TX2B2D76
#define _TX2B2D77 C1TX2B4bits.TX2B2D77

/* C1TX2CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C1TX1SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C1TX1EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C1TX1DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C1TX1B1 */
/* Bitname _TXB1D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D17 cannot be defined because it is used by more than one SFR */

/* C1TX1B2 */
/* Bitname _TXB1D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D37 cannot be defined because it is used by more than one SFR */

/* C1TX1B3 */
/* Bitname _TXB1D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D57 cannot be defined because it is used by more than one SFR */

/* C1TX1B4 */
/* Bitname _TXB1D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D77 cannot be defined because it is used by more than one SFR */

/* C1TX1CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C1TX0SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C1TX0EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C1TX0DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C1TX0B1 */
/* Bitname _TXB0D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D17 cannot be defined because it is used by more than one SFR */

/* C1TX0B2 */
/* Bitname _TXB0D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D37 cannot be defined because it is used by more than one SFR */

/* C1TX0B3 */
/* Bitname _TXB0D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D57 cannot be defined because it is used by more than one SFR */

/* C1TX0B4 */
/* Bitname _TXB0D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D77 cannot be defined because it is used by more than one SFR */

/* C1TX0CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C1RX1SID */
/* Bitname _RXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RX1EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RX1DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C1RX1B1 */
/* Bitname _TXB1D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D17 cannot be defined because it is used by more than one SFR */

/* C1RX1B2 */
/* Bitname _TXB1D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D37 cannot be defined because it is used by more than one SFR */

/* C1RX1B3 */
/* Bitname _TXB1D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D57 cannot be defined because it is used by more than one SFR */

/* C1RX1B4 */
/* Bitname _TXB1D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D77 cannot be defined because it is used by more than one SFR */

/* C1RX1CON */
/* Bitname _FILHIT cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTRRO cannot be defined because it is used by more than one SFR */
/* Bitname _RXFUL cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT0 cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT1 cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT2 cannot be defined because it is used by more than one SFR */

/* C1RX0SID */
/* Bitname _RXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C1RX0EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C1RX0DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C1RX0B1 */
/* Bitname _TXB0D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D17 cannot be defined because it is used by more than one SFR */

/* C1RX0B2 */
/* Bitname _TXB0D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D37 cannot be defined because it is used by more than one SFR */

/* C1RX0B3 */
/* Bitname _TXB0D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D57 cannot be defined because it is used by more than one SFR */

/* C1RX0B4 */
/* Bitname _TXB0D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D77 cannot be defined because it is used by more than one SFR */

/* C1RX0CON */
/* Bitname _FILHIT0 cannot be defined because it is used by more than one SFR */
/* Bitname _JTOFF cannot be defined because it is used by more than one SFR */
/* Bitname _DBEN cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTRRO cannot be defined because it is used by more than one SFR */
/* Bitname _RXFUL cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT cannot be defined because it is used by more than one SFR */
/* Bitname _RXB0DBEN cannot be defined because it is used by more than one SFR */

/* C1CTRL */
/* Bitname _ICODE cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP cannot be defined because it is used by more than one SFR */
/* Bitname _CANCKS cannot be defined because it is used by more than one SFR */
/* Bitname _ABAT cannot be defined because it is used by more than one SFR */
/* Bitname _CANSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _CANFRZ cannot be defined because it is used by more than one SFR */
/* Bitname _CANCAP cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE2 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE0 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE1 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE2 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP0 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP1 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP2 cannot be defined because it is used by more than one SFR */
/* Bitname _CSIDL cannot be defined because it is used by more than one SFR */

/* C1CFG1 */
/* Bitname _BRP cannot be defined because it is used by more than one SFR */
/* Bitname _SJW cannot be defined because it is used by more than one SFR */
/* Bitname _BRP0 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP1 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP2 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP3 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP4 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP5 cannot be defined because it is used by more than one SFR */
/* Bitname _SJW0 cannot be defined because it is used by more than one SFR */
/* Bitname _SJW1 cannot be defined because it is used by more than one SFR */

/* C1CFG2 */
/* Bitname _PRSEG cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH cannot be defined because it is used by more than one SFR */
/* Bitname _SAM cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PHTS cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH cannot be defined because it is used by more than one SFR */
/* Bitname _WAKFIL cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG0 cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG1 cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG2 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH0 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH1 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH2 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH0 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH1 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH2 cannot be defined because it is used by more than one SFR */

/* C1INTF */
/* Bitname _RXB0IF cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2IF cannot be defined because it is used by more than one SFR */
/* Bitname _ERRIF cannot be defined because it is used by more than one SFR */
/* Bitname _WAKIF cannot be defined because it is used by more than one SFR */
/* Bitname _IRXIF cannot be defined because it is used by more than one SFR */
/* Bitname _EWARN cannot be defined because it is used by more than one SFR */
/* Bitname _RXWARN cannot be defined because it is used by more than one SFR */
/* Bitname _TXWARN cannot be defined because it is used by more than one SFR */
/* Bitname _RXBP cannot be defined because it is used by more than one SFR */
/* Bitname _TXBP cannot be defined because it is used by more than one SFR */
/* Bitname _TXBO cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1OVFL cannot be defined because it is used by more than one SFR */
/* Bitname _RXB0OVFL cannot be defined because it is used by more than one SFR */
/* Bitname _RX0IF cannot be defined because it is used by more than one SFR */
/* Bitname _RX1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX0IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX2IF cannot be defined because it is used by more than one SFR */
/* Bitname _IVRIF cannot be defined because it is used by more than one SFR */
/* Bitname _RXWAR cannot be defined because it is used by more than one SFR */
/* Bitname _TXWAR cannot be defined because it is used by more than one SFR */
/* Bitname _RXEP cannot be defined because it is used by more than one SFR */
/* Bitname _TXEP cannot be defined because it is used by more than one SFR */
/* Bitname _RX1OVR cannot be defined because it is used by more than one SFR */
/* Bitname _RX0OVR cannot be defined because it is used by more than one SFR */

/* C1INTE */
/* Bitname _RXB0IE cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2IE cannot be defined because it is used by more than one SFR */
/* Bitname _ERRIE cannot be defined because it is used by more than one SFR */
/* Bitname _WAKIE cannot be defined because it is used by more than one SFR */
/* Bitname _IRXIE cannot be defined because it is used by more than one SFR */
/* Bitname _RX0IE cannot be defined because it is used by more than one SFR */
/* Bitname _RX1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX0IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX2IE cannot be defined because it is used by more than one SFR */
/* Bitname _IVRIE cannot be defined because it is used by more than one SFR */

/* C1EC */
/* Bitname _REC0 cannot be defined because it is used by more than one SFR */
/* Bitname _REC1 cannot be defined because it is used by more than one SFR */
/* Bitname _REC2 cannot be defined because it is used by more than one SFR */
/* Bitname _REC3 cannot be defined because it is used by more than one SFR */
/* Bitname _REC4 cannot be defined because it is used by more than one SFR */
/* Bitname _REC5 cannot be defined because it is used by more than one SFR */
/* Bitname _REC6 cannot be defined because it is used by more than one SFR */
/* Bitname _REC7 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC0 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC1 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC2 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC3 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC4 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC5 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC6 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC7 cannot be defined because it is used by more than one SFR */

/* C2RXF0SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF0EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF0EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXF1SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF1EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF1EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXF2SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF2EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF2EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXF3SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF3EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF3EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXF4SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF4EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF4EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXF5SID */
/* Bitname _EXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXF5EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXF5EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXM0SID */
/* Bitname _MIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXM0EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXM0EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2RXM1SID */
/* Bitname _MIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RXM1EIDH */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RXM1EIDL */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2TX2SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C2TX2EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C2TX2DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C2TX2B1 */
/* Bitname _TXB2D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D17 cannot be defined because it is used by more than one SFR */

/* C2TX2B2 */
/* Bitname _TXB2D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D37 cannot be defined because it is used by more than one SFR */

/* C2TX2B3 */
#define _TXB2D40 C2TX2B3bits.TXB2D40
#define _TXB2D41 C2TX2B3bits.TXB2D41
#define _TXB2D42 C2TX2B3bits.TXB2D42
#define _TXB2D43 C2TX2B3bits.TXB2D43
#define _TXB2D44 C2TX2B3bits.TXB2D44
#define _TXB2D45 C2TX2B3bits.TXB2D45
#define _TXB2D46 C2TX2B3bits.TXB2D46
#define _TXB2D47 C2TX2B3bits.TXB2D47
/* Bitname _TXB2D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2D57 cannot be defined because it is used by more than one SFR */

/* C2TX2B4 */
#define _TXB2D60 C2TX2B4bits.TXB2D60
#define _TXB2D61 C2TX2B4bits.TXB2D61
#define _TXB2D62 C2TX2B4bits.TXB2D62
#define _TXB2D63 C2TX2B4bits.TXB2D63
#define _TXB2D64 C2TX2B4bits.TXB2D64
#define _TXB2D65 C2TX2B4bits.TXB2D65
#define _TXB2D66 C2TX2B4bits.TXB2D66
#define _TXB2D67 C2TX2B4bits.TXB2D67
#define _TXB2D70 C2TX2B4bits.TXB2D70
#define _TXB2D71 C2TX2B4bits.TXB2D71
#define _TXB2D72 C2TX2B4bits.TXB2D72
#define _TXB2D73 C2TX2B4bits.TXB2D73
#define _TXB2D74 C2TX2B4bits.TXB2D74
#define _TXB2D75 C2TX2B4bits.TXB2D75
#define _TXB2D76 C2TX2B4bits.TXB2D76
#define _TXB2D77 C2TX2B4bits.TXB2D77

/* C2TX2CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C2TX1SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C2TX1EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C2TX1DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C2TX1B1 */
/* Bitname _TXB1D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D17 cannot be defined because it is used by more than one SFR */

/* C2TX1B2 */
/* Bitname _TXB1D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D37 cannot be defined because it is used by more than one SFR */

/* C2TX1B3 */
/* Bitname _TXB1D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D57 cannot be defined because it is used by more than one SFR */

/* C2TX1B4 */
/* Bitname _TXB1D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D77 cannot be defined because it is used by more than one SFR */

/* C2TX1CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C2TX0SID */
/* Bitname _TXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10_6 cannot be defined because it is used by more than one SFR */

/* C2TX0EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13_6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17_14 cannot be defined because it is used by more than one SFR */

/* C2TX0DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _TXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */

/* C2TX0B1 */
/* Bitname _TXB0D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D17 cannot be defined because it is used by more than one SFR */

/* C2TX0B2 */
/* Bitname _TXB0D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D37 cannot be defined because it is used by more than one SFR */

/* C2TX0B3 */
/* Bitname _TXB0D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D57 cannot be defined because it is used by more than one SFR */

/* C2TX0B4 */
/* Bitname _TXB0D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D77 cannot be defined because it is used by more than one SFR */

/* C2TX0CON */
/* Bitname _TXPRI cannot be defined because it is used by more than one SFR */
/* Bitname _TXREQ cannot be defined because it is used by more than one SFR */
/* Bitname _TXERR cannot be defined because it is used by more than one SFR */
/* Bitname _TXLARB cannot be defined because it is used by more than one SFR */
/* Bitname _TXABT cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI0 cannot be defined because it is used by more than one SFR */
/* Bitname _TXPRI1 cannot be defined because it is used by more than one SFR */

/* C2RX1SID */
/* Bitname _RXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RX1EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RX1DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C2RX1B1 */
/* Bitname _TXB1D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D17 cannot be defined because it is used by more than one SFR */

/* C2RX1B2 */
/* Bitname _TXB1D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D37 cannot be defined because it is used by more than one SFR */

/* C2RX1B3 */
/* Bitname _TXB1D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D57 cannot be defined because it is used by more than one SFR */

/* C2RX1B4 */
/* Bitname _TXB1D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1D77 cannot be defined because it is used by more than one SFR */

/* C2RX1CON */
/* Bitname _FILHIT cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTRRO cannot be defined because it is used by more than one SFR */
/* Bitname _RXFUL cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT0 cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT1 cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT2 cannot be defined because it is used by more than one SFR */

/* C2RX0SID */
/* Bitname _RXIDE cannot be defined because it is used by more than one SFR */
/* Bitname _SRR cannot be defined because it is used by more than one SFR */
/* Bitname _SID0 cannot be defined because it is used by more than one SFR */
/* Bitname _SID1 cannot be defined because it is used by more than one SFR */
/* Bitname _SID2 cannot be defined because it is used by more than one SFR */
/* Bitname _SID3 cannot be defined because it is used by more than one SFR */
/* Bitname _SID4 cannot be defined because it is used by more than one SFR */
/* Bitname _SID5 cannot be defined because it is used by more than one SFR */
/* Bitname _SID6 cannot be defined because it is used by more than one SFR */
/* Bitname _SID7 cannot be defined because it is used by more than one SFR */
/* Bitname _SID8 cannot be defined because it is used by more than one SFR */
/* Bitname _SID9 cannot be defined because it is used by more than one SFR */
/* Bitname _SID10 cannot be defined because it is used by more than one SFR */
/* Bitname _SID cannot be defined because it is used by more than one SFR */

/* C2RX0EID */
/* Bitname _EID6 cannot be defined because it is used by more than one SFR */
/* Bitname _EID7 cannot be defined because it is used by more than one SFR */
/* Bitname _EID8 cannot be defined because it is used by more than one SFR */
/* Bitname _EID9 cannot be defined because it is used by more than one SFR */
/* Bitname _EID10 cannot be defined because it is used by more than one SFR */
/* Bitname _EID11 cannot be defined because it is used by more than one SFR */
/* Bitname _EID12 cannot be defined because it is used by more than one SFR */
/* Bitname _EID13 cannot be defined because it is used by more than one SFR */
/* Bitname _EID14 cannot be defined because it is used by more than one SFR */
/* Bitname _EID15 cannot be defined because it is used by more than one SFR */
/* Bitname _EID16 cannot be defined because it is used by more than one SFR */
/* Bitname _EID17 cannot be defined because it is used by more than one SFR */

/* C2RX0DLC */
/* Bitname _DLC cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB0 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRB1 cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTR cannot be defined because it is used by more than one SFR */
/* Bitname _EID0 cannot be defined because it is used by more than one SFR */
/* Bitname _EID1 cannot be defined because it is used by more than one SFR */
/* Bitname _EID2 cannot be defined because it is used by more than one SFR */
/* Bitname _EID3 cannot be defined because it is used by more than one SFR */
/* Bitname _EID4 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5 cannot be defined because it is used by more than one SFR */
/* Bitname _EID5_0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC0 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC1 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC2 cannot be defined because it is used by more than one SFR */
/* Bitname _DLC3 cannot be defined because it is used by more than one SFR */

/* C2RX0B1 */
/* Bitname _TXB0D00 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D01 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D02 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D03 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D04 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D05 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D06 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D07 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D10 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D11 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D12 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D13 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D14 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D15 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D16 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D17 cannot be defined because it is used by more than one SFR */

/* C2RX0B2 */
/* Bitname _TXB0D20 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D21 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D22 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D23 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D24 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D25 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D26 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D27 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D30 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D31 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D32 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D33 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D34 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D35 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D36 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D37 cannot be defined because it is used by more than one SFR */

/* C2RX0B3 */
/* Bitname _TXB0D40 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D41 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D42 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D43 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D44 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D45 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D46 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D47 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D50 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D51 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D52 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D53 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D54 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D55 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D56 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D57 cannot be defined because it is used by more than one SFR */

/* C2RX0B4 */
/* Bitname _TXB0D60 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D61 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D62 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D63 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D64 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D65 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D66 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D67 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D70 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D71 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D72 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D73 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D74 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D75 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D76 cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0D77 cannot be defined because it is used by more than one SFR */

/* C2RX0CON */
/* Bitname _FILHIT0 cannot be defined because it is used by more than one SFR */
/* Bitname _JTOFF cannot be defined because it is used by more than one SFR */
/* Bitname _DBEN cannot be defined because it is used by more than one SFR */
/* Bitname _RXRTRRO cannot be defined because it is used by more than one SFR */
/* Bitname _RXFUL cannot be defined because it is used by more than one SFR */
/* Bitname _FILHIT cannot be defined because it is used by more than one SFR */
/* Bitname _RXB0DBEN cannot be defined because it is used by more than one SFR */

/* C2CTRL */
/* Bitname _ICODE cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP cannot be defined because it is used by more than one SFR */
/* Bitname _CANCKS cannot be defined because it is used by more than one SFR */
/* Bitname _ABAT cannot be defined because it is used by more than one SFR */
/* Bitname _CANSIDL cannot be defined because it is used by more than one SFR */
/* Bitname _CANFRZ cannot be defined because it is used by more than one SFR */
/* Bitname _CANCAP cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE0 cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE1 cannot be defined because it is used by more than one SFR */
/* Bitname _ICODE2 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE0 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE1 cannot be defined because it is used by more than one SFR */
/* Bitname _OPMODE2 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP0 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP1 cannot be defined because it is used by more than one SFR */
/* Bitname _REQOP2 cannot be defined because it is used by more than one SFR */
/* Bitname _CSIDL cannot be defined because it is used by more than one SFR */

/* C2CFG1 */
/* Bitname _BRP cannot be defined because it is used by more than one SFR */
/* Bitname _SJW cannot be defined because it is used by more than one SFR */
/* Bitname _BRP0 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP1 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP2 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP3 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP4 cannot be defined because it is used by more than one SFR */
/* Bitname _BRP5 cannot be defined because it is used by more than one SFR */
/* Bitname _SJW0 cannot be defined because it is used by more than one SFR */
/* Bitname _SJW1 cannot be defined because it is used by more than one SFR */

/* C2CFG2 */
/* Bitname _PRSEG cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH cannot be defined because it is used by more than one SFR */
/* Bitname _SAM cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PHTS cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH cannot be defined because it is used by more than one SFR */
/* Bitname _WAKFIL cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG0 cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG1 cannot be defined because it is used by more than one SFR */
/* Bitname _PRSEG2 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH0 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH1 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG1PH2 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH0 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH1 cannot be defined because it is used by more than one SFR */
/* Bitname _SEG2PH2 cannot be defined because it is used by more than one SFR */

/* C2INTF */
/* Bitname _RXB0IF cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2IF cannot be defined because it is used by more than one SFR */
/* Bitname _ERRIF cannot be defined because it is used by more than one SFR */
/* Bitname _WAKIF cannot be defined because it is used by more than one SFR */
/* Bitname _IRXIF cannot be defined because it is used by more than one SFR */
/* Bitname _EWARN cannot be defined because it is used by more than one SFR */
/* Bitname _RXWARN cannot be defined because it is used by more than one SFR */
/* Bitname _TXWARN cannot be defined because it is used by more than one SFR */
/* Bitname _RXBP cannot be defined because it is used by more than one SFR */
/* Bitname _TXBP cannot be defined because it is used by more than one SFR */
/* Bitname _TXBO cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1OVFL cannot be defined because it is used by more than one SFR */
/* Bitname _RXB0OVFL cannot be defined because it is used by more than one SFR */
/* Bitname _RX0IF cannot be defined because it is used by more than one SFR */
/* Bitname _RX1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX0IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX1IF cannot be defined because it is used by more than one SFR */
/* Bitname _TX2IF cannot be defined because it is used by more than one SFR */
/* Bitname _IVRIF cannot be defined because it is used by more than one SFR */
/* Bitname _RXWAR cannot be defined because it is used by more than one SFR */
/* Bitname _TXWAR cannot be defined because it is used by more than one SFR */
/* Bitname _RXEP cannot be defined because it is used by more than one SFR */
/* Bitname _TXEP cannot be defined because it is used by more than one SFR */
/* Bitname _RX1OVR cannot be defined because it is used by more than one SFR */
/* Bitname _RX0OVR cannot be defined because it is used by more than one SFR */

/* C2INTE */
/* Bitname _RXB0IE cannot be defined because it is used by more than one SFR */
/* Bitname _RXB1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB0IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TXB2IE cannot be defined because it is used by more than one SFR */
/* Bitname _ERRIE cannot be defined because it is used by more than one SFR */
/* Bitname _WAKIE cannot be defined because it is used by more than one SFR */
/* Bitname _IRXIE cannot be defined because it is used by more than one SFR */
/* Bitname _RX0IE cannot be defined because it is used by more than one SFR */
/* Bitname _RX1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX0IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX1IE cannot be defined because it is used by more than one SFR */
/* Bitname _TX2IE cannot be defined because it is used by more than one SFR */
/* Bitname _IVRIE cannot be defined because it is used by more than one SFR */

/* C2EC */
/* Bitname _REC0 cannot be defined because it is used by more than one SFR */
/* Bitname _REC1 cannot be defined because it is used by more than one SFR */
/* Bitname _REC2 cannot be defined because it is used by more than one SFR */
/* Bitname _REC3 cannot be defined because it is used by more than one SFR */
/* Bitname _REC4 cannot be defined because it is used by more than one SFR */
/* Bitname _REC5 cannot be defined because it is used by more than one SFR */
/* Bitname _REC6 cannot be defined because it is used by more than one SFR */
/* Bitname _REC7 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC0 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC1 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC2 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC3 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC4 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC5 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC6 cannot be defined because it is used by more than one SFR */
/* Bitname _TEC7 cannot be defined because it is used by more than one SFR */

/* RCON */
#define _POR RCONbits.POR
#define _BOR RCONbits.BOR
#define _IDLE RCONbits.IDLE
#define _SLEEP RCONbits.SLEEP
#define _WDTO RCONbits.WDTO
#define _SWDTEN RCONbits.SWDTEN
#define _SWR RCONbits.SWR
#define _EXTR RCONbits.EXTR
#define _LVDL RCONbits.LVDL
#define _LVDEN RCONbits.LVDEN
#define _BGST RCONbits.BGST
#define _IOPUWR RCONbits.IOPUWR
#define _TRAPR RCONbits.TRAPR
#define _LVDL0 RCONbits.LVDL0
#define _LVDL1 RCONbits.LVDL1
#define _LVDL2 RCONbits.LVDL2
#define _LVDL3 RCONbits.LVDL3

/* OSCCON */
#define _OSWEN OSCCONbits.OSWEN
#define _LPOSCEN OSCCONbits.LPOSCEN
#define _CF OSCCONbits.CF
#define _LOCK OSCCONbits.LOCK
#define _POST OSCCONbits.POST
#define _NOSC OSCCONbits.NOSC
#define _COSC OSCCONbits.COSC
#define _POST0 OSCCONbits.POST0
#define _POST1 OSCCONbits.POST1
#define _NOSC0 OSCCONbits.NOSC0
#define _NOSC1 OSCCONbits.NOSC1
#define _NOSC2 OSCCONbits.NOSC2
#define _COSC0 OSCCONbits.COSC0
#define _COSC1 OSCCONbits.COSC1
#define _COSC2 OSCCONbits.COSC2

/* OSCTUN */
#define _TUN0 OSCTUNbits.TUN0
#define _TUN1 OSCTUNbits.TUN1
#define _TUN2 OSCTUNbits.TUN2
#define _TUN3 OSCTUNbits.TUN3

/* BSRAM */
#define _RL_BSR BSRAMbits.RL_BSR
#define _IR_BSR BSRAMbits.IR_BSR
#define _IW_BSR BSRAMbits.IW_BSR

/* SSRAM */
#define _RL_SSR SSRAMbits.RL_SSR
#define _IR_SSR SSRAMbits.IR_SSR
#define _IW_SSR SSRAMbits.IW_SSR

/* NVMCON */
#define _PROGOP NVMCONbits.PROGOP
#define _TWRI NVMCONbits.TWRI
#define _WRERR NVMCONbits.WRERR
#define _WREN NVMCONbits.WREN
#define _WR NVMCONbits.WR
#define _PROGOP0 NVMCONbits.PROGOP0
#define _PROGOP1 NVMCONbits.PROGOP1
#define _PROGOP2 NVMCONbits.PROGOP2
#define _PROGOP3 NVMCONbits.PROGOP3
#define _PROGOP4 NVMCONbits.PROGOP4
#define _PROGOP5 NVMCONbits.PROGOP5
#define _PROGOP6 NVMCONbits.PROGOP6

/* NVMADRU */
#define _NVMADR NVMADRUbits.NVMADR

/* PMD1 */
#define _ADCMD PMD1bits.ADCMD
#define _C1MD PMD1bits.C1MD
#define _C2MD PMD1bits.C2MD
#define _SPI1MD PMD1bits.SPI1MD
#define _SPI2MD PMD1bits.SPI2MD
#define _U1MD PMD1bits.U1MD
#define _U2MD PMD1bits.U2MD
#define _I2CMD PMD1bits.I2CMD
#define _DCIMD PMD1bits.DCIMD
#define _T1MD PMD1bits.T1MD
#define _T2MD PMD1bits.T2MD
#define _T3MD PMD1bits.T3MD
#define _T4MD PMD1bits.T4MD
#define _T5MD PMD1bits.T5MD

/* PMD2 */
#define _OC1MD PMD2bits.OC1MD
#define _OC2MD PMD2bits.OC2MD
#define _OC3MD PMD2bits.OC3MD
#define _OC4MD PMD2bits.OC4MD
#define _OC5MD PMD2bits.OC5MD
#define _OC6MD PMD2bits.OC6MD
#define _OC7MD PMD2bits.OC7MD
#define _OC8MD PMD2bits.OC8MD
#define _IC1MD PMD2bits.IC1MD
#define _IC2MD PMD2bits.IC2MD
#define _IC3MD PMD2bits.IC3MD
#define _IC4MD PMD2bits.IC4MD
#define _IC5MD PMD2bits.IC5MD
#define _IC6MD PMD2bits.IC6MD
#define _IC7MD PMD2bits.IC7MD
#define _IC8MD PMD2bits.IC8MD


/* ---------------------------------------------------------- */
/* Some useful macros for inline assembler instructions       */
/* ---------------------------------------------------------- */

#define Nop()    __builtin_nop()
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep()  {__asm__ volatile ("pwrsav #0");}
#define Idle()   {__asm__ volatile ("pwrsav #1");}

/*----------------------------------------------------------- */
/* Some useful macros for allocating data memory              */
/*----------------------------------------------------------- */

/* The following macros require an argument N that specifies  */
/* alignment. N must a power of two, minimum value = 2.       */
/* For example, to declare an uninitialized array in X memory */
/* that is aligned to a 32 byte address:                      */
/*                                                            */
/* int _XBSS(32) xbuf[16];                                    */
/*                                                            */
#define _XBSS(N)    __attribute__((space(xmemory),far,aligned(N)))
#define _XDATA(N)   __attribute__((space(xmemory),far,aligned(N)))
#define _YBSS(N)    __attribute__((space(ymemory),far,aligned(N)))
#define _YDATA(N)   __attribute__((space(ymemory),far,aligned(N)))
/*                                                            */
/* To declare an initialized array in data EEPROM without     */
/* special alignment:                                         */
/*                                                            */
/* int _EEDATA(2) table1[] = {0, 1, 1, 2, 3, 5, 8, 13, 21};   */
/*                                                            */
#define _EEDATA(N)   __attribute__((space(eedata),aligned(N)))

/* The following macros do not require an argument. They can  */
/* be used to locate a variable in persistent data memory or  */
/* in near data memory. For example, to declare two variables */
/* that retain their values across a device reset:            */
/*                                                            */
/* int _PERSISTENT var1,var2;                                 */
/*                                                            */
#define _PERSISTENT __attribute__((persistent))
#define _NEAR       __attribute__((near))

/* ---------------------------------------------------------- */
/* Some useful macros for declaring functions                 */
/* ---------------------------------------------------------- */

/* The following macros can be used to declare interrupt      */
/* service routines (ISRs). For example, to declare an ISR    */
/* for the timer1 interrupt:                                  */
/*                                                            */
/* void _ISR _T1Interrupt(void);                              */
/*                                                            */
/* To declare an ISR for the SPI1 interrupt with fast         */
/* context save:                                              */
/*                                                            */
/* void _ISRFAST _SPI1Interrupt(void);                        */
/*                                                            */
/* Note: ISRs will be installed into the interrupt vector     */
/* tables automatically if the reserved names listed in the   */
/* MPLAB C30 Compiler User's Guide (DS51284) are used.        */
/*                                                            */
#define _ISR __attribute__((interrupt))
#define _ISRFAST __attribute__((interrupt, shadow))

/* ---------------------------------------------------------- */
/* Some useful macros for changing the CPU IPL                */
/* ---------------------------------------------------------- */

/* The following macros can be used to modify the current CPU */
/* IPL. The definition of the macro may vary from device to   */
/* device.                                                    */
/*                                                            */
/* To safely set the CPU IPL, use SET_CPU_IPL(ipl); the       */
/* valid range of ipl is 0-7, it may be any expression.       */
/*                                                            */
/* SET_CPU_IPL(7);                                            */
/*                                                            */
/* To preserve the current IPL and save it use                */
/* SET_AND_SAVE_CPU_IPL(save_to, ipl); the valid range of ipl */
/* is 0-7 and may be any expression, save_to should denote    */
/* some temporary storage.                                    */
/*                                                            */
/* int old_ipl;                                               */
/*                                                            */
/* SET_AND_SAVE_CPU_IPL(old_ipl, 7);                          */
/*                                                            */
/* The IPL can be restored with RESTORE_CPU_IPL(saved_to)     */
/*                                                            */
/* RESTORE_CPU_IPL(old_ipl);                                  */

#define SET_CPU_IPL(ipl) {       \
  int DISI_save;                 \
                                 \
  DISI_save = DISICNT;           \
  asm volatile ("disi #0x3FFF"); \
  SRbits.IPL = ipl;              \
  DISICNT = DISI_save; } (void) 0;

#define SET_AND_SAVE_CPU_IPL(save_to, ipl) { \
  save_to = SRbits.IPL; \
  SET_CPU_IPL(ipl); } (void) 0;

#define RESTORE_CPU_IPL(saved_to) SET_CPU_IPL(saved_to)



/* -------------------------------------------------------- */
/* Macros for setting device configuration registers        */
/* -------------------------------------------------------- */

/* Register FUID0 (0x8005c0)                               */

extern __attribute__((space(prog))) int _FUID0;
#define _FUID0(x) __attribute__((section("__FUID0.sec"),space(prog))) int _FUID0 = (x);

/*
** Only one invocation of FUID0 should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FUID0.
** Multiple options may be combined, as shown:
**
** _FUID0( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
*/

/* Register FUID1 (0x8005c2)                               */

extern __attribute__((space(prog))) int _FUID1;
#define _FUID1(x) __attribute__((section("__FUID1.sec"),space(prog))) int _FUID1 = (x);

/*
** Only one invocation of FUID1 should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FUID1.
** Multiple options may be combined, as shown:
**
** _FUID1( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
*/

/* Register FUID2 (0x8005c4)                               */

extern __attribute__((space(prog))) int _FUID2;
#define _FUID2(x) __attribute__((section("__FUID2.sec"),space(prog))) int _FUID2 = (x);

/*
** Only one invocation of FUID2 should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FUID2.
** Multiple options may be combined, as shown:
**
** _FUID2( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
*/

/* Register FUID3 (0x8005c6)                               */

extern __attribute__((space(prog))) int _FUID3;
#define _FUID3(x) __attribute__((section("__FUID3.sec"),space(prog))) int _FUID3 = (x);

/*
** Only one invocation of FUID3 should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FUID3.
** Multiple options may be combined, as shown:
**
** _FUID3( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
*/

/* Register FOSC (0xf80000)                               */

extern __attribute__((space(prog))) int _FOSC;
#define _FOSC(x) __attribute__((section("__FOSC.sec"),space(prog))) int _FOSC = (x);

/*
** Only one invocation of FOSC should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FOSC.
** Multiple options may be combined, as shown:
**
** _FOSC( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   Oscillator:
**     LPOSC                Low-Power 32KHz Osc (No change to Primary Osc Mode bits)
**     LP                   Low-Power 32KHz Osc (No change to Primary Osc Mode bits)
**     FRC                  Internal Fast RC (No change to Primary Osc Mode bits)
**     LPRC                 Internal Low-Power RC (No change to Primary Osc Mode bits)
**     XTL                  XTL
**     HS                   HS
**     XT                   XT
**     ERCIO                ERCIO
**     ERC                  ERC
**     EC                   EC
**     ECIO                 ECIO
**     FRC_PLL4             FRC w/PLL 4x
**     FRC_PLL16            FRC w/PLL 16x
**     XT_PLL4              XT w/PLL 4x
**     XT_PLL8              XT w/PLL 8x
**     XT_PLL16             XT w/PLL 16x
**     FRC_PLL8             FRC w/PLL 8x
**     ECIO_PLL4            ECIO w/PLL 4x
**     ECIO_PLL8            ECIO w/PLL 8x
**     ECIO_PLL16           ECIO w/PLL 16x
**     HS2_PLL4             HS2 w/PLL 4x
**     HS2_PLL8             HS2 w/PLL 8x
**     HS2_PLL16            HS2 w/PLL 16x
**     HS3_PLL4             HS3 w/PLL 4x
**     HS3_PLL8             HS3 w/PLL 8x
**     HS3_PLL16            HS3 w/PLL 16x
**
**   Clock Switching and Monitor:
**     CSW_FSCM_ON          Sw Enabled, Mon Enabled
**     CSW_ON_FSCM_OFF      Sw Enabled, Mon Disabled
**     CSW_FSCM_OFF         Sw Disabled, Mon Disabled
**
*/

#define LPOSC                0xF8E0
#define LP                   0xF8E0
#define FRC                  0xF9E0
#define LPRC                 0xFAE0
#define XTL                  0xFBE0
#define HS                   0xFBE2
#define XT                   0xFBE4
#define ERCIO                0xFBE8
#define ERC                  0xFBE9
#define EC                   0xFBEB
#define ECIO                 0xFBEC
#define FRC_PLL4             0xFFE1
#define FRC_PLL16            0xFFE3
#define XT_PLL4              0xFFE5
#define XT_PLL8              0xFFE6
#define XT_PLL16             0xFFE7
#define FRC_PLL8             0xFFEA
#define ECIO_PLL4            0xFFED
#define ECIO_PLL8            0xFFEE
#define ECIO_PLL16           0xFFEF
#define HS2_PLL4             0xFFF1
#define HS2_PLL8             0xFFF2
#define HS2_PLL16            0xFFF3
#define HS3_PLL4             0xFFF5
#define HS3_PLL8             0xFFF6
#define HS3_PLL16            0xFFF7

#define CSW_FSCM_ON          0x3FFF
#define CSW_ON_FSCM_OFF      0x7FFF
#define CSW_FSCM_OFF         0xBFFF

/* Register FWDT (0xf80002)                               */

extern __attribute__((space(prog))) int _FWDT;
#define _FWDT(x) __attribute__((section("__FWDT.sec"),space(prog))) int _FWDT = (x);

/*
** Only one invocation of FWDT should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FWDT.
** Multiple options may be combined, as shown:
**
** _FWDT( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   WDT Prescaler B:
**     WDTPSB_1             1:1
**     WDTPSB_2             1:2
**     WDTPSB_3             1:3
**     WDTPSB_4             1:4
**     WDTPSB_5             1:5
**     WDTPSB_6             1:6
**     WDTPSB_7             1:7
**     WDTPSB_8             1:8
**     WDTPSB_9             1:9
**     WDTPSB_10            1:10
**     WDTPSB_11            1:11
**     WDTPSB_12            1:12
**     WDTPSB_13            1:13
**     WDTPSB_14            1:14
**     WDTPSB_15            1:15
**     WDTPSB_16            1:16
**
**   WDT Prescaler A:
**     WDTPSA_1             1:1
**     WDTPSA_8             1:8
**     WDTPSA_64            1:64
**     WDTPSA_512           1:512
**
**   Watchdog Timer:
**     WDT_OFF              Disabled
**     WDT_ON               Enabled
**
*/

#define WDTPSB_1             0xFFF0
#define WDTPSB_2             0xFFF1
#define WDTPSB_3             0xFFF2
#define WDTPSB_4             0xFFF3
#define WDTPSB_5             0xFFF4
#define WDTPSB_6             0xFFF5
#define WDTPSB_7             0xFFF6
#define WDTPSB_8             0xFFF7
#define WDTPSB_9             0xFFF8
#define WDTPSB_10            0xFFF9
#define WDTPSB_11            0xFFFA
#define WDTPSB_12            0xFFFB
#define WDTPSB_13            0xFFFC
#define WDTPSB_14            0xFFFD
#define WDTPSB_15            0xFFFE
#define WDTPSB_16            0xFFFF

#define WDTPSA_1             0xFFCF
#define WDTPSA_8             0xFFDF
#define WDTPSA_64            0xFFEF
#define WDTPSA_512           0xFFFF

#define WDT_OFF              0x7FFF
#define WDT_ON               0xFFFF

/* Register FBORPOR (0xf80004)                               */

extern __attribute__((space(prog))) int _FBORPOR;
#define _FBORPOR(x) __attribute__((section("__FBORPOR.sec"),space(prog))) int _FBORPOR = (x);

/*
** Only one invocation of FBORPOR should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FBORPOR.
** Multiple options may be combined, as shown:
**
** _FBORPOR( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   POR Timer Value:
**     PWRT_OFF             Timer Disabled
**     PWRT_4               4ms
**     PWRT_16              16ms
**     PWRT_64              64ms
**
**   Brown Out Voltage:
**     BORV_45              4.5V
**     BORV_42              4.2V
**     BORV_27              2.7V
**     NONE                 Reserved
**
**   PBOR Enable:
**     PBOR_OFF             Disabled
**     PBOR_ON              Enabled
**
**   Master Clear Enable:
**     MCLR_DIS             Disabled
**     MCLR_EN              Enabled
**
*/

#define PWRT_OFF             0xFFFC
#define PWRT_4               0xFFFD
#define PWRT_16              0xFFFE
#define PWRT_64              0xFFFF

#define BORV_45              0xFFCF
#define BORV_42              0xFFDF
#define BORV_27              0xFFEF
#define NONE                 0xFFFF

#define PBOR_OFF             0xFF7F
#define PBOR_ON              0xFFFF

#define MCLR_DIS             0x7FFF
#define MCLR_EN              0xFFFF

/* Register FBS (0xf80006)                               */

extern __attribute__((space(prog))) int _FBS;
#define _FBS(x) __attribute__((section("__FBS.sec"),space(prog))) int _FBS = (x);

/*
** Only one invocation of FBS should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FBS.
** Multiple options may be combined, as shown:
**
** _FBS( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   Boot Segment Program Memory Write Protect:
**     WR_PROTECT_BOOT_ON   Boot Segment Program Memory is write protected
**     WR_PROTECT_BOOT_OFF  Boot Segment Program Memory may be written
**
**   Boot Segment Program Flash Memory Code Protection:
**     HIGH_LARGE_BOOT_CODE High Security, Large-sized Boot Flash
**     HIGH_MEDIUM_BOOT_CODEHigh Security, Medium-sized Boot Flash
**     HIGH_SMALL_BOOT_CODE High Security, Small-sized Boot Flash
**     STRD_LARGE_BOOT_CODE Standard Security, Large-sized Boot Flash
**     STRD_MEDIUM_BOOT_CODEStandard Security, Medium-sized Boot Flash
**     STRD_SMALL_BOOT_CODE Standard Security, Small-sized Boot Flash
**     NO_BOOT_CODE         No Boot Segment
**
**   Boot Segment Data EEPROM Protection:
**     SMALL_BOOT_EEPROM    Boot EEPROM Enabled
**     NO_BOOT_EEPROM       No Boot EEPROM
**
**   Boot Segment Data RAM Protection:
**     LARGE_BOOT_RAM       Large-sized Boot RAM
**     MED_BOOT_RAM         Medium-sized Boot RAM
**     SMALL_BOOT_RAM       Small-sized Boot RAM
**     NO_BOOT_RAM          No Boot RAM
**
*/

#define WR_PROTECT_BOOT_ON   0xFFFE
#define WR_PROTECT_BOOT_OFF  0xFFFF

#define HIGH_LARGE_BOOT_CODE 0xFFF1
#define HIGH_MEDIUM_BOOT_CODE 0xFFF3
#define HIGH_SMALL_BOOT_CODE 0xFFF5
#define STRD_LARGE_BOOT_CODE 0xFFF9
#define STRD_MEDIUM_BOOT_CODE 0xFFFB
#define STRD_SMALL_BOOT_CODE 0xFFFD
#define NO_BOOT_CODE         0xFFFF

#define SMALL_BOOT_EEPROM    0xFEFF
#define NO_BOOT_EEPROM       0xFFFF

#define LARGE_BOOT_RAM       0xCFFF
#define MED_BOOT_RAM         0xDFFF
#define SMALL_BOOT_RAM       0xEFFF
#define NO_BOOT_RAM          0xFFFF

/* Register FSS (0xf80008)                               */

extern __attribute__((space(prog))) int _FSS;
#define _FSS(x) __attribute__((section("__FSS.sec"),space(prog))) int _FSS = (x);

/*
** Only one invocation of FSS should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FSS.
** Multiple options may be combined, as shown:
**
** _FSS( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   Secure Segment Program Write Protect:
**     WR_PROT_SEC_ON       Enabled
**     WR_PROT_SEC_OFF      Disabled
**
**   Secure Segment Program Flash Memory Code Protection:
**     HIGH_LARGE_SEC_CODE  High Security, Large-sized Secure Flash
**     HIGH_MEDIUM_SEC_CODE High Security, Medium-sized Secure Flash
**     HIGH_SMALL_SEC_CODE  High Security, Small-sized Secure Flash
**     STRD_LAR_SEC_CODE    Standard Security, Large-sized Secure Flash
**     STRD_MED_SEC_CODE    Standard Security, Medium-sized Secure Flash
**     STRD_SMALL_SEC_CODE  Standard Security, Small-sized Secure Flash
**     NO_SEC_CODE          No Secure Segment
**
**   Secure Segment Data EEPROM Protection:
**     LAR_SEC_EEPROM       Large-sized Secure Data EEPROM
**     MED_SEC_EEPROM       Medium-sized Secure Data EEPROM
**     SMALL_SEC_EEPROM     Small-sized Secure Data EEPROM
**     NO_SEC_EEPROM        No Segment Data EEPROM
**
**   Secure Segment Data RAM Protection:
**     LARGE_SEC_RAM        Large-sized Secure RAM
**     MED_SEC_RAM          Medium-sized Secure RAM
**     SMALL_SEC_RAM        Small-sized Secure RAM
**     NO_SEC_RAM           No Secure RAM
**
*/

#define WR_PROT_SEC_ON       0xFFFE
#define WR_PROT_SEC_OFF      0xFFFF

#define HIGH_LARGE_SEC_CODE  0xFFF1
#define HIGH_MEDIUM_SEC_CODE 0xFFF3
#define HIGH_SMALL_SEC_CODE  0xFFF5
#define STRD_LAR_SEC_CODE    0xFFF9
#define STRD_MED_SEC_CODE    0xFFFB
#define STRD_SMALL_SEC_CODE  0xFFFD
#define NO_SEC_CODE          0xFFFF

#define LAR_SEC_EEPROM       0xFCFF
#define MED_SEC_EEPROM       0xFDFF
#define SMALL_SEC_EEPROM     0xFEFF
#define NO_SEC_EEPROM        0xFFFF

#define LARGE_SEC_RAM        0xCFFF
#define MED_SEC_RAM          0xDFFF
#define SMALL_SEC_RAM        0xEFFF
#define NO_SEC_RAM           0xFFFF

/* Register FGS (0xf8000a)                               */

extern __attribute__((space(prog))) int _FGS;
#define _FGS(x) __attribute__((section("__FGS.sec"),space(prog))) int _FGS = (x);

/*
** Only one invocation of FGS should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FGS.
** Multiple options may be combined, as shown:
**
** _FGS( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   General Code Segment Write Protect:
**     GWRP_ON              Enabled
**     GWRP_OFF             Disabled
**
**   General Segment Code Protection:
**     GSS_HIGH             High Security Code Protection Enabled
**     GSS_ON               Enabled
**     GSS_OFF              Disabled
**     CODE_PROT_OFF        Disabled
**
*/

#define GWRP_ON              0xFFFE
#define GWRP_OFF             0xFFFF

#define GSS_HIGH             0xFFFB
#define GSS_ON               0xFFFD
#define GSS_OFF              0xFFFF
#define CODE_PROT_OFF        0xFFFF

/* Register FICD (0xf8000c)                               */

extern __attribute__((space(prog))) int _FICD;
#define _FICD(x) __attribute__((section("__FICD.sec"),space(prog))) int _FICD = (x);

/*
** Only one invocation of FICD should appear in a project,
** at the top of a C source file (outside of any function).
**
** The following constants can be used to set FICD.
** Multiple options may be combined, as shown:
**
** _FICD( OPT1_ON & OPT2_OFF & OPT3_PLL )
**
**   Comm Channel Select:
**     ICS_PGD3             Use EMUC3 and EMUD3
**     PGD3                 Use EMUC3 and EMUD3
**     ICS_PGD2             Use EMUC2 and EMUD2
**     PGD2                 Use EMUC2 and EMUD2
**     ICS_PGD1             Use EMUC1 and EMUD1
**     PGD1                 Use EMUC1 and EMUD1
**     ICS_PGD              Use PGC/EMUC and PGD/EMUD
**     PGD                  Use PGC/EMUC and PGD/EMUD
**
*/

#define ICS_PGD3             0xFFFC
#define PGD3                 0xFFFC
#define ICS_PGD2             0xFFFD
#define PGD2                 0xFFFD
#define ICS_PGD1             0xFFFE
#define PGD1                 0xFFFE
#define ICS_PGD              0xFFFF
#define PGD                  0xFFFF


#endif
