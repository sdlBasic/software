/*
_________________________________________________________________________________________________________________________

    SDLengine core of sdlBasic

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    __vroby__
    __vroby__@libero.it

_________________________________________________________________________________________________________________________
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_net.h"
#include "unzip.h"

#include "SDLengine.h"



//error print
#define SDLerr fprintf

//_________________________________________________________________________________________________________________________

//DECLARATIONS

//- SCREEN --------------------------------------------------------------------------------------------------------------

SDL_Surface *SDLdisplay;

//int GL_display;


int SDLautoback;
int SDLautobacktime;
int vbl;

int SDLupdatesprite;
int SDLupdatebob;
int SDLdualplayfield;

int SDLfps;
int SDLfps_count;
int SDLfps_flag;
int SDLfps_time;

int c_screen;
SDL_Surface *SDLscreen[NUM_SCREENS];
int Palette[256];
int enablePalette;
int colorcycling_s;
int colorcycling_e;
int colorcycling_d;
int colorcycling_delay;
int colorcycling_next;

SDL_Rect SDLscreen_rect[NUM_SCREENS];
int SDLscreen_flag[NUM_SCREENS];
int SDLscreen_live[NUM_SCREENS];
int SDLoffx[NUM_SCREENS];
int SDLoffy[NUM_SCREENS];
int lastscreen;

SDL_Color SDLcol;

//FADE PARAM
int fadeflag[NUM_SCREENS];
SDL_Surface* p_surf_black[NUM_SCREENS];
SDL_Surface* p_surf_screen_copy[NUM_SCREENS];
Uint32 ui_old_time[NUM_SCREENS], ui_curr_time[NUM_SCREENS], ui_time_ms[NUM_SCREENS] ;
int fadedelay[NUM_SCREENS];
double f_alpha[NUM_SCREENS] ;

Uint32 ui_flags[NUM_SCREENS];
SDL_Surface* p_surf_screen[NUM_SCREENS];
SDL_Surface* p_surf_img[NUM_SCREENS];
Uint32 ui_seconds[NUM_SCREENS];
int b_fade_out[NUM_SCREENS];

int crossfadeflag[NUM_SCREENS];
SDL_Surface* p_surf_img1[NUM_SCREENS];
SDL_Surface* p_surf_img2[NUM_SCREENS];





//------------------------------------------------------------------------------------------------------------------------

//- IMAGES --------------------------------------------------------------------------------------------------------------

int SDL_hsx[NUM_IMAGES];
int SDL_hsy[NUM_IMAGES];

SDL_Surface *SDLimage[NUM_IMAGES];
SDL_Surface *SDLimageCC[NUM_IMAGES];
Uint32 colorkey;

//------------------------------------------------------------------------------------------------------------------------

//- SPRITES --------------------------------------------------------------------------------------------------------------

int SDLspritex[NUM_SPRITES];
int SDLspritey[NUM_SPRITES];
int SDLspritefr[NUM_SPRITES];
int SDLspritelive[NUM_SPRITES];

SDL_Rect SDLspriteclip;
int lastsprite;

//------------------------------------------------------------------------------------------------------------------------

//- BOBS -----------------------------------------------------------------------------------------------------------------

int SDLbobscreen[NUM_BOBS];
int SDLbobx[NUM_BOBS];
int SDLboby[NUM_BOBS];
int SDLbobxold[NUM_BOBS];
int SDLbobyold[NUM_BOBS];
int SDLbobfr[NUM_BOBS];
int SDLboblive[NUM_BOBS];
SDL_Surface *SDLbobback[NUM_BOBS];
int lastbob;

//------------------------------------------------------------------------------------------------------------------------

//- SOUND AND MUSIC ------------------------------------------------------------------------------------------------------

int enabledsound;
Mix_Chunk *SDLsound[NUM_WAVES];
Mix_Music *SDLmusic;

//------------------------------------------------------------------------------------------------------------------------

//- CD SUPPORT -----------------------------------------------------------------------------------------------------------

SDL_CD *SDLcd[NUM_CD];

//------------------------------------------------------------------------------------------------------------------------

//- VIDEO MPEG ---------------------------------------------------------------------------------------------------------------------

SMPEG *mpeg;
SMPEG_Info info;
int mpeg_audio;

//------------------------------------------------------------------------------------------------------------------------

//- TEXT AND PRINT -------------------------------------------------------------------------------------------------------

TTF_Font *font;
char *fontPath;
SDL_Surface *SDLtext;

TTF_Font *font_mono;
char *fontPathMono;

SDL_Surface *SDLfont_mono[0xff];
int stx[NUM_SCREENS];
int sty[NUM_SCREENS];
int pen_color;
int paper_color;

int oldstx[NUM_SCREENS];
int oldsty[NUM_SCREENS];
SDL_Surface *SDLcursore[16];

int cursore_state[NUM_SCREENS];
SDL_Surface *SDLcursore_preserve[NUM_SCREENS];
int cursore_anim[NUM_SCREENS];

//------------------------------------------------------------------------------------------------------------------------

//- KEYBOARD --------------------------------------------------------------------------------------------------------------

int lastkey;

//------------------------------------------------------------------------------------------------------------------------

//- MOUSE --------------------------------------------------------------------------------------------------------------

int mousepointer;
int mousestate;

//------------------------------------------------------------------------------------------------------------------------

//- JOYSTICK ------------------------------------------------------------------------------------------------------

SDL_Joystick *SDLjoy;

//------------------------------------------------------------------------------------------------------------------------

//- SOCKS ----------------------------------------------------------------------------------------------------------------

int enabledsock;
TCPsocket SDLsock[NUM_SOCKS];

//------------------------------------------------------------------------------------------------------------------------

/*----- Error system ---*/
char *error_description;
int error_type;


//________________________________________________________________________________________________________________________________________

// COMMON INTERNAL RUTINES
//________________________________________________________________________________________________________________________________________


//________________________________________________________________________________________________________________________________________

//stopkey					:terminate program if press ctrl+c
int stopkey()
{
    Uint8 *keystate;
    getevent();
    keystate = SDL_GetKeyState(NULL);

    if (keystate[306] && keystate[99]){
		error_description="ctrl+c pressed by the user\n";
		error_type=2;
		SDLerr(stderr,error_description);
	return -1;
    }
    return 0;
}
//________________________________________________________________________________________________________________________________________

//getevent					: eventhandling
int getevent()
{
	SDL_Event event;

	while ( SDL_PollEvent(&event) )
		{
		switch(event.type){
		  case SDL_QUIT:
			terminate();
			exit(2);
			return 0;
			break;

		  case SDL_KEYDOWN:

			lastkey = event.key.keysym.unicode;
			if (lastkey==0){
				switch(event.key.keysym.sym){

				case 127:
					lastkey=127;
				case 256:
					lastkey=48;
					break;
				case 257:
					lastkey=49;
					break;
				case 258:
					lastkey=50;
					break;
				case 259:
					lastkey=51;
					break;
				case 260:
					lastkey=52;
					break;
				case 261:
					lastkey=53;
					break;
				case 262:
					lastkey=54;
					break;
				case 263:
					lastkey=55;
					break;
				case 264:
					lastkey=56;
					break;
				case 265:
					lastkey=57;
					break;
				case 266:
					lastkey=46;
					break;
				case 267:
					lastkey=47;
					break;
				case 271:
					lastkey=13;
					break;
				default:
					lastkey=0;
					break;
				}
			}
			break;

		  case SDL_KEYUP :
			lastkey=0;
			break;
		}
	}
	SDL_PumpEvents();
	return 0;
}
//________________________________________________________________________________________________________________________________________

// autotimer 				: collaborative automatic multitasking
int autotimer()
{
    int i;
	/* autofade handler */
	for(i=0;i<NUM_SCREENS;i++){
	    if (fadedelay[i]!=0){
		if (SDL_GetTicks()>fadedelay[i]){
		    if (fadeflag[i]>0)
			fadehandler(i);
		    if (crossfadeflag[i]>0)
			crossfadehandler(i);
		    fadedelay[i]=SDL_GetTicks()+10;
		}
	    }
	}

	/* musicfade handler */
	if (fademusic_t>0)
		if (timer()>fademusic_delay)
		    fademusichandler();

	/* colorcycling handler */

	if (colorcycling_delay>0)
		if (colorcycling_next<timer() ){
			colorcyclinghandler();
		}

	/* autoback handler */
	if (SDLautoback>0)
	    if (timer()>SDLautobacktime){
			if (screenswap()==-1)
				return -1;
		SDLautobacktime=timer()+SDLautoback;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

// SDL_EXTENSION
//________________________________________________________________________________________________________________________________________

// fileExist(filename)			: see if a file exists
int fileExist( char *name )
{
    FILE    *handle;
    /* try to open the file */
    handle = fopen( name, "rb" );
    if (handle == NULL) return 0;
    fclose( handle );
    return -1;
}
//________________________________________________________________________________________________________________________________________

// Load_Image(filename,trasparent)	: load a image using sdl_image library
SDL_Surface *Load_Image(char *datafile, int transparent)
{
    int bpp;
    Uint32 oldck;
	SDL_Surface *image, *surface;

	//image = IMG_Load(datafile);
	image=SDL_DisplayFormat(IMG_Load(datafile));
	if ( image == NULL ) {
		error_description="Can't load image \n";
		error_type=1;
		SDLerr(stderr, "Can't load image %s: %s\n",datafile, IMG_GetError());

		return(NULL);
	}
	if ( transparent ) {
		bpp = image->format->BytesPerPixel;

		oldck=colorkey;

		if (SDL_MUSTLOCK(image) )
		    SDL_LockSurface(image);

		switch(bpp) {
		    case 1:
			if (colorkey==-1)colorkey=*(Uint8 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint8) colorkey);//*(Uint8 *)image->pixels
			break;
		    case 2:
			if (colorkey==-1)colorkey=*(Uint16 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint16)colorkey);//*(Uint16 *)image->pixels
			break;
		    case 3:
			if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);//*(Uint32 *)image->pixels
			break;
		    case 4:
			if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);//*(Uint32 *)image->pixels
			break;
		    }
		    if (SDL_MUSTLOCK(image) )
			SDL_UnlockSurface(image);
		    colorkey=oldck;
	}

	surface = SDL_DisplayFormat(image);
	SDL_FreeSurface(image);
	return(surface);
}
//________________________________________________________________________________________________________________________________________

// Load_zip_Image(zipfile,filename,trasparent)	: load a image using sdl_image library
SDL_Surface *Load_zip_Image(char *zipfile,char *datafile, int transparent)
{
    unsigned char *mem;
    SDL_RWops *src;
    int size;
    int bpp;
    Uint32 oldck;
	SDL_Surface *image, *surface;

	if (load_zipped_file (zipfile, datafile, &mem, &size)==0){
	    src=SDL_RWFromMem(mem, size);
	    image=SDL_DisplayFormat(IMG_Load_RW(src, 0));
	    if ( image == NULL ) {
		error_description="Can't load image \n";
		error_type=1;
		SDLerr(stderr, "\n Load_zip_Image: Can't load image %s: %s \n",datafile, IMG_GetError());

		return(NULL);
	    }
	    if ( transparent ) {
		bpp = image->format->BytesPerPixel;

		oldck=colorkey;

		if (SDL_MUSTLOCK(image) )
		    SDL_LockSurface(image);

		switch(bpp) {
		    case 1:
			if (colorkey==-1)colorkey=*(Uint8 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint8) colorkey);//*(Uint8 *)image->pixels
			break;
		    case 2:
			if (colorkey==-1)colorkey=*(Uint16 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint16)colorkey);//*(Uint16 *)image->pixels
			break;
		    case 3:
			if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);//*(Uint32 *)image->pixels
			break;
		    case 4:
			if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);//*(Uint32 *)image->pixels
			break;
		    }
		    if (SDL_MUSTLOCK(image) )
			SDL_UnlockSurface(image);
		    colorkey=oldck;
	    }

	    surface = SDL_DisplayFormat(image);
	    SDL_FreeSurface(image);
	    return(surface);
	}
	else
	    return(NULL);
}
//________________________________________________________________________________________________________________________________________


//Save_Image(filename,image) 	: save the image using sdl rutines you can save only in bmp format
int Save_Image(char *datafile, SDL_Surface *image)
{
	if ( SDL_SaveBMP( image , datafile) ) {
		error_description="Can't save image \n";
		error_type=1;
		SDLerr(stderr, "Can't save image %s: %s\n",datafile, IMG_GetError());
		return -1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

// Image_colorkey(image)	: set trasparency on image
void Image_colorkey(SDL_Surface *image)
{
    int bpp;
    Uint32 oldck;


    if (SDL_MUSTLOCK(image))
	SDL_LockSurface(image);

    bpp = image->format->BytesPerPixel;
    oldck=colorkey;

    switch(bpp) {
	case 1:
	    if (colorkey==-1)colorkey=*(Uint8 *)image->pixels;
	    SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint8)colorkey);
	    break;
	case 2:
	    if (colorkey==-1)colorkey=*(Uint16 *)image->pixels;
	    SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint16)colorkey);
	    break;
	case 3:
	    if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
	    SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);
	    break;
	case 4:
	    if (colorkey==-1)colorkey=*(Uint32 *)image->pixels;
	    SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),(Uint32)colorkey);
	    break;
    }
    if (SDL_MUSTLOCK(image))
	SDL_UnlockSurface(image);

    colorkey=oldck;


}
//________________________________________________________________________________________________________________________________________

// this rutine was posted by Johannes Bauer in sdl-mailing list: tank'you Joe!!
// Function expects a 16-bit Stereo Sample (4 Byte/Sample) and a scale
// factor (>1 means longer sample, thus lower pitch, <1 means shorter
// sample, thus higher pitch)
Mix_Chunk* Change_Pitch(Mix_Chunk *Input_Sample, float Factor)
{
    Mix_Chunk *Sample_Modified;
    Uint32 Number_Input_Samples;
    Uint32 Number_Output_Samples;
    float Real_Factor;

    Uint32 Begin, End;
    Sint16 Left, Right;
    Sint64 AverageL, AverageR;
    Sint16 OutLeft, OutRight;

    Uint32 i;
    Uint32 j;
    /*
    if (Input_Sample->allocated!=1) {
	throw CGenericException(std::string("CSound::Change_Pitch"),std::string("Input sample not allocated."));
    }
    */

    Number_Input_Samples=Input_Sample->alen/4;
    Number_Output_Samples= (Uint32) ((float)Number_Input_Samples*Factor);
    Real_Factor = (float) Number_Output_Samples/ (float) Number_Input_Samples;

    Sample_Modified=(Mix_Chunk*)malloc(sizeof(Mix_Chunk));
    Sample_Modified->allocated=1;// Not yet, but will soon be
    Sample_Modified->alen=Number_Output_Samples*4;
    Sample_Modified->abuf=(Uint8*)malloc(Sample_Modified->alen*sizeof(Uint8));
    Sample_Modified->volume=Input_Sample->volume;

    for (i=0;i<Number_Output_Samples;i++) {
	Begin=(Uint32)((float)i/Real_Factor);
	End=(Uint32)((float)(i+1)/Real_Factor);
	AverageL=0;
	AverageR=0;

	// Destinataion i is the average of Begin-End of Input sample
	for (j=Begin;j<=End;j++) {
	    Left=(Input_Sample->abuf[(4*j)+1]<<8)|(Input_Sample->abuf[(4*j)+0]);
	    Right=(Input_Sample->abuf[(4*j)+3]<<8)|(Input_Sample->abuf[(4*j)+2]);

	    AverageL+=Left;
	    AverageR+=Right;
	}
	OutLeft=(Sint16)((float)AverageL/(float)(End-Begin+1));
	OutRight=(Sint16)((float)AverageR/(float)(End-Begin+1));

	Sample_Modified->abuf[(4*i)+1]=(OutLeft>>8)&0xff;
	Sample_Modified->abuf[(4*i)+0]=(OutLeft)&0xff;
	Sample_Modified->abuf[(4*i)+3]=(OutRight>>8)&0xff;
	Sample_Modified->abuf[(4*i)+2]=(OutRight)&0xff;
    }
    return Sample_Modified;
}
//________________________________________________________________________________________________________________________________________

/*
Rotozoom/Zoom function for SDL
========================================================================================
(C) LGPL, A. Schiffler, aschiffler@cogeco.com - see LICENSE for details.


 32bit Zoomer with optional anti-aliasing by bilinear interpolation.
 Zoomes 32bit RGBA/ABGR 'src' surface to 'dst' surface.
*/

int zoomSurfaceRGBA (SDL_Surface * src, SDL_Surface * dst, int smooth)
{
  int x, y, sx, sy, *sax, *say, *csax, *csay, csx, csy, ex, ey, t1, t2, sstep;
  tColorRGBA *c00, *c01, *c10, *c11;
  tColorRGBA *sp, *csp, *dp;
  int sgap, dgap, orderRGBA;

  /* Variable setup */
  if (smooth)
    {
      /* For interpolation: assume source dimension is one pixel */
      /* smaller to avoid overflow on right and bottom edge.     */
      sx = (int) (65536.0 * (float) (src->w - 1) / (float) dst->w);
      sy = (int) (65536.0 * (float) (src->h - 1) / (float) dst->h);
    }
  else
    {
      sx = (int) (65536.0 * (float) src->w / (float) dst->w);
      sy = (int) (65536.0 * (float) src->h / (float) dst->h);
    }

  /* Allocate memory for row increments */
  if ((sax = (int *) malloc ((dst->w + 1) * sizeof (Uint32))) == NULL)
    {
      return (-1);
    }
  if ((say = (int *) malloc ((dst->h + 1) * sizeof (Uint32))) == NULL)
    {
      free (sax);
      return (-1);
    }

  /* Precalculate row increments */
  csx = 0;
  csax = sax;
  for (x = 0; x <= dst->w; x++)
    {
      *csax = csx;
      csax++;
      csx &= 0xffff;
      csx += sx;
    }
  csy = 0;
  csay = say;
  for (y = 0; y <= dst->h; y++)
    {
      *csay = csy;
      csay++;
      csy &= 0xffff;
      csy += sy;
    }

  /* Pointer setup */
  sp = csp = (tColorRGBA *) src->pixels;
  dp = (tColorRGBA *) dst->pixels;
  sgap = src->pitch - src->w * 4;
  dgap = dst->pitch - dst->w * 4;
  orderRGBA = (src->format->Rmask == 0x000000ff);

  /* Switch between interpolating and non-interpolating code */
  if (smooth)
    {

      /* Interpolating Zoom */

      /* Scan destination */
      csay = say;
      for (y = 0; y < dst->h; y++)
	{
	  /* Setup color source pointers */
	  c00 = csp;
	  c01 = csp;
	  c01++;
	  c10 = (tColorRGBA *) ((Uint8 *) csp + src->pitch);
	  c11 = c10;
	  c11++;
	  csax = sax;
	  for (x = 0; x < dst->w; x++)
	    {
	      /* ABGR ordering */
	      /* Interpolate colors */
	      ex = (*csax & 0xffff);
	      ey = (*csay & 0xffff);
	      t1 = ((((c01->r - c00->r) * ex) >> 16) + c00->r) & 0xff;
	      t2 = ((((c11->r - c10->r) * ex) >> 16) + c10->r) & 0xff;
	      dp->r = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->g - c00->g) * ex) >> 16) + c00->g) & 0xff;
	      t2 = ((((c11->g - c10->g) * ex) >> 16) + c10->g) & 0xff;
	      dp->g = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->b - c00->b) * ex) >> 16) + c00->b) & 0xff;
	      t2 = ((((c11->b - c10->b) * ex) >> 16) + c10->b) & 0xff;
	      dp->b = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->a - c00->a) * ex) >> 16) + c00->a) & 0xff;
	      t2 = ((((c11->a - c10->a) * ex) >> 16) + c10->a) & 0xff;
	      dp->a = (((t2 - t1) * ey) >> 16) + t1;
	      /* Advance source pointers */
	      csax++;
	      sstep = (*csax >> 16);
	      c00 += sstep;
	      c01 += sstep;
	      c10 += sstep;
	      c11 += sstep;
	      /* Advance destination pointer */
	      dp++;
	    }
	  /* Advance source pointer */
	  csay++;
	  csp = (tColorRGBA *) ((Uint8 *) csp + (*csay >> 16) * src->pitch);
	  /* Advance destination pointers */
	  dp = (tColorRGBA *) ((Uint8 *) dp + dgap);
	}

    }
  else
    {

      /* Non-Interpolating Zoom */

      csay = say;
      for (y = 0; y < dst->h; y++)
	{
	  sp = csp;
	  csax = sax;
	  for (x = 0; x < dst->w; x++)
	    {
	      /* Draw */
	      *dp = *sp;
	      /* Advance source pointers */
	      csax++;
	      sp += (*csax >> 16);
	      /* Advance destination pointer */
	      dp++;
	    }
	  /* Advance source pointer */
	  csay++;
	  csp = (tColorRGBA *) ((Uint8 *) csp + (*csay >> 16) * src->pitch);
	  /* Advance destination pointers */
	  dp = (tColorRGBA *) ((Uint8 *) dp + dgap);
	}

    }

  /* Remove temp arrays */
  free (sax);
  free (say);

  return (0);
}
//________________________________________________________________________________________________________________________________________

/*
 8bit Zoomer without smoothing.
 Zoomes 8bit palette/Y 'src' surface to 'dst' surface.
*/

int zoomSurfaceY (SDL_Surface * src, SDL_Surface * dst)
{
  Uint32 x, y, sx, sy, *sax, *say, *csax, *csay, csx, csy;
  Uint8 *sp, *dp, *csp;
  int dgap;

  /* Variable setup */
  sx = (Uint32) (65536.0 * (float) src->w / (float) dst->w);
  sy = (Uint32) (65536.0 * (float) src->h / (float) dst->h);

  /* Allocate memory for row increments */
  if ((sax = (Uint32 *) malloc (dst->w * sizeof (Uint32))) == NULL)
    {
      return (-1);
    }
  if ((say = (Uint32 *) malloc (dst->h * sizeof (Uint32))) == NULL)
    {
      if (sax != NULL)
	{
	  free (sax);
	}
      return (-1);
    }

  /* Precalculate row increments */
  csx = 0;
  csax = sax;
  for (x = 0; x < dst->w; x++)
    {
      csx += sx;
      *csax = (csx >> 16);
      csx &= 0xffff;
      csax++;
    }
  csy = 0;
  csay = say;
  for (y = 0; y < dst->h; y++)
    {
      csy += sy;
      *csay = (csy >> 16);
      csy &= 0xffff;
      csay++;
    }

  csx = 0;
  csax = sax;
  for (x = 0; x < dst->w; x++)
    {
      csx += (*csax);
      csax++;
    }
  csy = 0;
  csay = say;
  for (y = 0; y < dst->h; y++)
    {
      csy += (*csay);
      csay++;
    }

  /* Pointer setup */
  sp = csp = (Uint8 *) src->pixels;
  dp = (Uint8 *) dst->pixels;
  dgap = dst->pitch - dst->w;

  /* Draw */
  csay = say;
  for (y = 0; y < dst->h; y++)
    {
      csax = sax;
      sp = csp;
      for (x = 0; x < dst->w; x++)
	{
	  /* Draw */
	  *dp = *sp;
	  /* Advance source pointers */
	  sp += (*csax);
	  csax++;
	  /* Advance destination pointer */
	  dp++;
	}
      /* Advance source pointer (for row) */
      csp += ((*csay) * src->pitch);
      csay++;
      /* Advance destination pointers */
      dp += dgap;
    }

  /* Remove temp arrays */
  free (sax);
  free (say);

  return (0);
}
//________________________________________________________________________________________________________________________________________

/*
 32bit Rotozoomer with optional anti-aliasing by bilinear interpolation.
 Rotates and zoomes 32bit RGBA/ABGR 'src' surface to 'dst' surface.
*/

