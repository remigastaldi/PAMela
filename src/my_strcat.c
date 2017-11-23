/**
* @Author: Matthias Prost <prost_m>
* @Date:   2017-11-23T20:15:03+01:00
* @Email:  matthias.porst@epitech.eu
* @Filename: my_strcat.c
 * @Last modified by:   prost_m
 * @Last modified time: 2017-11-23T22:25:08+01:00
*/


#include "pam.h"

char			*my_strcat(char *dest, const char *src)
{
  char			*tmp;
  int			len_src;
  int			len_dest;
  int     i = -1;
  int     j = -1;

  len_src = strlen(src);
  if (dest == NULL)
  {
    if ((dest = malloc((len_src + 1))) == NULL)
      return (NULL);
    while (src[++i])
      dest[i] = src[i];
    dest[i] = '\0';
    return (dest);
  }
  len_dest = strlen(dest);
  if ((tmp = malloc((len_src + len_dest + 1))) == NULL)
    return (NULL);
  while (dest[++j])
    tmp[j] = dest[j];
  while (src[++i])
    tmp[j++] = src[i];
  tmp[j] = '\0';
  free(dest);
  return (tmp);
}
