#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
	pixel order is *((unsigned char*)(surface->parray) + pixel_num*4 + 0) = blue channel
				   *((unsigned char*)(surface->parray) + pixel_num*4 + 1) = green channel
				   *((unsigned char*)(surface->parray) + pixel_num*4 + 2) = red channel
				   *((unsigned char*)(surface->parray) + pixel_num*4 + 3) = alpha channel

*/

typedef struct I_Surface Surface;

struct MwmHints 
{
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
};

enum 
{
    MWM_HINTS_FUNCTIONS = (1L << 0),
    MWM_HINTS_DECORATIONS =  (1L << 1),

    MWM_FUNC_ALL = (1L << 0),
    MWM_FUNC_RESIZE = (1L << 1),
    MWM_FUNC_MOVE = (1L << 2),
    MWM_FUNC_MINIMIZE = (1L << 3),
    MWM_FUNC_MAXIMIZE = (1L << 4),
    MWM_FUNC_CLOSE = (1L << 5)
};

typedef struct I_R_Window R_Window;

typedef struct Timer
{
	struct timespec ts_start;
    struct timespec ts_end;
    double elapsedTime;
    char timer_id[128];
}Timer;

Timer create_timer(const char* timer_id)
{
	Timer t;
	strcpy(t.timer_id, timer_id);
	return t;
}

void timer_start(Timer* timer)
{
	clock_gettime(CLOCK_MONOTONIC, &timer->ts_start);
}

double timer_end(Timer* timer)
{
	clock_gettime(CLOCK_MONOTONIC, &timer->ts_end);
	timer->elapsedTime = ((timer->ts_end.tv_nsec - timer->ts_start.tv_nsec) > 0) ? 
	(((timer->ts_end.tv_nsec - timer->ts_start.tv_nsec) / 1000000.0) + ((timer->ts_end.tv_sec - timer->ts_start.tv_sec)*1000) ) : 
	(((1000000000 + timer->ts_end.tv_nsec - timer->ts_start.tv_nsec) / 1000000.0) + (( (timer->ts_end.tv_sec - 1) - timer->ts_start.tv_sec)*1000) );
	printf("timer with id: %s recorded a time of: %f ms\n", timer->timer_id, timer->elapsedTime);
	return timer->elapsedTime;
}

typedef struct Color
{
	unsigned char r, g, b, a;
}Color;

typedef struct Point
{
	int x;
	int y;
}Point;

typedef struct Pixel
{
	Point p;
	Color c;
}Pixel;

struct I_R_Window
{
	Window w;
	Display* display;
	XVisualInfo vinfo;
	GC gc;
	int width;
	int height;
	XSetWindowAttributes attr;
	void (*set_position) (R_Window*, int, int);
	void (*set_decorations) (R_Window*, int);
	void (*clear)  (R_Window*);
};

struct I_Surface
{
	int w, h;
	unsigned int* parray;
	
	//drawing functions
	void (*put_pixel) (Surface*, Point, Color);
	void (*line) (Surface*, Point, Point, Color);
	void (*ellipse) (Surface*, Point, int, int, Color);
	void (*circle) (Surface*, Point, int, Color);
	void (*bezier) (Surface*, Point, Point, Point, Color);
	void (*triangle) (Surface*, Point, Point, Point, Color);
	void (*rect) (Surface*, Point, Point, Point, Point, Color);
	void (*polygon) (Surface*, Point*, int, Color);
	//croping, scaling, and rotating functions
	void (*resize) (Surface*, int, int);
	void (*get_cur_size) (Surface*, int*, int*);
	//bluring functions
	
	//contrast and brightening functions
	void (*set_color) (Surface*, Color);
	void (*brighten) (Surface*, int);
	void (*contrast) (Surface*, int);
	void (*grayscale) (Surface*);
	void (*gamma) (Surface*, float);
	//rendering/blending functions
	void (*render_to_window) (Surface*, R_Window*, int, int, int, int);
	void (*render_to_surface) (Surface*, Surface*, int, int, int, int);
	void (*adjust_alpha) (Surface*, int);
};

//**************************** Internal Functions *********************************************************
void clear_pixmap(Display* disp, XVisualInfo vi, Drawable p, int x, int y, int w, int h)				//*
{																										//*
	Picture pict = XRenderCreatePicture(disp, p, XRenderFindVisualFormat(disp, vi.visual), 0, 0);		//*
	XRenderColor col = { .red=0, .green=0, .blue=0, .alpha=0 };											//*
	XRenderFillRectangle(disp, PictOpSrc, pict, &col, x, y, w, h);										//*
	XRenderFreePicture(disp, pict);																		//*
}																										//*
																										//*