void transformSurfaceRGBA (SDL_Surface * src, SDL_Surface * dst, int cx, int cy,int isin, int icos, int smooth)
{
  int x, y, t1, t2, dx, dy, xd, yd, sdx, sdy, ax, ay, ex, ey, sw, sh;
  tColorRGBA c00, c01, c10, c11;
  tColorRGBA *pc, *sp;
  int gap, orderRGBA;

  /* Variable setup */
  xd = ((src->w - dst->w) << 15);
  yd = ((src->h - dst->h) << 15);
  ax = (cx << 16) - (icos * cx);
  ay = (cy << 16) - (isin * cx);
  sw = src->w - 1;
  sh = src->h - 1;
  pc = dst->pixels;
  gap = dst->pitch - dst->w * 4;
  orderRGBA = (src->format->Rmask == 0x000000ff);

  /* Switch between interpolating and non-interpolating code */
  if (smooth)
    {
      for (y = 0; y < dst->h; y++)
	{
	  dy = cy - y;
	  sdx = (ax + (isin * dy)) + xd;
	  sdy = (ay - (icos * dy)) + yd;
	  for (x = 0; x < dst->w; x++)
	    {
	      dx = (sdx >> 16);
	      dy = (sdy >> 16);
	      if ((dx >= -1) && (dy >= -1) && (dx < src->w) && (dy < src->h))
		{
		  if ((dx >= 0) && (dy >= 0) && (dx < sw) && (dy < sh))
		    {
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      sp += dx;
		      c00 = *sp;
		      sp += 1;
		      c01 = *sp;
		      sp = (tColorRGBA *) ((Uint8 *) sp + src->pitch);
		      sp -= 1;
		      c10 = *sp;
		      sp += 1;
		      c11 = *sp;
		    }
		  else if ((dx == sw) && (dy == sh))
		    {
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      sp += dx;
		      c00 = *sp;
		      c01 = *pc;
		      c10 = *pc;
		      c11 = *pc;
		    }
		  else if ((dx == -1) && (dy == -1))
		    {
		      sp = (tColorRGBA *) (src->pixels);
		      c00 = *pc;
		      c01 = *pc;
		      c10 = *pc;
		      c11 = *sp;
		    }
		  else if ((dx == -1) && (dy == sh))
		    {
		      sp = (tColorRGBA *) (src->pixels);
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      c00 = *pc;
		      c01 = *sp;
		      c10 = *pc;
		      c11 = *pc;
		    }
		  else if ((dx == sw) && (dy == -1))
		    {
		      sp = (tColorRGBA *) (src->pixels);
		      sp += dx;
		      c00 = *pc;
		      c01 = *pc;
		      c10 = *sp;
		      c11 = *pc;
		    }
		  else if (dx == -1)
		    {
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      c00 = *pc;
		      c01 = *sp;
		      c10 = *pc;
		      sp = (tColorRGBA *) ((Uint8 *) sp + src->pitch);
		      c11 = *sp;
		    }
		  else if (dy == -1)
		    {
		      sp = (tColorRGBA *) (src->pixels);
		      sp += dx;
		      c00 = *pc;
		      c01 = *pc;
		      c10 = *sp;
		      sp += 1;
		      c11 = *sp;
		    }
		  else if (dx == sw)
		    {
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      sp += dx;
		      c00 = *sp;
		      c01 = *pc;
		      sp = (tColorRGBA *) ((Uint8 *) sp + src->pitch);
		      c10 = *sp;
		      c11 = *pc;
		    }
		  else if (dy == sh)
		    {
		      sp =
			(tColorRGBA *) ((Uint8 *) src->pixels +
					src->pitch * dy);
		      sp += dx;
		      c00 = *sp;
		      sp += 1;
		      c01 = *sp;
		      c10 = *pc;
		      c11 = *pc;
		    }
		  /* Interpolate colors */
		  ex = (sdx & 0xffff);
		  ey = (sdy & 0xffff);
		  t1 = ((((c01.r - c00.r) * ex) >> 16) + c00.r) & 0xff;
		  t2 = ((((c11.r - c10.r) * ex) >> 16) + c10.r) & 0xff;
		  pc->r = (((t2 - t1) * ey) >> 16) + t1;
		  t1 = ((((c01.g - c00.g) * ex) >> 16) + c00.g) & 0xff;
		  t2 = ((((c11.g - c10.g) * ex) >> 16) + c10.g) & 0xff;
		  pc->g = (((t2 - t1) * ey) >> 16) + t1;
		  t1 = ((((c01.b - c00.b) * ex) >> 16) + c00.b) & 0xff;
		  t2 = ((((c11.b - c10.b) * ex) >> 16) + c10.b) & 0xff;
		  pc->b = (((t2 - t1) * ey) >> 16) + t1;
		  t1 = ((((c01.a - c00.a) * ex) >> 16) + c00.a) & 0xff;
		  t2 = ((((c11.a - c10.a) * ex) >> 16) + c10.a) & 0xff;
		  pc->a = (((t2 - t1) * ey) >> 16) + t1;

		}
	      sdx += icos;
	      sdy += isin;
	      pc++;
	    }
	  pc = (tColorRGBA *) ((Uint8 *) pc + gap);
	}
    }
  else
    {
      for (y = 0; y < dst->h; y++)
	{
	  dy = cy - y;
	  sdx = (ax + (isin * dy)) + xd;
	  sdy = (ay - (icos * dy)) + yd;
	  for (x = 0; x < dst->w; x++)
	    {
	      dx = (short) (sdx >> 16);
	      dy = (short) (sdy >> 16);
	      if ((dx >= 0) && (dy >= 0) && (dx < src->w) && (dy < src->h))
		{
		  sp =
		    (tColorRGBA *) ((Uint8 *) src->pixels + src->pitch * dy);
		  sp += dx;
		  *pc = *sp;
		}
	      sdx += icos;
	      sdy += isin;
	      pc++;
	    }
	  pc = (tColorRGBA *) ((Uint8 *) pc + gap);
	}
    }
}


/*
 8bit Rotozoomer without smoothing
 Rotates and zoomes 8bit palette/Y 'src' surface to 'dst' surface.
*/

void transformSurfaceY (SDL_Surface * src, SDL_Surface * dst, int cx, int cy,int isin, int icos)
{
  int x, y, dx, dy, xd, yd, sdx, sdy, ax, ay, sw, sh;
  tColorY *pc, *sp;
  int gap;

  /* Variable setup */
  xd = ((src->w - dst->w) << 15);
  yd = ((src->h - dst->h) << 15);
  ax = (cx << 16) - (icos * cx);
  ay = (cy << 16) - (isin * cx);
  sw = src->w - 1;
  sh = src->h - 1;
  pc = dst->pixels;
  gap = dst->pitch - dst->w;
  /* Clear surface to colorkey */
  memset (pc, (unsigned char) (src->format->colorkey & 0xff),
	  dst->pitch * dst->h);
  /* Iterate through destination surface */
  for (y = 0; y < dst->h; y++)
    {
      dy = cy - y;
      sdx = (ax + (isin * dy)) + xd;
      sdy = (ay - (icos * dy)) + yd;
      for (x = 0; x < dst->w; x++)
	{
	  dx = (short) (sdx >> 16);
	  dy = (short) (sdy >> 16);
	  if ((dx >= 0) && (dy >= 0) && (dx < src->w) && (dy < src->h))
	    {
	      sp = (tColorY *) (src->pixels);
	      sp += (src->pitch * dy + dx);
	      *pc = *sp;
	    }
	  sdx += icos;
	  sdy += isin;
	  pc++;
	}
      pc += gap;
    }
}
//________________________________________________________________________________________________________________________________________

/*
 rotozoomSurface()
 Rotates and zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'angle' is the rotation in degrees. 'zoom' a scaling factor. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.

*/

#define VALUE_LIMIT	0.001

SDL_Surface * rotozoomSurface (SDL_Surface * src, double angle, double zoom, int smooth)
{
  SDL_Surface *rz_src;
  SDL_Surface *rz_dst;
  double zoominv;
  double radangle, sanglezoom, canglezoom, sanglezoominv, canglezoominv;
  int dstwidthhalf, dstwidth, dstheighthalf, dstheight;
  double x, y, cx, cy, sx, sy;
  int is32bit;
  int i, src_converted;

  /* Sanity check */
  if (src == NULL)
    return (NULL);

  /* Determine if source surface is 32bit or 8bit */
  is32bit = (src->format->BitsPerPixel == 32);
  if ((is32bit) || (src->format->BitsPerPixel == 8))
    {
      /* Use source surface 'as is' */
      rz_src = src;
      src_converted = 0;
    }
  else
    {
      /* New source surface is 32bit with a defined RGBA ordering */
      rz_src =
	SDL_CreateRGBSurface (SDL_SWSURFACE, src->w, src->h, 32, 0x000000ff,
			      0x0000ff00, 0x00ff0000, 0xff000000);
	    //patch for colorkey setted
	if (colorkey!=-1)
	    SDL_FillRect(rz_src,NULL,colorkey);
	else
	    SDL_FillRect(rz_src,NULL,*(Uint32*)src->pixels);


      SDL_BlitSurface (src, NULL, rz_src, NULL);
      src_converted = 1;
      is32bit = 1;
    }

  /* Sanity check zoom factor */
  if (zoom < VALUE_LIMIT)
    {
      zoom = VALUE_LIMIT;
    }
  zoominv = 65536.0 / zoom;

  /* Check if we have a rotozoom or just a zoom */
  if (fabs (angle) > VALUE_LIMIT)
    {

      /* Angle!=0: full rotozoom */
      /* ----------------------- */

      /* Calculate target factors from sin/cos and zoom */
      radangle = angle * (M_PI / 180.0);
      sanglezoom = sanglezoominv = sin (radangle);
      canglezoom = canglezoominv = cos (radangle);
      sanglezoom *= zoom;
      canglezoom *= zoom;
      sanglezoominv *= zoominv;
      canglezoominv *= zoominv;

      /* Determine destination width and height by rotating a centered source box */
      x = rz_src->w / 2;
      y = rz_src->h / 2;
      cx = canglezoom * x;
      cy = canglezoom * y;
      sx = sanglezoom * x;
      sy = sanglezoom * y;
      dstwidthhalf =
	MAX ((int)
	     ceil (MAX
		   (MAX
		    (MAX (fabs (cx + sy), fabs (cx - sy)), fabs (-cx + sy)),
		    fabs (-cx - sy))), 1);
      dstheighthalf =
	MAX ((int)
	     ceil (MAX
		   (MAX
		    (MAX (fabs (sx + cy), fabs (sx - cy)), fabs (-sx + cy)),
		    fabs (-sx - cy))), 1);
      dstwidth = 2 * dstwidthhalf;
      dstheight = 2 * dstheighthalf;

      /* Alloc space to completely contain the rotated surface */
      rz_dst = NULL;
      if (is32bit)
	{
	  /* Target surface is 32bit with source RGBA/ABGR ordering */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 32,
				  rz_src->format->Rmask,
				  rz_src->format->Gmask,
				  rz_src->format->Bmask,
				  rz_src->format->Amask);
	    //patch for colorkey setted
	    if (colorkey!=-1)
		SDL_FillRect(rz_dst,NULL,colorkey);
	    else
		SDL_FillRect(rz_dst,NULL,*(Uint32*)src->pixels);
	}
      else
	{
	  /* Target surface is 8bit */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0,
				  0, 0);
    	    //patch for colorkey setted
	    if (colorkey!=-1)
		SDL_FillRect(rz_dst,NULL,colorkey);
	    else
		SDL_FillRect(rz_dst,NULL,*(Uint8 *)src->pixels);
	}

      /* Lock source surface */
      SDL_LockSurface (rz_src);
      /* Check which kind of surface we have */
      if (is32bit)
	{
	  /* Call the 32bit transformation routine to do the rotation (using alpha) */
	  transformSurfaceRGBA (rz_src, rz_dst, dstwidthhalf, dstheighthalf,
				(int) (sanglezoominv),
				(int) (canglezoominv), smooth);
	  /* Turn on source-alpha support */
	  SDL_SetAlpha (rz_dst, SDL_SRCALPHA, 255);
	}
      else
	{
	  /* Copy palette and colorkey info */
	  for (i = 0; i < rz_src->format->palette->ncolors; i++)
	    {
	      rz_dst->format->palette->colors[i] =
		rz_src->format->palette->colors[i];
	    }
	  rz_dst->format->palette->ncolors = rz_src->format->palette->ncolors;
	  /* Call the 8bit transformation routine to do the rotation */
	  transformSurfaceY (rz_src, rz_dst, dstwidthhalf, dstheighthalf,
			     (int) (sanglezoominv), (int) (canglezoominv));
	  SDL_SetColorKey (rz_dst, SDL_SRCCOLORKEY| SDL_RLEACCEL ,rz_src->format->colorkey);
	}
      /* Unlock source surface */
      SDL_UnlockSurface (rz_src);

    }
  else
    {

      /* Angle=0: Just a zoom */
      /* -------------------- */

      /* Calculate target size and set rect */
      dstwidth = (int) ((double) rz_src->w * zoom);
      dstheight = (int) ((double) rz_src->h * zoom);
      if (dstwidth < 1)
	{
	  dstwidth = 1;
	}
      if (dstheight < 1)
	{
	  dstheight = 1;
	}

      /* Alloc space to completely contain the zoomed surface */
      rz_dst = NULL;
      if (is32bit)
	{
	  /* Target surface is 32bit with source RGBA/ABGR ordering */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 32,
				  rz_src->format->Rmask,
				  rz_src->format->Gmask,
				  rz_src->format->Bmask,
				  rz_src->format->Amask);
	    //patch for colorkey setted
	    if (colorkey!=-1)
		SDL_FillRect(rz_dst,NULL,colorkey);
	    else
		SDL_FillRect(rz_dst,NULL,*(Uint32 *)src->pixels);
	}
      else
	{
	  /* Target surface is 8bit */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0,
				  0, 0);
	    	    //patch for colorkey setted
	    if (colorkey!=-1)
		SDL_FillRect(rz_dst,NULL,colorkey);
	    else
		SDL_FillRect(rz_dst,NULL,*(Uint8 *)src->pixels);

	}

      /* Lock source surface */
      SDL_LockSurface (rz_src);
      /* Check which kind of surface we have */
      if (is32bit)
	{
	  /* Call the 32bit transformation routine to do the zooming (using alpha) */
	  zoomSurfaceRGBA (rz_src, rz_dst, smooth);
	  /* Turn on source-alpha support */
	  SDL_SetAlpha (rz_dst, SDL_SRCALPHA, 255);
	}
      else
	{
	  /* Copy palette and colorkey info */
	  for (i = 0; i < rz_src->format->palette->ncolors; i++)
	    {
	      rz_dst->format->palette->colors[i] =
		rz_src->format->palette->colors[i];
	    }
	  rz_dst->format->palette->ncolors = rz_src->format->palette->ncolors;
	  /* Call the 8bit transformation routine to do the zooming */
	  zoomSurfaceY (rz_src, rz_dst);
	  SDL_SetColorKey (rz_dst, SDL_SRCCOLORKEY| SDL_RLEACCEL, rz_src->format->colorkey);
	}
      /* Unlock source surface */
      SDL_UnlockSurface (rz_src);
    }

  /* Cleanup temp surface */
  if (src_converted)
    {
      SDL_FreeSurface (rz_src);
	rz_src=NULL;
    }

  /* Return destination surface */

  return (rz_dst);
}
//________________________________________________________________________________________________________________________________________

/*
 zoomSurface()
 Zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
*/

#define VALUE_LIMIT	0.001

SDL_Surface *zoomSurface (SDL_Surface * src, double zoomx, double zoomy, int smooth)
{
  SDL_Surface *rz_src;
  SDL_Surface *rz_dst;
  int dstwidth, dstheight;
  int is32bit;
  int i, src_converted;

  /* Sanity check */
  if (src == NULL)
    return (NULL);

  /* Determine if source surface is 32bit or 8bit */
  is32bit = (src->format->BitsPerPixel == 32);
  if ((is32bit) || (src->format->BitsPerPixel == 8))
    {
      /* Use source surface 'as is' */
      rz_src = src;
      src_converted = 0;
    }
  else
    {
      /* New source surface is 32bit with a defined RGBA ordering */
      rz_src =
	SDL_CreateRGBSurface (SDL_SWSURFACE, src->w, src->h, 32, 0x000000ff,
			      0x0000ff00, 0x00ff0000, 0xff000000);
      SDL_BlitSurface (src, NULL, rz_src, NULL);
      src_converted = 1;
      is32bit = 1;
    }

  /* Sanity check zoom factors */
  if (zoomx < VALUE_LIMIT)
    {
      zoomx = VALUE_LIMIT;
    }
  if (zoomy < VALUE_LIMIT)
    {
      zoomy = VALUE_LIMIT;
    }

  /* Calculate target size and set rect */
  dstwidth = (int) ( rz_src->w * zoomx);//(double)
  dstheight = (int) ( rz_src->h * zoomy);//(double)
  if (dstwidth < 1)
    {
      dstwidth = 1;
    }
  if (dstheight < 1)
    {
      dstheight = 1;
    }

  /* Alloc space to completely contain the zoomed surface */
  rz_dst = NULL;
  if (is32bit)
    {
      /* Target surface is 32bit with source RGBA/ABGR ordering */
      rz_dst =
	SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 32,
			      rz_src->format->Rmask, rz_src->format->Gmask,
			      rz_src->format->Bmask, rz_src->format->Amask);
    }
  else
    {
      /* Target surface is 8bit */
      rz_dst =
	SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0, 0,
			      0);
    }

  /* Lock source surface */
  SDL_LockSurface (rz_src);
  /* Check which kind of surface we have */
  if (is32bit)
    {
      /* Call the 32bit transformation routine to do the zooming (using alpha) */
      zoomSurfaceRGBA (rz_src, rz_dst, smooth);
      /* Turn on source-alpha support */
      SDL_SetAlpha (rz_dst, SDL_SRCALPHA, 255);
    }
  else
    {
      /* Copy palette and colorkey info */
      for (i = 0; i < rz_src->format->palette->ncolors; i++)
	{
	  rz_dst->format->palette->colors[i] =
	    rz_src->format->palette->colors[i];
	}
      rz_dst->format->palette->ncolors = rz_src->format->palette->ncolors;
      /* Call the 8bit transformation routine to do the zooming */
      zoomSurfaceY (rz_src, rz_dst);
      SDL_SetColorKey (rz_dst, SDL_SRCCOLORKEY| SDL_RLEACCEL, rz_src->format->colorkey);
    }
  /* Unlock source surface */
  SDL_UnlockSurface (rz_src);

  /* Cleanup temp surface */
  if (src_converted)
    {
      SDL_FreeSurface (rz_src);
	rz_src=NULL;
    }

/* Return destination surface */
  return (rz_dst);
}
//________________________________________________________________________________________________________________________________________

/*
	This code was extracted from fade tutorial written by PatricK Kooman
*/

/* Fades the given surface in or out to the given screen within the given time
 If the image surface is the screen surface (pointer are equal), a copy is made
 first. We must do that because we are overwriting the Screen Surface. */


void fade (int s, SDL_Surface* in_p_surf_screen, SDL_Surface* in_p_surf_img, Uint32 in_ui_seconds, int in_b_fade_out)
{
    /* copy the parameters in stared variables */
    p_surf_screen[s]=in_p_surf_screen;
    p_surf_img[s]=in_p_surf_img;
    ui_seconds[s]=in_ui_seconds;
    b_fade_out[s]=in_b_fade_out;


    /* initialize vars */
    p_surf_black[s] = NULL;
    p_surf_screen_copy[s] = NULL;
    ui_flags[s] = SDL_SRCALPHA;

  /* Create the black surface in the format of the given screen */
  if ((p_surf_screen[s]->flags & SDL_HWSURFACE)) {
    ui_flags[s] |= SDL_HWSURFACE ;
  }
  else {
    ui_flags[s] |= SDL_SWSURFACE ;
  }
  if ((p_surf_black[s] = SDL_CreateRGBSurface (ui_flags[s],
    p_surf_screen[s]->w, p_surf_screen[s]->h,
    p_surf_screen[s]->format->BitsPerPixel,
    p_surf_screen[s]->format->Rmask, p_surf_screen[s]->format->Gmask, p_surf_screen[s]->format->Bmask,
    p_surf_screen[s]->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
	error_description="SDLengine error fade: could not create the black Surface";
	error_type=1;
      fprintf (stderr, "fade: could not create the black Surface. (%s)\n", SDL_GetError ()) ;
      return ;
  }
  /* Fill the Surface with black */
  SDL_FillRect (p_surf_black[s], NULL, SDL_MapRGB (p_surf_screen[s]->format, 0, 0, 0)) ;
  /* Should we make a copy of the Screen? */
  if (p_surf_screen[s] == p_surf_img[s]) {
    if ((p_surf_screen_copy[s] = SDL_CreateRGBSurface (ui_flags[s],
    p_surf_screen[s]->w, p_surf_screen[s]->h,
    p_surf_screen[s]->format->BitsPerPixel,
    p_surf_screen[s]->format->Rmask, p_surf_screen[s]->format->Gmask, p_surf_screen[s]->format->Bmask,
    p_surf_screen[s]->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
	error_description="SDLengine error fade: could not create a copy of the Screen Surface";
	error_type=1;
      fprintf (stderr, "fade: could not create a copy of the Screen Surface. (%s)\n", SDL_GetError ()) ;
      SDL_FreeSurface (p_surf_black[s]) ;
      return ;
    }
    /* Ok, copy surfaces and set surfact pointer */
    SDL_BlitSurface (p_surf_screen[s], NULL, p_surf_screen_copy[s], NULL) ;
    p_surf_img[s] = p_surf_screen_copy[s] ;
  }

  	/* Ok, we are now ready for the fade. These are the steps (looped):
	1. Draw p_surf_img onto p_surf_screen, just an ordinary blit.
	2. Decrease or increase (depends on fading in or out) the alpha value,
	based on the elapsed time since the previous loop-iteration.
	3. Draw p_surf_black onto p_surf_screen in the current alpha value.*/
	ui_old_time[s] = SDL_GetTicks () ;
	ui_curr_time[s] = ui_old_time[s] ;
	/* Convert the given time in seconds into miliseconds. */
	ui_time_ms[s] = ui_seconds[s] * 1000 ;


  if (b_fade_out[s])
    f_alpha[s] = 0.0 ;
  else
    f_alpha[s] = 255.0 ;

  fadeflag[s]=1;
  fadedelay[s]=SDL_GetTicks()+5;
}

void fadehandler(int s)
 {
  if (b_fade_out[s]){
    /* Draw the image onto the screen */
    SDL_BlitSurface (p_surf_img[s], NULL, p_surf_screen[s], NULL) ;
    /* Draw the black surface onto the screen */
    SDL_SetAlpha (p_surf_black[s], SDL_SRCALPHA, (Uint8) f_alpha[s]) ;
    SDL_BlitSurface (p_surf_black[s], NULL, p_surf_screen[s], NULL) ;
    /* Update the timer variables */
    ui_old_time[s] = ui_curr_time[s] ;
    ui_curr_time[s] = SDL_GetTicks();
    /* Flip the screen Surface */
    SDL_Flip (p_surf_screen[s]) ;
    /* Calculate the next alpha value */
    f_alpha[s] += (255.0 * ((double) (ui_curr_time[s] - ui_old_time[s]) / ui_time_ms[s])) ;
    //printf("%f %d %d %d \n",f_alpha, ui_curr_time, ui_old_time, ui_time_ms);
    if (f_alpha[s] >= 255.0)
	fadeflag[s]=0;
  }
  else {

      /* Draw the image onto the screen */
      SDL_BlitSurface (p_surf_img[s], NULL, p_surf_screen[s], NULL) ;
      /* Draw the black surface onto the screen */
      SDL_SetAlpha (p_surf_black[s], SDL_SRCALPHA, (Uint8) f_alpha[s]) ;
      SDL_BlitSurface (p_surf_black[s], NULL, p_surf_screen[s], NULL) ;
      /* Update the timer variables */
      ui_old_time[s] = ui_curr_time[s] ;
      ui_curr_time[s] = SDL_GetTicks() ;
      /* Flip the screen Surface */
      SDL_Flip (p_surf_screen[s]) ;
      /* Calculate the next alpha value */
      f_alpha[s] -= 255 * ((double) (ui_curr_time[s] - ui_old_time[s]) / ui_time_ms[s]) ;
      if (f_alpha[s] <= 0.0)
	  fadeflag[s]=0;
  }
  if (fadeflag[s]==0){
  /* Free the black Surface */
  SDL_FreeSurface (p_surf_black[s]) ;
  /* Free the Screen copy, if used */
  if (p_surf_screen_copy[s] != NULL) {
    SDL_FreeSurface (p_surf_screen_copy[s]) ;
  }
  }
    screenswap();
}
//________________________________________________________________________________________________________________________________________

/* Cross-Fades the given surfaces onto the given screen within the given time
 If the image surface is the screen surface (pointer are equal), a copy is made
 first. We must do that because we are overwriting the Screen Surface. */

void crossfade (int s, SDL_Surface* in_p_surf_screen, SDL_Surface* in_p_surf_img1, SDL_Surface* in_p_surf_img2, Uint32 in_ui_seconds)
{
    p_surf_screen[s]=in_p_surf_screen;
    p_surf_img1[s]=in_p_surf_img1;
    p_surf_img2[s]=in_p_surf_img2;
    ui_seconds[s]=in_ui_seconds;

  /* These are the steps to perform a cross-fade (looped):
  1. Draw p_surf_img1 onto p_surf_screen, just an ordinary blit.
  2. Increase the alpha value, based on the elapsed time since the previous loop-iteration.
  3. Draw p_surf_img2 onto p_surf_screen in the current alpha value.*/

 /* Convert the given time in seconds into miliseconds. */
  ui_time_ms[s] = ui_seconds[s] * 1000 ;

  ui_old_time[s] = SDL_GetTicks () ;
  ui_curr_time[s] = ui_old_time[s] ;

    f_alpha[s] = 0.0 ;

  crossfadeflag[s]=1;
  fadedelay[s]=SDL_GetTicks()+5;

}

void crossfadehandler(int s)
{

    /* Draw the image onto the screen */
    SDL_BlitSurface (p_surf_img1[s], NULL, p_surf_screen[s], NULL) ;
    /* Draw the black surface onto the screen */
    SDL_SetAlpha (p_surf_img2[s], SDL_SRCALPHA, (Uint8) f_alpha[s]) ;
    SDL_BlitSurface (p_surf_img2[s], NULL, p_surf_screen[s], NULL) ;
    /* Update the timer variables */
    ui_old_time[s] = ui_curr_time[s];
    ui_curr_time[s] = SDL_GetTicks();
    /* Flip the screen Surface */
    SDL_Flip (p_surf_screen[s]) ;
    /* Calculate the next alpha value */
    f_alpha[s] += (255.0 * ((double) (ui_curr_time[s] - ui_old_time[s]) / ui_time_ms[s])) ;

    screenswap();

    if (f_alpha[s] >=255.0)
	crossfadeflag[s]=0;
}
//________________________________________________________________________________________________________________________________________

// hit_pixelperfect(SDLbobfr[n],xa1,ya1,xa2,ya2,SDLbobfr[x],xb1,yb1,xb2,yb2)	:rutine with graphic image precision collision
int hit_pixelperfect(SDL_Surface *imga,int xa1,int ya1,int xa2,int ya2,SDL_Surface *imgb,int xb1,int yb1,int xb2,int yb2)
{
    int xaa,yaa,xbb,ybb;
    int xw,yh;
    int x,y;
    int cc;


    xaa=0;
    xbb=0;
    if (xa1<xb1)
	xaa=MAX(xa1,xb1)-MIN(xa1,xb1);
    else
	xbb=MAX(xa1,xb1)-MIN(xa1,xb1);

    yaa=0;
    ybb=0;
   if (ya1<yb1)
	yaa=MAX(ya1,yb1)-MIN(ya1,yb1);
    else
	ybb=MAX(ya1,yb1)-MIN(ya1,yb1);

    xw=MIN(xa2,xb2)-MAX(xa1,xb1);
    yh=MIN(ya2,yb2)-MAX(ya1,yb1);


    if (SDL_MUSTLOCK(imga)){
		SDL_LockSurface(imga);
		SDL_LockSurface(imgb);
	}

    cc=colorkey;
    if (cc==-1)cc=getpixel(imga,0,0);


    for (y=0; y<yh; y++){
	for (x=0; x<xw; x++){
	    if (getpixel(imga,xaa+x,yaa+y)!=cc){
		if(getpixel(imgb,xbb+x,ybb+y)!=cc){
		    if (SDL_MUSTLOCK(imga)){
			SDL_UnlockSurface(imga);
			SDL_UnlockSurface(imgb);
		    }
		    return 1;
		}
	    }
	}
    }

    if (SDL_MUSTLOCK(imga)){
		SDL_UnlockSurface(imga);
		SDL_UnlockSurface(imgb);
	}
    return 0;
}
//________________________________________________________________________________________________________________________________________

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */

Uint32 getpixel8(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x ;
	return *(Uint8 *)p;
    }
    else
	return 0;
}

Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 2;
	return *(Uint16 *)p;
    }
    else
	return 0;
}

Uint32 getpixel24BE(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 3;
	return p[0] << 16 | p[1] << 8 | p[2];
    }
    else
	return 0;
}

