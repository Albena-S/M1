#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "image-pnm.h"


#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

#define CHANNELS 3 // we have 3 channels corresponding to RGB
#define B 1



  // The input image is encoded as unsigned characters [0, 255]
  
  __global__ void colorConvert(unsigned char * grayImage,
                              unsigned char * rgbImage,
                              int width, int height) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if (x < width && y < height) {
      // get 1D coordinate for the grayscale image
      int grayOffset = y*width + x;
      // one can think of the RGB image having
      // CHANNEL times columns than the gray scale image
      int rgbOffset = grayOffset*CHANNELS;
      unsigned char r = rgbImage[rgbOffset ]; // red value for pixel
      unsigned char g = rgbImage[rgbOffset + 1]; // green value for pixel
      unsigned char b = rgbImage[rgbOffset+ 2]; // blue value for pixel
      // perform the rescaling and store it
      // We multiply by floating point constants
      grayImage[grayOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
    }

  }

    __global__ void colorConvert(unsigned char * grayImage,
                              unsigned char * rgbImage,
                              int width, int height) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if (x < width && y < height) {
      // get 1D coordinate for the grayscale image
      int minX = x - B < 0 : 0 ? x - B,
          minY = y - B < 0 : 0 ? x - B,
          maxX = x + B > width : width ? x + B,
          maxY = y + B > height : height ? y + B;
        


      // perform the rescaling and store it
      // We multiply by floating point constants
      grayImage[grayOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
    }

  }

int main(int argc, char** argv){
    size_t width = 512, height = 512;
    Image<float> in = Image<float>("P2", width, height);
    in.read("./image-grey.pgm");

    Image<float> out = Image<float>("P2", width, height);

    float* outData = out.getData();
    float* inData = in.getData();
    for (unsigned int i = 0; i < width ; i ++ ){
       for (unsigned int j = 0; j < height ; j ++){
          outData[i * width + j] = inData[i * width + j];
          
      }
    }
    out.write("copy.pgm");



  return 0;
}