void put_pixel(unsigned int* data, int w, int x, int y, Color c)										//*
{																										//*
	data[y * w + x] = (c.a << 24)|(c.r << 16)|(c.g  << 8)|(c.b << 0);									//*
}

void quad_bezier_internal(Surface* s, Point p1, Point p2, Point p3, Color c)
{                             
	/* plot a limited quadratic Bezier segment */
	int sx = p3.x-p2.x, sy = p3.y-p2.y;
	long xx = p1.x-p2.x, yy = p1.y-p2.y, xy;         /* relative values for checks */
	double dx, dy, err, cur = xx*sy-yy*sx;        /* curvature */
	assert(xx*sx <= 0 && yy*sy <= 0);  /* sign of gradient must not change */
	if(sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) 
	{ 
		/* begin with longer part */
		p3.x = p1.x; 
		p1.x = sx+p2.x; 
		p3.y = p1.y; 
		p1.y = sy+p2.y; 
		cur = -cur;
		/* swap P0 P2 */
	}  
	if(cur != 0) 
	{                              
		/* no straight line */
		xx += sx; xx *= sx = (p1.x < p3.x) ? 1 : -1;     /* x step direction */
		yy += sy; yy *= sy = (p1.y < p3.y) ? 1 : -1;     /* y step direction */
		xy = 2*xx*yy; xx *= xx; yy *= yy;          /* differences 2nd degree */
		if(cur*sx*sy < 0) 
		{                       
		    /* negated curvature? */
		     xx = -xx; 
		     yy = -yy; 
		     xy = -xy; 
		     cur = -cur;
		}
		dx = 4.0*sy*cur*(p2.x-p1.x)+xx-xy;             /* differences 1st degree */
		dy = 4.0*sx*cur*(p1.y-p2.y)+yy-xy;
		xx += xx; 
		yy += yy; 
		err = dx+dy+xy; 							/* error 1st step */
		do
		{                              
			put_pixel(s->parray, s->w, p1.x, p1.y, c);                           /* plot curve */
			if(p1.x == p3.x && p1.y == p3.y) 
				return;  			  /* last pixel -> curve finished */
			p2.y = 2*err < dx;         /* save value for test of y step */
			if(2*err > dy) 
			{ 
				p1.x += sx; 
				dx -= xy; 
				err += dy += yy; 
			} 
			/* x step */
			if(y1) 
			{ 
				p1.y += sy; 
				dy -= xy; 
				err += dx += xx; 
			} 
			/* y step */
		}while(dy < 0 && dx > 0);   
		/* gradient negates -> algorithm fails */
	}  
	int line_dx =  abs(p3.x-p1.x), line_sx = p1.x<p3.x ? 1 : -1;
	int line_dy = -abs(p3.y-p1.y), line_sy = p1.y<p3.y ? 1 : -1; 
	int line_err = line_dx+line_dy, e2; /* error value e_xy */

	for(;;){  /* loop */
		put_pixel(s->parray, s->w, p1.x, p1.y, c);
		if (p1.x==p3.x && p1.y==p3.y) break;
		e2 = 2*line_err;
		if (e2 >= line_dy) { line_err += line_dy; p1.x += line_sx; } /* e_xy+e_x > 0 */
		if (e2 <= line_dx) { line_err += line_dx; p1.y += line_sy; } /* e_xy+e_y < 0 */
	}         
	/* plot remaining part to end */
}																										//*

//*********************************************************************************************************


void I_set_window_location(R_Window* w, int x, int y)
{
	XSizeHints s_hints;
	s_hints.flags = PPosition | PSize;
	s_hints.x = 0;
	s_hints.y = 24;
	s_hints.width = 400;
	s_hints.height = 400;
	XSetNormalHints(w->display, w->w, &s_hints);
}

void I_set_window_decorations(R_Window* w, int is_decorated)
{
	struct MwmHints d_hints;
	d_hints.flags = MWM_HINTS_DECORATIONS;
	int test = (is_decorated > 0) ? 1 : 0;
	d_hints.decorations = test;
	Atom mwmHintsProperty = XInternAtom(w->display, "_MOTIF_WM_HINTS", 0);
	XChangeProperty(w->display, w->w, mwmHintsProperty, mwmHintsProperty, 32, PropModeReplace, (unsigned char *)&d_hints, 5);
}

void I_clear_window(R_Window* w)
{
	clear_pixmap(w->display, w->vinfo, w->w, 0, 0, w->width, w->height);
}