Uint32 getpixel24(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 3;
	return p[0] | p[1] << 8 | p[2] << 16;
    }
    else
	return 0;
}

Uint32 getpixel32(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 4;
	return *(Uint32 *)p;
    }
    else
	return 0;
}

//________________________________________________________________________________________________________________________________________

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */

void putpixel8(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
	if (x>=0 && y>=0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x ;
		*p= col;
	}
}

void putpixel16(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
	if (x>=0 && y>=0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x * 2;
		*(Uint16 *)p=col;
	}
}

void putpixel24BE(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
		p=(Uint8 *) surface->pixels + (y * surface->pitch) + x * 3;
		p[0] = (col >> 16) & 0xff;
		p[1] = (col >> 8) & 0xff;
		p[2] = col & 0xff;
    }

}

void putpixel24(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
    if (x>=0 && y>=0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x * 3;
		p[0] = col & 0xff;
		p[1] = (col >> 8) & 0xff;
		p[2] = (col >> 16) & 0xff;
    }

}

void putpixel32(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
	if (x>=0 && y>=0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x * 4;
		*(Uint32 *)p=col;
	}
}

void putpixelDX(SDL_Surface *surface, int x, int y, Uint32 col )
{
    // rettangolo di un pixel di lato
    SDL_Rect d;

    d.x=x;
    d.y=y;
    d.w=1;
    d.h=1;
    SDL_FillRect (surface, &d, col);
}


//}
//________________________________________________________________________________________________________________________________________

/*  Draw a line to use with other function like rect) */
void draw_line(SDL_Surface * surface,int x1, int y1, int x2, int y2,Uint32 color)
{
    //  Draw a line WITHOUT REFRESH VIDEO (to use with other function like rect)
    static int c,x,y;
    double slope;

    int dx = x2 - x1;  /* change in x */
    int dy = y2 - y1;  /* change in y */

    slope = (double)dy / dx;

    if (abs(dx) >= abs(dy)) {
        /* draw left/right - right/left */
        c = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2 + c; x += c) {
            y = y1 + (int)( (x - x1) * slope + 0.5 );
            putpixel(surface, x, y, color);
        }
    }
    else
    {
        /* draw top/bottom - bottom/top */
        c = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2 + c; y += c) {
            x = x1 + (int)( (y - y1) / slope + 0.5 );
            putpixel(surface, x, y, color);
        }
    }
}
//________________________________________________________________________________________________________________________________________

/* draw empty rect */
void draw_box(SDL_Surface *surface,int x1,int y1,int x2,int y2,Uint32 color)
{
int i;
for(i=x1;i<=x2;i++){putpixel(surface,i,y1,color);putpixel(surface,i,y2,color);}
for(i=y1;i<=y2;i++){putpixel(surface,x1,i,color);putpixel(surface,x2,i,color);}
}
//________________________________________________________________________________________________________________________________________

/*draw empty circle */
void draw_circle(SDL_Surface *surface,int vx,int vy, int r,Uint32 color)
{

  Sint16 x = 0;
  Sint16 y = r-1;     /*radius zero == draw nothing*/
  Sint16 d = 3 - 2*r;
  Sint16 diagonalInc = 10 - 4*r;
  Sint16 rightInc = 6;

  while (x <= y) {

    putpixel(surface,vx+x,vy+y,color);
    putpixel(surface,vx+x,vy-y,color);
    putpixel(surface,vx-x,vy+y,color);
    putpixel(surface,vx-x,vy-y,color);

    putpixel(surface,vx+y,vy+x,color);
    putpixel(surface,vx+y,vy-x,color);
    putpixel(surface,vx-y,vy+x,color);
    putpixel(surface,vx-y,vy-x,color);


  if (d >=  0) {
      d += diagonalInc;
      diagonalInc += 8;
      y -= 1;
    } else {
      d += rightInc;
      diagonalInc += 4;
    }
    rightInc += 4;
    x += 1;
  }
}
//________________________________________________________________________________________________________________________________________

/* draw fill circle */
void draw_fillcircle(SDL_Surface *surface,int vx,int vy, int r,Uint32 color)
{

  Sint16 x = 0;
  Sint16 y = r-1;     /*radius zero == draw nothing*/
  Sint16 d = 3 - 2*r;
  Sint16 diagonalInc = 10 - 4*r;
  Sint16 rightInc = 6;

  while (x <= y) {

    draw_line(surface,vx+x,vy+y,vx-x,vy+y,color);
    draw_line(surface,vx+x,vy-y,vx-x,vy-y,color);
    draw_line(surface,vx+y,vy+x,vx-y,vy+x,color);
    draw_line(surface,vx+y,vy-x,vx-y,vy-x,color);

  if (d >=  0) {
      d += diagonalInc;
      diagonalInc += 8;
      y -= 1;
    } else {
      d += rightInc;
      diagonalInc += 4;
    }
    rightInc += 4;
    x += 1;
  }
}
//________________________________________________________________________________________________________________________________________

/* draw empty ellipse */
void draw_ellipse(SDL_Surface *surface,int vx,int vy,int Xradius,int Yradius,Uint32 color)
{
  Sint32 x, y;
  Sint32 Xchange, Ychange;
  Sint32 EllipseError;
  Sint32 TwoASquare, TwoBSquare;
  Sint32 StoppingX, StoppingY;

  TwoASquare = 2*Xradius*Xradius;
  TwoBSquare = 2*Yradius*Yradius;

  /*1st set of points*/
  x = Xradius-1;  /*radius zero == draw nothing*/
  y = 0;

  Xchange = Yradius*Yradius*(1-2*Xradius);
  Ychange = Xradius*Xradius;

  EllipseError = 0;

  StoppingX = TwoBSquare*Xradius;
  StoppingY = 0;

  /*Plot four ellipse points by iteration*/
  while (StoppingX > StoppingY) {

    putpixel(surface,vx+x,vy+y,color);
    putpixel(surface,vx+x,vy-y,color);
    putpixel(surface,vx-x,vy+y,color);
    putpixel(surface,vx-x,vy-y,color);


    ++y;
    StoppingY    += TwoASquare;
    EllipseError += Ychange;
    Ychange      += TwoASquare;
    if (( 2*EllipseError + Xchange) > 0) {
      --x;
      StoppingX    -= TwoBSquare;
      EllipseError += Xchange;
      Xchange      += TwoBSquare;
    }
  }/*while*/

  /*2nd set of points*/
  x = 0;
  y = Yradius-1;  /*radius zero == draw nothing*/
  Xchange = Yradius*Yradius;
  Ychange = Xradius*Xradius*(1-2*Yradius);
  EllipseError = 0;
  StoppingX = 0;
  StoppingY = TwoASquare*Yradius;

  /*Plot four ellipse points by iteration*/
  while (StoppingX < StoppingY) {

    putpixel(surface,vx+x,vy+y,color);
    putpixel(surface,vx+x,vy-y,color);
    putpixel(surface,vx-x,vy+y,color);
    putpixel(surface,vx-x,vy-y,color);

    ++x;
    StoppingX    += TwoBSquare;
    EllipseError += Xchange;
    Xchange      += TwoBSquare;
    if ((2*EllipseError + Ychange) > 0) {
      --y;
      StoppingY    -= TwoASquare;
      EllipseError += Ychange;
      Ychange      += TwoASquare;
    }
  }
}
//________________________________________________________________________________________________________________________________________

/* draw fill ellipse */
void draw_fillellipse(SDL_Surface *surface,int vx,int vy,int Xradius,int Yradius,Uint32 color)
{
  Sint32 x, y;
  Sint32 Xchange, Ychange;
  Sint32 EllipseError;
  Sint32 TwoASquare, TwoBSquare;
  Sint32 StoppingX, StoppingY;

  TwoASquare = 2*Xradius*Xradius;
  TwoBSquare = 2*Yradius*Yradius;

  /*1st set of points*/
  x = Xradius-1;  /*radius zero == draw nothing*/
  y = 0;

  Xchange = Yradius*Yradius*(1-2*Xradius);
  Ychange = Xradius*Xradius;

  EllipseError = 0;

  StoppingX = TwoBSquare*Xradius;
  StoppingY = 0;

  /*Plot four ellipse points by iteration*/
  while (StoppingX > StoppingY) {

    draw_line(surface,vx+x,vy+y,vx-x,vy+y,color);
    draw_line(surface,vx+x,vy-y,vx-x,vy-y,color);


    ++y;
    StoppingY    += TwoASquare;
    EllipseError += Ychange;
    Ychange      += TwoASquare;
    if (( 2*EllipseError + Xchange) > 0) {
      --x;
      StoppingX    -= TwoBSquare;
      EllipseError += Xchange;
      Xchange      += TwoBSquare;
    }
  }/*while*/

  /*2nd set of points*/
  x = 0;
  y = Yradius-1;  /*radius zero == draw nothing*/
  Xchange = Yradius*Yradius;
  Ychange = Xradius*Xradius*(1-2*Yradius);
  EllipseError = 0;
  StoppingX = 0;
  StoppingY = TwoASquare*Yradius;

  /*Plot four ellipse points by iteration*/
  while (StoppingX < StoppingY) {

    draw_line(surface,vx+x,vy+y,vx-x,vy+y,color);
    draw_line(surface,vx+x,vy-y,vx-x,vy-y,color);

    ++x;
    StoppingX    += TwoBSquare;
    EllipseError += Xchange;
    Xchange      += TwoBSquare;
    if ((2*EllipseError + Ychange) > 0) {
      --y;
      StoppingY    -= TwoASquare;
      EllipseError += Ychange;
      Ychange      += TwoASquare;
    }
  }
}
//________________________________________________________________________________________________________________________________________

// xrgb for internal use

/*
// this version was suggested from Paulo Silva still have problem of color in my machine
Uint32  xrgb8(int colR,int colG,int colB)
{
    return ((colR/43)*36) + ((colG/43)*6) + (colB/43);
}
//*/

//*
Uint32 xrgb8(int colR,int colG,int colB)
{
    if (enablePalette==2)return colB;
    else return (colB/64) + ((colG/64) * 8)+((colR/64)*8*8);
}
//*/

Uint32 xrgb16(int colR,int colG,int colB)
{
    return (colB/8) + ((colG/8) * 64)+((colR/8)*64*32);
}

Uint32 xrgb24(int colR,int colG,int colB)
{
    return colB + (colG * 256)+(colR*256*256);
}

Uint32 xrgb32(int colR,int colG,int colB)
{
    return  colB + (colG * 256)+(colR*256*256);
}


//________________________________________________________________________________________________________________________________________

// rrgb for internal use

/*
// this version was suggested from Paulo Silva still have problem of color in my machine
Uint32 rrgb8(int col)
{
    int colR,colG,colB;
    colB = (col & 6)*51;
    colG =((col / 6) & 6)*51 ;
    colR =((col / 36) & 6)*51;
    return colB + (colG * 256)+(colR*256*256);
}
//*/

//*
Uint32 rrgb8(int col)
{
    int colR,colG,colB;
    colB = (col & 3)*64;
    colG =((col / 8) & 3)*64 ;
    colR =((col / (8*8)) & 3)*64;
    if (enablePalette==2)return colB;
    else return colB + (colG * 256)+(colR*256*256);
}
//*/

Uint32 rrgb16(int col)
{
    int colR,colG,colB;
    colB = (col & 31) *8;
    colG =((col/32) & 31) *8;
    colR =((col/(64*32)) & 31)*8;
    return colB + (colG * 256)+(colR*256*256);
}

Uint32 rrgb24(int col)
{
    return col;
}

Uint32 rrgb32(int col)
{
    return col;
}

//________________________________________________________________________________________________________________________________________

//createfont: make a monospace font and set at 0,0 the cursor coordinates for internal use
int createfont()
{
	int i;
	paper(0);
	pen(rgb(255,255,255));
	for (i=0;i<NUM_SCREENS;i++){
		stx[c_screen]=0;
		sty[c_screen]=0;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

//print one char at current coordinates for internal use
int put_char(unsigned char n)
{
    SDL_Rect s;
    SDL_Rect d;
    SDL_Color oldc;


    oldc=SDLcol;

    if (n==13){
	stx[c_screen]=0;
	sty[c_screen]+=1;
	n=0;
    }
    else{
	ink (paper_color);
	bar(stx[c_screen]*XPRINT_SIZE,2+sty[c_screen]*(PRINT_SIZE+1),stx[c_screen]*XPRINT_SIZE+XPRINT_SIZE,2+sty[c_screen]*(PRINT_SIZE+1)+PRINT_SIZE);

	if (SDLfont_mono[n]!=NULL){
			s.x=0;
			s.y=0;
			s.w=XPRINT_SIZE-1;
			s.h=SDLfont_mono[n]->h;

			d.x=stx[c_screen]*XPRINT_SIZE;
			d.y=sty[c_screen]*(PRINT_SIZE+1);
			d.w=SDLfont_mono[n]->w;
			d.h=SDLfont_mono[n]->h;

			SDL_BlitSurface(SDLfont_mono[n], &s, SDLscreen[c_screen], &d );
	}

	stx[c_screen]=stx[c_screen]+1;
	if (stx[c_screen]+1>(screenwidth()/XPRINT_SIZE)){
			stx[c_screen]=0;
			sty[c_screen]=sty[c_screen]+1;
	}

    }

    if (sty[c_screen]+1>(screenheight()/(PRINT_SIZE+1))){
	screenswap();
	screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE+1),c_screen,0,0);
	ink(paper_color);
	bar(0,screenheight()-(PRINT_SIZE+1),screenwidth(),screenheight());

	sty[c_screen]=sty[c_screen]-1;
	screenswap();
    }

	SDLcol=oldc;
	return 0;
}
//________________________________________________________________________________________________________________________________________

/* make a cursor */
int creacursore()
{
	int i;

	font_mono = TTF_OpenFont(fontPathMono,PRINT_SIZE);
	if (font_mono ==0) {
		error_description="SDLengine error: Can't open true type font monospace: please verify installation!!!";
		error_type=1;
		SDLerr(stderr,"SDLengine error: Can't open true type font monospace: please verify installation!!!  %s\n",SDL_GetError());
		return -1;
	}

	for (i=0; i<16;i++) {
		ink(rgb(i*15,i*15,0));
		SDL_FreeSurface(SDLcursore[i]);
		SDLcursore[i] = TTF_RenderText_Solid(font_mono,"_",SDLcol);
	}
	TTF_CloseFont(font_mono);
	return 0;
}
//________________________________________________________________________________________________________________________________________

/*
this was my very old rutines for input used in 1995 on borland tc turbovision replacement project adapted
on SDLengine
*/
void _input(char instring[255],int x, int y,int displen)
{
#define k_delete	127
#define k_up   		273
#define k_down  	274
#define k_right 	275
#define k_left  	276
#define k_insert  	277
#define k_home  	278
#define k_end  		279
#define k_pageup  	280
#define k_pagedown  	281

  int i,n;
  int p,p1,pdisp;
  char provstring[255];
  char dispstring[255];
  int px;

  p=0;
  pdisp=0;
  strcpy(provstring,instring);
  p=strlen(provstring);

  if ((p-pdisp)>displen)pdisp=p-displen;
  if ((p-pdisp)<0)pdisp=p;
  strncpy(dispstring,provstring+pdisp,displen);
  memset(dispstring+displen,0,1);
  locate(x,y);fprints(dispstring);
  do
  {
    while (inkey()!=0);

    n=0;
    while(n==0){
    px=(p-pdisp);//*xdf;
    locate(x+px,y);
      n=inkey();
      if (key(k_left)){
	      if(p>0){
		      p--;
		      n=0;
		      break;
	      }
	while(key(k_left));
	} //cleft

      if (key(k_right)){
		if(p<strlen(provstring)){
			p++;
		}
		n=0;
	      break;

	while(key(k_right));
	} //crigth
      if (key(k_delete))
	 {
	 if(p<strlen(provstring))memcpy(provstring+p,provstring+p+1,strlen(provstring)-p);
	 n=0;
 	while(key(k_delete));
	 break;
	 } //cancel
      if (key(k_home)){p=0;n=0;break;}//inizio
      if (key(k_end)){p=strlen(provstring);n=0;break;}//fine
   }
   if (n==13)break;
   //backdel
   if (n==8)
    {
       n=0;
       if (p>0)
       {
       memcpy(provstring+p-1,provstring+p,strlen(provstring)-p+1);
       p--;
       }
    }
    //delete
    if (n==127)
	 {
	 if(p<strlen(provstring))memcpy(provstring+p,provstring+p+1,strlen(provstring)-p);
	 n=0;
	 } //cancel


  if (n!=0 && strlen(provstring)<254)
  {
   for(p1=strlen(provstring);p1>=p;p1--){memcpy(provstring+p1+1,provstring+p1,1);}
   memset(provstring+p,n,1);
   memset(provstring+strlen(provstring),0,1);
   p++;
  }
    if ((p-pdisp)>displen)pdisp=p-displen;
    if ((p-pdisp)<0)pdisp=p;
    strncpy(dispstring,provstring+pdisp,displen);
    memset(dispstring+displen,0,1);



    locate(x+strlen(dispstring),y);
    if(n==0)fprints(" ");
    if (n!=0 && stx[c_screen]==0 && sty[c_screen]+2>(screenheight()/(PRINT_SIZE)))y--;


     i=0;
    locate(x,y);
    fprints(dispstring);


    //curson();
    screenswap();
  }
  while (n !=13);
  strcpy (instring,provstring);
}
//_______________________________________________________________________________________________________________________


//==========================================================================================================================================
//							STARTING AND CLOSING SDLENGINE
//==========================================================================================================================================

//________________________________________________________________________________________________________________________________________

//initialize								: Start the engine and set the default parameter
int initialize(int audio,int socket)
{
    int i;

	/*reset error_description */
	error_description=NULL;
	error_type=0;

	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
		SDLerr(stderr, "Can't initialize SDL: %s\n",SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);

	if (audio!=0){
	    /* Open the audio device */
	    // parameters: 11025 | 22050 | 44100 - AUDIO_U8 | AUDIO_S16SYS - 1 | 2 - 512 | 8192
	    #ifdef OPENBSD
	    if ( Mix_OpenAudio(44100, AUDIO_U8,	2, 8192) < 0 ) {
	    #else
	    if ( Mix_OpenAudio(44100, AUDIO_S16SYS,	2, 8192) < 0 ) {
	    #endif
		SDLerr(stderr,"Warning: Can't set 44100 Hz 16-bit stereo audio\n- Reason: %s\n",SDL_GetError());
		enabledsound=0;
	    }
	    else{
		Mix_AllocateChannels(16);
		enabledsound=1;
	    }
	}
	else{
		enabledsound=0;
	}

	/* initialize the true font system */
#ifdef __MORPHOS__
	fontPath="MOSSYS:Fonts/_ttf/DejaVuSans.ttf";
	if (!fileExist(fontPath))fontPath="DejaVuSans.ttf";

	fontPathMono="MOSSYS:Fonts/_ttf/DejaVuSansMono-Roman.ttf";
	if (!fileExist(fontPathMono))fontPathMono="DejaVuSansMono-Roman.ttf";
#elif __amigaos4__
    fontPath="Fonts:_TrueType/DejaVuSans.ttf";
    if (!fileExist(fontPath))fontPath="DejaVuSans.ttf";

    fontPathMono="Fonts:_TrueType/DejaVuSansMono-Roman.ttf";
    if (!fileExist(fontPathMono))fontPathMono="DejaVuSansMono-Roman.ttf";
#elif UNIX
	fontPath="/usr/share/fonts/ttf/dejavu/DejaVuSans.ttf";
	if (!fileExist(fontPath))fontPath="./DejaVuSans.ttf";

	fontPathMono="/usr/share/fonts/ttf/dejavu/DejaVuSansMono-Roman.ttf";
	if (!fileExist(fontPathMono))fontPathMono="./DejaVuSansMono-Roman.ttf";
#else

	fontPath=(char *)malloc(256);
	strcpy(fontPath,getenv("WINDIR"));
	strcat(fontPath,"\\fonts\\DejaVuSans.ttf\0");
	if (!fileExist(fontPath))fontPath=".\\DejaVuSans.ttf";

	fontPathMono=(char *)malloc(256);
	strcpy(fontPathMono,getenv("WINDIR"));
	strcat(fontPathMono,"\\fonts\\DejaVuSansMono-Roman.ttf\0");
	if (!fileExist(fontPathMono))fontPathMono=".\\DejaVuSansMono-Roman.ttf";
#endif
	if (!fileExist(fontPath)){
		error_description="Fatal: DejaVuSans.ttf fonts not found ";
		error_type=1;
		SDLerr(stderr,"Fatal: DejaVuSans.ttf fonts not found \n ");
		return 1;
	}
	if (!fileExist(fontPathMono)){
		error_description="Fatal: DejaVuSansMono-Roman.ttf fonts not found ";
		error_type=1;
		SDLerr(stderr,"Fatal: DejaVuSansMono-Roman.ttf fonts not found \n ");
		return 1;
	}

	if ( TTF_Init() != 0 ) {
		SDLerr(stderr,"Warning: Can't initialize true type font system: %s\n",SDL_GetError());
	}
	else
	{
		/* enabled unicode*/
		SDL_EnableUNICODE(1);

		/* enabled autorepeat keys */
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

		/* create and initialize print font */
		createfont();

		/* create and initialize print cursor */
		creacursore();
	}


	if (socket!=0){
	    /* net initializing */
	    if ( SDLNet_Init() != 0 ) {
		SDLerr(stderr,"Warning: Can't initialize net system: %s\n",SDL_GetError());
		enabledsock=0;
	    }
	    else{
		enabledsock=1;
	    }
	}
	else{
	    enabledsock=0;
	}

	for (i=0;i<NUM_SCREENS;i++){
	    fadeflag[i]=0;
	    crossfadeflag[i]=0;
	    fadedelay[i]=0;
	}

	/* the color default is white */
	SDLcol.r=255;
	SDLcol.g=255;
	SDLcol.b=255;

	/* reset the image bank */
	for (i=0;i<NUM_IMAGES;i++){
		SDLimage[i]=NULL;
		SDLimageCC[i]=NULL;
	}

	/* reset the sound bank */
	for (i=0;i<NUM_WAVES;i++)
		SDLsound[i]=NULL;

	/* reset the screen  bank */
	for (i=0;i<NUM_SCREENS;i++){
		SDLscreen[i]=NULL;
		SDLscreen_rect[i].x=0;
		SDLscreen_rect[i].y=0;
		SDLscreen_rect[i].w=0;
		SDLscreen_rect[i].h=0;
		SDLscreen_flag[i]=0;
		SDLscreen_live[i]=0;

		cursore_state[i]=0;
		cursore_anim[i]=0;


	}
	//lastscreen=1;

	/*reset the sprites  */
	for (i=0;i<NUM_SPRITES;i++){
		SDLspritelive[i]=0;
		SDLspritefr[i]=-1;
		SDLspritex[i]=0;
		SDLspritey[i]=0;
	}
	//lastsprite=1;

	/*reset the bobs  */
	for (i=0;i<NUM_BOBS;i++){
		SDLbobscreen[i]=-1;
		SDLbobback[i]=NULL;
		SDLboblive[i]=0;
		SDLbobfr[i]=-1;
		SDLbobx[i]=0;
		SDLboby[i]=0;
	}
	//lastbob=1;

	/* set default palette*/
	for (i=0;i<255;i++)
	    color(i,rgb(255-i,255-i,255-i));

	color(0,0); //black
	color(1,rgb(0,0,255)); //blue
	color(2,rgb(0,255,0)); //green
	color(3,rgb(192,192,255)); //azzurro
	color(4,rgb(255,0,0)); //red
	color(5,rgb(255,255,0)); //magenta
	color(6,rgb(128,80,40)); //marrone
	color(7,rgb(255,255,255)); //white
	color(8,rgb(128,128,128)); //grigio
	color(9,rgb(128,128,255)); //blu chiaro
	color(10,rgb(128,255,128)); //verde chiaro
	color(11,rgb(128,128,255)); //azzurro chiaro
	color(12,rgb(255,128,128)); //red chiaro
	color(13,rgb(255,255,128)); //magenta chiaro
	color(14,rgb(255,255,0)); //giallo
	color(15,rgb(255,255,255)); //white


	/*default colorkey */
	colorkey=-1;

	/*default settings*/
	setdisplay(640,480,0,1);
	autoback(25);
	autoupdatesprite(1);
	autoupdatebob(1);
	dualplayfield(0);
	getevent();
	mouseshow();

    return 0;
}
//________________________________________________________________________________________________________________________________________

//terminate								: Stop the engine and reset parameter
int terminate()
{
	/* close all socks opens */
        int i;
	for (i=1;i<NUM_SOCKS;i++)
	    closesock(i);

	SDLNet_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	atexit(SDL_Quit);
	//exit(0);
    return 0;
}
//_________________________________________________________________________________________________________________________


//==========================================================================================================================================
//							COMMANDS
//==========================================================================================================================================

//_________________________________________________________________________________________________________________________

//SCREEN
//_________________________________________________________________________________________________________________________

//setdisplay(w,h,bpp,0=screen|1=window |2=resizable)			: open the screen/window in double buffer.
int setdisplay(int w,int h, int bpp, int mode)
{

	//GL_display=0;
	switch (mode){
	case 0:
		/* Open the display device full screen */
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_HWPALETTE|SDL_SRCALPHA );//|SDL_ASYNCBLIT
		if ( SDLdisplay == NULL ) {
			setdisplay(640,480,0,1);
			autoback(25);
			error_description="SDLengine error - setDisplay: Can't set requested video mode ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - setDisplay: Can't set video mode: %d ",mode);
			return -1;
		}
		break;

	case 1:
		/* Open the display device window */
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_HWPALETTE|SDL_SRCALPHA);//|SDL_ASYNCBLIT
		SDL_WM_SetCaption("sdlBasic",NULL );
		if ( SDLdisplay == NULL ) {
			setdisplay(640,480,0,1);
			autoback(25);
			error_description="SDLengine error - setDisplay: Can't set requested video mode ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - setDisplay: Can't set video mode: %d ", mode);
			return -1;

		}
		break;

	case 2:
		/* Open the display device window */
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_HWPALETTE|SDL_SRCALPHA);//|SDL_ASYNCBLIT
		SDL_WM_SetCaption("sdlBasic",NULL );
		if ( SDLdisplay == NULL ) {
			setdisplay(640,480,0,1);
			autoback(25);
			error_description="SDLengine error - setDisplay: Can't set requested video mode ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - setDisplay: Can't set video mode: %d ", mode);
			return -1;
		}
		break;
	default:
		error_description="SDLengine error - setDisplay: invalid video mode ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - setDisplay: %d is not a valid video mode ",mode);
		return -1;
		break;
	}

	SDL_FillRect(SDLdisplay,NULL,0);

	switch (SDLdisplay->format->BytesPerPixel){
	case 1:
			getpixel=getpixel8;
			putpixel=putpixel8;
			rrgb=rrgb8;
			enablePalette=0;
			break;
	case 2:
			getpixel=getpixel16;
			putpixel=putpixel16;
			rrgb=rrgb16;
			enablePalette=0;
			break;
	case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
				getpixel=getpixel24BE;
				putpixel=putpixel24BE;
				rrgb=rrgb24;
			}
			else{
				getpixel=getpixel24;
				putpixel=putpixel24;
				rrgb=rrgb24;
			}
			enablePalette=0;
			break;
	case 4:
			getpixel=getpixel32;
			putpixel=putpixel32;
			rrgb=rrgb32;
			enablePalette=0;
			break;
	default:
			error_description="SDLengine error - setDisplay: specified video bpp is not valid ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - setDisplay: %d is not a valid video bpp ",bpp);
			return -1;
			break;
	}

	SDLautobacktime=timer()+400;
	screenopen(0,SDLdisplay->w,SDLdisplay->h,0,0,SDLdisplay->w,SDLdisplay->h,0);
	spriteclip(0,0,SDLdisplay->w,SDLdisplay->h);

	/* set the pointer */
	if ( mode==0)
	changemouse(1);
	else
	changemouse(0);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//setcaption(title)						: change the display window title
