/* Lear's GIST implementation, version 1.1, (c) INRIA 2009, Licence: PSFL */


#ifndef GIST_H_INCLUDED
#define GIST_H_INCLUDED


//#include "standalone_image.h"
typedef struct {
  int width,height;     /* dimensions */
  int stride;		/* width of image in memory */

  float *data;		/* data in latin reading order */
} image_t;

typedef struct {
  int width,height;     /* here, stride = width */
  
  float *c1;		/* R */
  float *c2;		/* G */
  float *c3;		/* B */
  
} color_image_t;

image_t *image_new(int width, int height);

image_t *image_cpy(image_t *src);

void image_delete(image_t *image);


color_image_t *color_image_new(int width, int height);

color_image_t *color_image_cpy(color_image_t *src);

void color_image_delete(color_image_t *image);


typedef struct {
  int size;			/* Number of images in the list */
  int alloc_size;		/* Number of allocated images */

  image_t **data;		/* List of images */

} image_list_t;


image_list_t *image_list_new(void);

void image_list_append(image_list_t *list, image_t *image);

void image_list_delete(image_list_t *list);


/*! Graylevel GIST for various scales. Based on Torralba's Matlab
 * implementation. http://people.csail.mit.edu/torralba/code/spatialenvelope/
 *
 * Descriptor size is  w*w*sum(n_orientations[i],i=0..n_scale-1)
 *
 *    @param src Source image
 *    @param w Number of bins in x and y axis
 */

float *bw_gist_scaletab(image_t *src, int nblocks, int n_scale, const int *n_orientations);

/*! @brief implementation of grayscale GIST descriptor.
 * Descriptor size is w*w*(a+b+c)
 *
 *    @param src Source image
 *    @param w Number of bins in x and y axis
 */
float *bw_gist(image_t *scr, int nblocks, int a, int b, int c);

/*! @brief implementation of color GIST descriptor.
 *
 *    @param src Source image
 *    @param w Number of bins in x and y axis
 */
float *color_gist(color_image_t *src, int nblocks, int a, int b, int c);

/*! Color GIST for various scales. Based on Torralba's Matlab
 * implementation. http://people.csail.mit.edu/torralba/code/spatialenvelope/  */

float *color_gist_scaletab(color_image_t *src, int nblocks, int n_scale, const int *n_orientations);




#endif