R_Window* create_window(int w, int h)
{
	Display* display;
	XVisualInfo vinfo;
	GC gc;
	XSetWindowAttributes attr;
	display = XOpenDisplay(NULL);
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
    attr.event_mask = ExposureMask;
    attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    Window win = XCreateWindow(display, DefaultRootWindow(display), 0, 0, w, h, 0, vinfo.depth, InputOutput, vinfo.visual, CWEventMask | CWColormap | CWBorderPixel, &attr);
    gc = XCreateGC(display, win, 0, 0);
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, win, &wm_delete_window, 1);
    XMapWindow(display, win);
    R_Window* window = malloc(1*sizeof(struct I_R_Window));
    window->width = w;
    window->height = h;
    window->w = win;
    window->display = display;
    window->attr = attr;
    window->vinfo = vinfo;
    window->gc = gc;
    
    window->set_position = I_set_window_location;
    window->set_decorations = I_set_window_decorations;
    window->clear = I_clear_window;
    return window;
}

void free_window(R_Window* win)
{
	free(win);
}


//########################### Drawing Functions#########################
void I_put_pixel(Surface* img, Point p, Color c)
{
	put_pixel(img->parray, img->w, p.x, p.y, c);
}

void I_line(Surface* img, Point p1, Point p2, Color c)
{
	int dx =  abs(p2.x-p1.x), sx = p1.x<p2.x ? 1 : -1;
	int dy = -abs(p2.y-p1.y), sy = p1.y<p2.y ? 1 : -1; 
	int err = dx+dy, e2; /* error value e_xy */

	for(;;){  /* loop */
		put_pixel(img->parray, img->w, p1.x, p1.y, c);
		if (p1.x==p2.x && p1.y==p2.y) break;
		e2 = 2*err;
		if (e2 >= dy) { err += dy; p1.x += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; p1.y += sy; } /* e_xy+e_y < 0 */
	}
}

void I_ellipse(Surface* img, Point center, int a, int b, Color c)
{
	long x = -a, y = 0; /* II. quadrant from bottom left to top right */
	long e2 = b, dx = (1+2*x)*e2*e2;              /* error increment  */
	long dy = x*x, err = dx+dy;                    /* error of 1.step */
	do
	{
		put_pixel(img->parray, img->w, center.x-x, center.y+y, c);                        /*   I. Quadrant */
		put_pixel(img->parray, img->w, center.x+x, center.y+y, c);                        /*  II. Quadrant */
		put_pixel(img->parray, img->w, center.x+x, center.y-y, c);                        /* III. Quadrant */
		put_pixel(img->parray, img->w, center.x-x, center.y-y, c);                        /*  IV. Quadrant */
        e2 = 2*err;
        if(e2 >= dx) 
        { 
			x++; 
			err += dx += 2*(long)b*b; 
		}/* x step */
		if(e2 <= dy) 
		{ 
			y++;
			err += dy += 2*(long)a*a; 
		}/* y step */
   }while(x <= 0);
   
   while(y++ < b) 
   {    
		/* to early stop for flat ellipses with a=1, */
		put_pixel(img->parray, img->w, center.x, center.y+y, c);               
		/*-> finish tip of ellipse */
		put_pixel(img->parray, img->w, center.x, center.y-y, c);
   }
}

void I_circle(Surface* img, Point center, int r, Color c)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
      put_pixel(img->parray, img->w, center.x-x, center.y+y, c); /*   I. Quadrant */
      put_pixel(img->parray, img->w, center.x-y, center.y-x, c); /*  II. Quadrant */
      put_pixel(img->parray, img->w, center.x+x, center.y-y, c); /* III. Quadrant */
      put_pixel(img->parray, img->w, center.x+y, center.y+x, c); /*  IV. Quadrant */
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}