int setcaption(char *title)
{
    SDL_WM_SetCaption(title, title);
    if (autotimer()!=0)return -1;
    return 0;
}
//_______________________________________________________________________________________________________________________

//caption()							: return the display window title
int caption(char **title)
{
    char **icon;

    icon=NULL;
    SDL_WM_GetCaption(title,icon);
    if (autotimer()!=0)return -1;

    return 0;
}
//_______________________________________________________________________________________________________________________

//displaywidth()						: return the display width
int displaywidth()
{
	return SDLdisplay->w;
}
//_______________________________________________________________________________________________________________________
//displayheight()						: return the display height
int displayheight()
{
	return SDLdisplay->h;
}
//_______________________________________________________________________________________________________________________
//display.bpp							: return the display color depth
int displaybpp()
{
	return (SDLdisplay->format->BytesPerPixel * 8);
}

//_______________________________________________________________________________________________________________________

//screen n							: set the logic screen n  with n=-1 return the current screen
int screen (int n)
{
	if (n==-1)return c_screen;

	if (SDLscreen_live[n]!=0){
		c_screen=n;
	}
	else{
		error_description="SDLengine error - screen: screen does not exist ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screen: Screen %d does not exist ", n);
		return -1;
	}

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

// directscreen 						: direct drawing on display like a screen(more fast but sprite and offset does not works)
int directscreen ()
{
	if (SDLscreen_live[0]==1){
		SDL_FreeSurface(SDLscreen[0]);
	}

	SDLscreen[0]=SDLdisplay;
	SDLscreen_live[0]=-1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenopen n,w,h,dx,dy,dw,dh,flag				: open the logic screen n of dimension w,h in display coordinates dx,dy,dw,dh
int screenopen (int n,int w,int h,int dx,int dy,int dw,int dh,int flag)
{
	SDL_Surface *temp;
	int bpp;

	if (n<0 ||n>8){
			error_description="SDLengine error - screenOpen: Invalid screen number. Valid screens range from 0 to 8 ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - screenOpen: %d is an invalid screen number. Valid screens range from 0 to 8: ",n);
			return -1;
	}

	if (SDLscreen_live[n]!=0){
		SDL_FreeSurface(SDLscreen[n]);
		SDLscreen_live[n]=0;
	}

	if (SDLscreen_live[n]==0){

		temp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h,displaybpp(),0,0,0,0);//|SDL_ASYNCBLIT

		if(temp  == NULL) {
			error_description="SDLengine error - screenOpen: Internal error. CreateRGBSurface failed ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - screenOpen: Internal error. CreateRGBSurface failed: %s ", SDL_GetError());
			return -1;
		}

		if ( flag >1 ) {
			paper(0);
			SDL_FillRect(temp,NULL,0);
			if (SDL_MUSTLOCK(temp))
				SDL_LockSurface(temp);
			bpp = temp->format->BytesPerPixel;
			switch(bpp) {
				case 1:
					SDL_SetColorKey(temp, (SDL_SRCCOLORKEY),*(Uint8 *)temp->pixels);//|SDL_RLEACCEL
					break;
				case 2:
					SDL_SetColorKey(temp, (SDL_SRCCOLORKEY),*(Uint16 *)temp->pixels);//|SDL_RLEACCEL
					break;
				case 3:
					SDL_SetColorKey(temp, (SDL_SRCCOLORKEY),*(Uint32 *)temp->pixels);//|SDL_RLEACCEL
					break;
				case 4:
					SDL_SetColorKey(temp, (SDL_SRCCOLORKEY),*(Uint32 *)temp->pixels);//|SDL_RLEACCEL
					break;
			}
			if (SDL_MUSTLOCK(temp))
				SDL_UnlockSurface(temp);
		}


		SDLscreen[n]=SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		//SDLscreen[n]=temp;
		SDLoffx[n]=0;
		SDLoffy[n]=0;

		if (dw>w)dw=w;
		if (dh>h)dh=h;

		SDLscreen_rect[n].x=dx;
		SDLscreen_rect[n].y=dy;
		SDLscreen_rect[n].w=dw;
		SDLscreen_rect[n].h=dh;
		if (SDLoffx[c_screen]<0)
				SDLoffx[c_screen]=0;
		if (SDLoffx[c_screen]+SDLscreen_rect[c_screen].w >SDLscreen[c_screen]->w)
				SDLoffx[c_screen]=SDLscreen[c_screen]->w - SDLscreen_rect[c_screen].w;
		if (SDLoffy[c_screen]<0)
				SDLoffy[c_screen]=0;
		if (SDLoffy[c_screen]+SDLscreen_rect[c_screen].h >SDLscreen[c_screen]->h)
				SDLoffy[c_screen]=SDLscreen[c_screen]->h - SDLscreen_rect[c_screen].h;
		SDLscreen_flag[n]=flag;
		SDLscreen_live[n]=1;
		//printf("screenopen n=%d dx=%d dy=%d dw=%d dh=%d\n",n,dx,dy,dw,dh);
		SDL_FillRect(SDLscreen[n],NULL,0);

		screen(n);

		if (lastscreen<=n)lastscreen=n+1;

		if (lastscreen>8)lastscreen=8;


	if (autotimer()!=0)return -1;
	return 0;
	}
	else{
			error_description="SDLengine error: specified screen is yet opened ";
			error_type=1;
			SDLerr(stderr, "SDLengine error: Screen %d is yet opened ", n);
			return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenclose n							: close the logic screen n
int screenclose (int n)
{
	int i;

	if(c_screen==n){
		cursoff();
		for(i=7;i>=0;i--){
			if(SDLscreen_live[i]!=0 &&n!=i){
				c_screen=i;
				break;

			}
		}
	}

	if (c_screen!=n){
		if (SDLscreen[n]!=NULL){
			SDL_FreeSurface(SDLscreen[n]);
			SDLscreen_live[n]=0;
			return 0;
		}
		else{
			error_description="SDLengine error - screenClose: screen not yet opened ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - screenClose: screen %d not yet opened ", n);
			return -1;
		}
	}
	else {
		error_description="SDLengine error - screenClose: can't close screen, surface in use ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenClose: can't close screen %d, surface in use ", n);
		return -1;
	}

	i=lastscreen;
	while(SDLscreen_live[i]==0)i--;
	lastscreen=i+1;



	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenclone n,s,x,y,w,h,flag					: create a  new viewport in logic screen s
int screenclone(int n,int s,int x,int y,int w,int h,int flag)
{
	if (SDLscreen[s]!=NULL){
		SDLscreen[n]= SDLscreen[s];
		SDLoffx[n]=0;
		SDLoffy[n]=0;
		SDLscreen_rect[n].x=x;
		SDLscreen_rect[n].y=y;
		SDLscreen_rect[n].w=w;
		SDLscreen_rect[n].h=h;
		SDLscreen_flag[n]=flag;
		SDLscreen_live[n]=1;
		screen(n);
		if(lastscreen<=n)lastscreen=n+1;
		return 0;
	}else{
		error_description="SDLengine error - screenClone: screen not yet opened ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenClone: screen %d not yet opened ", s);
		return -1;
		}
}
//_______________________________________________________________________________________________________________________

//screencopy(n,x,y,w,h,nd,xd,yd)						: copy a portion of screen n in a screen nd
int screencopy(int n,int x,int y,int w,int h,int nd,int xd,int yd)
{


	SDL_Rect s;
	SDL_Rect d;

	s.x=x;
	s.y=y;
	s.w=w;
	s.h=h;

	d.x=xd;
	d.y=yd;
	d.w=w;
	d.h=h;

	if (SDLscreen[n]!=NULL && SDLscreen[nd]!=NULL){
		SDL_BlitSurface( SDLscreen[n], &s ,SDLscreen[nd], &d);
	}
	else {
		if (SDLscreen[n]==NULL){
			error_description="SDLengine error - screenCopy: source screen does not exist ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - screenCopy: source screen %d does not exist ", n);
			return -1;
		}else{
			error_description="SDLengine error - screenCopy: destination screen does not exist ";
			error_type=1;
			SDLerr(stderr, "SDLengine error - screenCopy: destination screen %d does not exist ", nd);
			return -1;
		}
	}

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenfade(n,t)								: fade the screen n in t time
int screenfade(int n,int t)
{
	if (autotimer()!=0)return -1;
	if(t==-1)return fadeflag[n];

	if (SDLscreen[n]!=NULL){
		fade(n,SDLscreen[n],SDLscreen[n],t,1);
	return 0;
	}
	else{
		error_description="SDLengine error - screenFadeOut: specified screen does not exist ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenFadeOut: Screen %d does not exist", n);
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenfadein(n,i,t)								: fade the screen n to image i in t time
int screenfadein(int n,int i,int t)
{
	if (autotimer()!=0)return -1;
 	if(t==-1)return fadeflag[n];

	if(imageexists(i)==0){
		error_description="SDLengine error - screenFadeIn: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenFadeIn: image slot %d is empty", i);
		return -1;
	}

	if (SDLscreen[n]!=NULL){
		fade(n,SDLscreen[n],SDLimage[i],t,0);
	return 0;
	}
	else{
		error_description="SDLengine error - screenFadeIn: specified screen does not exist ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenFadeIn: screen %d does not exist", n);
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screencrossfade(n,i,t)							: fade the screen n from current screen  to image i in t time
int screencrossfade(int n,int i,int t)
{
	SDL_Surface *tmp;

	if (autotimer()!=0)return -1;
	if(t==-1)return crossfadeflag[n];

	if(imageexists(i)==0){
		error_description="SDLengine error - screenCrossFade: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenCrossFade: image slot %d is empty", i);
		return -1;
	}
	if(t==0 || t<-1){
		error_description="SDLengine error - screenCrossFade: invalid fade duration value";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenCrossFade: %d is not a valid fade duration value", t);
		return -1;
	}

	if (SDLscreen[n]!=NULL){
		tmp=SDL_DisplayFormat(SDLscreen[n]);
		crossfade(n,SDLscreen[n],tmp,SDLimage[i],t);
	return 0;
	}
	else{
		error_description="SDLengine error - screenCrossFade: specified screen does not exist ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - screenCrossFade: screen %d does not exist", n);
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenalpha(n,a)								: set alpha(trasparency) of screen n
int screenalpha(int n,int a)
{
	if ( SDLscreen[n] != NULL ) {
		SDL_SetAlpha (SDLscreen[n], SDL_SRCALPHA, a);
	if (autotimer()!=0)return -1;
		return 0;
	}
	else
	{
		error_description="SDLengine error - screenAlpha: specified screen does not exist ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - screenAlpha: screen %d does not exist ", n);
		return -1;
	}

}
//_______________________________________________________________________________________________________________________

//screenlock(n)								: lock the  screen n for direct graphics access
int screenlock(int n)
{
    if (SDL_MUSTLOCK(SDLscreen[n]))
	SDL_LockSurface(SDLscreen[n]);
    return 0;
}
//_______________________________________________________________________________________________________________________

//screenunlock(n)							: unlock the  screen n for direct graphics access
int screenunlock(int n)
{
    if (SDL_MUSTLOCK(SDLscreen[n]))
	SDL_UnlockSurface(SDLscreen[n]);
    return 0;
}
//_______________________________________________________________________________________________________________________

//screenrect(x,y,w,h,flag)						: change the display output coordinates of the current screen
int screenrect (int x,int y,int w,int h,int flag)
{
	SDLscreen_rect[c_screen].x=x;
	SDLscreen_rect[c_screen].y=y;
	SDLscreen_rect[c_screen].w=w;
	SDLscreen_rect[c_screen].h=h;
	SDLscreen_flag[c_screen]=flag;

	if (SDLoffx[c_screen]<0)
		SDLoffx[c_screen]=0;
	if (SDLoffx[c_screen]+SDLscreen_rect[c_screen].w >SDLscreen[c_screen]->w)
		SDLoffx[c_screen]=SDLscreen[c_screen]->w - SDLscreen_rect[c_screen].w;
	if (SDLoffy[c_screen]<0)
		SDLoffy[c_screen]=0;
	if (SDLoffy[c_screen]+SDLscreen_rect[c_screen].h >SDLscreen[c_screen]->h)
		SDLoffy[c_screen]=SDLscreen[c_screen]->h - SDLscreen_rect[c_screen].h;


	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//xscreenrect								: return the x coordinate of current screen viewport
int xscreenrect()
{
	return SDLscreen_rect[c_screen].x;
}
//_______________________________________________________________________________________________________________________

//yscreenrect								: return the y coordinate of current screen viewport
int yscreenrect()
{
	return SDLscreen_rect[c_screen].y;
}
//_______________________________________________________________________________________________________________________

//wscreenrect								: return the w value of current screen viewport
int wscreenrect()
{
	return SDLscreen_rect[c_screen].w;
}
//_______________________________________________________________________________________________________________________

//hscreenrect								: return the h value of current screen viewport
int hscreenrect()
{
	return SDLscreen_rect[c_screen].h;
}
//_______________________________________________________________________________________________________________________

//flagscreenrect							: return the flag value of current screen viewport
int flagscreenrect()
{
	return SDLscreen_flag[c_screen];
}
//_______________________________________________________________________________________________________________________

//screenwidth								: return the current screen width
int screenwidth()
{
	return SDLscreen[c_screen]->w;
}
//_______________________________________________________________________________________________________________________

//screenheight								: return the current screen height
int screenheight()
{
	return SDLscreen[c_screen]->h;
}
//_______________________________________________________________________________________________________________________

//offset(x,y)								: set the logical current screen position
int offset(int x,int y)
{
	SDLoffx[c_screen]=x;
	SDLoffy[c_screen]=y;
	if (SDLoffx[c_screen]<0)
		SDLoffx[c_screen]=0;
	if (SDLoffx[c_screen]+SDLscreen_rect[c_screen].w >SDLscreen[c_screen]->w)
		SDLoffx[c_screen]=SDLscreen[c_screen]->w - SDLscreen_rect[c_screen].w;
	if (SDLoffy[c_screen]<0)
		SDLoffy[c_screen]=0;
	if (SDLoffy[c_screen]+SDLscreen_rect[c_screen].h >SDLscreen[c_screen]->h)
		SDLoffy[c_screen]=SDLscreen[c_screen]->h - SDLscreen_rect[c_screen].h;

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//xoffset								: return the x coordinate offset in current screen
int xoffset()
{
	return SDLoffx[c_screen];
}
//_______________________________________________________________________________________________________________________

//yoffset								: return the y coordinate offset in current screen
int yoffset()
{
	return SDLoffy[c_screen];
}
//_______________________________________________________________________________________________________________________

//cls 									: clear the current logic screen
int cls()
{
    int r,g,b;
	b = paper_color & 255;
	g =( paper_color / 256) & 255 ;
	r =( paper_color / (256*256)) & 255;

	stx[c_screen]=0;
	sty[c_screen]=0;
	SDL_FillRect(SDLscreen[c_screen],NULL,(Uint32)SDL_MapRGB(SDLdisplay->format,r,g,b));
	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenswap								: update display,bobsystem,spritesystem
int screenswap()
{

	SDL_Rect d;
	SDL_Rect s;
	int i;

	getevent();
	if (stopkey()==-1)return -1;


	/* the negative mode of screenswap */
	if (SDLautoback<0){
		while (timer()<SDLautobacktime)getevent();
		SDLautobacktime=timer()+abs(SDLautoback);
	}


	if (vbl!=-1){
	    vbl=-1;

	/* if mouse cursor is software update sprite 0 with image 0 as mouse pointer */
	if (mousepointer>0 && mousestate!=0)
	    sprite(0,xmouse(),ymouse(),0);

	/* update the textprint cursor system */
	/*
		states of textprint cursor system:
		0:disable
		1:on enable
		2:enable
		3:on desable (will be change)
	*/

	    for(i=0;i<lastscreen;i++){
		if(cursore_state[i]>0) {

		    if (cursore_state[i]==1){
			SDLcursore_preserve[i] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, XPRINT_SIZE,PRINT_SIZE*2,32,0,0,0,0);
			if(SDLcursore_preserve[i]  == NULL) {
			    error_description="SDLengine error: CreateRGBSurface failed ";
			    error_type=1;
			    SDLerr(stderr, "SDLengine error: CreateRGBSurface failed: %s\n", SDL_GetError());
			    return -1;
			}
			cursore_state[i]=2;
		    }

		    if (cursore_state[i]>2){
		    	SDL_FreeSurface(SDLcursore_preserve[i]);
			cursore_state[i]=0;
		    }
		    if(cursore_state[i]==2) {
			oldstx[i]=stx[i];
			oldsty[i]=sty[i];

			//print the textprint cursor and  update the frame animation
			d.x=oldstx[i]*XPRINT_SIZE;
			d.y=oldsty[i]*(PRINT_SIZE+1);
			d.w=XPRINT_SIZE;
			d.h=PRINT_SIZE*2;
			s.x=0;
			s.y=0;
			s.w= XPRINT_SIZE;
			s.h= PRINT_SIZE*2;
			//preserve cursor background in cursor 0 anim
			SDL_BlitSurface( SDLscreen[i], &d, SDLcursore_preserve[i], &s);

			SDL_BlitSurface(SDLcursore[cursore_anim[i]], &s, SDLscreen[i], &d );

			cursore_anim[i]++;
			if (cursore_anim[i]>14)cursore_anim[i]=0;
		    }
		}
	}
	/***************************/



	/* update the bob system */
	if (SDLupdatebob!=0){

		//take the background
	    	for (i=0;i<lastbob;i++){
			if (SDLboblive[i]==1){
				if (SDLbobback[i]==NULL)
				{
					d.x=SDLbobxold[i];
					d.y=SDLbobyold[i];
					d.w=SDLimageCC[SDLbobfr[i]]->w;
					d.h=SDLimageCC[SDLbobfr[i]]->h;

					s.x=0;
					s.y=0;
					s.w= d.w;
					s.h= d.h;
					SDLbobback[i] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, s.w, s.h,32,0,0,0,0);
					if(SDLbobback[i]  == NULL) {
						error_description="SDLengine error: CreateRGBSurface failed ";
						error_type=1;
						SDLerr(stderr, "SDLengine error: CreateRGBSurface failed: %s\n", SDL_GetError());
						return -1;
					}
					//SDLbobback[i]=SDL_DisplayFormat(SDLbobback[i]);
				}

				if (SDLimageCC[SDLbobfr[i]]->w !=SDLbobback[i]->w || SDLimageCC[SDLbobfr[i]]->h !=SDLbobback[i]->h){
					SDL_FreeSurface(SDLbobback[i]);

					d.x=SDLbobxold[i];
					d.y=SDLbobyold[i];
					d.w=SDLimageCC[SDLbobfr[i]]->w;
					d.h=SDLimageCC[SDLbobfr[i]]->h;

					s.x=0;
					s.y=0;
					s.w= d.w;
					s.h= d.h;

					SDLbobback[i] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, s.w, s.h,32,0,0,0,0);
					if(SDLbobback[i]  == NULL) {
						error_description="SDLengine error: CreateRGBSurface failed ";
						error_type=1;
						SDLerr(stderr, "SDLengine error: CreateRGBSurface failed: %s\n", SDL_GetError());
						return -1;
					}
					//SDLbobback[i]=SDL_DisplayFormat(SDLbobback[i]);
				}


				SDLbobxold[i]=SDLbobx[i];
				SDLbobyold[i]=SDLboby[i];

				d.x=SDLbobxold[i];
				d.y=SDLbobyold[i];
				d.w=SDLimageCC[SDLbobfr[i]]->w;
				d.h=SDLimageCC[SDLbobfr[i]]->h;

				s.x=0;
				s.y=0;
				s.w= d.w;
				s.h= d.h;


				d.x=SDLbobxold[i];
				d.y=SDLbobyold[i];
				SDL_BlitSurface(SDLscreen[SDLbobscreen[i]], &d, SDLbobback[i], &s );
			}//end if
		}//end for

		//paste the bob
		for (i=0;i<lastbob;i++){
			if (SDLboblive[i]==1){

				d.x=SDLbobxold[i];
				d.y=SDLbobyold[i];
				d.w=SDLimageCC[SDLbobfr[i]]->w;
				d.h=SDLimageCC[SDLbobfr[i]]->h;

				s.x=0;
				s.y=0;
				s.w= d.w;
				s.h= d.h;

				if (SDLimageCC[SDLbobfr[i]]!=NULL){
					SDL_BlitSurface(SDLimageCC[SDLbobfr[i]], &s, SDLscreen[SDLbobscreen[i]], &d );
				}
				else {
					error_description="SDLengine error: bob image does not exist ";
					error_type=1;
					SDLerr(stderr, "SDLengine error: bob image does not exist");
					return -1;
				}//end if


			}//end if
		}//end for
	}//end if
	/***********************/


	/* update the display system under sprites*/
	for (i=0;i<=lastscreen;i++){
		if (SDLscreen_live[i]==1){
			if ((SDLscreen_flag[i] & 1)==0){
				s.x=SDLoffx[i];
				s.y=SDLoffy[i];
				s.w=SDLscreen_rect[i].w;
				s.h=SDLscreen_rect[i].h;

				SDL_BlitSurface(SDLscreen[i],&s,SDLdisplay,&SDLscreen_rect[i]);
			}
		}
	}

	/* update the sprite system */
	if(SDLupdatesprite!=0){
		if(SDLupdatesprite==2)SDLupdatesprite=0;
		for (i=1;i<lastsprite;i++){
			if (SDLspritelive[i]==1){


				d.x=SDLspritex[i];
				d.y=SDLspritey[i];
				d.w=MIN(SDLimageCC[SDLspritefr[i]]->w,(SDLspriteclip.w+SDLspriteclip.x-d.x));
				d.h=MIN(SDLimageCC[SDLspritefr[i]]->h,(SDLspriteclip.h+SDLspriteclip.y- d.y));

				if (d.x <SDLspriteclip.x ){
					s.x=0-(d.x-SDLspriteclip.x);
					d.x=d.x+s.x;
				}
				else{
					s.x=0;
				}
				if (d.y < SDLspriteclip.y){
					s.y=0-(d.y-SDLspriteclip.y);
					d.y=d.y+s.y;
				}
				else{
					s.y=0;
				}

				s.w= d.w - s.x;
				s.h= d.h - s.y;

				//no check: we are in critical mission real time
				if(d.x<SDLspriteclip.w+SDLspriteclip.x)
					if(d.y<SDLspriteclip.h+SDLspriteclip.y)
						SDL_BlitSurface(SDLimageCC[SDLspritefr[i]], &s, SDLdisplay, &d );


			}//end if
		}//end for
		/* update sprite 0 alias the mouse pointer */
		if (SDLspritelive[0]==1){
		    d.x=SDLspritex[0];
		    d.y=SDLspritey[0];
		    d.w=MIN(SDLimageCC[SDLspritefr[0]]->w,(SDLspriteclip.w+SDLspriteclip.x-d.x));
		    d.h=MIN(SDLimageCC[SDLspritefr[0]]->h,(SDLspriteclip.h+SDLspriteclip.y- d.y));

		    if (d.x <SDLspriteclip.x ){
			s.x=0-(d.x-SDLspriteclip.x);
			d.x=d.x+s.x;
		    }
		    else{
			s.x=0;
		    }
		    if (d.y < SDLspriteclip.y){
			s.y=0-(d.y-SDLspriteclip.y);
			d.y=d.y+s.y;
		    }
		    else{
			s.y=0;
		    }

		    s.w= d.w - s.x;
		    s.h= d.h - s.y;

		    //no check: we are in critical mission real time

		    if(d.x<SDLspriteclip.w+SDLspriteclip.x)
			if(d.y<SDLspriteclip.h+SDLspriteclip.y)
			    SDL_BlitSurface(SDLimageCC[SDLspritefr[0]], &s, SDLdisplay, &d );


		    }//end if

	}//end if
	/***********************/

	/* update the display system on sprites*/

	if (SDLdualplayfield!=0){
		for (i=0;i<lastscreen;i++){
			if (SDLscreen_live[i]==1){
				if ((SDLscreen_flag[i] & 1)==1 ){
					s.x=SDLoffx[i];
					s.y=SDLoffy[i];
					s.w=SDLscreen_rect[i].w;
					s.h=SDLscreen_rect[i].h;

					SDL_BlitSurface(SDLscreen[i],&s,SDLdisplay,&SDLscreen_rect[i]);
				}
			}
		}
	}
	/***********************/


	/* screen debug */

	if (SDLscreen_live[8]==1){
	    if(cursore_state[8]>0) {
		if (cursore_state[8]==1){
		    SDLcursore_preserve[8] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, XPRINT_SIZE,PRINT_SIZE*2,32,0,0,0,0);
		    if(SDLcursore_preserve[8]  == NULL) {
			error_description="SDLengine error: CreateRGBSurface failed ";
			error_type=1;
			SDLerr(stderr, "SDLengine error: CreateRGBSurface failed: %s\n", SDL_GetError());
			return -1;
		    }
		    cursore_state[8]=2;
		}

		if (cursore_state[8]>2){
		    SDL_FreeSurface(SDLcursore_preserve[8]);
		    cursore_state[8]=0;
		}
		if(cursore_state[8]==2) {
		    oldstx[8]=stx[8];
		    oldsty[8]=sty[8];

		    //print the textprint cursor and  update the frame animation
		    d.x=oldstx[8]*XPRINT_SIZE;
		    d.y=oldsty[8]*PRINT_SIZE;
		    d.w=XPRINT_SIZE;
		    d.h=PRINT_SIZE*2;
		    s.x=0;
		    s.y=0;
		    s.w= XPRINT_SIZE;
		    s.h= PRINT_SIZE*2;
		    //preserve cursor background in cursor 0 anim
		    SDL_BlitSurface( SDLscreen[8], &d, SDLcursore_preserve[8], &s);

		    SDL_BlitSurface(SDLcursore[cursore_anim[8]], &s, SDLscreen[8], &d );

		    cursore_anim[8]++;
		    if (cursore_anim[8]>14)cursore_anim[8]=0;
		}
	    }

	    s.x=SDLoffx[8];
	    s.y=SDLoffy[8];
	    s.w=SDLscreen_rect[8].w;
	    s.h=SDLscreen_rect[8].h;
	    SDL_BlitSurface(SDLscreen[8],&s,SDLdisplay,&SDLscreen_rect[8]);
	}




	/* fps system */
	if(SDLfps_flag!=0){
	    if (SDLfps_time < SDL_GetTicks()){
			SDLfps=SDLfps_count;
			SDLfps_count=0;
			SDLfps_time=SDL_GetTicks()+1000;
	    }
	    else
		SDLfps_count++;
	}


	SDL_Flip(SDLdisplay);


	// restore the background of bobs
	if (SDLupdatebob!=0){
		if (SDLupdatebob ==2)SDLupdatebob=0;
			//restore the background if exist
			for (i=0;i<NUM_BOBS;i++){
			    if (SDLboblive[i]==1){

				if (SDLbobback[i]!=NULL){
					d.x=SDLbobxold[i];
					d.y=SDLbobyold[i];
					d.w=SDLbobback[i]->w;
					d.h=SDLbobback[i]->h;

					s.x=0;
					s.y=0;
					s.w= d.w;
					s.h= d.h;
					SDL_BlitSurface(SDLbobback[i], &s, SDLscreen[SDLbobscreen[i]], &d );
				}

			    }
			    else{
				if (SDLbobback[i]!=NULL){

					d.x=SDLbobxold[i];
					d.y=SDLbobyold[i];
					d.w=SDLbobback[i]->w;
					d.h=SDLbobback[i]->h;

					s.x=0;
					s.y=0;
					s.w= d.w;
					s.h= d.h;

					SDL_BlitSurface(SDLbobback[i], &s, SDLscreen[SDLbobscreen[i]], &d );
					SDL_FreeSurface(SDLbobback[i]);
					SDLbobback[i]=NULL;
					SDLbobscreen[i]=-1;
				}

			}
		}//end for
	    }//endif

	vbl=0;
	}
	    for(i=0;i<lastscreen;i++){
		    if(cursore_state[i]>1) {
			//print the textprint cursor 0
			d.x=oldstx[i]*XPRINT_SIZE;
			d.y=oldsty[i]*(PRINT_SIZE+1);
			d.w=XPRINT_SIZE;
			d.h=PRINT_SIZE*2;
			s.x=0;
			s.y=0;
			s.w= XPRINT_SIZE;
			s.h= PRINT_SIZE*2;
			SDL_BlitSurface(SDLcursore_preserve[i], &s, SDLscreen[i], &d );
			//SDL_BlitSurface(SDLcursore[0], &s, SDLscreen[i], &d );
		    }
	    }
		/* cursore debug screen */
		if(cursore_state[8]>1) {
			//print the textprint cursor 0
			d.x=oldstx[8]*XPRINT_SIZE;
			d.y=oldsty[8]*(PRINT_SIZE+1);
			d.w=XPRINT_SIZE;
			d.h=PRINT_SIZE*2;
			s.x=0;
			s.y=0;
			s.w= XPRINT_SIZE;
			s.h= PRINT_SIZE*2;
			SDL_BlitSurface(SDLcursore_preserve[8], &s, SDLscreen[8], &d );
			//SDL_BlitSurface(SDLcursore[0], &s, SDLscreen[i], &d );
		    }


	return 0;
}
//_______________________________________________________________________________________________________________________

