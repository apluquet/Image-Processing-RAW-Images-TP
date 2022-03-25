#pragma once

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

struct pixels {
  uint8_t v1;
  uint8_t v2;
  uint8_t v3;
  uint8_t v4;
  struct endPixel {
    uint8_t a1 : 2;
    uint8_t a2 : 2;
    uint8_t a3 : 2;
    uint8_t a4 : 2;
  } v5;
};

class Image {
 public:
  Image(int width, int height, const std::string &path)
      : width(width), height(height) {
    data = (double *)malloc(width * height * sizeof(double));
    readRaw(path, data);
  }

  ~Image() { free(data); }

 private:
  static void readRaw(const std::string &path, double *data) {
    struct pixels pixels;
    FILE *fp;
    fp = fopen(path.c_str(), "rb");
    int data_cpt = 0;
    while (1) {
      fread(&pixels, sizeof(pixels), 1, fp);
      if (feof(fp)) break;

      data[data_cpt++] = pixels.v1 << 2 + pixels.v5.a1;
      data[data_cpt++] = pixels.v2 << 2 + pixels.v5.a2;
      data[data_cpt++] = pixels.v3 << 2 + pixels.v5.a3;
      data[data_cpt++] = pixels.v4 << 2 + pixels.v5.a4;
    }
    std::cout << "compteur : " << data_cpt << std::endl;
    fclose(fp);
  }

 public:
  int width;
  int height;
  double *data;
};