/**
 * @file image.h
 * @author Adèle PLUQUET and Adrien ANTON LUDWIG
 * @brief Image class
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <stdlib.h>

#include <algorithm>
#include <cmath>
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
    data = reinterpret_cast<double *>(malloc(width * height * sizeof(double)));
    r = reinterpret_cast<double *>(malloc(width * height * sizeof(double)));
    g = reinterpret_cast<double *>(malloc(width * height * sizeof(double)));
    b = reinterpret_cast<double *>(malloc(width * height * sizeof(double)));
    readRaw(path, data);
    blackPointDetection();
    demosaicing();
    maxR = *std::max_element(r, r + (width * height));
    maxG = *std::max_element(g, g + (width * height));
    maxB = *std::max_element(b, b + (width * height));
    // std::cout << "max " << maxR << " " << maxG << " " << maxB << std::endl;
  }
  ~Image() {
    free(data);
    free(r);
    free(g);
    free(b);
  }

  void toPpm();

 private:
  void readRaw(const std::string &path, double *data);
  void blackPointDetection();
  void demosaicing();
  double getCross(int i, int j);
  double getDiag(int i, int j);
  double getLine(int i, int j);
  double getCol(int i, int j);

 public:
  int width;
  int height;
  double *data;
  double *r;
  double *g;
  double *b;
  double maxR = 0;
  double maxG = 0;
  double maxB = 0;
};
