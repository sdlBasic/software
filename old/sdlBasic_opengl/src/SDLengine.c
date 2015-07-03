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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "SDLengine.h"



//error print
#define SDLerr fprintf

SDL_Surface *SDLdisplay;
int GL_display;


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
SDL_Surface *SDLscreen[NUM_SCREEN];
int Palette[256];



SDL_Rect SDLscreen_rect[NUM_SCREEN];
int SDLscreen_flag[NUM_SCREEN];
int SDLscreen_live[NUM_SCREEN];
int SDLoffx[NUM_SCREEN];
int SDLoffy[NUM_SCREEN];
int lastscreen;


SDL_Color SDLcol;

int SDL_hsx[NUM_IMAGES];
int SDL_hsy[NUM_IMAGES];

SDL_Surface *SDLimage[NUM_IMAGES];
SDL_Surface *SDLimageCC[NUM_IMAGES];


int SDLspritex[NUM_SPRITES];
int SDLspritey[NUM_SPRITES];
int SDLspritefr[NUM_SPRITES];
int SDLspritelive[NUM_SPRITES];

SDL_Rect SDLspriteclip;
int lastsprite;

int SDLbobscreen[NUM_BOBS];
int SDLbobx[NUM_BOBS];
int SDLboby[NUM_BOBS];
int SDLbobxold[NUM_BOBS];
int SDLbobyold[NUM_BOBS];
int SDLbobfr[NUM_BOBS];
int SDLboblive[NUM_BOBS];
SDL_Surface *SDLbobback[NUM_BOBS];
int lastbob;



Mix_Chunk *SDLsound[NUM_WAVES];
Mix_Music *SDLmusic;

TTF_Font *font;
char *fontPath;
SDL_Surface *SDLtext;


TTF_Font *font_mono;
char *fontPathMono;

SDL_Surface *SDLfont_mono[255];
int stx[NUM_SCREEN];
int sty[NUM_SCREEN];
int pen_color;
int paper_color;

int oldstx[NUM_SCREEN];
int oldsty[NUM_SCREEN];
SDL_Surface *SDLcursore[16];

int cursore_state[NUM_SCREEN];
int cursore_anim[NUM_SCREEN];

int lastkey;

//________________________________________________________________________________________________________________________________________

// common internal rutines
//________________________________________________________________________________________________________________________________________


int getevent()
{
	SDL_Event event;

	while ( SDL_PollEvent(&event) )
		{
		switch(event.type){
		  case SDL_QUIT:
			terminate();
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

//multitread autoback callback function: not used at this time
SDL_TimerCallback autoback_callback()
{
	if( SDLautoback	== 0)
		SDL_SetTimer(0, NULL);
	screenswap();

	return (SDL_TimerCallback)(Uint32)(SDLautobacktime);
}
//________________________________________________________________________________________________________________________________________

// collaborative automatic screenswap
void autoback_timer()
{
	if (timer()>SDLautobacktime){
		screenswap();
		SDLautobacktime=timer()+SDLautoback;
	}
}
//________________________________________________________________________________________________________________________________________

/* see if a file exists */
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

SDL_Surface *Load_Image(char *datafile, int transparent)
{
    int bpp;
	SDL_Surface *image, *surface;

	image = IMG_Load(datafile);
	if ( image == NULL ) {
		SDLerr(stderr, "Couldn't load image %s: %s\n",datafile, IMG_GetError());

		return(NULL);
	}
	if ( transparent ) {

		bpp = image->format->BytesPerPixel;
		switch(bpp) {
		    case 1:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint8 *)image->pixels);
			break;
		    case 2:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint16 *)image->pixels);
			break;
		    case 3:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint32 *)image->pixels);
			break;
		    case 4:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint32 *)image->pixels);
			break;
		    }

	}
	surface = SDL_DisplayFormat(image);
	SDL_FreeSurface(image);
	return(surface);
}
//________________________________________________________________________________________________________________________________________

int Save_Image(char *datafile, SDL_Surface *image)
{
	if ( SDL_SaveBMP( image , datafile) ) {
		SDLerr(stderr, "Couldn't save image %s: %s\n",datafile, IMG_GetError());
		return 1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

 void Image_colorkey(SDL_Surface *image)
{
	 int bpp;
	if (SDL_MUSTLOCK(image))
		SDL_LockSurface(image);


		bpp = image->format->BytesPerPixel;
		switch(bpp) {
		    case 1:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint8 *)image->pixels);
			break;
		    case 2:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint16 *)image->pixels);
			break;
		    case 3:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint32 *)image->pixels);
			break;
		    case 4:
			SDL_SetColorKey(image, (SDL_SRCCOLORKEY|SDL_RLEACCEL),*(Uint32 *)image->pixels);
			break;
		    }

	if (SDL_MUSTLOCK(image))
		SDL_UnlockSurface(image);


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
	}
      else
	{
	  /* Target surface is 8bit */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0,
				  0, 0);
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
	}
      else
	{
	  /* Target surface is 8bit */
	  rz_dst =
	    SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0,
				  0, 0);
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
  dstwidth = (int) ((double) rz_src->w * zoomx);
  dstheight = (int) ((double) rz_src->h * zoomy);
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
    }

  /* Return destination surface */
  return (rz_dst);
}
//________________________________________________________________________________________________________________________________________

/*
	This codei was extract from fade tutorial written by

*/
/* Fades the given surface in or out to the given screen within the given time
 If the image surface is the screen surface (pointer are equal), a copy is made
 first. We must do that because we are overwriting the Screen Surface. */