void I_bezier(Surface* img, Point p1, Point p2, Point p3, Color c)
{                                  
	/* plot any quadratic Bezier curve */
	int x = p1.x-p2.x, y = p1.y-p2.y;
	double t = p1.x-2*p2.x+p3.x, r;
	if((long)x*(p3.x-p2.x) > 0) 
	{                
		if((long)y*(p3.y-p2.y) > 0)         /* horizontal cut at P4? */    
			if(fabs((p1.y-2*p2.y+p3.y)/t*x) > abs(y))	/* vertical cut at P6 too? */
			{       
				/* which first? */
				p1.x = p3.x;
				p3.x = x+p2.x; 
				p1.y = p3.y; 
				p3.y = y+p2.y;    
				/* swap points */
			}                    
		/* now horizontal cut at P4 comes first */
		t = (p1.x-p2.x)/t;
		r = (1-t)*((1-t)*p1.y+2.0*t*p2.y)+t*t*p3.y;               
		/* By(t=P4) */
		t = (p1.x*p3.x-p2.x*p2.x)*t/(p1.x-p2.x);               
		/* gradient dP4/dx=0 */
		x = floor(t+0.5); 
		y = floor(r+0.5);
		r = (p2.y-p1.y)*(t-p1.x)/(p2.x-p1.x)+p1.y;          
		/* intersect P3 | P0 P1 */
		Point i_p = {.x = x, .y = floor(r+0.5)};
		Point i_p1 = {.x = x, .y = y};
		quad_bezier_internal(img, p1, i_p, i_p1, c);
		r = (p2.y-p3.y)*(t-p3.x)/(p2.x-p3.x)+p3.y;          
		/* intersect P4 | P1 P2 */
		p1.x = p2.x = x; 
		p1.y = y; 
		p2.y = floor(r+0.5);     
		/* P0 = P4, P1 = P8 */
    } 
	if((long)(p1.y-p2.y)*(p3.y-p2.y) > 0) 
	{            
		/* vertical cut at P6? */
	    t = p1.y-2*p2.y+p3.y; t = (p1.y-p2.y)/t; 
	    r = (1-t)*((1-t)*p1.x+2.0*t*p2.x)+t*t*p3.x;               
		/* Bx(t=P6) */
	    t = (p1.y*p3.y-p2.y*p2.y)*t/(p1.y-p2.y);               
		/* gradient dP6/dy=0 */
	    x = floor(r+0.5); 
	    y = floor(t+0.5);
	    r = (p2.x-p1.x)*(t-p1.y)/(p2.y-p1.y)+p1.x;          
		/* intersect P6 | P0 P1 */
		Point i_p = {.x = floor(r+0.5), .y = y};
		Point i_p1 = {.x = x, .y = y};
		quad_bezier_internal(img, p1, i_p, i_p1, c);
	    r = (p2.x-p3.x)*(t-p3.y)/(p2.y-p3.y)+p3.x;          
		/* intersect P7 | P1 P2 */
	    p1.x = x; 
	    p2.x = floor(r+0.5); 
	    p1.y = p2.y = y;     
		/* P0 = P6, P1 = P7 */
	} 
	quad_bezier_internal(img, p1, p2, p3, c);          
	/* remaining part */
}

void I_triangle(Surface* img, Point p1, Point p2, Point p3, Color c)
{
	img->line(img, p1, p2, c);
	img->line(img, p2, p3, c);
	img->line(img, p3, p1, c);
}

void I_rect(Surface* img, Point p1, Point p2, Point p3, Point p4, Color c)
{
	img->line(img, p1, p2, c);
	img->line(img, p2, p3, c);
	img->line(img, p3, p4, c);
	img->line(img, p4, p1, c);
}

void I_polygon(Surface* img, Point* parray, int sz, Color c)
{
	int no_of_sides = sz;
	for(int iter = 0; iter < (no_of_sides - 1); iter++)
	{
		img->line(img, parray[iter], parray[iter + 1], c);
	}
	img->line(img, parray[no_of_sides - 1], parray[0], c);
}


//############################# Sizing and rotating functions ##########
void I_resize(Surface* s, int w, int h)
{
	s->parray = realloc(s->parray, (w*h*sizeof(unsigned int)));
	s->w = w;
	s->h = h;
}

void I_get_cur_size(Surface* s, int* w, int* h)
{
	*w = s->w;
	*h = s->h;
}

void I_set_to_color(Surface* s, Color c)
{
	for(int i = 0; i < (s->w*s->h); i++)
	{
		*(s->parray + i) = (c.a << 24)|(c.r << 16)|(c.g  << 8)|(c.b << 0);
	}
}

//void I_rotate(Surface* s, 


