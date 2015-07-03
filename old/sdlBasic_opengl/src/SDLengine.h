/*
_________________________________________________________________________________________________________________________

    SDLengine core of sdlBasic

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    __vroby__
    __vroby__@libero.it

_________________________________________________________________________________________________________________________
*/
/* ---- Defines */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include<string.h>

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"


#ifndef MIN
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MID(x,y,z)   MAX((x), MIN((y), (z)))
#endif

#define PRINT_SIZE	12
#define XPRINT_SIZE	(PRINT_SIZE*3/4)

#define NUM_SCREEN	8
#define NUM_IMAGES	4096

#define NUM_SPRITES 	512
#define NUM_BOBS 	512

#define NUM_WAVES	512


#ifndef M_PI
#define M_PI	3.141592654
#endif

#define SMOOTHING_OFF		0
#define SMOOTHING_ON		1
//_________________________________________________________________________________________________________________________

extern SDL_Surface *SDLdisplay;


extern int SDLautoback;
extern int SDLautobacktime;
extern int vbl;

extern int SDLupdatesprite;
extern int SDLupdatebob;
extern int SDLdualplayfield;

extern int SDLfps;
extern int SDLfps_count;
extern int SDLfps_flag;
extern int SDLfps_time;

extern int c_screen;
extern SDL_Surface *SDLscreen[NUM_SCREEN];
extern int Palette[256];

extern SDL_Rect SDLscreen_rect[NUM_SCREEN];
extern int SDLscreen_flag[NUM_SCREEN];
extern int SDLscreen_live[NUM_SCREEN];
extern int SDLoffx[NUM_SCREEN];
extern int SDLoffy[NUM_SCREEN];
extern int lastscreen;


extern SDL_Color SDLcol;

extern int SDL_hsx[NUM_IMAGES];
extern int SDL_hsy[NUM_IMAGES];

extern SDL_Surface *SDLimage[NUM_IMAGES];
extern SDL_Surface *SDLimageCC[NUM_IMAGES];

extern int SDLspritex[NUM_SPRITES];
extern int SDLspritey[NUM_SPRITES];
extern int SDLspritefr[NUM_SPRITES];
extern int SDLspritelive[NUM_SPRITES];
extern SDL_Rect SDLspriteclip;
extern int lastsprite;

extern int SDLbobscreen[NUM_BOBS];
extern int SDLbobx[NUM_BOBS];
extern int SDLboby[NUM_BOBS];
extern int SDLbobxold[NUM_BOBS];
extern int SDLbobyold[NUM_BOBS];
extern int SDLbobfr[NUM_BOBS];
extern int SDLboblive[NUM_BOBS];
extern SDL_Surface *SDLbobback[NUM_BOBS];
extern int lastbob;

extern Mix_Chunk *SDLsound[NUM_WAVES];
extern Mix_Music *SDLmusic;

extern TTF_Font *font;
extern char *fontPath;
extern SDL_Surface *SDLtext;

extern TTF_Font *font_mono;
extern char *fontPathMono;
extern SDL_Surface *SDLfont_mono[255];
extern int stx[NUM_SCREEN];
extern int sty[NUM_SCREEN];
extern int pen_color;
extern int paper_color;

extern int oldstx[NUM_SCREEN];
extern int oldsty[NUM_SCREEN];
extern SDL_Surface *SDLcursore[16];
extern int cursore_state[NUM_SCREEN];
extern int cursore_anim[NUM_SCREEN];


extern int lastkey;
//_________________________________________________________________________________________________________________________

/* ---- Structures */

  typedef struct tColorRGBA
  {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
  }
  tColorRGBA;

  typedef struct tColorY
  {
    Uint8 y;
  }
  tColorY;

//_________________________________________________________________________________________________________________________