//autoback(m)								: enable / disable automatic screenswap  m=0 disable m>0 wait m milliseconds and perform screenswap m<0 perform the invocate in code screenswap after m milliseconds
int autoback( int m)
{
	if (m!=-1){
		SDLautoback=m;
		if (m>0)
			SDLautobacktime=timer()+m;
	}
	return SDLautoback;
}
//_______________________________________________________________________________________________________________________

//dualplayfield(m)							: set/unset automatic update of a screen upper sprite -1 return state
int dualplayfield(int m)
{
	if (m!=-1){
		SDLdualplayfield=m;
		if (autotimer()!=0)return -1;
		return 0;
	}
	else
		return SDLdualplayfield;

}
//_______________________________________________________________________________________________________________________

//waitvbl								: wait automatic screenswap
int waitvbl()
{
	if (autoback(-1)>0){

		vbl=1;
		while(vbl!=0)if (autotimer()!=0)return -1;
	}
	return 0;
}
//_______________________________________________________________________________________________________________________

//fps(n)								: set/unset or return (with -1) the current frame rate counter (0/1/-1)
int fps(int n)
{
    if (n==-1)
		return SDLfps;
    else{
		SDLfps_flag=n;
		SDLfps_count=0;
		SDLfps=0;
		SDLfps_time=SDL_GetTicks()+1000;
    }
    return 0;
}
//_________________________________________________________________________________________________________________________

//GRAPHICS
//_________________________________________________________________________________________________________________________

//rgb(r,g,b)								: return current color in Uint32 format
int rgb(int colR,int colG,int colB)
{
    int c;
    c=colB + (colG * 256)+(colR*256*256);
    return c;
}
//_________________________________________________________________________________________________________________________

//enablepalette(state=0|1|-1 disable|enable|check)   			: enable disable and check the palettemode in 8 bit display
int enablepalette(int state)
{
    int i;

    switch(state)
    {
	case 0:
	    if (enablePalette!=0)enablePalette=0;
	    break;
	case 1:
    	    if (enablePalette==0)
		enablePalette=1;
	    for(i=0;i<256;i++)
		color(i,Palette[i]);
	    break;
	default:
	    return enablePalette;
    }
    return 0;
}


//_________________________________________________________________________________________________________________________

//color (c,optional v)							: set  palette color c with value v se missing return the current color c
int color(int c,int v)
{
    SDL_Color c_color;
    if (v !=-1 ){
	if (c<0)return -1;
	if (c>255)return -1;
	Palette[c]=v;

	if (enablePalette!=0){
	    c_color.b = v & 255;
	    c_color.g =( v / 256) & 255 ;
	    c_color.r =( v / (256*256)) & 255;

	    /* Set palette */
	    SDL_SetPalette(SDLdisplay, SDL_LOGPAL|SDL_PHYSPAL, &c_color, c, 1);
	}
    return 0;
    }
    else{
	return Palette[c];
    }
}
//_________________________________________________________________________________________________________________________

//colorcycling(s,e,d=0|1,delay)			: move the palettecolor one color forward or back work only in 8bit display

int colorcycling(int s,int e,int d,int delay)
{
    if (s==-1){
	colorcycling_delay=0;
	return 0;
    }
    if (e>256)e=256;
    if (s<0)s=0;

    colorcycling_s=s;
    colorcycling_e=e;
    colorcycling_d=d;
    colorcycling_delay=delay;
    colorcycling_next=delay+SDL_GetTicks();

    if (delay==0)colorcyclinghandler();
    return delay;
}

void colorcyclinghandler()
{
    int s,e,d;
    SDL_Color colors[256];
    int i,c;

    s=colorcycling_s;
    e=colorcycling_e;
    d=colorcycling_d;

    if(d==0){
	c=Palette[s];
	for(i=s;i<=e;i++){
	    //color(i,Palette[i+1]);
	    colors[i].b = Palette[i+1] & 255;
	    colors[i].g =( Palette[i+1] / 256) & 255 ;
	    colors[i].r =( Palette[i+1] / (256*256)) & 255;
	    Palette[i]=	Palette[i+1];
	}
	//color(e,c);
	colors[e].b = c & 255;
	colors[e].g =( c / 256) & 255 ;
	colors[e].r =( c / (256*256)) & 255;
	Palette[e]=c;
    }
    else{
	c=Palette[e];
	for(i=e;i>=s;i--){
	    //color(i,Palette[i-1]);
	    colors[i].b = Palette[i-1] & 255;
	    colors[i].g =( Palette[i-1] / 256) & 255 ;
	    colors[i].r =( Palette[i-1] / (256*256)) & 255;
	    Palette[i]=	Palette[i-1];
	}
	//color(s,c);
	colors[s].b = c & 255;
	colors[s].g =( c / 256) & 255 ;
	colors[s].r =( c / (256*256)) & 255;
	Palette[s]=c;
    }
    /* Set palette */
    SDL_SetPalette(SDLdisplay, SDL_LOGPAL|SDL_PHYSPAL, &colors[s], s, e-s);

    colorcycling_next=colorcycling_delay+timer();
}

//_________________________________________________________________________________________________________________________

//ink(c)								: select the current color in Uint32 format
int ink( int c)
{
    if (c==-1){
	return (Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b);
    }
    else{
	SDLcol.b = c & 255;
	SDLcol.g =( c / 256) & 255 ;
	SDLcol.r =( c / (256*256)) & 255;
    }
    return 0;
}
//_______________________________________________________________________________________________________________________

//point(x,y)								: return the color of x,y point
int point(int x,int y)
{
	Uint32 r;
    	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	r=getpixel(SDLscreen[c_screen],x,y);

    	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	return rrgb(r);


}
//_______________________________________________________________________________________________________________________

//dot(x,y)								: write x,y point with a current color
int dot(int x,int y)
{

    if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
	    SDL_LockSurface(SDLscreen[c_screen]);

	putpixel( SDLscreen[c_screen], x, y,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));
	if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
	    SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//dot_ex(x,y,c)								: write x,y point with a c color
int dot_ex(int x,int y,int c)
{
	ink(c);
	if (autotimer()!=0)return -1;
	return dot(x,y);
}
//_______________________________________________________________________________________________________________________

//line(x,y,x1,y1)							: write a line
int line(int x,int y,int x1,int y1)
{
		if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
			SDL_LockSurface(SDLscreen[c_screen]);

		draw_line(SDLscreen[c_screen],x,y,x1,y1,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

		if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
			SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//box(x,y,x1,y1)							: write a empty rettangle
int box(int x,int y,int x1,int y1)
{
    int tmp;
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	if (x1<x){
	    tmp=x1;
	    x1=x;
	    x=tmp;
	}
	if (y1<y){
	    tmp=y1;
	    y1=y;
	    y=tmp;
	}

	draw_box(SDLscreen[c_screen],x,y,x1,y1,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//bar(x,y,x1,y1)							: write a fill rettangle
int bar(int x,int y,int x1,int y1)
{
	SDL_Rect d;
	int tmp;

	if (x1<x){
	    tmp=x1;
	    x1=x;
	    x=tmp;
	}
	if (y1<y){
	    tmp=y1;
	    y1=y;
	    y=tmp;
	}

	d.x=x;
	d.y=y;
	d.w=x1-x+1;
	d.h=y1-y+1;


    SDL_FillRect (SDLscreen[c_screen], &d,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

    if (autotimer()!=0)return -1;

    return 0;
}
//_______________________________________________________________________________________________________________________

//rectangle(x,y,w,h,mode)						: write a rettangle w and h are the sizes mode=0 empty 1=filled

int rectangle(int x,int y,int w,int h,int mode)
{
    switch(mode){
	case 1:
	    return bar(x,y,x+w,y+h);
	    break;

	default:
	    return box(x,y,x+w,y+h);
	    break;

    }

}

//_______________________________________________________________________________________________________________________

//circle(x,y,r)								: write a circle
int circle(int x,int y,int r)
{

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_circle(SDLscreen[c_screen],x,y,r,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//fillcircle(x,y,r)							: write a fill circle
int fillcircle(int x,int y,int r)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_fillcircle(SDLscreen[c_screen],x,y,r,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//ellipse(x,y,rx,ry)							: write a ellipse
int ellipse(int x,int y,int rx,int ry)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_ellipse(SDLscreen[c_screen],x,y,rx,ry,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//fillellipse(x,y,rx,ry)						: write a fill ellipse
int fillellipse(int x,int y,int rx,int ry)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_fillellipse(SDLscreen[c_screen],x,y,rx,ry,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//paint(x,y)								: fill a close area
int paint(int x,int y)
{
	char *paintmap;
	int xstart,ystart;
	int src_col,col;
	char pm;




	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	paintmap= malloc(screenwidth()*screenheight());

	xstart=x;
	ystart=y;
	src_col=getpixel(SDLscreen[c_screen],x,y);//point(x,y);
	putpixel(SDLscreen[c_screen],x,y,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));//dot(x,y);
	col=getpixel(SDLscreen[c_screen],x,y);//point(x,y);
    pm=2;
    if (col==src_col)return -1;

    while(0==0) {
	//if (point(x,y)==src_col) {
	if (getpixel(SDLscreen[c_screen],x,y)==src_col) {
	putpixel(SDLscreen[c_screen],x,y,(Uint32)SDL_MapRGB(SDLdisplay->format,SDLcol.r,SDLcol.g,SDLcol.b));//dot(x,y);
		paintmap[y*screenwidth()+x]=pm;
    	}

	//if (point(x+1,y)==src_col && (x+1) < screenwidth()) {
	if (getpixel(SDLscreen[c_screen],x+1,y)==src_col && (x+1) < screenwidth()) {
		    x=x+1;
		    pm=1;
		}
	//else if (point(x-1,y)==src_col && (x-1) > 0 ) {
	else if (getpixel(SDLscreen[c_screen],x-1,y)==src_col && (x-1) >= 0 ) {
		    x=x-1;
		    pm=2;
		}
	//else if (point(x,y+1)==src_col && (y+1) < screenheight()) {
	else if (getpixel(SDLscreen[c_screen],x,y+1)==src_col && (y+1) < screenheight()) {
		    y=y+1;
		    pm=3;
		}
	//else if (point(x,y-1)==src_col && (y-1) > 0) {
	else if (getpixel(SDLscreen[c_screen],x,y-1)==src_col && (y-1) >= 0) {
		    y=y-1;
		    pm=4;
		}
	else
		{
		if (x==xstart && y==ystart)break;

		if (paintmap[y*screenwidth()+x]==1)
			x=x-1;
		else if (paintmap[y*screenwidth()+x]==2)
			x=x+1;
		else if (paintmap[y*screenwidth()+x]==3)
			y=y-1;
		else if (paintmap[y*screenwidth()+x]==4)
			y=y+1;
		}
	}
	free(paintmap);

	if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (autotimer()!=0)return -1;
    return 0;
}
//_______________________________________________________________________________________________________________________

//_______________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

// FILE INPUT OUTPUT
//_________________________________________________________________________________________________________________________

//getfreeimage								: return a number of image bank free
int getfreeimage()
{
    int i;
    for(i=1;i<NUM_IMAGES;i++)
	if(SDLimage[i]==NULL)return i;
    return -1;
}
//_________________________________________________________________________________________________________________________

//loadimage(filename,n)							: load a graphics file in a slot n if n=-1 use the first free and return as n
int loadimage(char *filename,int n)
{
    if(n==-1)n=getfreeimage();
    if(n<0)return -1;

    if(n>NUM_IMAGES){

	error_description="SDLengine error - loadImage: given slot exceed maximum slots number";
	error_type=1;
	SDLerr(stderr,"SDLengine error - loadImage: slot %d exceed maximum image slots number ",n);
	return -1;
    }

	if (fileExist(filename)){
		if ( SDLimage[n] != NULL ) {
			SDL_FreeSurface(SDLimage[n]);
			SDL_FreeSurface(SDLimageCC[n]);
		}

		SDLimage[n]= Load_Image(filename, 0);
		SDLimageCC[n]=Load_Image(filename, 1);

		SDL_hsx[n]=0;
		SDL_hsy[n]=0;
	}
	else{
		error_description="SDLengine error - loadImage: file not found";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadImage: file '%s' not found ",filename);
		//exit(2);
		return -1;
	}
	return n;
}
//_______________________________________________________________________________________________________________________

//loadzipimage(zipfile,filename,opt n)					: load a graphics file stored in a zip archive in a slot n if n=-1 use the first free and return as n
int loadzipimage(char *zipfile,char *filename,int n)
{
    if(n==-1)n=getfreeimage();
    if(n<0)return -1;

    if(n>NUM_IMAGES){

	error_description="SDLengine error - loadZipImage: specified slot exceed image slots maximum number";
	error_type=1;
	SDLerr(stderr,"SDLengine error - loadZipImage: slot %d exceed image slots maximum number ",n);
	return -1;
    }

	if (fileExist(zipfile)){
		if ( SDLimage[n] != NULL ) {
			SDL_FreeSurface(SDLimage[n]);
			SDL_FreeSurface(SDLimageCC[n]);
		}

		SDLimage[n]= Load_zip_Image(zipfile,filename, 0);
		SDLimageCC[n]=Load_zip_Image(zipfile,filename, 1);
		if (SDLimage[n]==NULL){
		    error_description="SDLengine error - loadZipImage: image not found inside specified zip file ";
		    error_type=1;
		    SDLerr(stderr,"SDLengine error - loadZipImage: '%s' not found in zip file '%s' \n", filename, zipfile);
		    return -1;
		}
		SDL_hsx[n]=0;
		SDL_hsy[n]=0;
	}
	else{
		error_description="SDLengine error - loadZipImage: zip file not found ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadZipImage: zip file '%s' not found ",zipfile);
		return -1;
	}
	return n;
}
//_______________________________________________________________________________________________________________________

//saveimage(filename,n)							: save slot n in a graphics file(only bmp)
int saveimage(char *filename,int n)
{
	Save_Image(filename,SDLimage[n]);
	return 0;
}
//_______________________________________________________________________________________________________________________

//getfreesound								: return a number of first sound bank free
int getfreesound()
{
    int i;
    for(i=1;i<NUM_WAVES;i++)
	if(SDLsound[i]==NULL)return i;
    return -1;
}
//_______________________________________________________________________________________________________________________

//loadsound(filename,n)							: load a wave file in a sound slot n if n=-1  use and return the first free
int loadsound(char *filename,int n)
{
	if(n==-1)n=getfreesound();
	if(n<0)return -1;
	if (n>NUM_WAVES){
		error_description="SDLengine error - loadSound: given slot exceed maximum slots number ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadSound: slot %d exceed maximum sound slots number ",n);
		return -1;
	}


	if (fileExist(filename)){
		 Mix_FreeChunk(SDLsound[n]);
		 SDLsound[n]=Mix_LoadWAV(filename);
	}
	else{
		error_description="SDLengine error - loadSound: file not found ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadSound: file '%s' not found ",filename);
		return -1;
	}
	return n;
}
//________________________________________________________________________________________________________________________________________

//loadzipsound(zipfile,filename,n)					: load a wave file in a sound slot n if n=-1  use and return the first free
int loadzipsound(char *zipfile,char *filename,int n)
{
    unsigned char *mem;
    SDL_RWops *src;
    int size;

	if(n==-1)n=getfreesound();
	if(n<0)return -1;
	if (n>NUM_WAVES){
		error_description="SDLengine error - loadZipSound: specified slot exceed maximum slots number ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadZipSound: slot %d exceed maximum sound slots number ",n);
		return -1;
	}


	if (fileExist(zipfile)){
	    	if (load_zipped_file (zipfile, filename, &mem, &size)==0){
		    src=SDL_RWFromMem(mem, size);
		    Mix_FreeChunk(SDLsound[n]);
		    SDLsound[n]=Mix_LoadWAV_RW(src,1);
		}
		else{
		    error_description="SDLengine error - loadZipSound: sound not found in specified zip file";
		    error_type=1;
		    SDLerr(stderr,"SDLengine error - loadZipSound: sound '%s' not found in file '%s' \n",filename, zipfile);
		    return -1;
		}
	}
	else{
	    error_description="SDLengine error - loadZipSound: zip file not found ";
	    error_type=1;
	    SDLerr(stderr,"SDLengine error - loadZipSound: '%s' not found ", zipfile);
	    return -1;
	}
	return n;
}
//________________________________________________________________________________________________________________________________________

//savesound(filename,n)							: save a wave file from sound slot n
int savesound(char *filename, int n)
{
    FILE *fwave;
    char lws,hws;
    int i;
    int alen;

    if ( SDLsound[n] != NULL ) {
	unsigned char headsound[]={82,73,70,70,86,23,0,0,87,65,86,69,102,109,116,32,16,0,0,0,1,0,1,0,17,43,0,0,17,43,0,0,1,0,8,0,100,97,116,97};
	alen=SDLsound[n]->alen/14;
	lws=alen & 255;
	hws=alen / 256;
	fwave=fopen(filename,"wb");

	    for(i=0;i<40;i++){
		fputc(headsound[i],fwave);
	    }
	    fputc(lws,fwave);
	    fputc(hws,fwave);
	    fputc(0,fwave);
	    fputc(0,fwave);
	    fputc(0,fwave);
	    fputc(0,fwave);
	    fputc(0,fwave);

	    for(i=0;i<SDLsound[n]->alen;i+=14)
		fputc(SDLsound[n]->abuf[i+1],fwave);
	fclose(fwave);

	return 0;
    }
    else
    {
	error_description="SDLengine error - saveSound: specified sound slot is empty ";
	error_type=1;
	SDLerr(stderr,"SDLengine error - saveSound: sound slot %d is empty ", n);
	return -1;

    }
}
//_______________________________________________________________________________________________________________________

//loadmusic(filename)							: load a music module (xm,mod,ogg and only for linux mp3)
int loadmusic(char *filename)
{
	if (fileExist(filename)){
		Mix_FreeMusic(SDLmusic);
		SDLmusic = Mix_LoadMUS(filename);
	}
	else{
		error_description="SDLengine error - loadMusic: file not found ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadMusic: file '%s' not found ", filename);
		return -1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

//loadzipmusic(zipfile,filename)					: load a zipped music module (xm,mod,ogg and only for linux mp3)

int loadzipmusic(char *zipfile,char *filename)
{

    char *tmpfile;

	if (fileExist(zipfile)){
	    #ifndef WIN32
		char *tmpdir="/tmp";
	    #else
		char *tmpdir=getenv("tmp");
	    #endif
		if (file_unzip(zipfile,filename,tmpdir)==0){
		    tmpfile=(char *) malloc(strlen(tmpdir)+strlen(filename)+2);
		    strcpy(tmpfile,tmpdir);
		    strcat(tmpfile,"/");
		    strcat(tmpfile,filename);
		    tmpfile[strlen(tmpfile)]='\0';
		    loadmusic(tmpfile);
		    remove(tmpfile);
		}
		else{
		    error_description="SDLengine error - loadZipMusic: music not found in specified zip file ";
		    error_type=1;
		    SDLerr(stderr,"SDLengine error - loadZipMusic: music '%s' not found in file '%s' \n", filename, zipfile);
		    return -1;
		}
	}
	else{
		error_description="SDLengine error - loadZipMusic: zip file not found ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - loadZipMusic: zip file'%s' not found ", zipfile);
		return -1;
	}
	return 0;
}

//________________________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

// IMAGE MANIPULATION
//_________________________________________________________________________________________________________________________

//hotspot(n,x,y)							: select the point of coordinates in a imageslot (n,0,0=up left(default) | n,1,1=center | n,2,2 =down right)n= image
int hotspot(int n,int x,int y)
{
    if (SDLimage[n]!=NULL){
	switch(x) {

	case 0:
		SDL_hsx[n]=0;
		break;
	case 1:
		SDL_hsx[n]=SDLimage[n]->w / 2;
		break;
	case 2:
		SDL_hsx[n]=SDLimage[n]->w;
		break;
	default:
		SDL_hsx[n]=x;
		break;
	}

	switch(y) {

	case 0:
		SDL_hsy[n]=0;
		break;
	case 1:
		SDL_hsy[n]=SDLimage[n]->h / 2;
		break;
	case 2:
		SDL_hsy[n]=SDLimage[n]->h;
		break;
	default:
		SDL_hsy[n]=y;
		break;
	}

	return 0;
    }
    else{
	error_description="SDLengine error - hotSpot: specified image slot is empty ";
	error_type=1;
	SDLerr(stderr,"SDLengine error - hotSpot: image slot %d is empty ", n);
	return -1;
    }
}
//_________________________________________________________________________________________________________________________

//setcolorkey(c)							: set the colorkey for bitmap transparency if set -1 (default ) will be used the left-up corner color.
int setcolorkey(int c)
{
    if (c==-1){
	colorkey=-1;
    }
    else{
	colorkey=(Uint32)SDL_MapRGB(SDLdisplay->format,(c & 255),((c/256)&255),(c/(256*256))&255);
    }
    return 0;
}
//_________________________________________________________________________________________________________________________

//imageexists(n)							: return 1 if the image buffer n exist 0 if empty
int imageexists( int n )
{
	if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] == NULL ) {
		return(0);
	}
	return 1;
}
//_________________________________________________________________________________________________________________________

//imagewidth(n)								: return the image n width or error if image n if empty
int imagewidth(int n)
{
	if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		return SDLimage[n]->w;
	}
	else
	{
		error_description="SDLengine error - imageWidth: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - imageWidth: image slot %d is empty ", n);
		return -1;
	}

}
//_________________________________________________________________________________________________________________________

//imageheight(n)							: return the image n height or error if image n if empty
int imageheight(int n)
{
        if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		return SDLimage[n]->h;
	}
	else
	{
		error_description="SDLengine error - imageHeight: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - imageHeight: image slot %d is empty", n);
		return -1;
	}
}
//________________________________________________________________________________________________________________________________________