//############################## Rendering and blending functions ######
void I_render_to_window(Surface* img, R_Window* win, int x, int y, int w, int h)
{
	int w_i, h_i;
	w_i = (w <= img->w) ? w : img->w;
	h_i = (h <= img->h) ? h : img->h;
	w_i = ((w_i + x) <= win->width) ? w_i : (win->width - x);
	h_i = ((h_i + y) <= win->height) ? h_i : (win->height - y);
	//printf("w_i is: %d\n and h_i is: %d\n and img->h is: %d\n and img->w is: %d\n", w_i, h_i, img->h, img->w);
	unsigned char* imdata = malloc(w_i*h_i*sizeof(unsigned int));
	//for(int i = 0; i < w*h; i++)
	//{
		//unsigned int p = *(img->parray + i);
		//*(imdata + (i * 4) + 0) = (p >> 24) & 0xFF;
		//*(imdata + (i * 4) + 1) = (p >> 16) & 0xFF;
		//*(imdata + (i * 4) + 2) = (p >> 8) & 0xFF;
		//*(imdata + (i * 4) + 3) = p & 0xFF;
	//}
	if(w_i == img->w)
	{
		memcpy((void*)imdata, (const void*)img->parray, w_i*h_i*4);
	}
	else
	{
		for(int i = 0; i < h_i; i++)
		{
			memcpy((void*)((unsigned char*)(imdata) + i*w_i*4), (const void*)((unsigned char*)(img->parray) + i*img->w*4), w_i*4);
		}
	}
	
	XImage *ximg = XCreateImage(win->display, CopyFromParent, 32, ZPixmap, 0, imdata, w_i, h_i, 32, 0);
	Pixmap pmap_tmp = XCreatePixmap(win->display, DefaultRootWindow(win->display), w_i, h_i, 32);
	GC gc = XCreateGC(win->display, pmap_tmp, 0, 0);
	XPutImage(win->display, pmap_tmp, gc, ximg, 0, 0, 0, 0, w_i, h_i);
	XDestroyImage(ximg);	//we dont have to free(imdata) because xdestroyimage does that for us
	XFreeGC(win->display, gc);
	Picture pict_image = XRenderCreatePicture(win->display, pmap_tmp, XRenderFindStandardFormat(win->display, PictStandardARGB32), 0, 0);
	Picture pict_drawable = XRenderCreatePicture(win->display, win->w, XRenderFindVisualFormat(win->display, win->vinfo.visual), 0, 0);
	XRenderComposite(win->display, PictOpIn, pict_image, None, pict_image, 0, 0, 0, 0, 0, 0, w_i, h_i);
	XRenderComposite(win->display, PictOpOver, pict_image, None, pict_drawable, 0, 0, 0, 0, x, y, w_i, h_i);
	XFreePixmap(win->display, pmap_tmp);
	XRenderFreePicture(win->display, pict_image);
	XRenderFreePicture(win->display, pict_drawable);
}

void I_render_to_surface(Surface* src, Surface* dest, int x, int y, int w, int h)
{
	//must alpha blend src onto dest use the fast formula {C_out = dest_in + src_alpha(src_in - dest_in)}
	//does not modify src in any way only blends it onto dest. does modify dest pixel values
	int i_w = ((w + x) > src->w) ? src->w : w; //make sure that the width given + the x offset isnt bigger than the width of the image to be rendered
	i_w = ((i_w + x) > dest->w) ? dest->w : i_w; //make sure that the width given + the x offset isnt bigger than the width of the dest image
	int i_h = ((h + y) > src->h) ? src->h : h;
	i_h = ((i_h + y) > dest->h) ? dest->h : i_h;
	int dest_start = (y*dest->w + x)*4;
	//should optimize with SIMD also should parallelize and possibly for large images even consider CUDA/OpenCL
	if(src->w == i_w)
	{
		for(int i = 0; i < i_w*i_h*4; i += 4)
		{
			float src_alpha_i = (*((unsigned char*)(src->parray) + i + 3) / 255.0); //stored as ARGB alpha bit is first but is little endian so BGRA is how it is stored in memory
			*((unsigned char*)(dest->parray) + dest_start + i + 0) = *((unsigned char*)(dest->parray) + i + 0) 
			+ src_alpha_i*((*((unsigned char*)(src->parray) + i + 0)) - (*((unsigned char*)(dest->parray) + i + 0)));
			*((unsigned char*)(dest->parray) + dest_start + i + 1) = *((unsigned char*)(dest->parray) + i + 1) 
			+ src_alpha_i*((*((unsigned char*)(src->parray) + i + 1)) - (*((unsigned char*)(dest->parray) + i + 1)));
			*((unsigned char*)(dest->parray) + dest_start + i + 2) = *((unsigned char*)(dest->parray) + i + 2) 
			+ src_alpha_i*((*((unsigned char*)(src->parray) + i + 2)) - (*((unsigned char*)(dest->parray) + i + 2)));
		}
	}
	else
	{
		for(int i = 0; i < i_h; i++)
		{
			for(int p = 0; p < i_w*4; p += 4)
			{
				float src_alpha_i = (*((unsigned char*)(src->parray) + (i*src->w + p) + 3) / 255.0); //stored as ARGB alpha bit is first but is little endian so BGRA is how it is stored in memory
				*((unsigned char*)(dest->parray) + dest_start + (i*i_w + p) + 0) = *((unsigned char*)(dest->parray) + i + 0) 
				+ src_alpha_i*((*((unsigned char*)(src->parray) + (i*src->w + p) + 0)) - (*((unsigned char*)(dest->parray) + i + 0)));
				*((unsigned char*)(dest->parray) + dest_start + (i*i_w + p) + 1) = *((unsigned char*)(dest->parray) + i + 1) 
				+ src_alpha_i*((*((unsigned char*)(src->parray) + (i*src->w + p) + 1)) - (*((unsigned char*)(dest->parray) + i + 1)));
				*((unsigned char*)(dest->parray) + dest_start + (i*i_w + p) + 2) = *((unsigned char*)(dest->parray) + i + 2) 
				+ src_alpha_i*((*((unsigned char*)(src->parray) + (i*src->w + p) + 2)) - (*((unsigned char*)(dest->parray) + i + 2)));
			}
		}
	}
	
}