#ifdef __cplusplus
extern "C" {
#endif

//_________________________________________________________________________________________________________________________

//RUTINE INTERNE

int getevent();
SDL_TimerCallback autoback_callback();
void autoback_timer();
SDL_Surface *Load_Image(char *datafile, int transparent);
int Save_Image(char *datafile, SDL_Surface *image);
void Image_colorkey(SDL_Surface *image);
int zoomSurfaceRGBA (SDL_Surface * src, SDL_Surface * dst, int smooth);
int zoomSurfaceY (SDL_Surface * src, SDL_Surface * dst);
void transformSurfaceRGBA (SDL_Surface * src, SDL_Surface * dst, int cx, int cy,int isin, int icos, int smooth);
void transformSurfaceY (SDL_Surface * src, SDL_Surface * dst, int cx, int cy,int isin, int icos);
SDL_Surface * rotozoomSurface (SDL_Surface * src, double angle, double zoom, int smooth);
SDL_Surface *zoomSurface (SDL_Surface * src, double zoomx, double zoomy, int smooth);
void fade (SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img, Uint32 ui_seconds, int b_fade_out);
void crossfade (SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img1, SDL_Surface* p_surf_img2, Uint32 ui_seconds);
int hit_pixelperfect(SDL_Surface *imga,int xa1,int ya1,int xa2,int ya2,SDL_Surface *imgb,int xb1,int yb1,int xb2,int yb2);

Uint32(*getpixel)(SDL_Surface *surface, int x, int y);
Uint32 getpixel8(SDL_Surface *surface, int x, int y);
Uint32 getpixel16(SDL_Surface *surface, int x, int y);
Uint32 getpixel24BE(SDL_Surface *surface, int x, int y);
Uint32 getpixel24(SDL_Surface *surface, int x, int y);
Uint32 getpixel32(SDL_Surface *surface, int x, int y);

void (*putpixel)(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixel8(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixel16(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixel24BE(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixel24(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixel32(SDL_Surface *surface, int x, int y, Uint32 col );
void putpixelDX(SDL_Surface *surface, int x, int y, Uint32 col );

void draw_line(SDL_Surface * surface,int x1, int y1, int x2, int y2,Uint32 color);
void draw_box(SDL_Surface *surface,int x1,int y1,int x2,int y2,Uint32 color);
void draw_circle(SDL_Surface *surface,int vx,int vy, int r,Uint32 color);
void draw_fillcircle(SDL_Surface *surface,int vx,int vy, int r,Uint32 color);
void draw_ellipse(SDL_Surface *surface,int vx,int vy,int Xradius,int Yradius,Uint32 color);
void draw_fillellipse(SDL_Surface *surface,int vx,int vy,int Xradius,int Yradius,Uint32 color);

Uint32 (*xrgb)(int colR,int colG,int colB);
Uint32 xrgb8(int colR,int colG,int colB);
Uint32 xrgb16(int colR,int colG,int colB);
Uint32 xrgb24(int colR,int colG,int colB);
Uint32 xrgb32(int colR,int colG,int colB);

Uint32 (*rrgb)(int col);
Uint32 rrgb8(int col);
Uint32 rrgb16(int col);
Uint32 rrgb24(int col);
Uint32 rrgb32(int col);

int createfont();
int put_char(unsigned char n);
int creacursore();
void _input(char instring[255],int x, int y, int displen);
//_________________________________________________________________________________________________________________________

//SISTEMA

//initialize								: avvia l'engine e i parametri di default
void initialize();

//terminate								: termina sdl libera la memoria e resettta i parametri
void terminate();

//_________________________________________________________________________________________________________________________

//SCREEN

//setdisplay  w,h,0=screen|1=window |2=resizable				: apre lo schermo/finestra in double buffer
int setdisplay(int w,int h,int bpp, int mode);

//display.width								: restituisce la larghezza del display
int displaywidth();

//display.height							: restituisce l'altezza del display
int displayheight();

//display.bpp								: restituisce la profondita di colore del display
int displaybpp();

//screen n								: attiva lo schgermo logico n
int screen (int n);

// directscreen 							: apre direttamente il display come screen
int directscreen ();

//screenopen n,w,h,dx,dy,dw,dh,flag						: apre lo schermo logico n di dimensione w,h nelle coordinate dx,dy,dw,dh, del display
int screenopen (int n,int w,int h,int dx,int dy,int dw,int dh,int flag);

//screenclose n								: chiude lo schermo logico n
int screenclose (int n);

//screenclone n,s,x,y,w,h,flag						: crea una nuova entrata nello schermo n
int screenclone(int n,int s,int x,int y,int w,int h,int flag);


//screencopy(n,x,y,w,h,nd,xd,yd)          						: copia una porzione di schermo da n a nd
int screencopy(int n,int x,int y,int w,int h,int nd,int xd,int yd);

//screenfade(n,t)								: sfuma lo screen n in t time
int screenfade(int n,int t);

//screenfadein(n,i,t)							: sfuma lo screen n verso l'immagine i in t time
int screenfadein(int n,int i,int t);

//screencrossfade(n,i,t)							: sfuma lo screen n verso dall'imagine correntel'immagine i in t time
int screencrossfade(int n,int i,int t);

//screenalpha(n,a)								: setta il parametro alpha(trasparenza) allo screen n
int screenalpha(int n,int a);

//screenlock(n)									: blocca il display per l'accesso diretto dai comandi grafici
int screenlock(int n);

//screenunlock(n)									: sblocca il display per l'accesso diretto dai comandi grafici
int screenunlock(int n);

//screenrect x,y,w,h,flag							: modifica le coordinate di output sul display dello screen corrente
int screenrect (int x,int y,int w,int h,int flag);

//xscreenrect								: restituisce la coordinata x del rettangolo di output dello screen
int xscreenrect();

//yscreenrect								: restituisce la coordinata y del rettangolo di output dello screen
int yscreenrect();

//wscreenrect								: restituisce la coordinata w del rettangolo di output dello screen
int wscreenrect();

//hscreenrect								: restituisce la coordinata h del rettangolo di output dello screen
int hscreenrect();

//flagscreenrect								: restituisce lo stato del flag del rettangolo di output dello screen
int flagscreenrect();

//screen.width								: restituisce la larghezza dello screen
int screenwidth();

//screen.height								: restituisce l'altezza dello screen
int screenheight();

// offset x,y								: posiziona la posiziona di visione del display sullo schermo logico
int offset(int x,int y);

// xoffset									: restituisce la coordinata x dell' offset
int xoffset();

// yoffset									: restituisce la coordinata y dell' offset
int yoffset();

//cls 									:  cancella lo schermo logico
int cls();

//screenswap								:  scambia lo schermo logico con quello fisico
int screenswap();

//autoback m								: attiva / disattiva lo screenswap automatico
int autoback(int m);

//dualplayfield m 							: attiva  / disattiva l'aggiornamento degli screen sopra al piano dei bob
int dualplayfield(int m);

//waitvbl								: attende lo screenswap automatico
int waitvbl();

//fps(n)								: attiva/disattiva/restituisce il numero di frame per secondo attuali
int fps(int n);
//_________________________________________________________________________________________________________________________

//GEOMETRIA

//rgb  r,g,b								: restituisce il colore corrente in formato Uint32
int rgb(int colR,int colG,int colB);

//color(c,v)								: setta il colore c (0-255) nella paletta conil valore v
int color(int c,int v);

//ink c									: seleziona il colore corrente dal formato Uint32
int ink(int c);

//point(x,y)								: restituisce la componente  colore alla coordinata x,y
int point(int x,int y);

//dot(x,y)									: scrive un punto con il colore ink alla coordinata x,y
int dot(int x,int y);

//dot_ex(x,y,c)								: scrive un punto con il colore c alla coordinata x,y
int dot_ex(int x,int y,int c);

//line x,y,x1,y1								: traccia una linea
int line(int x,int y,int x1,int y1);

//box x,y,x1,y1								: traccia un rettangolo vuoto
int box(int x,int y,int x1,int y1);

//bar x,y,x1,y1								: traccia un rettangolo pieno
int bar(int x,int y,int x1,int y1);

//circle x,y,r								: traccia un cerchio
int circle(int x,int y,int r);

//fillcircle x,y,r								: traccia un cerchio pieno
int fillcircle(int x,int y,int r);

//ellipse x,y,rx,ry								: traccia un ellisse
int ellipse(int x,int y,int rx,int ry);

//fillellipse x,y,rx,ry							: traccia un ellisse piena
int fillellipse(int x,int y,int rx,int ry);

//paint x,y								: riempie un area chiusa
int paint(int x,int y);
//_________________________________________________________________________________________________________________________

// FILE INPUT OUTPUT

//loadimages n,filename							: carica in un banco un file grafico  non trasparente
int loadimage(char *filename,int n);

//saveimage filename,n							: salva il banco n nel file grafico (deve essere bmp)
int saveimage(char *filename,int n);

//loadsound n,filename							: carica in un banco un file wav
int loadsound(char *filename,int n);

//soundexists( n)								: restituisce 1 se il banco sonoro n esiste 0 se vuoto
int soundexists(int n);

//loadmusic n,filename							: carica in un banco un file xm ,mod o it
int loadmusic(char *filename);

//musicexist()								: restituisce 1 se la musica e' caricata 0 se vuoto
int musicexists();
//_________________________________________________________________________________________________________________________

// IMAGE MANIPULATION

//hotspot(n,x,y)								: stabilisce il punto delle coordinate (n,0,0=alto sx | n,1,1=centro | n,2,2 =basso dx)n= image
int hotspot(int n,int x,int y);

//imageexists( n)								: restituisce 1 se il banco n esiste 0 se vuoto
int imageexists(int n);

//imagewidth(n)								: restituisce la larghezza dell'immagine n
int imagewidth(int n);

//imageheight(n)								: restituisce l'altezza dell'immagine n
int imageheight(int n);

//deleteimage(n)								: cancella l'immagine n dalla memoria
int deleteimage(int n);

//copyimage(s,d)								: copia l'immagine s nel banco d
int copyimage(int s,int d);

// setalpha(n,a)								:set trasparency
int setalpha(int n,int a);

//zoomimage(n,zoom)							: zoom image
int zoomimage(int n,double zoomx,double zoomy);

//rotateimage(n,angle)							: rotate image angle (0-360)
int rotateimage(int n,double angle);

//rotozoomimage(n,angle,zoom)						: rotate and zoom image (0-360)
int rotozoomimage(int n,double angle,double zoom);
//_________________________________________________________________________________________________________________________


// BLITTING

//blt n,sx,sy,sw,sh,dx,dy							: copia un banco nello schermo nascosto
int blt(int n,int sx,int sy,int sw,int sh,int dx,int dy);

//pastebob x,y,n								: copia sullo schermo il banco n alle coordinate x,y ritagliando
int pastebob(int x,int y,int n);

//pasteicon x,y,n								: copia sullo schermo il banco n alle coordinate x,y
int pasteicon(int x,int y,int n);

//grab n,x,y,w,h								: ritaglia dallo schermo e copia nel banco
int grab(int n,int x,int y,int w,int h);
//_________________________________________________________________________________________________________________________

//SPRITE SYSTEM

//spriteclip x,y,w,h								:setta il clipping dell'area sprite
int spriteclip(int x,int y,int w,int h);

// sprite n,x,y,fr								:attiva o sposta il sprite n alla coordinata x,y con l'immagine fr
int sprite(int n,int x,int y,int fr);

// delete sprite n								: disattiva il sprite n
int deletesprite(int n);

//xsprite									: restituisce la coordinata x del sprite n
int xsprite(int n);

//ysprite									: restituisce la coordinata y del sprite n
int ysprite(int n);

//sprite.width								: restituisce la larghezza del sprite n
int spritewidth(int n);

//sprite.height								:  restituisce l'altezza del sprite n
int spriteheight(int n);

//frsprite									:  restituisce il frame del sprite n
int frsprite(int n);

//livesprite								:  restituisce 1 se il sprite n e' "vivo"
int livesprite(int n);

//spritehit								:  restituisce 1 se il sprite n collide con il sprite x se 0 con tutti
int spritehit(int n,int x);

//autoupdatesprite m 							: attiva  / disattiva l'aggiornamento automatico dei sprite allo screenswap
int autoupdatesprite(int m);

//updatesprite								: attiva  un aggiornamento manuale degli sprite allo screenswap
int updatesprite();

//_________________________________________________________________________________________________________________________

//BOB SYSTEM

// setbob n,scr								:assegna il bob n allo screen scr
int setbob(int n,int scr);

// bob n,x,y,fr								:attiva o sposta il bob n alla coordinata x,y con l'immagine fr
int bob(int n,int x,int y,int fr);

// deletebob n								: disattiva il bob n
int deletebob(int n);

//xbob									: restituisce la coordinata x del bob n
int xbob(int n);

//ybob									: restituisce la coordinata y del bob n
int ybob(int n);

//bob.width								: restituisce la larghezza del bob n
int bobwidth(int n);

//bob.height								:  restituisce l'altezza del bob n
int bobheight(int n);

//frbob									:  restituisce il frame del bob n
int frbob(int n);

//livebob									:  restituisce 1 se il bob n e' "vivo"
int livebob(int n);

//bobhit									:  restituisce 1 se il bob n collide con il bob x se 0 con tutti
int bobhit(int n,int x);

//autoupdatebob m 								: attiva  / disattiva l'aggiornamento automatico dei bob allo screenswap
int autoupdatebob(int m);

//updatebob  								: attiva   un aggiornamento manuale dei bob allo screenswap
int updatebob();
//_________________________________________________________________________________________________________________________

// TEXT DRAW

//text x,y,s,testo,colR,colG,colB						: stampa il testo sul video con s size e col colore(RGB)
int text (int x, int y, int s, char *testo);

//setfont path								:seleziona il font d'uso
int setfont(char *path);

//textrender testo,s,n  						                  :crea il banco grafico n contenente il "testo" scritto con il font corrente con size s
int textrender (char *testo,int s,int n);
//_________________________________________________________________________________________________________________________

// PRINT SCREEN

//pen(c)									: setta il colore di print
int pen(int c);
//paper(c)								: settail colore di sfondo del carattere
int paper(int c);
//fprints(testo)								: stampa un testo monospace nella griglia senza andare a capo
int fprints(char *testo);
//prints(testo)								: stampa un testo monospace nella griglia
int prints(char *testo);
//locate(x,y)								: muove il cursore di stampa alle coordinate x y
int locate(int x,int y);
//atx									: restituisce la coordinata x del cursore
int atx();
//aty									: restituisce la coordinata y del cursore
int aty();
//curson									: visualizza il cursore lampeggiante sul video alle coordinate (atx,aty)
int curson();
//cursoff									: spegne il cursore lampeggiante sul video
int cursoff();
//inputs(request,default)							: raccoglie l'input da tastiera fino alla pressione di invio e lo restituisce
int inputs(char *request,char defs[255]);
//zoneinputs(x,y,l,default)							: raccoglie l'input da tastiera fino alla pressione nell'area specificata da x,y,l
int zoneinputs(int x,int y,int l,char defs[255]);

//_________________________________________________________________________________________________________________________

//SOUND

//playsound n,c,l								: suona il suono n nel canale c l volte
int playsound(int n,int c,int l);

//playfreqsound n,c,rate,l							: suona il suono n nel canale c a frequenza rate,l volte
int playfreqsound(int n,int c,int rate,int l);

//volumesound c,v							: cambia il volume del canale c (-1 tutti) al valore v (0-128)
int volumesound(int c,int v);

//stopsound(c)								: ferma l'emissione del wav dal canale c
int stopsound(int c);

//pausesound(c)								: mette in pausa il canale c
int pausesound(int c);

//resumesound(c)							: sblocca dalla pausa il canale c
int resumesound(int c);

//vumetersound(c)                                                       : restituisce lo stato corrente del canale
int vumetersound(int c);

//positionsound(c,angle,dist)                                           : posiziona il suono
int positionsound(int c,int angle,int dist);

// soundChannels(n)					                :definisce dinamicamente il numero di canali
int soundchannels(int n);

//playmusic  n								: suona il track xm,mod
int playmusic ( int n);

//positionmusic(p)                                                      : sposta l'esecuzione al secondo indicato
int positionmusic(int p);

//stopmusic 								: ferma il track xm,mod
int stopmusic();

//pausemusic                                                            : mette in pausa la musica in esecuzione
int pausemusic();

//resumemusic                                                           : riavvia la musica in pausa
int resumemusic();

//rewindmusic                                                          : riavvolge la musica corrente
int rewindmusic();

//fademusic t								: esegue la dissolvenza progressiva della musica
int fademusic(int t);

//volumemusic v								: cambia il volume della musica al valore v (0-128)
int volumemusic(int v);

//speedmusic(v)                                                         : cambia la velocita di esecuzione della musica
int speedmusic(int v);

//_________________________________________________________________________________________________________________________

// KEYBOARD

//key keycode								: restitusce 1 se e' premuto il tasto keycode
int key (int keycode);

//inkey									: restituisce quale tasto e' premuto
int inkey();

//waitkey(keycode)								: attende la pressione di un tasto(0 qualsiasi)
int waitkey(int keycode);

//_________________________________________________________________________________________________________________________

//MOUSE

//xmouse									: restituisce la coordinata x del mouse del display
int xmouse();

//ymouse									: restituisce la coordinata y del mouse del display
int ymouse();

//xmousescreen(n)								: restituisce la coordinata x del mouse dello screen n
int xmousescreen(int n);

//ymousescreen(n)								: restituisce la coordinata y del mouse dello screen n
int ymousescreen(int n);


//bmouse 								: restituisce il pulsante premuto del mouse
int bmouse();

//mouseshow								: visualizza il cursore del mouse
int mouseshow();

//mousehide								: nasconde il cursore del mouse
int mousehide();

//mousezone(x,y,w,h)							: restituisce 1 se il mouse occupa questa parte dello screen
int mousezone(int x,int y,int w,int h);

//_________________________________________________________________________________________________________________________

//JOYSTIK

//xjoy									:non testata
int xjoy();

//yjoy									:non testata
int yjoy();

//bjoy									:non implementata
int bjoy();
//_________________________________________________________________________________________________________________________

//wait t									:attende il trascorrere di t millisecondi
int wait(int t);

//timer									:restituisce il ticks corrente
int timer();

//_________________________________________________________________________________________________________________________

#ifdef __cplusplus
}
#endif