//deleteimage(n)							: erase the image n of the memory
int deleteimage(int n)
{
        if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		SDL_FreeSurface(SDLimage[n]);
		SDL_FreeSurface(SDLimageCC[n]);
		SDLimage[n]=NULL;
		SDLimageCC[n]=NULL;
		return 0;
	}
	else
	{
		error_description="SDLengine error - deleteImage: specified image slot is yet empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - deleteImage: image slot %d is yet empty", n);
		return -1;

	}

}
//________________________________________________________________________________________________________________________________________

//copyimage(s,d)  							: copy image s to image d
int copyimage(int s,int d)
{
	if (s<0 || s>NUM_IMAGES)return -1;
	if (d<0 || d>NUM_IMAGES)return -1;

	if ( SDLimage[s] != NULL ) {
		if ( SDLimage[d] != NULL ) {
			SDL_FreeSurface(SDLimage[d]);
			SDL_FreeSurface(SDLimageCC[d]);
		}

		SDLimage[d]=SDL_DisplayFormat(SDLimage[s]);
		if (SDLimage[d]==NULL)return -1;
		SDLimageCC[d]=SDL_DisplayFormat(SDLimageCC[s]);
		if (SDLimageCC[d]==NULL)return -1;
		//Image_colorkey(SDLimageCC[d]);

		return 0;
	}
	else
	{
		error_description="SDLengine error - copyImage: source image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - copyImage: source image slot %d is empty ", s);
		return -1;

	}

}
//________________________________________________________________________________________________________________________________________

//setalpha(n,a)								: set trasparency
int setalpha(int n,int a)
{
        if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		SDL_SetAlpha (SDLimage[n], SDL_SRCALPHA, a) ;
		SDL_SetAlpha (SDLimageCC[n], SDL_SRCALPHA, a) ;
		return 0;
	}
	else
	{
		error_description="SDLengine error - imageAlpha: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - imageAlpha: image slot %d is empty ", n);
		return -1;
	}
}
//_________________________________________________________________________________________________________________________

//zoomimage(n,zoomx,zoomy)						: zoom image
int zoomimage(int n,double zoomx,double zoomy)
{
	SDL_Surface *tmp;

	if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)waitTime(1);
		vbl=-1;
		tmp=zoomSurface(SDLimage[n], zoomx, zoomy, 0);
		if(tmp==NULL){
			error_description="SDLengine error - zoomImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - zoomImage: image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=zoomSurface(SDLimageCC[n], zoomx, zoomy, 0);
		if(tmp==NULL){
			error_description="SDLengine error - zoomImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - zoomImage: image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		error_description="SDLengine error - zoomImage: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - zoomImage: image slot %d is empty", n);
		return -1;
	}
	return 0;
}
//_________________________________________________________________________________________________________________________

//rotateimage(n,angle)							: rotate image
int rotateimage(int n,double angle)
{
	SDL_Surface *tmp;

        if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)waitTime(1);
		vbl=-1;

		tmp=rotozoomSurface (SDLimage[n], angle, 1, 1);
		if(tmp==NULL){
			error_description="SDLengine error - rotateImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - rotateImage: image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=rotozoomSurface (SDLimageCC[n], angle, 1, 1);
		if(tmp==NULL){
	    		error_description="SDLengine error - rotateImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - rotateImage: image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		error_description="SDLengine error - rotateImage: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - rotateImage: image slot %d is empty", n);
		return -1;
	}
	return 0;

}
//_________________________________________________________________________________________________________________________

//rotozoomimage(n,angle,zoom)						: rotate and zoom image
int rotozoomimage(int n,double angle,double zoom)
{
	SDL_Surface *tmp;

        if (n<0 || n>NUM_IMAGES)return -1;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)waitTime(1);
		vbl=-1;

		tmp=rotozoomSurface (SDLimage[n], angle, zoom, 1);
		if(tmp==NULL){
			error_description="SDLengine error - rotoZoomImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - rotoZoomImage: image not found %s\n",SDL_GetError());
			return -1;
		}

		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=rotozoomSurface(SDLimageCC[n], angle, zoom, 1);
		if(tmp==NULL){
			error_description="SDLengine error - rotoZoomImage: image not found ";
			error_type=1;
			SDLerr(stderr,"SDLengine error - rotoZoomImage: image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		error_description="SDLengine error - rotoZoomImage: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - rotoZoomImage: image slot %d is empty", n);
		return -1;
	}
	return 0;

}
//_________________________________________________________________________________________________________________________

//mirrorimage(n,x,y)							:vertical-orizontal mirror
int mirrorimage(int n,int x,int y)
{
    int px,py,dx,dy,w,h;
    SDL_Surface *tmp;
    SDL_Surface *tmpCC;
    if (n<0 || n>NUM_IMAGES)return -1;

    if ( SDLimage[n] != NULL ) {
	while(vbl==-1)waitTime(1);
	vbl=-1;

	w=SDLimage[n]->w-1;
	h=SDLimage[n]->h-1;

	tmp=SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL,SDLimage[n]->w,SDLimage[n]->h,32,0,0,0,0));
	tmpCC=SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL,SDLimage[n]->w,SDLimage[n]->h,32,0,0,0,0));
	if(tmp==NULL || tmpCC==NULL){
	    error_description="SDLengine error - mirrorImage: image not created ";
	    error_type=1;
	    SDLerr(stderr,"SDLengine error - mirrorImage: image not created %s\n",SDL_GetError());
	    return -1;
	}

	if (SDL_MUSTLOCK(SDLimage[n])){
	    SDL_LockSurface(SDLimage[n]);
	    SDL_LockSurface(SDLimageCC[n]);
	    SDL_LockSurface(tmp);
	    SDL_LockSurface(tmpCC);
	}
	for (py=0; py<h; py++){
	    for (px=0; px<w; px++){
		    if (x==0)
			dx=px;
		    else
			dx=w-px;

		    if (y==0)
			dy=py;
		    else
			dy=h-py;

		    putpixel(tmp,px,py,getpixel(SDLimage[n],dx,dy));
		    putpixel(tmpCC,px,py,getpixel(SDLimage[n],dx,dy));
	    }
	}
	if (SDL_MUSTLOCK(SDLimage[n])){
	    SDL_UnlockSurface(SDLimage[n]);
	    SDL_UnlockSurface(SDLimageCC[n]);
	    SDL_UnlockSurface(tmp);
	    SDL_UnlockSurface(tmpCC);
	}
	SDL_FreeSurface(SDLimage[n]);
	SDLimage[n]=tmp;
	SDL_FreeSurface(SDLimageCC[n]);
	SDLimageCC[n]=tmpCC;
	Image_colorkey(SDLimageCC[n]);

	vbl=0;
    }
    else
    {
	error_description="SDLengine error - mirrorImage: specified image slot is empty ";
	error_type=1;
	SDLerr(stderr,"SDLengine error - mirrorImage: image slot %d is empty", n);
	return -1;
    }

    return 0;
}
//_________________________________________________________________________________________________________________________

// BLITTING
//_________________________________________________________________________________________________________________________

//blt(n,sx,sy,sw,sh,dx,dy)						: copy a part of graphics slot in screen
int blt(int n,int sx,int sy,int sw,int sh,int dx,int dy)
{
	SDL_Rect s;
	SDL_Rect d;

	s.x=sx;
	s.y=sy;
	s.w=sw;
	s.h=sh;

	d.x=dx;
	d.y=dy;
	d.w=sw;
	d.h=sh;

        if (n<0 || n>NUM_IMAGES)return -1;

	if (SDLimage[n]!=NULL){
		SDL_BlitSurface( SDLimage[n], &s ,SDLscreen[c_screen], &d);
	}
	else {
		error_description="SDLengine error - blt: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - blt: image slot %d is empty", n);
		return-1;
	}

	if (autotimer()!=0)return -1;
	return 0;
}
//__________________________________________________________________________________________________________________________

//pastebob(x,y,n)							: copy on screen image n at x,y performing clip
int pastebob(int x,int y,int n)
{
	SDL_Rect s;
	SDL_Rect d;

	if (n<0 || n>NUM_IMAGES){
		error_description="SDLengine error - pasteBob: invalid image number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - pasteBob: %d is an invalid image number", n);
		return -1;
	}

	if (SDLimageCC[n]!=NULL){
		s.x=0;
		s.y=0;
		s.w=SDLimageCC[n]->w;
		s.h=SDLimageCC[n]->h;

		d.x=x-SDL_hsx[n];
		d.y=y-SDL_hsy[n];
		d.w=SDLimageCC[n]->w;
		d.h=SDLimageCC[n]->h;

		SDL_BlitSurface(SDLimageCC[n], &s, SDLscreen[c_screen], &d );
	}
	else {
		error_description="SDLengine error - pasteBob: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - pasteBob: image slot %d is empty", n);
		return -1;
	}

	if (autotimer()!=0)return -1;
	return 0;
}
//__________________________________________________________________________________________________________________________

//pasteicon(x,y,n)							: copy on screen image n at x,y without clipping
int pasteicon(int x,int y,int n)
{
	SDL_Rect s;
	SDL_Rect d;

        if (n<0 || n>NUM_IMAGES){
		error_description="SDLengine error - pasteIcon: invalid image number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - pasteIcon: %d is an invalid image number", n);
		return -1;
	}

	if (SDLimage[n]!=NULL){
		s.x=0;
		s.y=0;
		s.w=SDLimage[n]->w;
		s.h=SDLimage[n]->h;

		d.x=x-SDL_hsx[n];
		d.y=y-SDL_hsy[n];
		d.w=SDLimage[n]->w;
		d.h=SDLimage[n]->h;

		SDL_BlitSurface(SDLimage[n], &s, SDLscreen[c_screen], &d );
	}
	else {
		error_description="SDLengine error: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine errorpasteIcon: image slot %d is empty", n);
		return -1;
	}

	if (autotimer()!=0)return -1;
	return 0;
}
//__________________________________________________________________________________________________________________________

//grab(n,x,y,w,h)							: grab a a selectarea and copy it in slot n
int grab(int n,int x,int y,int w,int h)
{
	SDL_Rect s;
	SDL_Rect d;
	int bpp;
	int oldck;

	s.x=x;
	s.y=y;
	s.w=w;
	s.h=h;

	d.x=0;
	d.y=0;
	d.w=w;
	d.h=h;

	if (n<0 || n>NUM_IMAGES)return -1;

	if (SDLimage[n]!=NULL){
	    SDL_FreeSurface(SDLimage[n]);
	    SDLimage[n] = NULL;
	    SDL_FreeSurface(SDLimageCC[n]);
	    SDLimageCC[n] = NULL;
	}

	SDLimage[n] = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h,32,0,0,0,0);
	if(SDLimage[n]  == NULL) {
		error_description="SDLengine error - grab: CreateRGBSurface failed ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - grab: CreateRGBSurface failed: %s\n", SDL_GetError());
		return -1;
	}
	SDL_BlitSurface(SDLscreen[c_screen], &s, SDLimage[n], &d );

	SDLimageCC[n]=SDL_DisplayFormat(SDLimage[n]);
	if(SDLimageCC[n]  == NULL) {
		error_description="SDLengine error - grab: CreateRGBSurface failed ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - grab: CreateRGBSurfaceCC failed: %s\n", SDL_GetError());
		return -1;
	}

	bpp = SDLimageCC[n]->format->BytesPerPixel;
	oldck=colorkey;

	if (colorkey==-1){
	    if (SDL_MUSTLOCK(SDLimageCC[n]))
		SDL_LockSurface(SDLimageCC[n]);

	    colorkey=*(Uint32 *)SDLimageCC[n]->pixels;

	    if (SDL_MUSTLOCK(SDLimageCC[n]))
		SDL_UnlockSurface(SDLimageCC[n]);
	}

	switch(bpp) {
	    case 1:
		SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),(Uint8)colorkey);//|SDL_RLEACCEL
		break;
	    case 2:
		SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),(Uint16)colorkey);//|SDL_RLEACCEL
		break;
	    case 3:
		SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),(Uint32)colorkey);//|SDL_RLEACCEL
		break;
	    case 4:
		SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),(Uint32)colorkey);//|SDL_RLEACCEL
		break;
	}
	colorkey=oldck;

	SDL_hsx[n]=0;
	SDL_hsy[n]=0;

	return 0;
}
//__________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________

// SPRITE SYSTEM
//_________________________________________________________________________________________________________________________

//spriteclip(x,y,w,h)							: set the visibilty area of sprites
int spriteclip(int x,int y,int w,int h)
{
	SDLspriteclip.x=x;
	SDLspriteclip.y=y;
	SDLspriteclip.w=w;
	SDLspriteclip.h=h;
	return 0;
}
//__________________________________________________________________________________________________________________________

//sprite(n,x,y,fr)							: set or move the sprite n at x,y with image fr
int sprite(int n,int x,int y,int fr)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - sprite: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - sprite: %d is an invalid sprite number", n);
		return -1;
	}
	if (SDLimageCC[fr]!=NULL) {
		SDLspritelive[n]=1;
		SDLspritex[n]=x-SDL_hsx[fr];
		SDLspritey[n]=y-SDL_hsy[fr];
		SDLspritefr[n]=fr;

	}
	else {

		error_description="SDLengine error - sprite: specified image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - sprite: image slot %d is empty", n);
		return -1;
	}
	if (lastsprite<=n)lastsprite=n+1;


	if (autotimer()!=0)return -1;
	return 0;

}
//__________________________________________________________________________________________________________________________

//deletesprite(n)							: unset sprite n
int deletesprite(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - deleteSprite: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - deleteSprite: %d is an invalid sprite number", n);
		return -1;
	}

	SDLspritelive[n]=0; //morto!!
	SDLspritex[n]=0;
	SDLspritey[n]=0;
	SDLspritefr[n]=-1;

	if (lastsprite==n+1)
	    while (SDLspritelive[lastsprite-1]==0)lastsprite--;

	if (autotimer()!=0)return -1;
	return SDLspritelive[n];
}
//__________________________________________________________________________________________________________________________

//xsprite(n)								: return the x of sprite n
int xsprite(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteX: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteX: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLspritex[n]+SDL_hsx[SDLspritefr[n]];
}
//_________________________________________________________________________________________________________________________

//ysprite(n)								: return the y of sprite n
int ysprite(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteY: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteY: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLspritey[n]+SDL_hsy[SDLspritefr[n]];
}
//_________________________________________________________________________________________________________________________

//spritewidth(n)							: return the width of sprite n
int spritewidth(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteWidth: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteWidth: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
	if (SDLspritefr[n]!=-1)
		return SDLimageCC[SDLspritefr[n]]->w;
	return 0;
}
//_________________________________________________________________________________________________________________________

//spriteheight(n)							: return the height of sprite n
int spriteheight(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteHeight: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteHeight: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
	if (SDLspritefr[n]!=-1)
		return SDLimageCC[SDLspritefr[n]]->h;
	return 0;
}
//_________________________________________________________________________________________________________________________

//frsprite(n)								: return the frame of sprite n
int frsprite(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteImage: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteImage: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLspritefr[n];
}
//_________________________________________________________________________________________________________________________

//livesprite(n)								: return 1 if sprite n is "live"
int livesprite(int n)
{
	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteExist: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteExist: %d is an invalid sprite number ", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLspritelive[n];
}
//_________________________________________________________________________________________________________________________

//spritehit(n,x)							: return 1 if sprite n have a collission with sprite x if x=-1 with any
int spritehit(int n,int x)
{
    int xa1,ya1,xa2,ya2;
    int xb1,yb1,xb2,yb2;
	int i,ret;

	if (n>NUM_SPRITES || n<0 ){
		error_description="SDLengine error - spriteHit: invalid sprite number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - spriteHit: %d is an invalid sprite number ", n);
		return -1;
	}

    xa1=SDLspritex[n];
    ya1=SDLspritey[n];
    xa2=xa1+spritewidth(n);
    ya2=ya1+spriteheight(n);

    if (x!=-1){
		if (x>NUM_SPRITES || x<0 ){
		    error_description="SDLengine error - spriteHit: invalid sprite number for collision target";
		    error_type=1;
		    SDLerr(stderr, "SDLengine error - spriteHit: collision target sprite %d is an invalid sprite number ", x);
		return -1;
		}

		if (SDLspritelive[x]==1){
			xb1=SDLspritex[x];
			yb1=SDLspritey[x];
			xb2=xb1+spritewidth(x);
			yb2=yb1+spriteheight(x);

			if (xb2<xa1)return 0;
			if (yb2<ya1)return 0;
			if (xa2<xb1)return 0;
			if (ya2<yb1)return 0;
			//return 1;
			return hit_pixelperfect(SDLimageCC[SDLspritefr[n]],xa1,ya1,xa2,ya2,SDLimageCC[SDLspritefr[x]],xb1,yb1,xb2,yb2);
		}

    }
    else{
		for (i=0;i<NUM_SPRITES;i++){
			if (n != i){
				if (SDLspritelive[i]==1){
					xb1=SDLspritex[i];
					yb1=SDLspritey[i];
					xb2=xb1+spritewidth(i);
					yb2=yb1+spriteheight(i);

					ret=1;
					if (xb2<xa1)ret=0;
					if (yb2<ya1)ret=0;
					if (xa2<xb1)ret=0;
					if (ya2<yb1)ret=0;
					//if (ret==1)return 1;
					if (ret==1)return hit_pixelperfect(SDLimageCC[SDLspritefr[n]],xa1,ya1,xa2,ya2,SDLimageCC[SDLspritefr[i]],xb1,yb1,xb2,yb2);

				}
			}
		}

    }
    return 0;
}
//_________________________________________________________________________________________________________________________

//autoupdatesprite(m)							: set/ unset automatic sprites update at screenswap
int autoupdatesprite(int m)
{
	if (autotimer()!=0)return -1;
	SDLupdatesprite = m;
	return SDLupdatesprite;

}
//_________________________________________________________________________________________________________________________

//updatesprite								: manual sprites updates at next screenswap
int updatesprite()
{
	SDLupdatesprite = 2;
	if (autotimer()!=0)return -1;
	return SDLupdatesprite;

}
//_________________________________________________________________________________________________________________________

//BOB SYSTEM
//_________________________________________________________________________________________________________________________

//setbob(n,scr)								: set bob n at logic screen scr
int setbob(int n,int scr)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - setBob: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - setBob: invalid bob number");
		return -1;
	}
	SDLbobscreen[n]=scr;
	if (autotimer()!=0)return -1;
	 return 0;
}
//_________________________________________________________________________________________________________________________

//bob(n,x,y,fr)								: set or move bob n at x,y with frame fr
int bob(int n,int x,int y,int fr)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bob: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bob: invalid bob number");
		return -1;
	}

	if (SDLimageCC[fr]!=NULL){
		SDLboblive[n]=1;
		SDLbobx[n]=x-SDL_hsx[fr];
		SDLboby[n]=y-SDL_hsy[fr];
		SDLbobfr[n]=fr;
	}
	else {
		error_description="SDLengine error - bob: given image slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bob: image slot %d is empty", fr);
		return -1;
	}
	if (SDLbobscreen[n]==-1)SDLbobscreen[n]=c_screen;

	if (lastbob<=n)lastbob=n+1;


	if (autotimer()!=0)return -1;
	return 0;
}
//__________________________________________________________________________________________________________________________

//deletebob(n)								: unset bob n
int deletebob(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - deleteBob: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - deleteBob: %d is an invalid bob number", n);
		return -1;
	}
	SDLboblive[n]=0; //morto!!
	SDLbobx[n]=0;
	SDLboby[n]=0;
	SDLbobfr[n]=-1;

	if (lastbob==n+1)
	    while (SDLboblive[lastbob-1]==0)lastbob--;

	if (autotimer()!=0)return -1;
	return SDLboblive[n];
}
//__________________________________________________________________________________________________________________________

//xbob(n)								: return x of bob n
int xbob(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobX: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobX: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLbobx[n]-SDL_hsx[SDLbobfr[n]];
}
//_________________________________________________________________________________________________________________________

//ybob(n)								: return y of bob n
int ybob(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobY: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobY: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLboby[n]-SDL_hsy[SDLbobfr[n]];
}
//_________________________________________________________________________________________________________________________

//bobwidth(n)								: return width of bob n
int bobwidth(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobWidth: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobWidth: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
	if (SDLbobfr[n]!=-1)
		return SDLimageCC[SDLbobfr[n]]->w;
	return 0;
}
//_________________________________________________________________________________________________________________________