void I_adjust_alpha(Surface* surface, int alpha_increase)
{
	int __alpha = (alpha_increase > 255) ? 255 : ((alpha_increase < -255) ? -255 : alpha_increase);
	int  __a;
	for(int i = 0; i < surface->w*surface->h*4; i += 4)
	{
		__a = *((unsigned char*)(surface->parray) + i + 3);
		__a = ((__a + __alpha) > 255) ? 255 : (((__a + __alpha) < 0) ? 0 : (__a + __alpha));
		*((unsigned char*)(surface->parray) + i + 3) = (unsigned char)(__a);
	}
}
//############################# bluring and brigtening functions #######
void I_brighten(Surface* img, int brightness)
{
	//red and blue channels should be swapped 
	int ib = (brightness > 255) ? 255 : brightness;
	ib = (brightness < -255) ? -255 : brightness;
	int r, g, b;
	for(int i = 0; i < img->w*img->h*4; i += 4)
	{
		r = *((unsigned char*)(img->parray) + i);
		g = *((unsigned char*)(img->parray) + i + 1); 
		b = *((unsigned char*)(img->parray) + i + 2); 
		r = ((r + ib) > 255) ? 255 : (r + ib);
		r = (r < 0) ? 0 : r;
		g = ((g + ib) > 255) ? 255 : (g + ib);
		g = (g < 0) ? 0 : g;
		b = ((b + ib) > 255) ? 255 : (b + ib);
		b = (b < 0) ? 0 : b;
		*((unsigned char*)(img->parray) + i) = r; 
		*((unsigned char*)(img->parray) + i + 1) = g; 
		*((unsigned char*)(img->parray) + i + 2) = b; 
	}
}

void I_box_blur(Surface* s, int w, int h, int r)
{
	
}

void I_gausian_blur()
{
	
}

void I_contrast(Surface* s, int contrast)
{
	int ib = (contrast > 255) ? 255 : contrast;
	ib = (contrast < -255) ? -255 : contrast;
	int r, g, b;
	float k = ((float)(259*(ib + 255)))/((float)(255*(259 - ib)));
	for(int i = 0; i < s->w*s->h*4; i += 4)
	{
		r = *((unsigned char*)(s->parray) + i); 
		g = *((unsigned char*)(s->parray) + i + 1); 
		b = *((unsigned char*)(s->parray) + i + 2); 
		r = ((k*(r - 128) + 128) > 255.0) ? 255 : (int)floorf(k*(r - 128) + 128);
		r = (r < 0) ? 0 : r;
		g = ((k*(g - 128) + 128) > 255.0) ? 255 : (int)floorf(k*(g - 128) + 128);
		g = (g < 0) ? 0 : g;
		b = ((k*(b - 128) + 128) > 255.0) ? 255 : (int)floorf(k*(b - 128) + 128);
		b = (b < 0) ? 0 : b;
		*((unsigned char*)(s->parray) + i) = r; 
		*((unsigned char*)(s->parray) + i + 1) = g; 
		*((unsigned char*)(s->parray) + i + 2) = b; 
	}
}

void I_grayscale(Surface* s)
{
	int new_val, r, g, b;
	for(int i = 0; i < s->w*s->h; i++)
	{
		b = *((unsigned char*)(s->parray) + i*4 + 0);
		g = *((unsigned char*)(s->parray) + i*4 + 1);
		r = *((unsigned char*)(s->parray) + i*4 + 2);
		new_val = floorf( (0.299)*(float)(r) + (0.587)*(float)(g) + (0.114)*(float)(b) );
		*((unsigned char*)(s->parray) + i*4 + 0) = (unsigned char)(new_val);
		*((unsigned char*)(s->parray) + i*4 + 1) = (unsigned char)(new_val);
		*((unsigned char*)(s->parray) + i*4 + 2) = (unsigned char)(new_val);
	}
}

