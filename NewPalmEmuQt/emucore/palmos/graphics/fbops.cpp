/*
class FBWriter{
private:
	uint16_t* test;//hack
	offset_68k location;
	uint16_t rowbytes;
	int16_t width;
	uint8_t pixelsize;

public:
	FBWriter(offset_68k addr, uint16_t buffwidth, uint8_t bpp);

	uint16_t getpixel(int16_t x,int16_t y);
	void setpixel(int16_t x,int16_t y,uint16_t color);
	void line(int16_t x, int16_t y, int16_t x2, int16_t y2, int prams, uint16_t color);
	void rect(int16_t x, int16_t y, int16_t w, int16_t h, int prams, uint16_t color, uint16_t round);
	void copyrect(RAWimg& host, int16_t startx, int16_t starty, int16_t rectw, int16_t recth, int16_t outx, int16_t outy);
	void draw(RAWimg& smlimg, int16_t x, int16_t y);
	void draw(char letter, RAWfnt& chrimgs, int16_t x, int16_t y);
	bool draw5x7(int16 x, int16 y, uint16_t color, char letter);//hack //remove this
};
*/

#include "m68k.h"
#include "minifunc.h"
#include "fbops.h"

#include <stdlib.h> //needed for abs()

//HACK use memcpy for better performance

//the active framebuffers data and info
uint16_t* FB_data;
int FB_width,FB_height;
int FB_linethickness;

//the bitmap to copy from if doing a bitmap operation
uint16_t* FB_bitmapdata;
int FB_bitmapwidth,FB_bitmapheight;
int FB_bitmaptransparentcolor;//-1 for none

/*
//macros to read and write pixel data
#define FB_pixel(x,y) (FB_data[(y) * FB_width + (x)])
#define FB_bitmappixel(x,y) (FB_bitmapdata[(y) * FB_bitmapwidth + (x)])

#define FB_getpixel(x,y) FB_pixel((x),(y))
#define FB_setpixel(x,y,color) FB_pixel((x),(y)) = (color)
*/



void FB_line(int x,int y,int x2,int y2,uint16_t color){
	//taken from rosetta code
	int dx = abs(x2 - x), sx = x < x2 ? 1 : -1;
	int dy = abs(y2 - y), sy = y < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while(true){
		FB_setpixel(x,y,color);
		if(x == x2 && y == y2) break;
		e2 = err;
		if(e2 > -dx){ err -= dy; x += sx; }
		if(e2 < dy){ err += dx; y += sy; }
	}
}

void FB_rect(int x,int y,int w,int h,uint16_t color){
	FB_line(x,y,x,y + h,color);
	FB_line(x,y,x + w,y,color);
	FB_line(x + w,y,x + w,y + h,color);
	FB_line(x,y + h,x + w,y + h,color);
}

void FB_fillrect(int x,int y,int w,int h,uint16_t color){
	for(int hcnt = 0;hcnt < h;hcnt++){
		for(int wcnt = 0;wcnt < w;wcnt++){
			FB_setpixel(x + wcnt,y + hcnt,color);
		}
	}
}

void FB_copyrect(int x,int y,int w,int h,int fromx,int fromy){
	for(int hcnt = 0;hcnt < h;hcnt++){
		for(int wcnt = 0;wcnt < w;wcnt++){
			//this function has no regard for transparency,for that use FB_drawbitmap instead
			FB_pixel(x + wcnt,y + hcnt) = FB_bitmappixel(fromx + wcnt,fromy + hcnt);
		}
	}
}

//this function just copys the raw bitmap data into the framebuffer(it ignores transparency)
void FB_copybitmap(int x,int y){
	//dont check for transparency
	for(int hcnt = 0;hcnt < FB_bitmapheight;hcnt++){
		for(int wcnt = 0;wcnt < FB_bitmapwidth;wcnt++){
			FB_pixel(x + wcnt,y + hcnt) = FB_bitmappixel(wcnt,hcnt);
		}
	}
}

void FB_drawbitmap(int x,int y){
	if(FB_bitmaptransparentcolor != -1){
		//check for transparency
		uint16_t currentcolor;
		for(int hcnt = 0;hcnt < FB_bitmapheight;hcnt++){
			for(int wcnt = 0;wcnt < FB_bitmapwidth;wcnt++){
				currentcolor = FB_bitmappixel(wcnt,hcnt);
				if(currentcolor != FB_bitmaptransparentcolor)FB_pixel(x + wcnt,y + hcnt) = currentcolor;
			}
		}
	}
	else FB_copybitmap(x,y);//just copy raw data
}
