/*--------------------------------------------------------------------

  Title     : Eeprom read and write routines Version 1.0 
  Filename  : eeprom_rw.s 
  
  Copyright @ BobTheBass

/*------------------------------------------------------------------

  CHANGE HISTORY

  Issue     Modifier  Date      Change Description
  1.0       RW        01/11/07  First Issue
  Software under version control, 
  see log file for changes.
      
--------------------------------------------------------------------*/

        .include "p30f6012a.inc"

        .global _WriteWord
        .global _ReadWord

;------ eeprom address

        .equiv  ADDRESS_HI,     0x007F
        .equiv  ADDRESS_LO,     0xf000
        .equiv  EEPROM_SIZE,    0x1000
      
;------ Memory opType.
        
        .equiv  ERASE_WORD, 0x4044
        .equiv  WRITE_WORD, 0x4004

;------ Write a word to eeprom

_WriteWord:
        push    w4
        mov     #ADDRESS_HI,w4
        mov     w4,TBLPAG
        mov     w4,NVMADRU
        mov     #ADDRESS_LO,w4
        sl      w0,#1,w0
        add     w0,w4,w0
        mov     w0,NVMADR
        rcall   EraseWord
        mov     #WRITE_WORD,w4
        mov     w4,NVMCON
        TBLWTL  w1,[w0]
        rcall   KeySequence
        pop     w4
        return

;------ Read a word from eeprom

_ReadWord:
        push    w4
        mov     #ADDRESS_HI,w4
        mov     w4,TBLPAG
        mov     w4,NVMADRU
        mov     #ADDRESS_LO,w4
        mov     w4,NVMADR
        sl      w0,#1,w0
        add     w0,w4,w4
        tblrdl  [w4],w0
        pop     w4
        return

;------ Erase a word from eeprom 

EraseWord:
        mov     #ERASE_WORD,w4
        mov     w4,NVMCON

;------ Required key sequence for write or erase.

KeySequence:
        disi    #16
        mov     #0x55,w4
        mov     w4,NVMKEY
        mov     #0xaa,w4
        mov     w4,NVMKEY
        bset    NVMCON,#WR
        nop
        nop
K1wait: btsc    NVMCON,#WR 
        bra     K1wait
        return

        .end
        
;------ End of file.        
