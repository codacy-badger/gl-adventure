/*
 *  Copyright 2008-2010 Jerome PASQUIER
 * 
 *  This file is part of glAdventure - A little game using XGL.
 *
 *  glAdventure is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  glAdventure is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with glAdventure; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 *  02110-1301  USA
 *
 */

/** \file load_bmp.c
  * Implements functions to load BMP images and convert it to OpenGL textures
  *
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "load_bmp.h"

#include "ptypes.h"

/** simple loader for 24bit bitmaps (data is in rgb-format) 
  * 
  * \param filename The image file name
  * \param texture  The object where we store image data
  *
  */
int loadBMP(char *filename, textureImage *texture){
    FILE *file;
    uint16 bfType;
    int32 bfOffBits;
    int16 biPlanes;// short int
    int16 biBitCount;  // short int
    int32 biSizeImage;
    int i;
    unsigned char temp;
    /* make sure the file is there and open it read-only (binary) */
    if ((file = fopen(filename, "rb")) == NULL){
        printf("File not found : %s\n", filename);
        return 0;
    }
    if(!fread(&bfType, sizeof(uint16), 1, file)){
        printf("Error reading file!\n");
        return 0;
    }
    /* check if file is a bitmap */
    if (bfType != 19778){
        printf("Not a Bitmap-File!\n");
        return 0;
    }        
    /* get the file size */
    /* skip file size and reserved fields of bitmap file header */
    fseek(file, 8, SEEK_CUR);
    /* get the position of the actual bitmap data */
    if (!fread(&bfOffBits, sizeof(int32), 1, file)){
        printf("Error reading file!\n");
        return 0;
    }
    printf("Data at Offset: %ld\n", bfOffBits);
    /* skip size of bitmap info header */
    fseek(file, 4, SEEK_CUR);
    /* get the width of the bitmap */
    fread(&texture->width, sizeof(int32), 1, file);
    printf("Width of Bitmap: %d\n", texture->width);
    /* get the height of the bitmap */
    fread(&texture->height, sizeof(int32), 1, file);
    printf("Height of Bitmap: %d\n", texture->height);
    /* get the number of planes (must be set to 1) */
    fread(&biPlanes, sizeof(int16), 1, file);
    if (biPlanes != 1){
      printf("Error: number of Planes is not 1 (it is %d)!\n", biPlanes);
      return 0;
    }
    
    /* get the number of bits per pixel */
    if (!fread(&biBitCount, sizeof(int16), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Bits per Pixel: %d\n", biBitCount);
    if (biBitCount != 24)
    {
        printf("Bits per Pixel not 24\n");
        return 0;
    }
    /* calculate the size of the image in bytes */
    biSizeImage = texture->width * texture->height * 3;
    printf("Size of the image data: %ld\n", biSizeImage);
    texture->data = malloc(biSizeImage);
    /* seek to the actual data */
    fseek(file, bfOffBits, SEEK_SET);
    if (!fread(texture->data, biSizeImage, 1, file))
    {
        printf("Error loading file!\n");
        return 0;
    }
    /* swap red and blue (bgr -> rgb) */
    for (i = 0; i < biSizeImage; i += 3)
    {
        temp = texture->data[i];
        texture->data[i] = texture->data[i + 2];
        texture->data[i + 2] = temp;
    }
    return 1;
}

/** Load one OpenGL texture
  *
  * \param id       The OpenGL texture identifer
  * \param filename The image filename
  *
  * \return \c true if success
  *
  */
bool load_one_textures(GLuint* id, char* filename){
  bool status;
  textureImage *texti;
    
  status = false;
  texti = malloc(sizeof(textureImage));
  if (loadBMP(filename, texti)){
    status = true;
    // create the texture 
    glGenTextures(1, id);   
    glBindTexture(GL_TEXTURE_2D, (*id));
    // actually generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texti->width, texti->height, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, texti->data);
    // enable linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  // free the memory we used in our texture generation process
  if (texti){
    if (texti->data)
      free(texti->data);
    free(texti);
  }   

  return status; 
}


/** Load Bitmaps And Convert To Textures 
  *
  */
bool loadGLTextures(){
  bool status;
  status = load_one_textures(&texture[0], "images/NeHe.bmp");
  assert(status && "Cannot load texture 'images/NeHe.bmp'");

  status = load_one_textures(&texture[1], "images/wall.bmp");
  assert(status && "Cannot load texture 'images/wall.bmp'");

  status = load_one_textures(&texture[2], "images/floor.bmp");
  assert(status && "Cannot load texture 'images/floor.bmp'");

 
  return status;
}