//bobheight(n)								: return height of bob n
int bobheight(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobHeight: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobHeight: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
	if (SDLbobfr[n]!=-1)
		return SDLimageCC[SDLbobfr[n]]->h;
	return 0;
}
//_________________________________________________________________________________________________________________________

//frbob(n)								: return the frame of bob n
int frbob(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobImage: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobImage: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLbobfr[n];
}
//_________________________________________________________________________________________________________________________

//livebob(n)								: return 1 if bob n is "live"
int livebob(int n)
{
	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobExist: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobExist: %d is an invalid bob number", n);
		return -1;
	}
	if (autotimer()!=0)return -1;
		return SDLboblive[n];
}
//_________________________________________________________________________________________________________________________

//bobhit(n,x)								: return 1 if bob n have a collision with bob x if x=-1 with any
int bobhit(int n,int x)
{
    int xa1,ya1,xa2,ya2;
    int xb1,yb1,xb2,yb2;
	int i,ret;

	if (n>NUM_BOBS || n<0){
		error_description="SDLengine error - bobHit: invalid bob number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - bobHit: %d is an invalid bob number", n);
		return -1;
	}

    xa1=SDLbobx[n];
    ya1=SDLboby[n];
    xa2=xa1+bobwidth(n);
    ya2=ya1+bobheight(n);

    if (x!=-1){
		if (x>NUM_BOBS || x<0){
		    error_description="SDLengine error - bobHit: invalid bob numberd as target ";
		    error_type=1;
		    SDLerr(stderr, "SDLengine error - bobHit: collision target bob %d is an invalid bob number", x);
		    return -1;
		}
		if (SDLboblive[x]==1){
			xb1=SDLbobx[x];
			yb1=SDLboby[x];
			xb2=xb1+bobwidth(x);
			yb2=yb1+bobheight(x);

			if (xb2<xa1)return 0;
			if (yb2<ya1)return 0;
			if (xa2<xb1)return 0;
			if (ya2<yb1)return 0;
			//return 1;
			return hit_pixelperfect(SDLimageCC[SDLbobfr[n]],xa1,ya1,xa2,ya2,SDLimageCC[SDLbobfr[x]],xb1,yb1,xb2,yb2);
		}

    }
    else{
		for (i=0;i<NUM_BOBS;i++){
			if (n != i){
				if (SDLboblive[i]==1){
					xb1=SDLbobx[i];
					yb1=SDLboby[i];
					xb2=xb1+bobwidth(i);
					yb2=yb1+bobheight(i);

					ret=1;
					if (xb2<xa1)ret=0;
					if (yb2<ya1)ret=0;
					if (xa2<xb1)ret=0;
					if (ya2<yb1)ret=0;
					//if (ret==1)return 1;
					if (ret==1)return hit_pixelperfect(SDLimageCC[SDLbobfr[n]],xa1,ya1,xa2,ya2,SDLimageCC[SDLbobfr[i]],xb1,yb1,xb2,yb2);
				}
			}
		}

    }
    return 0;
}

//_________________________________________________________________________________________________________________________

//autoupdatebob(m) 							: set/ unset automatic bobs update at screenswap
int autoupdatebob(int m)
{
	SDLupdatebob=m;
	if (autotimer()!=0)return -1;
	return SDLupdatebob;

}
//_________________________________________________________________________________________________________________________

//updatebob  								:  manual bobs updates at next screenswap
int updatebob()
{
	SDLupdatebob=2;
	if (autotimer()!=0)return -1;
	return SDLupdatebob;

}

//_________________________________________________________________________________________________________________________

// TEXT DRAW
//_________________________________________________________________________________________________________________________

//text(x,y,s,text)							: print the text testo  on current screen with s size
int text ( int x, int y, int s, char *testo,int option)
{
	SDL_Rect d;
	int w,h;
	SDL_Color bg;

	font = TTF_OpenFont(fontPath,s);
	if (font ==0) {
		error_description="SDLengine: Warning Can't open true type font ";
		error_type=1;
		SDLerr(stderr,"SDLengine: Warning Can't open true type font.  %s\n",SDL_GetError());
		return -1;
	}


	TTF_SizeText(font, testo, &w, &h);


	switch(option){
	    case 1:
		bg.b = paper_color & 255;
		bg.g =( paper_color / 256) & 255 ;
		bg.r =( paper_color / (256*256)) & 255;
		SDLtext = TTF_RenderUTF8_Shaded(font,testo,SDLcol,bg);
	    case 2:
		SDLtext = TTF_RenderUTF8_Blended(font,testo,SDLcol);
	    default:
		SDLtext = TTF_RenderUTF8_Solid(font,testo,SDLcol);
	}

	d.x=x;
	d.y=y;
	d.w=w;
	d.h=h;

	SDL_BlitSurface(SDLtext, NULL, SDLscreen[c_screen], &d );
	SDL_FreeSurface(SDLtext);
	TTF_CloseFont(font);
	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//setfont(path)								: select the font
int setfont(char *path)
{
	if (fileExist(path)) {


		fontPath=path;
	}
	else {
		error_description="SDLengine error: file not found ";
		error_type=1;
		SDLerr(stderr,"SDLengine error: file not found.  %s\n",SDL_GetError());
		return -1;
	}
	return 0;
}
//_______________________________________________________________________________________________________________________

//textrender(text,s,n)							: make an image slot n with the text  write with a current font and size s
int textrender (char *testo,int s,int n, int option)
{
	SDL_Color bg;


	if (n==-1)
		n=getfreeimage();

	if (n>NUM_IMAGES){
		error_description="SDLengine error - textRender: image number exceed maximum image bank number ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - textRender: image %d exceed maximum image bank number", n);
		return -1;
	}

	font = TTF_OpenFont(fontPath,s);
	if (font == 0) {
		error_description="SDLengine error - textRender: Can't open true type font. Please verify installation";
		error_type=1;
		SDLerr(stderr,"SDLengine error - textRender: Can't open true type font. Please verify installation %s\n",SDL_GetError());
		return -1;
	}

	SDL_FreeSurface(SDLimage[n]);
	SDL_FreeSurface(SDLimageCC[n]);

    	switch(option){
	    case 1:
		bg.b = paper_color & 255;
		bg.g =( paper_color / 256) & 255 ;
		bg.r =( paper_color / (256*256)) & 255;
		SDLimage[n] = TTF_RenderUTF8_Shaded(font,testo,SDLcol,bg);
		SDLimageCC[n] = TTF_RenderUTF8_Shaded(font,testo,SDLcol,bg);
	    case 2:
		SDLimage[n] = TTF_RenderUTF8_Blended(font,testo,SDLcol);
		SDLimageCC[n] = TTF_RenderUTF8_Blended(font,testo,SDLcol);
	    default:
		SDLimage[n] = TTF_RenderUTF8_Solid(font,testo,SDLcol);
		SDLimageCC[n] = TTF_RenderUTF8_Solid(font,testo,SDLcol);
	}


	TTF_CloseFont(font);
	if (autotimer()!=0)return -1;
	return n;
}


//_______________________________________________________________________________________________________________________

// PRINT SCREEN
//_________________________________________________________________________________________________________________________

//pen(c)								: set prints color (with -1 parameter return the current color)
int pen(int c)
{
    Uint16 i;
    SDL_Color oldc;


    if(c!=-1){

	oldc=SDLcol;
	ink(c);
	pen_color=c;

	font_mono = TTF_OpenFont(fontPathMono,PRINT_SIZE-1);
	if (font_mono ==0) {
	    error_description="SDLengine error - pen: Can't open monospace font. Please verify installation";
	    error_type=1;
	    SDLerr(stderr,"SDLengine error - pen: Can't open monospace font. Please verify installation.  %s\n",SDL_GetError());
	    return -1;
	}

	for (i=0; i<0xff;i++){
	    SDL_FreeSurface(SDLfont_mono[i]);
	    //SDLfont_mono[i] = TTF_RenderGlyph_Solid(font_mono,i,SDLcol); //make a problem with vertical collocation of charackters
	    SDLfont_mono[i] = TTF_RenderText_Solid(font_mono,(char *) &i,SDLcol);

	}

	TTF_CloseFont(font_mono);
	SDLcol=oldc;
	return 0;
    }
    else{
	    return pen_color;
    }
}
//_______________________________________________________________________________________________________________________

//paper(c)								: set caractere back color (with -1 parameter return the current color)
int paper(int c)
{
	if (c!=-1){
		paper_color=c;
		return 0;
	}
	else
	{
		return paper_color;
	}
}
//_______________________________________________________________________________________________________________________

//fprints(text)								: print a text monospace without cariage return
int fprints(char *testo)
{
	int flag_cursore;
	unsigned int i,i1;
	i=0;
	flag_cursore=cursore_state[c_screen];
    	if (flag_cursore!=0)cursoff();
	for (i=0;i<strlen(testo);i++){
	    if (testo[i]==9){
		for(i1=0;i1<8;i1++){
		    put_char(32);
		}
	    }
	    else{
		put_char(testo[i]);
	    }
	}
	if (flag_cursore!=0)curson();

	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//prints(text)								: print a text monospace
int prints(char *testo)
{
	int flag_cursore;
	unsigned int i,i1;
	i=0;
	flag_cursore=cursore_state[c_screen];
    	if (flag_cursore!=0)cursoff();
	for (i=0;i<strlen(testo);i++){
	    if (testo[i]==9){
		for(i1=0;i1<8;i1++){
		    put_char(32);
		}
	    }
	    else{
		put_char(testo[i]);
	    }
	}
	put_char(13);
	if (flag_cursore!=0)curson();

	if (autotimer()!=0)return -1;
	return 0;

}
//_______________________________________________________________________________________________________________________

//locate(x,y)								: move the cursor at x y
int locate(int x,int y)
{
	stx[c_screen]=x;
	sty[c_screen]=y;
	while(stx[c_screen]+1>(screenwidth()/XPRINT_SIZE))
	{
		if (stx[c_screen]+1>(screenwidth()/XPRINT_SIZE)){
			sty[c_screen]++;
			stx[c_screen]=stx[c_screen]-(screenwidth()/XPRINT_SIZE);
		}
	}

	while (sty[c_screen]+1>(screenheight()/(PRINT_SIZE))){
		screenswap();
		screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
		ink(paper_color);
		bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

		sty[c_screen]=sty[c_screen]-1;
		screenswap();
	}


	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//atx									: return x of cursor
int atx()
{
	return stx[c_screen];
}
//_______________________________________________________________________________________________________________________

//aty									: return y of cursor
int aty()
{
	return sty[c_screen];
}
//_______________________________________________________________________________________________________________________

//curson								: show the text cursor on screen at(atx,aty)
int curson()
{
    if (cursore_state[c_screen]==0){
	cursore_state[c_screen]=1;
	cursore_anim[c_screen]=0;
    }
    screenswap();
    if (autotimer()!=0)return -1;
    return 0;
}
//_______________________________________________________________________________________________________________________

//cursoff								: hide the text cursor
int cursoff()
{
    if (cursore_state[c_screen]==2){
	cursore_state[c_screen]=3;
	cursore_anim[c_screen]=0;
    }
    screenswap();
    if (autotimer()!=0)return -1;
    return 0;
}
//_______________________________________________________________________________________________________________________

//inputs(prompt,defs)							: return the string insert to keyboard(default is default text)
int inputs(char *request,char defs[255])
{


	if (cursore_state[c_screen]==2)
	{
	while ((sty[c_screen]+1)>(screenheight()/(PRINT_SIZE)))
		{
			screenswap();
			screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
			ink(paper_color);
			bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

			sty[c_screen]=sty[c_screen]-1;
			screenswap();
		}
		fprints(request);
		_input(defs,stx[c_screen],sty[c_screen],255);//screenwidth()/XPRINT_SIZE
		put_char(13);
	}
	else
	{
		while ((sty[c_screen]+1)>(screenheight()/(PRINT_SIZE)))
		{
			screenswap();
			screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
			ink(paper_color);
			bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

			sty[c_screen]=sty[c_screen]-1;
			screenswap();
		}
		fprints(request);
		curson();
		_input(defs,stx[c_screen],sty[c_screen],255);//screenwidth()/XPRINT_SIZE
		put_char(13);
		cursoff();
	}
	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//zoneinputs(x,y,l,default)						: return the string insert to keyboard in  x,y coordinates with l lenght
int zoneinputs(int x,int y,int l,char defs[255])
{


	if (cursore_state[c_screen]==2)
	{
		_input(defs,x,y,l);
		put_char(13);
	}
	else
	{
		locate(x,y);
		curson();
		_input(defs,x,y,l);
		put_char(13);
		cursoff();
	}
	if (autotimer()!=0)return -1;
	return 0;
}
//_________________________________________________________________________________________________________________________

//SOUND
//_________________________________________________________________________________________________________________________

//isenabledsound()							: return 1 if sdlsound was enabled
int isenabledsound()
{
    return enabledsound;
}
//________________________________________________________________________________________________________________________________________

//soundexists(n)							: return 1 if the sound buffer n exist 0 if empty
int soundexists( int n )
{
	if (n>NUM_WAVES || n<0){
		error_description="SDLengine error - soundExists: invalid sound slot number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - soundExists: %d is an invalid sound slot number", n);
		return -1;
	}
	if ( SDLsound[n] == NULL ) {
		return(0);
	}
	return 1;
}
//________________________________________________________________________________________________________________________________________

//deletesound(n)							: delete from memory sound n
int deletesound(int n)
{
	if (n>NUM_WAVES || n<0){
		error_description="SDLengine error - deleteSound: invalid sound slot number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - deleteSound: %d is an invalid sound slot number", n);
		return -1;
	}
    	if ( SDLsound[n] != NULL ) {
		Mix_FreeChunk(SDLsound[n]);
		SDLsound[n]=NULL;
		return 0;
	}
	else
	{
		error_description="SDLengine error - deleteSound: sound slot is yet empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - deleteSound: sound slot %d is yet empty", n);
		return -1;

	}


}
//________________________________________________________________________________________________________________________________________

//copysound(s,d)							: copy  sound bank s in sound bank d
int copysound(int s, int d)
{
	if (s>NUM_WAVES || s<0){
		error_description="SDLengine error - copySound: invalid source sound slot number";
		error_type=1;
		SDLerr(stderr, "SDLengine error - copySound: source slot %d is an invalid sound slot number", s);
		return -1;
	}
	if (d>NUM_WAVES || d<0){
		error_description="SDLengine error - copySound: invalid target sound slot number";
		error_type=1;
		SDLerr(stderr, "SDLengine error - copySound: destination slot %d is an invalid sound slot number", d);
		return -1;
	}

    	if ( SDLsound[s] != NULL ) {
		Mix_FreeChunk(SDLsound[d]);
		SDLsound[d]= Mix_QuickLoad_RAW(SDLsound[s]->abuf,SDLsound[s]->alen);
		return 0;
	}
	else
	{
		error_description="SDLengine error - copySound: source sound slot is empty ";
		error_type=1;
		SDLerr(stderr,"SDLengine error - copySound: source sound slot %d is empty", s);
		return -1;

	}
}
//________________________________________________________________________________________________________________________________________

//musicexists()								: return 1 if the music is load 0 if empty
int musicexists()
{
	if ( SDLmusic == NULL ) {
		return(0);
	}
	return 1;
}
//__________________________________________________________________________________________________________________________

//playsound(n,c,l)							: play the sound n in  channel c l loops
int playsound (int n,int c,int l)
{
	if (n>NUM_WAVES || n<0){
		error_description="SDLengine error - playSound: invalid sound slot number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - playSound: %d is an invalid sound slot number", n);
		return -1;
	}
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enabled");
		return 0;
	}
	if (SDLsound[n]!=NULL){
		Mix_PlayChannel(c,SDLsound[n], l);
		return 0;
	}
	else {
		error_description="SDLengine error - playSound: sound slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - playSound: sound slot %d is empty", n);
		return -1;
	}
}
//_________________________________________________________________________________________________________________________

//playfreqsound(n,c,pitch,l)						: play the sound n in  channel c l loops at freq variation (negative number=low positive number =high)
int playfreqsound ( int n, int c, float pitch, int l)
{
    	if (n>NUM_WAVES || n<0){
		error_description="SDLengine error - playFreqSound: invalid sound slot number ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - playFreqSound: %d is an invalid sound slot number", n);
		return -1;
	}
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled\n");
		return 0;
	}
	if (SDLsound[n]!=NULL){
	    Mix_PlayChannel(c,Change_Pitch(SDLsound[n],pow(2 , 0-(pitch/12))), l);
	    return 0;
	}
	else {
		error_description="SDLengine error - playFreqSound: sound slot is empty ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - playFreqSound: sound slot %d is empty", n);
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//volumesound(c,v)							: change the volume of channel c (-1  all) at value v (0-128) with v = -1 return the current volume
int volumesound(int c,int v)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
	return Mix_Volume(c,v);
}
//_______________________________________________________________________________________________________________________

//stopsound(c)								: stop the  wave play in channel c (-1 =all)
int stopsound(c)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    return Mix_HaltChannel(c);
}
//_______________________________________________________________________________________________________________________

//pausesound(c)								: paused channel c (-1 =all)
int pausesound(int c)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    Mix_Pause(c);
    return 0;
}
//_______________________________________________________________________________________________________________________

//resumesound(c)							: unpaused channel c (-1 =all)
int resumesound(int c)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    Mix_Resume(c);
    return 0;
}
//_______________________________________________________________________________________________________________________

//vumetersound(c)							: return the current state of sound channel (-1 =all)
int vumetersound(int c)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    return Mix_Playing(c);
}
//_______________________________________________________________________________________________________________________

//positionsound(c,angle,dist)						: position sound in 3d space
int positionsound(int c,int angle,int dist)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    return Mix_SetPosition(c, (Sint16)angle, (Uint8) dist);
}
//_______________________________________________________________________________________________________________________

//soundchannels(n)							: dinamically change the number of channells managed by sdlBasic
int soundchannels(int n)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    return Mix_AllocateChannels(n);
}
//_______________________________________________________________________________________________________________________

//playmusic(n)								: play track xm,mod,ogg,mp3 n=number of ripetition(-1 always)
int playmusic ( int n)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
	if (SDLmusic!=NULL){
		Mix_PlayMusic(SDLmusic, n);
		return 0;
	}
	else {
		error_description="SDLengine error - playMusic: music not loaded ";
		error_type=1;
		SDLerr(stderr, "SDLengine error - playMusic: music not loaded");
		return -1;
	}
}
//______________________________________________________________________________________________________________________

//positionmusic(p)							: move the execution at p second
int positionmusic(int p)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
        return Mix_SetMusicPosition((double) p);
}
//______________________________________________________________________________________________________________________

//stopmusic								: terminate the music play
int stopmusic()
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
	Mix_HaltMusic();
	return 0;
}
//_______________________________________________________________________________________________________________________

//pausemusic								: set pause of the current music
int pausemusic()
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
        Mix_PauseMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//resumemusic								: unset pause of musica
int resumemusic()
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
        Mix_ResumeMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//rewindmusic								: rewind at start the music
int rewindmusic()
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
       Mix_RewindMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//fademusic(t)								: music fade

int fademusic_t,fademusic_delay;

int fademusic(int t)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}

	if (t!=-1){
	    fademusic_t=(t*1000)/Mix_VolumeMusic(-1);
	    fademusic_delay=timer()+fademusic_t;
	}
	return Mix_VolumeMusic(-1);

}

void fademusichandler()
{
    int t;
    t=fademusic_t;
 	Mix_VolumeMusic(Mix_VolumeMusic(-1)-1);
	fademusic_delay=timer()+t;
	if (Mix_VolumeMusic(-1)<=0){
	    Mix_HaltMusic();
	    fademusic_t=0;
	}
}
//_______________________________________________________________________________________________________________________

//volumemusic(v)							: change the volume of music (0-128) -1 return the current volume
int volumemusic(int v)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
 	return Mix_VolumeMusic(v);

}
//_______________________________________________________________________________________________________________________

//speedmusic(v)								: change the speed of music
int speedmusic(int v)
{
	if (enabledsound==0){
		SDLerr(stderr, "Warning: sounds are not enbled");
		return 0;
	}
    return Mix_SetSynchroValue(v);
}
//_________________________________________________________________________________________________________________________


//CD SUPPORT
//_________________________________________________________________________________________________________________________

//numdrivescd 				: returns the number of cd-rom drives on the system.
int numdrivescd()
{
	return SDL_CDNumDrives();
}
//_________________________________________________________________________________________________________________________

//namecd(drive)				: returns a human-readable, system-dependent identifier for the cd-rom.
int namecd(char name[255],int drive)
{
    char *pname;
    pname =(char *)SDL_CDName(drive);
    strcpy(name,pname);
    if (name==NULL)return -1;
    return 0;
}
//_________________________________________________________________________________________________________________________

//getfreecd				: return first free channel Cd
int getfreecd()
{
    int i;
    for(i=0; i<NUM_CD; i++ )
	if(SDLcd[i]==NULL)return i;
    return -1;
}
//_________________________________________________________________________________________________________________________

//opencd(n,drive)			: opens a cd-rom drive for access.
int opencd(int n, int drive)
{
    if(n==-1)n=getfreecd();
    if(n<0 ||n>NUM_CD)return -1;

    if(SDLcd[n]!=NULL)free(SDLcd[n]);

    SDLcd[n]=SDL_CDOpen(drive);

    if(SDLcd[n]!=NULL)
	return n;
    else
	return -1;
}
//_________________________________________________________________________________________________________________________

//indrivecd(n)					: return 1 if cd is in driver
int indrivecd(int n)
{
    if(n<0 ||n>NUM_CD)return -1;

    if(CD_INDRIVE(SDL_CDStatus(SDLcd[n])))
	return 1;
    else
	return 0;
}
//_________________________________________________________________________________________________________________________

//trackscd(cd) 				: returns the number of tracks in cd.
int trackscd(int n)
{
    if (n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    return SDLcd[n]->numtracks;
}
//_________________________________________________________________________________________________________________________

//curtrackcd(n)				: return the current track in cd
int curtrackcd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    SDL_CDStatus(SDLcd[n]);
    return SDLcd[n]->cur_track;
}
//_________________________________________________________________________________________________________________________

//curframecd(n)				: return the current frame in cd
int curframecd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    SDL_CDStatus(SDLcd[n]);
    return SDLcd[n]->cur_frame;
}
//_________________________________________________________________________________________________________________________

//playcd(n,s,l)				: play a cd
int playcd(int n,int s,int l)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    if (CD_INDRIVE(SDL_CDStatus(SDLcd[n])))
	return SDL_CDPlay(SDLcd[n],s,l);
    return 0;
}
//_________________________________________________________________________________________________________________________

//playtrackscd(n,trk1,fr1,ntrks,nfrs)	: play the given cd track(s)0=all
int playtrackscd(int n,int trk1,int fr1,int ntrks,int nfrs)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;

    if(CD_INDRIVE(SDL_CDStatus(SDLcd[n])))
	return SDL_CDPlayTracks(SDLcd[n], trk1, fr1, ntrks, nfrs);
    return 0;
}
//_________________________________________________________________________________________________________________________

//pausecd(n) 				: pauses a cdrom
int pausecd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;

    return SDL_CDPause(SDLcd[n]);
}

//_________________________________________________________________________________________________________________________

//resumecd(n) 				: resumes a cdrom
int resumecd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;

    return SDL_CDResume(SDLcd[n]);
}

//_________________________________________________________________________________________________________________________

//stopcd(n)				: stops a cdrom
int stopcd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;

    return SDL_CDStop(SDLcd[n]);
}
//_________________________________________________________________________________________________________________________

//ejectcd(n) 				: ejects a cdrom
int ejectcd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;

    return SDL_CDEject(SDLcd[n]);
}
//_________________________________________________________________________________________________________________________

//closecd(n)				: closes a cd handle
int closecd(int n)
{
    if( n<0 ||n>NUM_CD)return -1;
    if (SDLcd[n]==NULL)return -1;

    SDL_CDClose(SDLcd[n]);
    return 0;
}
//_________________________________________________________________________________________________________________________

