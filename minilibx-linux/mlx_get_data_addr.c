/*
** mlx_get_data_addr.c for MiniLibX in raytraceur
** 
** Made by Charlie Root
** Login   <ol@epitech.net>
** 
** Started on  Mon Aug 14 15:45:57 2000 Charlie Root
** Last update Thu Sep 27 19:05:25 2001 Charlie Root
*/



#include	"mlx_int.h"

/**
 * @brief The function returns a pointer to the raw
 * pixel buffer of an image created by `mlx_new_image`.
 * it fills n the vlaues for bits per pixel, size of each
 * image line in bytes, and the endian-ness of the image
 * data
 * @param img pointer returned by `mlx_new_image`
 * @param bits_per_pixel pointer to int, will be set to nbr of bits per pixel
 * @param size_line pointer to int will be set to the nbr of bytes in a row
 * @param endian pointer to int, will be set to the endian-ness (0 : 1)
 * @return a pointer to the start of the image's pixel buffer in memory
 * SUMMARY:
 * it gives us a direct access to the image's pixel data
 * so we can read or write pixels
 * it provides infor about how the data is laid out (bpp,
 * line size, endian).
 * @note
 * it allows us to draw pixels ourself: we can set the color of any pixel
 * in the image by writing directly to the buffer.
 * we can draw lines, shapes, gradients, or anything else by manipulating the pixel data
 * it's much faster to update many pixels in memory and thend display the image, rather than calling
 * a function like mlx_pixel_put() for every pixel
 * 
 * in concrete, if we want to redraw a red pixel at (10, 10)
 * we use this functon to get the buffer, then write the color value
 * at the correct offset for (10,10). this is how we build up images,
 * render shapes, or do animations efficiently.
 */
char	*mlx_get_data_addr(t_img *img,int *bits_per_pixel,
			   int *size_line,int *endian)
{
  *bits_per_pixel = img->bpp;
  *size_line = img->size_line;
  *endian = img->image->byte_order;
  return (img->data);
}