void fade (SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img, Uint32 ui_seconds, int b_fade_out) {
  /* Becomes the black surface */
  SDL_Surface* p_surf_black = NULL ;
  /* Used when the Screen Surface equals the Image Surface */
  SDL_Surface* p_surf_screen_copy = NULL ;
  /* Used to calculate the steps to make a fade in the given time: */
  Uint32 ui_old_time, ui_curr_time, ui_time_ms ;
  float f_alpha ;
  /* Becomes flag to pass when creating a Surface */
  Uint32 ui_flags = SDL_SRCALPHA ;
  /* Create the black surface in the format of the given screen */
  if ((p_surf_screen->flags & SDL_HWSURFACE)) {
    ui_flags |= SDL_HWSURFACE ;
  }
  else {
    ui_flags |= SDL_SWSURFACE ;
  }
  if ((p_surf_black = SDL_CreateRGBSurface (ui_flags,
    p_surf_screen->w, p_surf_screen->h,
    p_surf_screen->format->BitsPerPixel,
    p_surf_screen->format->Rmask, p_surf_screen->format->Gmask, p_surf_screen->format->Bmask,
    p_surf_screen->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
      fprintf (stderr, "fade: could not create the black Surface. (%s)\n", SDL_GetError ()) ;
      return ;
  }
  /* Fill the Surface with black */
  SDL_FillRect (p_surf_black, NULL, SDL_MapRGB (p_surf_screen->format, 0, 0, 0)) ;
  /* Should we make a copy of the Screen? */
  if (p_surf_screen == p_surf_img) {
    if ((p_surf_screen_copy = SDL_CreateRGBSurface (ui_flags,
    p_surf_screen->w, p_surf_screen->h,
    p_surf_screen->format->BitsPerPixel,
    p_surf_screen->format->Rmask, p_surf_screen->format->Gmask, p_surf_screen->format->Bmask,
    p_surf_screen->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
      fprintf (stderr, "fade: could not create a copy of the Screen Surface. (%s)\n", SDL_GetError ()) ;
      SDL_FreeSurface (p_surf_black) ;
      return ;
    }
    /* Ok, copy surfaces and set surfact pointer */
    SDL_BlitSurface (p_surf_screen, NULL, p_surf_screen_copy, NULL) ;
    p_surf_img = p_surf_screen_copy ;
  }
  /* Ok, we are now ready for the fade. These are the steps (looped):
  1. Draw p_surf_img onto p_surf_screen, just an ordinary blit.
  2. Decrease or increase (depends on fading in or out) the alpha value,
    based on the elapsed time since the previous loop-iteration.
  3. Draw p_surf_black onto p_surf_screen in the current alpha value.*/
  ui_old_time = SDL_GetTicks () ;
  ui_curr_time = ui_old_time ;
  /* Convert the given time in seconds into miliseconds. */
  ui_time_ms = ui_seconds * 1000 ;
  if (b_fade_out) {
    f_alpha = 0.0 ;
    /* Loop until the alpha value exceeds 255 (this is the maximum alpha value) */
    while (f_alpha < 255.0) {
      /* Draw the image onto the screen */
      SDL_BlitSurface (p_surf_img, NULL, p_surf_screen, NULL) ;
      /* Draw the black surface onto the screen */
      SDL_SetAlpha (p_surf_black, SDL_SRCALPHA, (Uint8) f_alpha) ;
      SDL_BlitSurface (p_surf_black, NULL, p_surf_screen, NULL) ;
      /* Update the timer variables */
      ui_old_time = ui_curr_time ;
      ui_curr_time = SDL_GetTicks () ;
      /* Flip the screen Surface */
      SDL_Flip (p_surf_screen) ;
      /* Calculate the next alpha value */
      f_alpha += 255 * ((float) (ui_curr_time - ui_old_time) / ui_time_ms) ;
	screenswap();
    }
  }
  else {
    f_alpha = 255.0 ;
    /* Loop until the alpha value exceeds 255 */
    while (f_alpha > 0.0) {
      /* Draw the image onto the screen */
      SDL_BlitSurface (p_surf_img, NULL, p_surf_screen, NULL) ;
      /* Draw the black surface onto the screen */
      SDL_SetAlpha (p_surf_black, SDL_SRCALPHA, (Uint8) f_alpha) ;
      SDL_BlitSurface (p_surf_black, NULL, p_surf_screen, NULL) ;
      /* Update the timer variables */
      ui_old_time = ui_curr_time ;
      ui_curr_time = SDL_GetTicks () ;
      /* Flip the screen Surface */
      SDL_Flip (p_surf_screen) ;
      /* Calculate the next alpha value */
      f_alpha -= 255 * ((float) (ui_curr_time - ui_old_time) / ui_time_ms) ;
	screenswap();
    }
  }
  /* Free the black Surface */
  SDL_FreeSurface (p_surf_black) ;
  /* Free the Screen copy, if used */
  if (p_surf_screen_copy != NULL) {
    SDL_FreeSurface (p_surf_screen_copy) ;
  }
}
//________________________________________________________________________________________________________________________________________

/* Cross-Fades the given surfaces onto the given screen within the given time
 If the image surface is the screen surface (pointer are equal), a copy is made
 first. We must do that because we are overwriting the Screen Surface. */

void crossfade (SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img1, SDL_Surface* p_surf_img2, Uint32 ui_seconds) {
  /* Used to calculate the steps to make a fade in the given time: */
  Uint32 ui_old_time, ui_curr_time, ui_time_ms ;
  float f_alpha ;
  /* These are the steps to perform a cross-fade (looped):
  1. Draw p_surf_img1 onto p_surf_screen, just an ordinary blit.
  2. Increase the alpha value, based on the elapsed time since the previous loop-iteration.
  3. Draw p_surf_img2 onto p_surf_screen in the current alpha value.*/
  ui_old_time = SDL_GetTicks () ;
  ui_curr_time = ui_old_time ;
  /* Convert the given time in seconds into miliseconds. */
  ui_time_ms = ui_seconds * 1000 ;
  f_alpha = 0.0 ;
  /* Loop until the alpha value exceeds 255 (this is the maximum alpha value) */
  while (f_alpha < 255.0) {
    /* Draw the image onto the screen */
    SDL_BlitSurface (p_surf_img1, NULL, p_surf_screen, NULL) ;
    /* Draw the black surface onto the screen */
    SDL_SetAlpha (p_surf_img2, SDL_SRCALPHA, (Uint8) f_alpha) ;
    SDL_BlitSurface (p_surf_img2, NULL, p_surf_screen, NULL) ;
    /* Update the timer variables */
    ui_old_time = ui_curr_time ;
    ui_curr_time = SDL_GetTicks () ;
    /* Flip the screen Surface */
    SDL_Flip (p_surf_screen) ;
    /* Calculate the next alpha value */
    f_alpha += 255 * ((float) (ui_curr_time - ui_old_time) / ui_time_ms) ;
	screenswap();
  }
}
//________________________________________________________________________________________________________________________________________

// hit_pixelperfect(SDLbobfr[n],xa1,ya1,xa2,ya2,SDLbobfr[x],xb1,yb1,xb2,yb2);
int hit_pixelperfect(SDL_Surface *imga,int xa1,int ya1,int xa2,int ya2,SDL_Surface *imgb,int xb1,int yb1,int xb2,int yb2)
{
    int xaa,yaa,xbb,ybb;
    int xw,yh;
    int x,y;

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

    for (y=0; y<yh; y++){
	for (x=0; x<xw; x++){
	    if (getpixel(imga,xaa+x,yaa+y)!=0){
		if(getpixel(imgb,xbb+x,ybb+y)!=0){
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
    if (x>0 && y>0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x ;
	return *(Uint8 *)p;
    }
    else
	return 0;
}

Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 2;
	return *(Uint16 *)p;
    }
    else
	return 0;
}

Uint32 getpixel24BE(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 3;
	return p[0] << 16 | p[1] << 8 | p[2];
    }
    else
	return 0;
}

Uint32 getpixel24(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
	p=(Uint8 *)surface->pixels + y * surface->pitch + x * 3;
	return p[0] | p[1] << 8 | p[2] << 16;
    }
    else
	return 0;
}

Uint32 getpixel32(SDL_Surface *surface, int x, int y)
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
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
	if (x>0 && y>0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x ;
		*p= col;
	}
}

void putpixel16(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
	if (x>0 && y>0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x * 2;
		*(Uint16 *)p=col;
	}
}

void putpixel24BE(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
		p=(Uint8 *) surface->pixels + (y * surface->pitch) + x * 3;
		p[0] = (col >> 16) & 0xff;
		p[1] = (col >> 8) & 0xff;
		p[2] = col & 0xff;
    }

}

void putpixel24(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
    if (x>0 && y>0 && x<surface->w && y<surface->h){
		p=(Uint8 *)surface->pixels + (y * surface->pitch) + x * 3;
		p[0] = col & 0xff;
		p[1] = (col >> 8) & 0xff;
		p[2] = (col >> 16) & 0xff;
    }

}

void putpixel32(SDL_Surface *surface, int x, int y, Uint32 col )
{
    Uint8 *p;
	if (x>0 && y>0 && x<surface->w && y<surface->h){
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
    float slope;

    int dx = x2 - x1;  /* change in x */
    int dy = y2 - y1;  /* change in y */

    slope = (float)dy / dx;

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

void draw_box(SDL_Surface *surface,int x1,int y1,int x2,int y2,Uint32 color)
{
int i;
for(i=x1;i<x2;i++){putpixel(surface,i,y1,color);putpixel(surface,i,y2,color);}
for(i=y1;i<y2;i++){putpixel(surface,x1,i,color);putpixel(surface,x2,i,color);}
}
//________________________________________________________________________________________________________________________________________

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

// xrgb per uso interno

Uint32 xrgb8(int colR,int colG,int colB)
{
    return (colB/64) + ((colG/64) * 8)+((colR/64)*8*8);
}

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

// rrgb per uso interno

Uint32 rrgb8(int col)
{
    int colR,colG,colB;
    colB = (col & 3)*64;
    colG =((col / 8) & 3)*64 ;
    colR =((col / (8*8)) & 3)*64;
    return colB + (colG * 256)+(colR*256*256);
}

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

//create font crea il font monospace  setta a 0,0 le coordinate del cursore a nero colore sfondo e bianco fore color per uso interno
int createfont()
{
	int i;
	paper(0);
	pen(rgb(255,255,255));
	for (i=0;i<NUM_SCREEN;i++){
		stx[c_screen]=0;
		sty[c_screen]=0;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

//stamp un carattere alle coordinate correnti per uso interno
int put_char(unsigned char n)
{

	SDL_Rect s;
	SDL_Rect d;
	SDL_Color oldc;



	oldc=SDLcol;
	if (n==13){
	    stx[c_screen]=0;
	    sty[c_screen]=sty[c_screen]+1;
		n=0;
	}
    else
	{
		ink (paper_color);
		bar(stx[c_screen]*XPRINT_SIZE,2+sty[c_screen]*PRINT_SIZE,stx[c_screen]*XPRINT_SIZE+XPRINT_SIZE,3+sty[c_screen]*PRINT_SIZE+PRINT_SIZE);



		if (SDLfont_mono[n]!=NULL){

			s.x=0;
			s.y=0;
			s.w=XPRINT_SIZE-1;
			s.h=SDLfont_mono[n]->h;

			d.x=stx[c_screen]*XPRINT_SIZE;
			d.y=sty[c_screen]*PRINT_SIZE;
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
	if (sty[c_screen]+1>(screenheight()/(PRINT_SIZE))){
		if (cursore_state[c_screen]==1){
			cursoff();
			screenswap();
			screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
			ink(paper_color);
			bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

			sty[c_screen]=sty[c_screen]-1;
			screenswap();
			curson();
		}
		else{
			screenswap();
			screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
			ink(paper_color);
			bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

			sty[c_screen]=sty[c_screen]-1;
			screenswap();
		}
	}

	SDLcol=oldc;
	return 0;
}
//________________________________________________________________________________________________________________________________________

int creacursore()
{
	int i;

	font_mono = TTF_OpenFont(fontPathMono,PRINT_SIZE);
	if (font_mono ==0) {
		SDLerr(stderr,"SDLengine:Couldn't open true font monospace: verify installation!!!  %s\n",SDL_GetError());
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

// this is my very old rutines for input used in 1995 on borland tc turbovision replacement project adapted on SDLengine
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
  cursoff();
  locate(x,y);fprints(dispstring);
  curson();
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
    locate(x,y);
    for (i=0;i<displen;i++){
	    locate(x+i,y);
	    fprints(" ");
    }
     i=0;
    cursoff();
    locate(x,y);
    fprints(dispstring);
    curson();
    screenswap();
  }
  while (n !=13);
  strcpy (instring,provstring);
}
//_______________________________________________________________________________________________________________________


//==========================================================================================================================================
//							Inizializzazione e chiusura sdlLibrary
//==========================================================================================================================================

//________________________________________________________________________________________________________________________________________

void initialize()
{
    int i;
	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
		SDLerr(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);

	/* Open the audio device */
	/*
	 parametri
	11025 22050 44100
	AUDIO_U8 AUDIO_S16SYS
	1 2
	512 8192
	*/
	if ( Mix_OpenAudio(44100, AUDIO_S16SYS,	2, 8192) < 0 ) {
		SDLerr(stderr,"Warning: Couldn't set 44100 Hz 16-bit stereo audio\n- Reason: %s\n",SDL_GetError());
	}
	Mix_AllocateChannels(16);

	/* initialize the true font system */
#if UNIX
	fontPath="/usr/share/fonts/ttf/vera/Vera.ttf";
	fontPathMono="/usr/share/fonts/ttf/vera/VeraMono.ttf";
#else

	//fontPath="c:\\windows\\fonts\\Vera.ttf";
	fontPath=(char *)malloc(256);
	strcpy(fontPath,getenv("WINDIR"));
	strcat(fontPath,"\\fonts\\Vera.ttf\0");

//	fontPathMono ="c:\\windows\\fonts\\VeraMono.ttf";
	fontPathMono=(char *)malloc(256);
	strcpy(fontPathMono,getenv("WINDIR"));
	strcat(fontPathMono,"\\fonts\\VeraMono.ttf\0");
#endif
	if ( TTF_Init() != 0 ) {
		SDLerr(stderr,"Warning: Couldn't initialize true fonf system: %s\n",SDL_GetError());
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
	for (i=0;i<NUM_SCREEN;i++){
		SDLscreen[i]=NULL;
		SDLscreen_rect[i].x=0;
		SDLscreen_rect[i].y=0;
		SDLscreen_rect[i].w=0;
		SDLscreen_rect[i].h=0;
		SDLscreen_flag[i]=0;
		SDLscreen_live[i]=0;
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


	/*default settings*/
	setdisplay(640,480,0,1);
	autoback(25);
	autoupdatesprite(1);
	autoupdatebob(1);
	dualplayfield(0);
	cursoff();
	getevent();

}
//________________________________________________________________________________________________________________________________________

void terminate()
{
	TTF_Quit();
	Mix_CloseAudio();
	atexit(SDL_Quit);
	exit(0);
}
//_________________________________________________________________________________________________________________________


//==========================================================================================================================================
//							COMANDI
//==========================================================================================================================================

//_________________________________________________________________________________________________________________________

//SCHERMO
//_________________________________________________________________________________________________________________________

//set display  w,h,0=screen|1=window 									: apre lo schermo/finestra in double buffer
int setdisplay(int w,int h, int bpp, int mode)
{
	GL_display=0;
	switch (mode){
	   case 0:
		/* Open the display device full screen */
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF|SDL_ANYFORMAT );//|SDL_ASYNCBLIT
		if ( SDLdisplay == NULL ) {
			SDLerr(stderr, "SDLengine:Couldn't set the video mode selected: %s\n",SDL_GetError());
			return -1;
		}
		break;

	   case 1:
		/* Open the display device window */
#if UNIX
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_ANYFORMAT);//|SDL_ASYNCBLIT|SDL_DOUBLEBUF
#else
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT);//|SDL_ASYNCBLIT
#endif
		SDL_WM_SetCaption("sdlBasic",NULL );
		if ( SDLdisplay == NULL ) {
			SDLerr(stderr, "SDLengine:Couldn't set video mode selected: %s\n",SDL_GetError());
			return -1;

		}
		break;

	  case 2:
		/* Open the display device window */
#if UNIX
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_RESIZABLE|SDL_ANYFORMAT);//|SDL_ASYNCBLIT|SDL_DOUBLEBUF
#else
		SDLdisplay = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF|SDL_ANYFORMAT);//|SDL_ASYNCBLIT
#endif
		SDL_WM_SetCaption("sdlBasic",NULL );
		if ( SDLdisplay == NULL ) {
			SDLerr(stderr, "SDLengine:Couldn't set video mode selected: %s\n",SDL_GetError());
			return -1;

		}
		break;

	default:
		SDLerr(stderr, "SDLengine:video mode selected not  valid: %d\n",mode);
		return -1;
		break;
	}

	SDL_FillRect(SDLdisplay,NULL,0);

	switch (SDLdisplay->format->BytesPerPixel){
	    case 1:
			getpixel=getpixel8;
			putpixel=putpixel8;
			xrgb=xrgb8;
			rrgb=rrgb8;
			break;
	    case 2:
			getpixel=getpixel16;
			putpixel=putpixel16;
			xrgb=xrgb16;
			rrgb=rrgb16;
			break;
	    case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
				getpixel=getpixel24BE;
				putpixel=putpixel24BE;
				xrgb=xrgb24;
				rrgb=rrgb24;
			}
			else{
				getpixel=getpixel24;
				putpixel=putpixel24;
				xrgb=xrgb24;
				rrgb=rrgb24;
			}
			break;
	    case 4:
			getpixel=getpixel32;
			putpixel=putpixel32;
			xrgb=xrgb32;
			rrgb=rrgb32;
			break;
	    default:
			SDLerr(stderr, "SDLengine:video bpp not  valid: %d\n",bpp);
			return -1;
			break;
	}



	SDLautobacktime=timer()+400;
	screenopen(0,SDLdisplay->w,SDLdisplay->h,0,0,SDLdisplay->w,SDLdisplay->h,0);
	spriteclip(0,0,SDLdisplay->w,SDLdisplay->h);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//display.width											:restituisce la larghezza del display
int displaywidth()
{
	return SDLdisplay->w;
}
//_______________________________________________________________________________________________________________________
//display.height											:restituisce l'altezza del display
int displayheight()
{
	return SDLdisplay->h;
}
//_______________________________________________________________________________________________________________________
//display.bpp											:restituisce la profondita di colore del display
int displaybpp()
{
	return (SDLdisplay->format->BytesPerPixel * 8);
}

//_______________________________________________________________________________________________________________________

//screen n											: attiva lo schgermo logico n
int screen (int n)
{
	if (n==-1)return c_screen;

	if (SDLscreen_live[n]!=0){
		c_screen=n;
	}
	else{
		SDLerr(stderr, "SDLengine:Surface not found: %s\n", SDL_GetError());
		return -1;
	}

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

// directscreen 											: apre direttamente il display come screen
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

//screenopen n,w,h,dx,dy,dw,dh,flag									: apre lo schermo logico n di dimensione w,h nelle coordinate dx,dy,dw,dh, del display
int screenopen (int n,int w,int h,int dx,int dy,int dw,int dh,int flag)
{
	SDL_Surface *temp;
	int bpp;

	if (n<0 ||n>7){
			SDLerr(stderr, "SDLengine:screen are 8(0-7): %s\n",SDL_GetError());
			return -1;
	}

	if (SDLscreen_live[n]!=0){
		SDL_FreeSurface(SDLscreen[n]);
		SDLscreen_live[n]=0;
	}

	if (SDLscreen_live[n]==0){
		temp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h,32,0,0,0,0);//|SDL_ASYNCBLIT
		if(temp  == NULL) {
			SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
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


		//SDLscreen[n]=SDL_DisplayFormat(temp);
		//SDL_FreeSurface(temp);
		SDLscreen[n]=temp;
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


	if (SDLautoback>0)autoback_timer();
	return 0;
	}
	else{
			SDLerr(stderr, "SDLengine:screen already open: %s\n",SDL_GetError());
			return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenclose n											: chiude lo schermo logico n
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

		SDL_FreeSurface(SDLscreen[n]);
		SDLscreen_live[n]=0;

		return 0;
	}
	else {
		SDLerr(stderr, "SDLengine:Surface in use %s\n", SDL_GetError());
		return -1;
	}

	i=lastscreen;
	while(SDLscreen_live[i]==0)i--;
	lastscreen=i+1;



	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenclone n,s,x,y,w,h,flag						: crea una nuova entrata nello schermo n
int screenclone(int n,int s,int x,int y,int w,int h,int flag)
{

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
}
//_______________________________________________________________________________________________________________________

//screencopy(n,x,y,w,h,nd,xd,yd)          						: copia una porzione di schermo da n a nd
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
		SDLerr(stderr, "SDLengine:screen not open??", SDL_GetError());
		return -1;
	}

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenfade(n,t)								: sfuma lo screen n in t time
int screenfade(int n,int t)
{
	if (SDLscreen[n]!=NULL){
		fade(SDLscreen[n],SDLscreen[n],t,1);
	if (SDLautoback>0)autoback_timer();
	return 0;
	}
	else{
		SDLerr(stderr, "SDLengine:screen not open??", SDL_GetError());
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenfadein(n,i,t)								: sfuma lo screen nverso l'immagine i in t time
int screenfadein(int n,int i,int t)
{
	if(imageexists(i)==0){
		SDLerr(stderr, "SDLengine:image empty", SDL_GetError());
		return -1;
	}

	if (SDLscreen[n]!=NULL){
		fade(SDLscreen[n],SDLimage[i],t,0);
	if (SDLautoback>0)autoback_timer();
	return 0;
	}
	else{
		SDLerr(stderr, "SDLengine:screen not open??", SDL_GetError());
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screencrossfade(n,i,t)								: sfuma lo screen n verso dall'imagine correntel'immagine i in t time
int screencrossfade(int n,int i,int t)
{
	if(imageexists(i)==0){
		SDLerr(stderr, "SDLengine:image empty", SDL_GetError());
		return -1;
	}
	if(imageexists(t)==0){
		SDLerr(stderr, "SDLengine:image empty", SDL_GetError());
		return -1;
	}

	if (SDLscreen[n]!=NULL){
		crossfade(SDLscreen[n],SDLscreen[n],SDLimage[i],t);
	if (SDLautoback>0)autoback_timer();
	return 0;
	}
	else{
		SDLerr(stderr, "SDLengine:screen not open??", SDL_GetError());
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//screenalpha(n,a)								: setta il parametro alpha(trasparenza) allo screen n
int screenalpha(int n,int a)
{
	if ( SDLscreen[n] != NULL ) {
		SDL_SetAlpha (SDLscreen[n], SDL_SRCALPHA, a);
		if (SDLautoback>0)autoback_timer();
		return 0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}

}
//_______________________________________________________________________________________________________________________

//screenlock(n)									: blocca il display per l'accesso diretto dai comandi grafici
int screenlock(int n)
{
    if (SDL_MUSTLOCK(SDLscreen[n]))
	SDL_LockSurface(SDLscreen[n]);
    return 0;
}
//_______________________________________________________________________________________________________________________

//screenunlock(n)									: sblocca il display per l'accesso diretto dai comandi grafici
int screenunlock(int n)
{
    if (SDL_MUSTLOCK(SDLscreen[n]))
	SDL_UnlockSurface(SDLscreen[n]);
    return 0;
}
//_______________________________________________________________________________________________________________________

//screenrect x,y,w,h,flag							: modifica le coordinate di output sul display dello screen corrente
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


	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//xscreenrect								: restituisce la coordinata x del rettangolo di output dello screen
int xscreenrect()
{
	return SDLscreen_rect[c_screen].x;
}
//_______________________________________________________________________________________________________________________

//yscreenrect								: restituisce la coordinata y del rettangolo di output dello screen
int yscreenrect()
{
	return SDLscreen_rect[c_screen].y;
}
//_______________________________________________________________________________________________________________________

//wscreenrect								: restituisce la coordinata w del rettangolo di output dello screen
int wscreenrect()
{
	return SDLscreen_rect[c_screen].w;
}
//_______________________________________________________________________________________________________________________

//hscreenrect								: restituisce la coordinata h del rettangolo di output dello screen
int hscreenrect()
{
	return SDLscreen_rect[c_screen].h;
}
//_______________________________________________________________________________________________________________________

//flagscreenrect								: restituisce lo stato del flag del rettangolo di output dello screen
int flagscreenrect()
{
	return SDLscreen_flag[c_screen];
}
//_______________________________________________________________________________________________________________________

//screen.width											:restituisce la larghezza dello screen
int screenwidth()
{
	return SDLscreen[c_screen]->w;
}
//_______________________________________________________________________________________________________________________
//screen.height											:restituisce l'altezza dello screen
int screenheight()
{
	return SDLscreen[c_screen]->h;
}
//_______________________________________________________________________________________________________________________

// offset x,y											: posiziona la posiziona di visione del display sullo schermo logico
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

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

// xoffset
int xoffset()
{
	return SDLoffx[c_screen];
}
//_______________________________________________________________________________________________________________________

// yoffset
int yoffset()
{
	return SDLoffy[c_screen];
}
//_______________________________________________________________________________________________________________________

//cls 												: cancella lo schermo logico
int cls()
{
	stx[c_screen]=0;
	sty[c_screen]=0;
	SDL_FillRect(SDLscreen[c_screen],NULL,paper_color);
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//screenswap											: scambia lo schermo logico con quello fisico
int screenswap()
{

	SDL_Rect d;
	SDL_Rect s;
	int i;


	/* il modo negativo di screenswap */
	if (SDLautoback<0){
		while (timer()<SDLautobacktime)getevent();
		SDLautobacktime=timer()+abs(SDLautoback);
	}


	if (vbl!=-1){
	    vbl=-1;
	/* aggiorna il cursor system */
	/*
		stati del cursor system:
		0:disattivo
		1:attivo
		2:in disattivazione
	*/

	    for(i=0;i<lastscreen;i++){
		if(cursore_state[i]!=0) {
			//stampa il cursore 0
			d.x=oldstx[i]*XPRINT_SIZE;
			d.y=oldsty[i]*PRINT_SIZE+2;
			d.w=XPRINT_SIZE;
			d.h=PRINT_SIZE*2;
			s.x=0;
			s.y=0;
			s.w= XPRINT_SIZE;
			s.h= PRINT_SIZE*2;
			SDL_BlitSurface(SDLcursore[0], &s, SDLscreen[i], &d );

			if (cursore_state[i]==2){
				cursore_state[i]=0;
			}
			if(cursore_state[i]==1) {
				oldstx[i]=stx[i];
				oldsty[i]=sty[i];

				//stampa il cursore e aggiorna l'animazione
				d.x=oldstx[i]*XPRINT_SIZE;
				d.y=oldsty[i]*PRINT_SIZE+2;
				d.w=XPRINT_SIZE;
				d.h=PRINT_SIZE*2;
				s.x=0;
				s.y=0;
				s.w= XPRINT_SIZE;
				s.h= PRINT_SIZE*2;
				SDL_BlitSurface(SDLcursore[cursore_anim[i]], &s, SDLscreen[i], &d );

				cursore_anim[i]++;
				if (cursore_anim[i]>14)cursore_anim[i]=0;
			}
		}
	}
	/***************************/



	/* aggiorna il bob system */
	if (SDLupdatebob!=0){

		//preleva il background
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
						SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
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
						SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
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

		//pasta il bob
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
					SDLerr(stderr, "SDLengine:frame bob not exist", SDL_GetError());
					return -1;
				}//end if


			}//end if
		}//end for
	}//end if
	/***********************/


	/* aggiorna il display system sotto agli sprite*/
	for (i=0;i<lastscreen;i++){
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

	/* aggiorna lo sprite system */
	if(SDLupdatesprite!=0){
		if(SDLupdatesprite==2)SDLupdatesprite=0;
		for (i=0;i<lastsprite;i++){
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

				/*niente verifiche: siamo in realtime */
				if(d.x<SDLspriteclip.w+SDLspriteclip.x)
					if(d.y<SDLspriteclip.h+SDLspriteclip.y)
						SDL_BlitSurface(SDLimageCC[SDLspritefr[i]], &s, SDLdisplay, &d );


			}//end if
		}//end for
	}//end if
	/***********************/

	/* aggiorna il display system sopra agli sprite*/

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


	// ripristina lo sfondo dei bob
	if (SDLupdatebob!=0){
		if (SDLupdatebob ==2)SDLupdatebob=0;
			//ripristina il background se esiste
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
	return 0;
}
//_______________________________________________________________________________________________________________________

//autoback m								: attiva / disattiva lo screenswap automatico
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

//dualplayfield m 								: attiva  / disattiva l'aggiornamento degli screen sopra al piano dei bob
int dualplayfield(int m)
{
	if (m!=-1){
		SDLdualplayfield=m;
		if (SDLautoback>0)autoback_timer();
		return 0;
	}
	else
		return SDLdualplayfield;

}
//_______________________________________________________________________________________________________________________

//waitvbl									: attende lo screenswap automatico
int waitvbl()
{
 if (autoback(-1)!=0){

	vbl=1;
	while(vbl!=0)
		if (SDLautoback>0)autoback_timer();
		getevent();
	}
	return 0;
}
//_______________________________________________________________________________________________________________________

//fps(n)								: attiva/disattiva/restituisce il numero di frame per secondo attuali
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

//GEOMETRIA
//_________________________________________________________________________________________________________________________



// rgb int colR,int colG,int colB)
int rgb(int colR,int colG,int colB)
{
	int c;
	c= colB + (colG * 256)+(colR*256*256);

	return c;
}
//_________________________________________________________________________________________________________________________

//color(c,v)								: setta il colore c (0-255) nella paletta conil valore v
int color(int c,int v)
{
    if (v !=-1 ){
	if (c<0)return -1;
	if (c>255)return -1;
	Palette[c]=v;
    return 0;
    }
    else{
	return Palette[c];
    }
}
//_________________________________________________________________________________________________________________________

//ink												:seleziona il colore corrente in formato RGB
int ink( int c)
{
	SDLcol.b = c & 255;
	SDLcol.g =( c / 256) & 255 ;
	SDLcol.r =( c / (256*256)) & 255;
	return 0;
}
//_______________________________________________________________________________________________________________________

//point(x,y)											:restituisce la componente R colore alla coordinata x,y
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

//dot(x,y)												:scrive un punto con il colore ink alla coordinata x,y
int dot(int x,int y)
{

    if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
	    SDL_LockSurface(SDLscreen[c_screen]);

	putpixel( SDLscreen[c_screen], x, y, xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
	    SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//dot_ex(x,y,c)												:scrive un punto con il colore ink alla coordinata x,y
int dot_ex(int x,int y,int c)
{
	ink(c);
	if (SDLautoback>0)autoback_timer();
	return dot(x,y);
}
//_______________________________________________________________________________________________________________________

//line x,y,x1,y1
int line(int x,int y,int x1,int y1)
{
		if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
			SDL_LockSurface(SDLscreen[c_screen]);

		draw_line(SDLscreen[c_screen],x,y,x1,y1,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

		if (SDL_MUSTLOCK(SDLscreen[c_screen]) )
			SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//box x,y,x1,y1
int box(int x,int y,int x1,int y1)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_box(SDLscreen[c_screen],x,y,x1,y1,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//bar x,y,x1,y1
int bar(int x,int y,int x1,int y1)
{

	SDL_Rect d;

	d.x=x;
	d.y=y;
	d.w=x1-x;
	d.h=y1-y;

    SDL_FillRect (SDLscreen[c_screen], &d,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

    if (SDLautoback>0)autoback_timer();

    return 0;
}
//_______________________________________________________________________________________________________________________

//circle x,y,r								: traccia un cerchio
int circle(int x,int y,int r)
{

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_circle(SDLscreen[c_screen],x,y,r,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//fillcircle x,y,r								: traccia un cerchio pieno
int fillcircle(int x,int y,int r)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_fillcircle(SDLscreen[c_screen],x,y,r,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//ellipse x,y,rx,ry								: traccia un ellisse
int ellipse(int x,int y,int rx,int ry)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_ellipse(SDLscreen[c_screen],x,y,rx,ry,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//fillellipse x,y,rx,ry							: traccia un ellisse piena
int fillellipse(int x,int y,int rx,int ry)
{
	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_LockSurface(SDLscreen[c_screen]);

	draw_fillellipse(SDLscreen[c_screen],x,y,rx,ry,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));

	if (SDL_MUSTLOCK(SDLscreen[c_screen]))
		SDL_UnlockSurface(SDLscreen[c_screen]);

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//paint x,y								: riempie un area chiusa
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
	putpixel(SDLscreen[c_screen],x,y,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));//dot(x,y);
	col=getpixel(SDLscreen[c_screen],x,y);//point(x,y);
    pm=2;
    if (col==src_col)return -1;

    while(0==0) {
	//if (point(x,y)==src_col) {
	if (getpixel(SDLscreen[c_screen],x,y)==src_col) {
	putpixel(SDLscreen[c_screen],x,y,xrgb(SDLcol.r,SDLcol.g,SDLcol.b));//dot(x,y);
		paintmap[y*screenwidth()+x]=pm;
    	}

	//if (point(x+1,y)==src_col && (x+1) < screenwidth()) {
	if (getpixel(SDLscreen[c_screen],x+1,y)==src_col && (x+1) < screenwidth()) {
		    x=x+1;
		    pm=1;
		}
	//else if (point(x-1,y)==src_col && (x-1) > 0 ) {
	else if (getpixel(SDLscreen[c_screen],x-1,y)==src_col && (x-1) > 0 ) {
		    x=x-1;
		    pm=2;
		}
	//else if (point(x,y+1)==src_col && (y+1) < screenheight()) {
	else if (getpixel(SDLscreen[c_screen],x,y+1)==src_col && (y+1) < screenheight()) {
		    y=y+1;
		    pm=3;
		}
	//else if (point(x,y-1)==src_col && (y-1) > 0) {
	else if (getpixel(SDLscreen[c_screen],x,y-1)==src_col && (y-1) > 0) {
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

	if (SDLautoback>0)autoback_timer();
    return 0;
}
//_______________________________________________________________________________________________________________________

//_______________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

// FILE INPUT OUTPUT
//_________________________________________________________________________________________________________________________

//loadimage filename,n										: carica in un banco un file grafico  non trasparente
int loadimage(char *filename,int n)
{
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
		SDLerr(stderr,"SDLengine:Error file not found: %s\n",filename);
		//exit(2);
		return -1;
	}
	return 0;
}
//_______________________________________________________________________________________________________________________

//saveimage filename,n										: salva il banco n nel file grafico (deve essere bmp)
int saveimage(char *filename,int n)
{
	Save_Image(filename,SDLimage[n]);
	return 0;
}
//_______________________________________________________________________________________________________________________

//loadsound filename,n										: carica in un banco un file wav
int loadsound(char *filename,int n)
{
	if (fileExist(filename)){
		 Mix_FreeChunk(SDLsound[n]);
		 SDLsound[n]=Mix_LoadWAV(filename);
	}
	else{
		SDLerr(stderr,"SDLengine:Error file not found  %s\n",SDL_GetError());
		return -1;
	}
	return 0;
}
//_______________________________________________________________________________________________________________________


//soundexists( n)											: restituisce 1 se il banco sonoro n esiste 0 se vuoto
int soundexists( int n )
{
	if ( SDLsound[n] == NULL ) {
		return(0);
	}
	return 1;
}
//________________________________________________________________________________________________________________________________________

//loadmusic filename,n										: carica in un banco un file xm mod
int loadmusic(char *filename)
{
	if (fileExist(filename)){
		Mix_FreeMusic(SDLmusic);
		SDLmusic = Mix_LoadMUS(filename);
	}
	else{
		SDLerr(stderr,"SDLengine:Error file not found  %s\n",SDL_GetError());
		return -1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

//musicexists( )											: restituisce 1 se la musica e' caricata  0 se vuoto
int musicexists()
{
	if ( SDLmusic == NULL ) {
		return(0);
	}
	return 1;
}
//__________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

// IMAGE MANIPULATION
//_________________________________________________________________________________________________________________________

//hotspot(n,x,y)								: stabilisce il punto delle coordinate (n,0,0=alto sx | n,1,1=centro | n,2,2 =basso dx)n= image
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
	SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
	return -1;
    }
}
//_________________________________________________________________________________________________________________________

//imageexists( n)											: restituisce 1 se il banco n esiste 0 se vuoto
int imageexists( int n )
{
	if ( SDLimage[n] == NULL ) {
		return(0);
	}
	return 1;
}
//_________________________________________________________________________________________________________________________

//imagewidth(n)								: restituisce la larghezza dell'immagine n
int imagewidth(int n)
{
	if ( SDLimage[n] != NULL ) {
		return SDLimage[n]->w;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}

}
//_________________________________________________________________________________________________________________________

//imageheight(n)								: restituisce l'altezza dell'immagine n
int imageheight(int n)
{
	if ( SDLimage[n] != NULL ) {
		return SDLimage[n]->h;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}
}
//________________________________________________________________________________________________________________________________________

//deleteimage(n)											: cancella l'immagine n dalla memoria
int deleteimage(int n)
{
	if ( SDLimage[n] != NULL ) {
		SDL_FreeSurface(SDLimage[n]);
		SDL_FreeSurface(SDLimageCC[n]);
		return 0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;

	}

}
//________________________________________________________________________________________________________________________________________

//copyimage(s,d)											: copia l'immagine s nel banco d
int copyimage(int s,int d)
{
	if ( SDLimage[s] != NULL ) {
		if ( SDLimage[d] != NULL ) {
			SDL_FreeSurface(SDLimage[d]);
			SDL_FreeSurface(SDLimageCC[d]);
		}

		SDLimage[d] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL,SDLimage[s]->w,SDLimage[s]->h,32,0,0,0,0);
		if(SDLimage[d]  == NULL) {
			SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
			return -1;
		}
		SDLimage[d]=SDL_DisplayFormat(SDLimage[s]);
		SDL_BlitSurface(SDLimage[s], NULL, SDLimage[d], NULL );

		SDLimageCC[d] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, SDLimageCC[s]->w, SDLimageCC[s]->h,32,0,0,0,0);
		if(SDLimageCC[d]  == NULL) {
			SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
			return -1;
		}
		SDLimageCC[d]=SDL_DisplayFormat(SDLimageCC[s]);
		SDL_BlitSurface(SDLimageCC[s], NULL, SDLimageCC[d], NULL );
		Image_colorkey(SDLimageCC[d]);

		return 0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;

	}

}
//________________________________________________________________________________________________________________________________________
// setalpha(n,a)											:set trasparency
int setalpha(int n,int a)
{
	if ( SDLimage[n] != NULL ) {
		SDL_SetAlpha (SDLimage[n], SDL_SRCALPHA, a) ;
		SDL_SetAlpha (SDLimageCC[n], SDL_SRCALPHA, a) ;
		return 0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}
}
//_________________________________________________________________________________________________________________________

//zoomimage(n,zoom)							: zoom image
int zoomimage(int n,double zoomx,double zoomy)
{
	SDL_Surface *tmp;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)wait(1);
		vbl=-1;
		tmp=zoomSurface (SDLimage[n], zoomx, zoomy, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=zoomSurface (SDLimageCC[n], zoomx, zoomy, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}
	return 0;
}
//_________________________________________________________________________________________________________________________

//rotateimage(n,angle)							: rotate image angle (0-360)
int rotateimage(int n,double angle)
{
	SDL_Surface *tmp;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)wait(1);
		vbl=-1;

		tmp=rotozoomSurface (SDLimage[n], angle, 1, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=rotozoomSurface (SDLimageCC[n], angle, 1, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}
	return 0;

}
//_________________________________________________________________________________________________________________________

//rotozoomimage(n,angle,zoom)						: rotate and zoom image (0-360)
int rotozoomimage(int n,double angle,double zoom)
{
	SDL_Surface *tmp;

	if ( SDLimage[n] != NULL ) {
		while(vbl==-1)wait(1);
		vbl=-1;

		tmp=rotozoomSurface (SDLimage[n], angle, zoom, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}

		SDL_FreeSurface(SDLimage[n]);
		SDLimage[n]=tmp;

		tmp=rotozoomSurface(SDLimageCC[n], angle, zoom, 1);
		if(tmp==NULL){
			SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
			return -1;
		}
		SDL_FreeSurface(SDLimageCC[n]);
		Image_colorkey(tmp);
		SDLimageCC[n]=tmp;
		vbl=0;
	}
	else
	{
		SDLerr(stderr,"SDLengine:image not found %s\n",SDL_GetError());
		return -1;
	}
	return 0;

}

//_________________________________________________________________________________________________________________________

// BLITTING
//_________________________________________________________________________________________________________________________

//blt n,sx,sy,sw,sh,dx,dy										: copia un tile nello schermo
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

	if (SDLimage[n]!=NULL){
		SDL_BlitSurface( SDLimage[n], &s ,SDLscreen[c_screen], &d);
	}
	else {
		SDLerr(stderr, "SDLengine:frame not exist", SDL_GetError());
		return-1;
	}

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//__________________________________________________________________________________________________________________________

//pastebob x,y,n											: copia sullo schermo il banco n alle coordinate
//													x,y ritagliando
int pastebob(int x,int y,int n)
{
	SDL_Rect s;
	SDL_Rect d;


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
		SDLerr(stderr, "SDLengine:frame not exist", SDL_GetError());
		return -1;
	}

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//__________________________________________________________________________________________________________________________

//pasteicon x,y,n									: copia sullo schermo il banco n alle coordinate
//													x,y
int pasteicon(int x,int y,int n)
{
	SDL_Rect s;
	SDL_Rect d;


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
		SDLerr(stderr, "SDLengine:frame not exist", SDL_GetError());
		return -1;
	}

	if (SDLautoback>0)autoback_timer();
	return 0;
}
//__________________________________________________________________________________________________________________________

//grab n,x,y,w,h											: ritaglia dallo schermo e copia nel banco
int grab(int n,int x,int y,int w,int h)
{
	SDL_Rect s;
	SDL_Rect d;
	int bpp;

	s.x=x;
	s.y=y;
	s.w=w;
	s.h=h;

	d.x=0;
	d.y=0;
	d.w=w;
	d.h=h;


	SDL_FreeSurface(SDLimage[n]);
	SDL_FreeSurface(SDLimageCC[n]);


	SDLimage[n] = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_HWACCEL, w, h,32,0,0,0,0);
	if(SDLimage[n]  == NULL) {
		SDLerr(stderr, "SDLengine:CreateRGBSurface failed: %s\n", SDL_GetError());
		return -1;
	}

	//SDLimage[n]=SDL_DisplayFormat(SDLimage[n]);



	SDL_BlitSurface(SDLscreen[c_screen], &s, SDLimage[n], &d );

	SDLimageCC[n]=SDL_DisplayFormat(SDLimage[n]);
	if(SDLimageCC[n]  == NULL) {
		SDLerr(stderr, "SDLengine:CreateRGBSurfaceCC failed: %s\n", SDL_GetError());
		return -1;
	}

	if (SDL_MUSTLOCK(SDLimageCC[n]))
		SDL_LockSurface(SDLimageCC[n]);

	bpp = SDLimageCC[n]->format->BytesPerPixel;
		switch(bpp) {
		    case 1:
			SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),*(Uint8 *)SDLimageCC[n]->pixels);//|SDL_RLEACCEL
			break;
		    case 2:
			SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),*(Uint16 *)SDLimageCC[n]->pixels);//|SDL_RLEACCEL
			break;
		    case 3:
			SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),*(Uint32 *)SDLimageCC[n]->pixels);//|SDL_RLEACCEL
			break;
		    case 4:
			SDL_SetColorKey(SDLimageCC[n], (SDL_SRCCOLORKEY),*(Uint32 *)SDLimageCC[n]->pixels);//|SDL_RLEACCEL
			break;
		    }
	if (SDL_MUSTLOCK(SDLimageCC[n]))
		SDL_UnlockSurface(SDLimageCC[n]);

	SDL_hsx[n]=0;
	SDL_hsy[n]=0;

	return 0;
}
//__________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________

// SPRITE SYSTEM
//_________________________________________________________________________________________________________________________

//spriteclip x,y,w,h								:setta il clipping dell'area sprite
int spriteclip(int x,int y,int w,int h)
{
	SDLspriteclip.x=x;
	SDLspriteclip.y=y;
	SDLspriteclip.w=w;
	SDLspriteclip.h=h;
	return 0;
}
//__________________________________________________________________________________________________________________________

// sprite n,x,y,fr											:attiva o sposta il sprite n alla coordinata x,y
//													con l'immagine fr
int sprite(int n,int x,int y,int fr)
{
	if (n>NUM_SPRITES){
		SDLerr(stderr, "SDLengine:sprite number invalid", SDL_GetError());
		return -1;
	}
	if (SDLimageCC[fr]!=NULL) {
		SDLspritelive[n]=1;
		SDLspritex[n]=x-SDL_hsx[fr];
		SDLspritey[n]=y-SDL_hsy[fr];
		SDLspritefr[n]=fr;

	}
	else {

		SDLerr(stderr, "SDLengine:framesprite not exist", SDL_GetError());
		return -1;
	}
	if (lastsprite<=n)lastsprite=n+1;


	if (SDLautoback>0)autoback_timer();
	return 0;

}
//__________________________________________________________________________________________________________________________
// delete sprite n											: disattiva il sprite n
int deletesprite(int n)
{
    SDLspritelive[n]=0; //morto!!
	SDLspritex[n]=0;
	SDLspritey[n]=0;
	SDLspritefr[n]=-1;

	if (lastsprite==n+1)
	    while (SDLspritelive[lastsprite-1]==0)lastsprite--;

	if (SDLautoback>0)autoback_timer();
	return SDLspritelive[n];
}
//__________________________________________________________________________________________________________________________

//xsprite												: restituisce la coordinata x del sprite n
int xsprite(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLspritex[n]+SDL_hsx[SDLspritefr[n]];
}
//_________________________________________________________________________________________________________________________

//ysprite												: restituisce la coordinata y del sprite n
int ysprite(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLspritey[n]+SDL_hsy[SDLspritefr[n]];
}
//_________________________________________________________________________________________________________________________

//sprite.width											: restituisce la larghezza del sprite n
int spritewidth(int n)
{
	if (SDLautoback>0)autoback_timer();
	if (SDLspritefr[n]!=-1)
		return SDLimageCC[SDLspritefr[n]]->w;
	return 0;
}
//_________________________________________________________________________________________________________________________

//sprite.height											:  restituisce l'altezza del sprite n
int spriteheight(int n)
{
	if (SDLautoback>0)autoback_timer();
	if (SDLspritefr[n]!=-1)
		return SDLimageCC[SDLspritefr[n]]->h;
	return 0;
}
//_________________________________________________________________________________________________________________________

//frsprite												:  restituisce il frame del sprite n
int frsprite(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLspritefr[n];
}
//_________________________________________________________________________________________________________________________

//livesprite												:  restituisce 1 se il sprite n e' "vivo"
int livesprite(int n)
{
		if (SDLautoback>0)autoback_timer();
		return SDLspritelive[n];
}
//_________________________________________________________________________________________________________________________

//spritehit												:  restituisce 1 se il sprite n collide con il sprite x se -1 con tutti
int spritehit(int n,int x)
{
    int xa1,ya1,xa2,ya2;
    int xb1,yb1,xb2,yb2;
	int i,ret;

    xa1=SDLspritex[n];
    ya1=SDLspritey[n];
    xa2=xa1+spritewidth(n);
    ya2=ya1+spriteheight(n);

    if (x!=-1){
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

//autoupdatesprite m 								: attiva  / disattiva l'aggiornamento automatico degli sprite allo screenswap
int autoupdatesprite(int m)
{
	if (SDLautoback>0)autoback_timer();
	SDLupdatesprite = m;
	return SDLupdatesprite;

}
//_________________________________________________________________________________________________________________________

//updatesprite 									: attiva  un aggiornamento manuale degli sprite allo screenswap
int updatesprite()
{
	SDLupdatesprite = 2;
	if (SDLautoback>0)autoback_timer();
	return SDLupdatesprite;

}
//_________________________________________________________________________________________________________________________

//BOB SYSTEM
//_________________________________________________________________________________________________________________________

// setbob n,scr											:assegna il bob n allo screen scr
int setbob(int n,int scr)
{
	if (n>NUM_BOBS){
		SDLerr(stderr, "SDLengine:sprite number invalid", SDL_GetError());
		return -1;
	}
	SDLbobscreen[n]=scr;
	if (SDLautoback>0)autoback_timer();
	 return 0;
}
//_________________________________________________________________________________________________________________________

// bob n,x,y,fr											:attiva o sposta il bob n alla coordinata x,y
//													con l'immagine fr
int bob(int n,int x,int y,int fr)
{
	if (n>NUM_BOBS){
		SDLerr(stderr, "SDLengine:sprite number invalid", SDL_GetError());
		return -1;
	}

	if (SDLimageCC[fr]!=NULL){
		SDLboblive[n]=1;
		SDLbobx[n]=x-SDL_hsx[fr];
		SDLboby[n]=y-SDL_hsy[fr];
		SDLbobfr[n]=fr;
	}
	else {
		SDLerr(stderr, "SDLengine:framebob not exist", SDL_GetError());
		return -1;
	}
	if (SDLbobscreen[n]==-1)SDLbobscreen[n]=c_screen;

	if (lastbob<=n)lastbob=n+1;


	if (SDLautoback>0)autoback_timer();
	return 0;
}
//__________________________________________________________________________________________________________________________
// delete bob n											: disattiva il bob n
int deletebob(int n)
{
	SDLboblive[n]=0; //morto!!
	SDLbobx[n]=0;
	SDLboby[n]=0;
	SDLbobfr[n]=-1;

	if (lastbob==n+1)
	    while (SDLboblive[lastbob-1]==0)lastbob--;

	if (SDLautoback>0)autoback_timer();
	return SDLboblive[n];
}
//__________________________________________________________________________________________________________________________

//xbob												: restituisce la coordinata x del bob n
int xbob(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLbobx[n]-SDL_hsx[SDLbobfr[n]];
}
//_________________________________________________________________________________________________________________________

//ybob												: restituisce la coordinata y del bob n
int ybob(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLboby[n]-SDL_hsy[SDLbobfr[n]];
}
//_________________________________________________________________________________________________________________________

//bob.width											: restituisce la larghezza del bob n
int bobwidth(int n)
{
	if (SDLautoback>0)autoback_timer();
	if (SDLbobfr[n]!=-1)
		return SDLimageCC[SDLbobfr[n]]->w;
	return 0;
}
//_________________________________________________________________________________________________________________________

//bob.height											:  restituisce l'altezza del bob n
int bobheight(int n)
{
	if (SDLautoback>0)autoback_timer();
	if (SDLbobfr[n]!=-1)
		return SDLimageCC[SDLbobfr[n]]->h;
	return 0;
}
//_________________________________________________________________________________________________________________________

//frbob												:  restituisce il frame del bob n
int frbob(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLbobfr[n];
}
//_________________________________________________________________________________________________________________________

//livebob												:  restituisce 1 se il bob n e' "vivo"
int livebob(int n)
{
	if (SDLautoback>0)autoback_timer();
		return SDLboblive[n];
}
//_________________________________________________________________________________________________________________________

//bobhit												:  restituisce 1 se il bob n collide con il bob x se 0 con tutti
int bobhit(int n,int x)
{
    int xa1,ya1,xa2,ya2;
    int xb1,yb1,xb2,yb2;
	int i,ret;

    xa1=SDLbobx[n];
    ya1=SDLboby[n];
    xa2=xa1+bobwidth(n);
    ya2=ya1+bobheight(n);

    if (x!=-1){
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

//autoupdatebob m 								: attiva  / disattiva l'aggiornamento automatico dei bob allo screenswap
int autoupdatebob(int m)
{
	SDLupdatebob=m;
	if (SDLautoback>0)autoback_timer();
	return SDLupdatebob;

}
//_________________________________________________________________________________________________________________________

//updatebob m 								: attiva   un aggiornamento manuale dei bob allo screenswap
int updatebob()
{
	SDLupdatebob=2;
	if (SDLautoback>0)autoback_timer();
	return SDLupdatebob;

}



//_________________________________________________________________________________________________________________________

// TEXT DRAW
//_________________________________________________________________________________________________________________________

//text x,y,s,testo											:stampa il testo sul video con s size e col colore
int text ( int x, int y, int s, char *testo)
{
	SDL_Rect d;
	int w,h;

	font = TTF_OpenFont(fontPath,s);
	if (font ==0) {
		SDLerr(stderr,"SDLengine: Warning Couldn't open true fonf  %s\n",SDL_GetError());
		return -1;
	}

	TTF_SizeText(font, testo, &w, &h);


	SDLtext = TTF_RenderText_Solid(font,testo,SDLcol);

	d.x=x;
	d.y=y;
	d.w=w;
	d.h=h;

	SDL_BlitSurface(SDLtext, NULL, SDLscreen[c_screen], &d );
	SDL_FreeSurface(SDLtext);
	TTF_CloseFont(font);
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//setfont path											:seleziona il font d'uso
int setfont(char *path)
{
	if (fileExist(path)) {


		fontPath=path;
	}
	else {
		SDLerr(stderr,"SDLengine:Error file not found  %s\n",SDL_GetError());
		return -1;
	}
	return 0;
}
//_______________________________________________________________________________________________________________________

//textrender testo,s,n 			 						                  :crea il banco grafico n contenente il "testo" scritto con il font corrente con size s
int textrender (char *testo,int s,int n)
{
	font = TTF_OpenFont(fontPath,s);
	if (font ==0) {
		SDLerr(stderr,"SDLengine: Warning Couldn't open true fonf:verify installation!!! %s\n",SDL_GetError());
		return -1;
	}

	SDL_FreeSurface(SDLimage[n]);
	SDL_FreeSurface(SDLimageCC[n]);

	SDLimage[n] = TTF_RenderText_Solid(font,testo,SDLcol);
	SDLimageCC[n] = TTF_RenderText_Solid(font,testo,SDLcol);

	TTF_CloseFont(font);
	if (SDLautoback>0)autoback_timer();
	return 0;
}


//_______________________________________________________________________________________________________________________

// PRINT SCREEN
//_________________________________________________________________________________________________________________________

//pen(c)									: setta il colore di print
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
	    SDLerr(stderr,"SDLengine:Couldn't open true font monospace: verify installation!!!  %s\n",SDL_GetError());
	    return -1;
	}

	for (i=0; i<255;i++){
	    SDL_FreeSurface(SDLfont_mono[i]);
	    SDLfont_mono[i] = TTF_RenderText_Solid(font_mono,(char *)&i,SDLcol);

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

//paper(c)								: settail colore di sfondo del carattere
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

//fprints(testo)								: stampa un testo monospace nella griglia senza andarfe a capo
int fprints(char *testo)
{
	unsigned int i;
	i=0;
	for (i=0;i<strlen(testo);i++)
		put_char(testo[i]);
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//prints(testo)								: stampa un testo monospace nella griglia
int prints(char *testo)
{
	unsigned int i;
	i=0;
	for (i=0;i<strlen(testo);i++)
		put_char(testo[i]);
	put_char(13);

	if (SDLautoback>0)autoback_timer();
	return 0;

}
//_______________________________________________________________________________________________________________________

//locate(x,y)								: muove il cursore di stampa alle coordinate x y
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
		cursoff();
		screenswap();
		screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
		ink(paper_color);
		//ink(rgb(255,0,0));
		bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

		sty[c_screen]=sty[c_screen]-1;
		screenswap();
		curson();
	}


	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________


//atx									: restituisce la coordinata x del cursore
int atx()
{
	return stx[c_screen];
}
//_______________________________________________________________________________________________________________________
//aty									: restituisce la coordinata y del cursore
int aty()
{
	return sty[c_screen];
}
//_______________________________________________________________________________________________________________________

//curson									: visualizza il cursore lampeggiante sul video alle coordinate (atx,aty)
int curson()
{
	cursore_state[c_screen]=1;
	cursore_anim[c_screen]=0;
	screenswap();
	if (SDLautoback>0)autoback_timer();
	return 0;
}


//_______________________________________________________________________________________________________________________

//cursoff									: spegne il cursore lampeggiante sul video
int cursoff()
{
	cursore_state[c_screen]=2;
	cursore_anim[c_screen]=0;
	screenswap();
	if (SDLautoback>0)autoback_timer();
	return 0;

}
//_______________________________________________________________________________________________________________________

//inputs(request,default)								: raccoglie l'input da tastiera fino alla pressione di invio e lo restituisce
int inputs(char *request,char defs[255])
{


	if (cursore_state[c_screen]!=0)
	{
	while ((sty[c_screen]+5)>(screenheight()/(PRINT_SIZE)))
		{
			cursoff();
			screenswap();
			screencopy(c_screen,0,PRINT_SIZE,screenwidth(),screenheight()-(PRINT_SIZE),c_screen,0,0);
			ink(paper_color);
			bar(0,screenheight()-(PRINT_SIZE),screenwidth(),screenheight());

			sty[c_screen]=sty[c_screen]-1;
			screenswap();
			curson();
		}
		fprints(request);
		_input(defs,stx[c_screen],sty[c_screen],255);
		put_char(13);
	}
	else
	{
		while ((sty[c_screen]+5)>(screenheight()/(PRINT_SIZE)))
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
		_input(defs,stx[c_screen],sty[c_screen],255);
		put_char(13);
		cursoff();
	}
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//zoneinputs(x,y,l,default)					: raccoglie l'input da tastiera fino alla pressione nell'area specificata da x,y,l
int zoneinputs(int x,int y,int l,char defs[255])
{


	if (cursore_state[c_screen]!=0)
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
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________

//SOUND
//_________________________________________________________________________________________________________________________

//playsound n,c,l								: suona il suono n nel canale c l volte
int playsound (int n,int c,int l)
{
	if (SDLsound[n]!=NULL){
		Mix_PlayChannel(c,SDLsound[n], l);
		return 0;
	}
	else {
		SDLerr(stderr, "SDLengine:sound not exist", SDL_GetError());
		return -1;
	}
}
//_________________________________________________________________________________________________________________________

//playfreqsound n,c,rate,l							: suona il suono n nel canale c a frequenza rate, l volte
int playfreqsound ( int n, int c, int rate, int l)
{
	if (SDLsound[n]!=NULL){
	    if ( Mix_OpenAudio(rate, AUDIO_S16SYS, 2, 8192) < 0 ) {
		SDLerr(stderr,"Warning: Couldn't set 16-bit stereo audio\n- Reason: %s\n",SDL_GetError());
	    }
	    Mix_PlayChannel(c,SDLsound[n], l);
	    return 0;
	}
	else {
		SDLerr(stderr, "SDLengine:sound not exist", SDL_GetError());
		return -1;
	}
}
//_______________________________________________________________________________________________________________________

//volumesound c,v								: cambia il volume del canale c (-1 tutti) al valore v (0-128)
int volumesound(int c,int v)
{
	return Mix_Volume(c,v);
}
//_______________________________________________________________________________________________________________________

//stopsound(c)									: ferma l'emissione del wav dal canale c
stopsound(c)
{
    return Mix_HaltChannel(c);
}
//_______________________________________________________________________________________________________________________

//pausesound(c)								: mette in pausa il canale c
int pausesound(int c)
{
    Mix_Pause(c);
    return 0;
}
//_______________________________________________________________________________________________________________________

//resumesound(c)							: sblocca dalla pausa il canale c
int resumesound(int c)
{
    Mix_Resume(c);
    return 0;
}
//_______________________________________________________________________________________________________________________

//vumetersound(c)   			                                                    : restituisce lo stato corrente del canale
int vumetersound(int c)
{
    return Mix_Playing(c);
}
//_______________________________________________________________________________________________________________________

//positionsound(c,angle,dist)                                          				: posiziona il suono
int positionsound(int c,int angle,int dist)
{
    return Mix_SetPosition(c, (Sint16)angle, (Uint8) dist);
}
//_______________________________________________________________________________________________________________________

// soundChannels(n)								                :definisce dinamicamente il numero di canali
int soundchannels(int n)
{
    return Mix_AllocateChannels(n);
}
//_______________________________________________________________________________________________________________________


//playmusic  n											: suona il track xm,mod
int playmusic ( int n)
{
	if (SDLmusic!=NULL){
		Mix_PlayMusic(SDLmusic, n);
		return 0;
	}
	else {
		SDLerr(stderr, "SDLengine:music not load", SDL_GetError());
		return -1;
	}
}
//______________________________________________________________________________________________________________________

//positionmusic(p)                                                                             : sposta l'esecuzione al secondo indicato
int positionmusic(int p)
{
        return Mix_SetMusicPosition((double) p);
}
//______________________________________________________________________________________________________________________

//stopmusic 											: ferma il track xm,mod
int stopmusic()
{
	Mix_HaltMusic();
	return 0;
}
//_______________________________________________________________________________________________________________________

//pausemusic                                                                                    : mette in pausa la musica in esecuzione
int pausemusic()
{
        Mix_PauseMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//resumemusic                                                                                   : riavvia la musica in pausa
int resumemusic()
{
        Mix_ResumeMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//rewindmusic                                                                                   :riavvolge la musica corrente
int rewindmusic()
{
        Mix_RewindMusic();
        return 0;
}
//_______________________________________________________________________________________________________________________

//fademusic t											: esegue la dissolvenza progressiva della musica
int fademusic(int t)
{
	Mix_VolumeMusic(Mix_VolumeMusic(-1)-t);
	if (Mix_VolumeMusic(-1)<=0)Mix_HaltMusic();
	return Mix_VolumeMusic(-1);

}
//_______________________________________________________________________________________________________________________

//volumemusic v											: cambia il volume della musica al valore v (0-128)
int volumemusic(int v)
{
	return Mix_VolumeMusic(v);

}
//_______________________________________________________________________________________________________________________

//speedmusic(v)                                                         : cambia la velocita di esecuzione della musica
int speedmusic(int v)
{
    return Mix_SetSynchroValue(v);
}
//_______________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

// KEYBOARD
//_________________________________________________________________________________________________________________________

//key keycode											: restitusce 1 se e' premuto il tasto keycode
int key (int keycode)
{
	Uint8 *keystate;

	if (SDLautoback>0)autoback_timer();
	getevent();
	keystate = SDL_GetKeyState(NULL);
	if (keystate[keycode])return 1;
	return 0;
}
//_______________________________________________________________________________________________________________________

//inkey												: restituisce quale tasto e' premuto
int inkey()
{
	if (SDLautoback>0)autoback_timer();
	getevent();
	return lastkey;
}
//_______________________________________________________________________________________________________________________

//waitkey(keycode)								: attende la pressione di un tasto(0 qualsiasi)
int waitkey(int keycode)
{
	if (keycode==0)
		while(inkey()==0){
			if (SDLautoback>0)autoback_timer();
		}
	else
		while(inkey()!=keycode){
			if (SDLautoback>0)autoback_timer();
		}
	return 0;
}
//_______________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________

//MOUSE
//_________________________________________________________________________________________________________________________

//xmouse												: restituisce la coordinata x del mouse del display
int xmouse()
{
	int x,y;

	getevent();
	SDL_GetMouseState(&x, &y);
	if (SDLautoback>0)autoback_timer();
	return x;
}
//_______________________________________________________________________________________________________________________

//ymouse												: restituisce la coordinata y del mouse del display
int ymouse()
{
	int x,y;

	getevent();
	SDL_GetMouseState(&x, &y);
	if (SDLautoback>0)autoback_timer();
	return y;
}
//_______________________________________________________________________________________________________________________

//xmousescreen(n)											: restituisce la coordinata x del mouse dello screen n
int xmousescreen(int n)
{
	int x,y;

	getevent();
	SDL_GetMouseState(&x, &y);
	if (SDLautoback>0)autoback_timer();
	return x+SDLoffx[n]-SDLscreen_rect[n].x;
}
//_______________________________________________________________________________________________________________________

//ymousescreen(n)											: restituisce la coordinata y del mouse dello screen n
int ymousescreen(int n)
{
	int x,y;

	getevent();
	SDL_GetMouseState(&x, &y);
	if (SDLautoback>0)autoback_timer();
	return y+SDLoffy[n]-SDLscreen_rect[n].y;
}
//_______________________________________________________________________________________________________________________

//bmouse 											: restituisce il pulsante premuto del mouse
int bmouse()
{
	getevent();
	if (SDLautoback>0)autoback_timer();
	return SDL_GetMouseState(NULL, NULL);
}
//_______________________________________________________________________________________________________________________

//mouseshow											: visualizza il cursore del mouse
int mouseshow()
{
	SDL_ShowCursor(SDL_ENABLE);
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//mousehide											: nasconde il cursore del mouse
int mousehide()
{
	SDL_ShowCursor(SDL_DISABLE );
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_______________________________________________________________________________________________________________________

//mousezone(x,y,w,h)							: restituisce 1 se il mouse occupa questa parte dello screen
int mousezone(int x,int y,int w,int h)
{
	if (SDLautoback>0)autoback_timer();
	if ( xmousescreen(c_screen)>x)
		if (xmousescreen(c_screen)<(x+w))
			if(ymousescreen(c_screen)>y)
				if(ymousescreen(c_screen)<y+h)
					return 1;
	return 0;
}
//_______________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________

//JOYSTIK
//_________________________________________________________________________________________________________________________

//xjoy												:non testata
int xjoy()
{
	/*
	int delta_x, delta_y;
	SDL_Joystick *joy;

	getevent();
	SDL_JoystickUpdate();
	if(SDL_JoystickGetBall(joy, 0, &delta_x, &delta_y)==-1) {
		printf("SDLengine: TrackBall Read Error!\n");
		return -1;
	}
	return delta_x;
	*/
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_________________________________________________________________________________________________________________________

//yjoy												:non testata
int yjoy()
{
	/*
	int delta_x, delta_y;
	SDL_Joystick *joy;

	getevent();
	SDL_JoystickUpdate();
	if(SDL_JoystickGetBall(joy, 0, &delta_x, &delta_y)==-1) {
		printf("SDLengine: TrackBall Read Error!\n");
		return -1;
	}
	return delta_y;
	*/
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_________________________________________________________________________________________________________________________

//bjoy												:non implementata
int bjoy()
{
	getevent();
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________

// TIME
//_________________________________________________________________________________________________________________________

//wait t												:attende il trascorrere di t millisecondi
int wait(int t)
{
	 Uint32 ticks;
	ticks=SDL_GetTicks()+t;
	while(ticks>SDL_GetTicks());
	if (SDLautoback>0)autoback_timer();
	return 0;
}
//_________________________________________________________________________________________________________________________

//timer												:restituisce il ticks corrente
int timer()
{
	 Uint32 ticks;
	ticks=SDL_GetTicks();
	return ticks;
}