void I_gamma(Surface* s, float gamma_val)
{
	float gamma_correction = 1.0 / gamma_val;
	printf("gamma correction is now: %f\n", gamma_correction);
	int r, g, b;
	int i = 0;
	while(i < s->w*s->h*4)
	{
		//i += 100000;
		b = *((unsigned char*)(s->parray) + i + 0);
		g = *((unsigned char*)(s->parray) + i + 1);
		r = *((unsigned char*)(s->parray) + i + 2);
		//printf("red channel variable was: %d\n", r);
		b = floor(255.0 * pow((double)(b / 255.0), (double)(gamma_correction)));
		g = floor(255.0 * pow((double)(g / 255.0), (double)(gamma_correction)));
		r = floor(255.0 * pow((double)(r / 255.0), (double)(gamma_correction)));
		//printf("red channel variable is: %d\n", r);
		*((unsigned char*)(s->parray) + i + 0) = (unsigned char)(b);
		*((unsigned char*)(s->parray) + i + 1) = (unsigned char)(g);
		*((unsigned char*)(s->parray) + i + 2) = (unsigned char)(r);
		//printf("actual value of red channel after gamma correction is: %d\n", *((unsigned char*)(s->parray) + i + 2));
		i += 4;
	}
}

//############################ Create and destroy methods ##############
void static inline internal_function_set(Surface* s)
{
	s->put_pixel = I_put_pixel;
	s->line = I_line;
	s->ellipse = I_ellipse;
	s->circle = I_circle;
	s->triangle = I_triangle;
	s->rect = I_rect;
	s->polygon = I_polygon;
	s->brighten = I_brighten;
	s->contrast = I_contrast;
	s->grayscale = I_grayscale;
	s->gamma = I_gamma;
	s->bezier = I_bezier;
	s->render_to_surface = I_render_to_surface;
	s->resize = I_resize;
	s->set_color = I_set_to_color;
	s->render_to_window = I_render_to_window;
	s->adjust_alpha = I_adjust_alpha;
}

Surface* create_surface(void)
{
	Surface* s = malloc(1*sizeof(struct I_Surface));
	s->w = 0;
	s->h = 0;
	s->parray = malloc(0);
	internal_function_set(s);
	return s;
}

Surface* create_surface_from_data_ARGB(int w, int h, void* data)
{
	Surface* s = malloc(1*sizeof(struct I_Surface));
	s->w = w;
	s->h = h;
	s->parray = malloc(w*h*4);
	memcpy(s->parray, data, w*h*4);
	internal_function_set(s);
	return s;
}

Surface* create_surface_from_image(const char* filename)
{
	int w, h, bpp;
	unsigned char* imdata = stbi_load(filename, &w, &h, &bpp, 4);
	printf("bpp is: %d\n", bpp);
	Surface* s = malloc(1*sizeof(struct I_Surface));
	s->w = w;
	s->h = h;
	s->parray = malloc(w*h*sizeof(int));
	if(imdata == NULL)
	{
		printf("error loading image\n");
		goto fail;
	}
		
	//printf("\nsanity check imdata contents: %x %x %x %x\n in decimal: %d %d %d %d\n\n", *(imdata + 100 + 0), *(imdata + 100 + 1),
	//*(imdata + 100 + 2), *(imdata + 100 + 3), *(imdata + 100 + 0), *(imdata + 100 + 1), *(imdata + 100 + 2), *(imdata + 100 + 3));
	int r, g, b, a;
	if(bpp == 3)
	{
		a = 255;
		for(int i = 0; i<w*h*4;)
		{
			r = *(imdata + i + 0);		//red
			g = *(imdata + i + 1);		//green
			b = *(imdata + i + 2);		//blue
			
			*((unsigned char*)(s->parray) + i + 0) = (unsigned char)b;
			*((unsigned char*)(s->parray) + i + 1) = (unsigned char)g;
			*((unsigned char*)(s->parray) + i + 2) = (unsigned char)r;
			*((unsigned char*)(s->parray) + i + 3) = (unsigned char)a;
			i += 4;
		}
	}
	else
	{
		for(int i = 0; i<w*h*4;)
		{
			r = *(imdata + i + 0);		//red
			g = *(imdata + i + 1);		//green
			b = *(imdata + i + 2);		//blue
			a = *(imdata + i + 3);		//alpha
		
			*((unsigned char*)(s->parray) + i + 0) = (unsigned char)b;
			*((unsigned char*)(s->parray) + i + 1) = (unsigned char)g;
			*((unsigned char*)(s->parray) + i + 2) = (unsigned char)r;
			*((unsigned char*)(s->parray) + i + 3) = (unsigned char)a;
			i += 4;
		}
	}
	
	
	//printf("\ndebug check parray contents: %x %x %x %x\n in decimal: %d %d %d %d\n\n", *((unsigned char*)(s->parray) + 100 + 0), 
	//*((unsigned char*)(s->parray) + 100 + 1), *((unsigned char*)(s->parray) + 100 + 2), *((unsigned char*)(s->parray) + 100 + 3), 
	//*((unsigned char*)(s->parray) + 100 + 0), *((unsigned char*)(s->parray) + 100 + 1), *((unsigned char*)(s->parray) + 100 + 2), 
	//*((unsigned char*)(s->parray) + 100 + 3));
	
	stbi_image_free(imdata);
	fail:;
	internal_function_set(s);
	return s;
}