//tracktypecd(n,t)			: return SDL_AUDIO_TRACK(0...) or SDL_DATA_TRACK(1...)
int tracktypecd(int n,int t)
{
    if( n<0 ||n>NUM_CD)return -1;
    if(SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;
    if (t<0 || t > (SDLcd[n]->numtracks))return -1;
    return SDLcd[n]->track[t].type;
}
//_________________________________________________________________________________________________________________________

//tracklengthcd(n,t)			: return the length of track t
int tracklengthcd(int n,int t)
{
    if( n<0 ||n>NUM_CD)return -1;
    if(SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;
    if (t<0 || t>(SDLcd[n]->numtracks))return -1;
    return SDLcd[n]->track[t].length;
}
//_________________________________________________________________________________________________________________________

//trackoffsetcd(n,t)			: return the offset to the beginning of this track in frames
int trackoffsetcd(int n,int t)
{
    if( n<0 ||n>NUM_CD)return -1;
    if(SDLcd[n]==NULL)return -1;
    if (!CD_INDRIVE(SDL_CDStatus(SDLcd[n])))return -1;
    if (t<0 || t>(SDLcd[n]->numtracks))return -1;
    return SDLcd[n]->track[t].offset;
}
//_________________________________________________________________________________________________________________________

// VIDEO MPEG
//_________________________________________________________________________________________________________________________


void updatempeg(SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h)
{

    if ( screen->flags & SDL_DOUBLEBUF ) {
      SDL_Flip(screen);
    }
}
//_________________________________________________________________________________________________________________________

//loadmpeg(fname,usesound)		: load a mpeg video
int loadmpeg(char *filename,int useaudio)
{
    int use_video;
    SMPEG_Filter *filter;

    use_video = 1;
    mpeg_audio=useaudio;

    if (mpeg_audio==1 )
	Mix_CloseAudio();

    mpeg = SMPEG_new(filename, &info, mpeg_audio);
    if ( SMPEG_error(mpeg) ) {
	fprintf(stderr, "%s: %s\n", filename, SMPEG_error(mpeg));
	SMPEG_delete(mpeg);
	exit(2);
    }

    SMPEG_enableaudio(mpeg, mpeg_audio);
    SMPEG_enablevideo(mpeg, use_video);
    SMPEG_setvolume(mpeg, volumemusic(-1));

    //*
    filter = SMPEGfilter_bilinear();
    filter = SMPEG_filter( mpeg, filter );
    filter->destroy(filter);
    //*/

    return 0;
}

//_________________________________________________________________________________________________________________________

//plaympeg(filename,usesound,optional loop)	: play a mpeg1 video: Warning work only in windows
int plaympeg(int loop)
{
    SMPEG_setdisplay(mpeg, SDLscreen[c_screen], NULL, updatempeg);
    SMPEG_scaleXY(mpeg, SDLscreen[c_screen]->w, SDLscreen[c_screen]->h);
    if ( loop) {
	SMPEG_loop(mpeg, 1);
    }

    SMPEG_play(mpeg);
    return 0;
}
//_________________________________________________________________________________________________________________________

//stopmpeg()					: terminate the video play
int stopmpeg()
{
    SMPEG_stop(mpeg);
    return 0;
}
//_________________________________________________________________________________________________________________________

//deletempeg()				: unload mpeg video
int deletempeg()
{

    SMPEG_delete(mpeg);
    if (mpeg_audio==1){
	if ( Mix_OpenAudio(44100, AUDIO_S16SYS,	2, 8192) < 0 ) {
		enabledsound=0;
	}
	else{
	    Mix_AllocateChannels(16);
	    enabledsound=1;
	}
    }
    return 0;
}
//_________________________________________________________________________________________________________________________

//pausempeg()				: Pause/Resume playback of an SMPEG object
int pausempeg()
{
    SMPEG_pause(mpeg);
    return 0;
}
//_________________________________________________________________________________________________________________________

//rewindmpeg()				: Rewind the play position of an SMPEG object to the beginning of the MPEG
int rewindmpeg()
{
    SMPEG_rewind( mpeg );
    SMPEG_play(mpeg);
    return 0;
}
//_________________________________________________________________________________________________________________________

//seekmpeg(p)				: Seek 'bytes' bytes in the MPEG stream
int seekmpeg(int p)
{
    SMPEG_seek( mpeg, p);
    return 0;
}
//_________________________________________________________________________________________________________________________

//skipmpeg(s)				: Skip 'seconds' seconds in the MPEG stream
int skipmpeg(int s)
{
    SMPEG_skip( mpeg, (float) s );
    return 0;
}
//_________________________________________________________________________________________________________________________

//statusmpeg()				: return 1 if plaympeg work or 0 in other case
int statusmpeg()
{
    SMPEGstatus status;
    status=SMPEG_status( mpeg );
    if (status ==1)return 1;
    return 0;
}
//_________________________________________________________________________________________________________________________

// KEYBOARD
//_________________________________________________________________________________________________________________________

//key(keycode)								: return 1 if is press the key keycode
int key (int keycode)
{
	Uint8 *keystate;

	if (keycode<1 ||keycode>512){
	    error_description="SDLengine error - key: invalid keycode value ";
	    error_type=1;
	    SDLerr(stderr, "SDLengine error- key: %d is an invalid keycode value", keycode);
	    return -1;
	}
	if (autotimer()!=0)return -1;
	getevent();
	keystate = SDL_GetKeyState(NULL);
	if (keystate[keycode])return 1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//inkey									: return ascii code of key press
int inkey()
{
	if (autotimer()!=0)return -1;
	getevent();
	return lastkey;
}
//_______________________________________________________________________________________________________________________

//waitkey(keycode)							: wait a key pression (-1 =any key)
int waitkey(int keycode)
{
	if (keycode==0)
		while(inkey()==0){
		    if (autotimer()!=0)return -1;
		}
	else
		while(inkey()!=keycode){
		    if (autotimer()!=0)return -1;
		}
	return 0;
}
//_______________________________________________________________________________________________________________________

// MOUSE
//_________________________________________________________________________________________________________________________

//xmouse								: return mouse x coordinate on display
int xmouse()
{
	int x,y;

	SDL_GetMouseState(&x, &y);
	if (autotimer()!=0)return -1;
	return x;
}
//_______________________________________________________________________________________________________________________

//ymouse								: return mouse y coordinate on display
int ymouse()
{
	int x,y;

	SDL_GetMouseState(&x, &y);
	if (autotimer()!=0)return -1;
	return y;
}
//_______________________________________________________________________________________________________________________

//xmousescreen(n)							: return mouse x coordinate on screen
int xmousescreen(int n)
{
	int x,y;

	SDL_GetMouseState(&x, &y);
	if (autotimer()!=0)return -1;
	return x+SDLoffx[n]-SDLscreen_rect[n].x;
}
//_______________________________________________________________________________________________________________________

//ymousescreen(n)							: return mouse y coordinate on screen
int ymousescreen(int n)
{
	int x,y;

	SDL_GetMouseState(&x, &y);
	if (autotimer()!=0)return -1;
	return y+SDLoffy[n]-SDLscreen_rect[n].y;
}
//_______________________________________________________________________________________________________________________

//bmouse 								: return the buttonclick on the mouse
int bmouse()
{
	if (autotimer()!=0)return -1;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))return 1;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(2))return 2;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))return 3;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(4))return 4;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(5))return 5;

	return 0;
}
//_______________________________________________________________________________________________________________________


//changemouse(n)							: change mouse from default(0) to emulate with sprite 0 - image 0 (1,2,3)
int changemouse(int n)
{
    SDL_Surface *temp;
    SDL_Color tmpcol;
    int tempback,tempcc;

    if (n==-1)return mousepointer;
    tempback=autoback(-1);
    tempcc=colorkey;
    colorkey=0;
    autoback(0);
    switch(n){
	case 1:
	    SDL_ShowCursor(SDL_DISABLE );
	    if(mousestate==1){
		if (SDLimageCC[0]!=NULL)SDL_FreeSurface(SDLimageCC[0]);
		SDLimageCC[0]=SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 32,32,0,0,0,0));
		/* big hack: how to change an image in screen!!! */
		temp=SDLscreen[c_screen];
		SDLscreen[c_screen]=SDLimageCC[0];
		tmpcol=SDLcol;

		SDL_FillRect(SDLscreen[c_screen],NULL,0);
		ink(0xEEAA00);
		line(0,0,7,0);line(0,0,0,6);line(8,1,8,2);line(1,7,2,7);
		line(3,6,9,12);line(7,3,13,9);line(13,10,11,12);
		dot(10,12);
		ink(0xCC6600);
		line(1,1,11,11);paint(4,1);
		ink(0xEECC88);
		paint(1,4);
		SDL_hsx[0]=1;SDL_hsy[0]=1;

		/*resume hack */
		SDLscreen[c_screen]=temp;
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_LockSurface(SDLscreen[c_screen]);
		Image_colorkey(SDLimageCC[0]);
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_UnlockSurface(SDLscreen[c_screen]);
		SDLcol=tmpcol;

		}
	    mousepointer=1;
	    break;
	case 2:
	    SDL_ShowCursor(SDL_DISABLE );
	    if(mousestate==1){
		if (SDLimageCC[0]!=NULL)SDL_FreeSurface(SDLimageCC[0]);
		SDLimageCC[0]=SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 32,32,0,0,0,0));
		/* big hack: how to change an image in screen!!! */
		temp=SDLscreen[c_screen];
		SDLscreen[c_screen]=SDLimageCC[0];
		tmpcol=SDLcol;

		SDL_FillRect(SDLscreen[c_screen],NULL,0);
		ink(0xEECC88);
		line(7,0,7,3);line(7,11,7,14);line(0,7,3,7);line(11,7,14,7);
		ink(0xCC6600);
		dot(7,1);dot(7,3);dot(7,11);dot(7,13);dot(1,7);dot(3,7);dot(11,7);dot(13,7);
		ink(0xEEAA00);
		dot(6,6);dot(6,8);dot(8,6);dot(8,8);
		SDL_hsx[0]=7;SDL_hsy[0]=7;

		/*resume hack */
		SDLscreen[c_screen]=temp;
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_LockSurface(SDLscreen[c_screen]);
		Image_colorkey(SDLimageCC[0]);
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_UnlockSurface(SDLscreen[c_screen]);

		SDLcol=tmpcol;
		}
	    mousepointer=2;
	    break;
	case 3:
	    SDL_ShowCursor(SDL_DISABLE );
	    if(mousestate==1){
		if (SDLimageCC[0]!=NULL)SDL_FreeSurface(SDLimageCC[0]);
		SDLimageCC[0]=SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 32,32,0,0,0,0));
		/* big hack: how to change an image in screen!!! */
		temp=SDLscreen[c_screen];
		SDLscreen[c_screen]=SDLimageCC[0];
		tmpcol=SDLcol;

		SDL_FillRect(SDLscreen[c_screen],NULL,0);
		ink(0xCC6600);
		bar(0,2,1,14);bar(13,2,14,14);bar(1,1,4,7);bar(10,1,13,7);
		bar(3,0,11,6);
		ink(0xEEAA00);
		bar(3,2,11,4);bar(4,1,10,5);bar(5,0,9,6);line(5,11,7,6);
		ink(0xEECC88);
		bar(4,2,10,4);bar(5,1,9,5);bar(3,12,5,13);line(0,15,14,15);
		ink(0xCC6600);
		line(7,1,7,3);line(8,3,9,4);
		SDL_hsx[0]=7;SDL_hsy[0]=7;


		/*resume hack */
		SDLscreen[c_screen]=temp;
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_LockSurface(SDLscreen[c_screen]);
		Image_colorkey(SDLimageCC[0]);
		if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		    SDL_UnlockSurface(SDLscreen[c_screen]);
		SDLcol=tmpcol;
		}
	    mousepointer=3;
	    break;

	default:
	    if(mousestate==1){
		locatemouse(xmouse(),ymouse());
		deletesprite(0);
		mouseshow();
	    }
	    else{
		mousehide();
		deletesprite(0);
		}
	    mousepointer=0;
	    break;
    }
    colorkey=tempcc;
    autoback(tempback);
    if (autotimer()!=0)return -1;
    return 0;
}
//_______________________________________________________________________________________________________________________

//locatemouse(x,y)							: move mouse at x y coordinates
int locatemouse(int x, int y)
{
   SDL_WarpMouse(x,y);
   return 0;
}
//_______________________________________________________________________________________________________________________

//mouseshow								: show the mouse cursor
int mouseshow()
{
	SDL_ShowCursor(SDL_ENABLE);
	mousestate=1;
	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//mousehide								: hide the mouse cursor
int mousehide()
{
	SDL_ShowCursor(SDL_DISABLE );
	mousestate=0;
	if (autotimer()!=0)return -1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//mousezone(x,y,w,h)							: return 1 if the mouse pointer is in rectangle(xy with size wh)
int mousezone(int x,int y,int w,int h)
{
	if (autotimer()!=0)return -1;
	if ( xmousescreen(c_screen)>x)
		if (xmousescreen(c_screen)<(x+w))
			if(ymousescreen(c_screen)>y)
				if(ymousescreen(c_screen)<y+h)
					return 1;
	return 0;
}
//_________________________________________________________________________________________________________________________

//JOYSTIK
//_________________________________________________________________________________________________________________________

//numjoysticks 								: Count available joysticks.
int numjoysticks()
{
    return SDL_NumJoysticks();
}
//_________________________________________________________________________________________________________________________

//namejoystick(index)							: Get joystick name.
char *namejoystick(int index)
{
    return (char *)SDL_JoystickName(index);
}
//_________________________________________________________________________________________________________________________

//numaxesjoystick 							: Get the number of joystick axes
int numaxesjoystick(int index)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickNumAxes(SDLjoy);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;
}
//_________________________________________________________________________________________________________________________

//numballsjoystick(i) 							: Get the number of joystick trackballs
int numballsjoystick(int index)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickNumBalls(SDLjoy);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;

}
//_________________________________________________________________________________________________________________________

//numhatsjoystick(i) 							: Get the number of joystick hats
int numhatsjoystick(int index)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickNumHats(SDLjoy);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;
}
//_________________________________________________________________________________________________________________________

//numbuttonsjoystick(i) 						: Get the number of joysitck buttons
int numbuttonsjoystick(int index)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickNumButtons(SDLjoy);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;
}
//_________________________________________________________________________________________________________________________

//getaxisjoystick(i,a) 							: Get the current state of an axis
int getaxisjoystick(int index,int axis)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickGetAxis(SDLjoy,axis);
    else
	ret=0;
    SDL_JoystickClose(SDLjoy);
    return ret;
}
//_________________________________________________________________________________________________________________________

//gethatjoystick(i,a) 							: Get the current state of a joystick hat
int gethatjoystick(int index,int hat)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickGetHat(SDLjoy,hat);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;
}
//_________________________________________________________________________________________________________________________

//getbuttonjoystick(i,a)						: Get the current state of a given button on a given joystick
int getbuttonjoystick(int index,int button)
{
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickGetButton(SDLjoy,button);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    return ret;
}

//_________________________________________________________________________________________________________________________

//xgetballjoystick(i,a) 							: Get relative x trackball motion
int xgetballjoystick(int index,int ball)
{
    int x,y;
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickGetBall(SDLjoy,ball,&x,&y);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    if (ret!=-1)
	return x;
    else
	return ret;
}

//_________________________________________________________________________________________________________________________

//ygetballjoystick(i,a) 							: Get relative y trackball motion
int ygetballjoystick(int index,int ball)
{
    int x,y;
    int ret;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    if (SDLjoy)
	ret= SDL_JoystickGetBall(SDLjoy,ball,&x,&y);
    else
	ret=-1;
    SDL_JoystickClose(SDLjoy);
    if (ret!=-1)
	return y;
    else
	return ret;
}

//_________________________________________________________________________________________________________________________

//joy									: return joystick boolean coordinate
int joy(int index)
{
    int ret,tmp;
    if (index>=SDL_NumJoysticks())return -1;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    ret=0;
    if (SDLjoy && SDL_JoystickNumAxes(SDLjoy)>=2 ){
	tmp= SDL_JoystickGetAxis(SDLjoy,0);
	if (tmp<0)
	    ret+=1;
	if (tmp>0)
	    ret+=2;
	tmp= SDL_JoystickGetAxis(SDLjoy,1);
	if (tmp<0)
	    ret+=4;
	if (tmp>0)
	    ret+=8;
    }
    if (SDLjoy && SDL_JoystickNumHats(SDLjoy)>0 ){
	ret+=SDL_JoystickGetHat(SDLjoy,0);
    }
    SDL_JoystickClose(SDLjoy);
    if (autotimer()!=0)return -1;
    return ret;
}
//_________________________________________________________________________________________________________________________

//bjoy									: return joystick buttons pressed in boolean expression
int bjoy(int index)
{
    int ret,i,a;
    if (index>=SDL_NumJoysticks())return -1;
    ret=0;
    a=1;
    SDLjoy=SDL_JoystickOpen(index);
    SDL_JoystickUpdate();
    for(i=0;i<SDL_JoystickNumButtons(SDLjoy);i++){
	if(SDL_JoystickGetButton(SDLjoy,i))ret+=a;
	a*=2;
    }
    SDL_JoystickClose(SDLjoy);
    if (autotimer()!=0)return -1;
    return ret;
}
//_________________________________________________________________________________________________________________________

// TIME
//_________________________________________________________________________________________________________________________

//waitTime(t)								: wait t milliseconds
int waitTime(int t)
{
	 Uint32 ticks;
	ticks=SDL_GetTicks()+t;
	while(ticks>SDL_GetTicks()){
		getevent();
		if (stopkey()==-1)return -1;
	if (autotimer()!=0)return -1;
	}
	return 0;
}
//_________________________________________________________________________________________________________________________

//timer									: return the current tick
int timer()
{
	 Uint32 ticks;
	ticks=SDL_GetTicks();
	return ticks;
}
//_________________________________________________________________________________________________________________________

// SOCKET
//_________________________________________________________________________________________________________________________

//isenabledsock()							: return 1 if sdlnet was enabled
int isenabledsock()
{
    return enabledsock;
}
//_________________________________________________________________________________________________________________________

// getfreesock								: return the first free sock in sdlSocket array
int getfreesock()
{
    int i;
    for (i=1;i<NUM_SOCKS;i++)
	if (SDLsock[i]==NULL)return i;
    return -1;
}
//_________________________________________________________________________________________________________________________

/*OpenSock (sock,port) 							: Server side socket sock is the stream. in sdlBasic \
									  there are 256 stream and sintax is very similar of \
									  file open close. */
int opensock(int port)
{
    int sock;
    IPaddress my_ip;

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&my_ip,INADDR_ANY,port)==-1)
	{
	    error_description="SDLengine error - openSock: SDLNet_ResolveHost  ";
	    error_type=1;
		SDLerr(stderr,"SDLengine error - openSock: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return -1;
	}

    sock=getfreesock();
    SDLsock[sock] = SDLNet_TCP_Open( &my_ip);

    if (SDLsock[sock]==NULL)return 0;
    return sock;
}
//_________________________________________________________________________________________________________________________

//AcceptSock(serversockk)							: Accept the client connection
int acceptsock(int serversock)
{
    int clientsock;
    clientsock=getfreesock();
    SDLsock[clientsock]=SDLNet_TCP_Accept(SDLsock[serversock]);
    if(SDLsock[clientsock]!=NULL)return clientsock;
    return 0;
}


//_________________________________________________________________________________________________________________________

//IsServerReady(Sock)							: True/False if server is sending data    

int isserverready(int sock)
{
    int ret;
    SDLNet_SocketSet set;
    set=SDLNet_AllocSocketSet(1);

    if (SDLNet_TCP_AddSocket(set, SDLsock[sock])==-1){
	closesock(sock);
	return -1;
    }

    if (SDLNet_CheckSockets(set, 1)==-1){
	closesock(sock);
	return -1;
    }
    ret= SDLNet_SocketReady(SDLsock[sock]);
    SDLNet_TCP_DelSocket(set, SDLsock[sock]);
    return ret;
}


//_________________________________________________________________________________________________________________________

//ConnectSock(sock,ServerName,port)					: client side socket connection
int connectsock(char *servername,int port)
{
    int sock;
    IPaddress my_ip;

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&my_ip,servername,port)==-1)
	{
	    error_description="SDLengine error - connectSock: SDLNet_ResolveHost ";
	    error_type=1;
	    SDLerr(stderr,"SDLengine error - connectSock: SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		return -1;
	}

    sock=getfreesock();
    SDLsock[sock] = SDLNet_TCP_Open( &my_ip);

    if (SDLsock[sock]==NULL)return 0;
    return sock;
}


//_________________________________________________________________________________________________________________________

//ConnectionReadySock(sock)						: the server have accepted the connection

//_________________________________________________________________________________________________________________________

//IsClientReady(Sock)							: True/False if client is sending data
int isclientready(int sock)
{
    int ret;
    SDLNet_SocketSet set;
    set=SDLNet_AllocSocketSet(1);

    if (SDLNet_TCP_AddSocket(set, SDLsock[sock])==-1){
	closesock(sock);
	return -1;
    }

    if (SDLNet_CheckSockets(set, 1)==-1){
	closesock(sock);
	return -1;
    }
    ret= SDLNet_SocketReady(SDLsock[sock]);
    SDLNet_TCP_DelSocket(set, SDLsock[sock]);
    return ret;
}

//_________________________________________________________________________________________________________________________

//CloseSock(sock)							: Close the socket connection. Works for client and server
int closesock(int sock)
{
    SDLNet_TCP_Close(SDLsock[sock]);
    SDLsock[sock]=NULL;
    return 0;
}
//_________________________________________________________________________________________________________________________

//PeekSock(Sock, NumBytes)						: Peek information coming from socket, but do not clear.

//_________________________________________________________________________________________________________________________

//ReadSock(Sock, NumBytes)						: Read NumBytes
char *readsock(int sock,int numbytes)
{
    int len;
    char *message;
    message=(char *)malloc(numbytes);
    len=SDLNet_TCP_Recv(SDLsock[sock],message,numbytes);
    if (len<0){
	closesock(sock);
	return NULL;
    }
    return message;
}
//_________________________________________________________________________________________________________________________

//ReadByteSock(Sock)							: Read a single byte      
//char readbytesock(int sock)
int readbytesock(int sock)
{
    int len;
    char byte;
    len=SDLNet_TCP_Recv(SDLsock[sock],&byte,1);
    if (len<0){
	closesock(sock);
	return -1;
    }
    return byte;
}
//_________________________________________________________________________________________________________________________

//ReadLineSock(Sock)							: Read complete line
char *readlinesock(int sock)
{
//    char c;
    char c2;
    int c = 0;
    char *message;
    message=(char *)malloc(1024);
    strcpy(message,"");
    while(c!='\13'){
	c=readbytesock(sock)&255;
	if (c<0){
	    closesock(sock);
	    return NULL;
	}
//	strncat(message,&c,1);
   c2 = c;
   strncat(message, &c2, 1);
	if(c==10)break;
	if(c==13)break;
    }
    strncat(message,"\0",1);
    return message;
}
//_________________________________________________________________________________________________________________________

//WriteSock(Sock, data,len)						: Sends NumBytes from Message
int writesock(int sock,char *data,int len)
{
    int result;
    result=SDLNet_TCP_Send(SDLsock[sock],data,len);
    if (result<0){
	closesock(sock);
	return -1;
    }
    return result;
}
//_________________________________________________________________________________________________________________________

//WriteByteSock(Sock, Byte)						: Sends a single byte
int writebytesock(int sock,char byte)
{
    int result;
    result=SDLNet_TCP_Send(SDLsock[sock],&byte,1);
    if (result<0){
	closesock(sock);
	return -1;
    }
    return result;
}
//_________________________________________________________________________________________________________________________

//WriteLineSock(Sock, Message)						: Send Message
int writelinesock(int sock,char *message)
{
    int result,len;
    len=strlen(message);
    result=SDLNet_TCP_Send(SDLsock[sock],message,len);
    if (result<0){
	closesock(sock);
	return -1;
    }
    writebytesock(sock,13);
    return result;
}
//_________________________________________________________________________________________________________________________

//getremoteip(sock)							: return the ip address of remote client connetted
char *getremoteip(sock)
{
    Uint32 ipaddr;
    IPaddress *remoteip;
    char *ret;

    ret=(char *)malloc(24);
    // get the clients IP and port number
    remoteip=SDLNet_TCP_GetPeerAddress(SDLsock[sock]);
    if(!remoteip)
    {
	error_description="SDLengine error - getRemoteIp: SDLNet_TCP_GetPeerAddress ";
	error_type=1;
	SDLerr(stderr,"SDLengine error - getRemoteIp: SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
	return NULL;
    }

    // print out the clients IP and port number
    ipaddr=SDL_SwapBE32(remoteip->host);
    sprintf(ret,"%d.%d.%d.%d\n",
			ipaddr>>24,
			(ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,
			ipaddr&0xff);
    return ret;
}
//_________________________________________________________________________________________________________________________

//getremoteport(sock)							: return the ip address of remote client connetted
int getremoteport(sock)
{
    Uint16 port;
    IPaddress *remoteip;

    // get the clients IP and port number
    remoteip=SDLNet_TCP_GetPeerAddress(SDLsock[sock]);
    if(!remoteip)
    {
	error_description="SDLengine error - getRemotePort: SDLNet_TCP_GetPeerAddress ";
	error_type=1;
	SDLerr(stderr,"SDLengine error - getRemotePort: SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
	return -1;
    }

    // print out the clients IP and port number
    port=SDL_SwapBE32(remoteip->port);
    return port;
}
//_________________________________________________________________________________________________________________________

//getlocalip()								: return the local ip
#ifndef WIN32 // has no gethostname that we can use here...
    int gethostname(char *name,int number);
#endif

char *getlocalip()
{
#ifndef WIN32 // has no gethostname that we can use here...

    IPaddress ip;
    Uint32 ipaddr;
    //Uint16 port;
    char *localhostname;
    char *ret;

    ret=(char *)malloc(24);
    localhostname=(char *)malloc(256);
    if((gethostname(localhostname, 256)>=0))
	{
	if(SDLNet_ResolveHost(&ip,localhostname,0)==-1)
	    {
		error_description="SDLengine error getLocalIp: Can't resolve host ";
		error_type=1;
		SDLerr(stderr,"SDLengine error getLocalIp: Can't resolve host \"%s\"\n%s\n", localhostname, SDLNet_GetError());
		return NULL;
	    }
	    else
	    {
		// use the IP as a Uint8[4]
		ipaddr= ip.host;

		// print out the clients IP
		sprintf(ret,"%d.%d.%d.%d\n",
			ipaddr>>24,
			(ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,
			ipaddr&0xff);

	    }
	}
    return ret;
#else
    return "localhost";
#endif
}
//_________________________________________________________________________________________________________________________










