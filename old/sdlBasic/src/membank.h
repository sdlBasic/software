/*
    Name:       membank.h
    Purpose:    memory bank support for wxBasic/sdlBasic
    Author:     Viola Roberto __vroby__
    Copyright:  (c) 2003 Viola Roberto <__vroby__@libero.it>
    Licence:    LGPL
*/
#define NUM_MEMBANKS 255

extern char *membank[NUM_MEMBANKS];
extern int banksize[NUM_MEMBANKS];
extern int curbank;


//--Internal ---------------------------------------------------
extern void init_membank();

//--Base ---------------------------------------------------
extern int reservebank(int bank,int size);
extern int baseimage(int bank,int n);
extern int baseimageCC(int bank,int n);
extern int basescreen(int bank,int n);
extern int basesound(int bank,int n);

extern int freebase(int bank);
extern int freebank(int bank);
extern int copybank(int s,int d);
extern int loadbank(char *filename,int bank);
extern int savebank(char *filename,int bank);

extern int setbank(int bank);
extern int sizebank(int bank);

//--poke ---------------------------------------------------
extern int poke(int bank,int address,unsigned char byte);
extern int doke(int bank,int address,unsigned short byte);
extern int loke(int bank,int address,unsigned int byte);

//--peek ---------------------------------------------------
extern int peek(int bank,int address);
extern int deek(int bank,int address);
extern int leek(int bank,int address);

//--fastcopy ---------------------------------------------------
extern int memcopy(int sbank,int s ,int dbank,int d,int size);