void free_surface(Surface* s)
{
	s->w = 0;
	s->h = 0;
	free(s->parray);
	free(s);
}


int main(int argc, char* argv[])
{
	Color c_back = {.r = 0x81, .g = 0x50, .b = 0xfe, .a = 255};
	Color dest_back = {.r = 128, .g = 128, .b = 128, .a = 255};
	R_Window* win = create_window(680, 340);
	Surface* img = create_surface_from_image("picture.png");
	Surface* dest = create_surface();
	dest->resize(dest, 340, 340);
	//Surface* img = create_surface();
	//img->resize(img, 300, 300);
	//img->set_color(img, c_back);
	dest->set_color(dest, dest_back);
	printf("color is: %x\n", *((unsigned char*)(img->parray) + 3));
	//render_surface_onto_window(win, &img, 20, 20, 30, 30);
	//free_image(img);
	XFlush(win->display);
    int keep_running = 1;
    XEvent event;

    while (keep_running) {
        XNextEvent(win->display, &event);

        switch(event.type) {
            case ClientMessage:
                if (event.xclient.message_type == XInternAtom(win->display, "WM_PROTOCOLS", 1) && (Atom)event.xclient.data.l[0] == XInternAtom(win->display, "WM_DELETE_WINDOW", 1))
                    keep_running = 0;

                break;
			case Expose:
			{
				double total;
				
				Point ptr[3];
				int num = 3;
				ptr[0] = (Point){.x = 150, .y = 100};
				ptr[1] = (Point){.x = 200, .y = 150};
				ptr[2] = (Point){.x = 100, .y = 150};
				Color col = {.r = 255, .g = 0, .b = 0, .a = 255};
				Color col2 = {.r = 0, .g = 128, .b = 128, .a = 255};
				Color col3 = {.r = 0, .g = 255, .b = 0, .a = 255};
				Point center = {.x = 150, .y = 133};
				Point e_center = {.x = 100, .y = 100};
				win->clear(win);
				Timer t1 = create_timer("rasterization time for 100000");		
				timer_start(&t1);
				img->adjust_alpha(img, -128);
				for(int i = 0; i<100000; i++)
				{
					img->polygon(img, ptr, num, col);
					img->circle(img, center, 52, col2);
					img->ellipse(img, e_center, 50, 75, col3);
					img->line(img, center, e_center, col);
					img->bezier(img, ptr[0], ptr[1], ptr[2], col2);
				}
				//img->contrast(img, 50);
				//img->brighten(img, -100);
				//img->grayscale(img);
				img->gamma(img, 2.0);
				img->render_to_surface(img, dest, 10, 10, 320, 320);
				total = timer_end(&t1);
				printf("100000 rasterizations of a polygon, circle, ellipse, line, and a "
				"random increase in contrast took: %fms in total\n and %fms per rasterization group\n", total, (total/100000.0));
				Timer t2 = create_timer("render time for 1");
				timer_start(&t2);
				//for(int i = 0; i < 100000; i++)
				//{
					img->render_to_window(img, win, 20, 20, 300, 300);
					dest->render_to_window(dest, win, 340, 10, 340, 340);
				//}
				total = timer_end(&t2);
				//printf("total render time for 100000 render operations was: %fms\n which is %fms per operation\n", total, (total/100000.0));
				free_surface(img);
				keep_running = 1;
				break;
			}

            default:
				keep_running = 1;
                break;
        }
    }

    XDestroyWindow(win->display, win->w);
    XCloseDisplay(win->display);
    return 0;
}
