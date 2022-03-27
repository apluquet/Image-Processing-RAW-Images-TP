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
    readRaw(path, data);
    blackPointDetection();
  }
  ~Image() { free(data); }

 private:
  void readRaw(const std::string &path, double *data);
  void blackPointDetection();

 public:
  int width;
  int height;
  double *data;
};
