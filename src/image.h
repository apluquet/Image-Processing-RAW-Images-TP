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
  void readRaw(const std::string &path, double *data) {
    struct pixels pixels;
    FILE *fp;
    fp = fopen(path.c_str(), "rb");
    int data_cpt = 0;
    while (1) {
      fread(&pixels, sizeof(pixels), 1, fp);
      if (feof(fp)) break;

      if (data_cpt >= width * height - 3)
        throw std::invalid_argument("Data cpt is too big\n");

      data[data_cpt++] = (pixels.v1 << 2) + pixels.v5.a1;
      data[data_cpt++] = (pixels.v2 << 2) + pixels.v5.a2;
      data[data_cpt++] = (pixels.v3 << 2) + pixels.v5.a3;
      data[data_cpt++] = (pixels.v4 << 2) + pixels.v5.a4;
    }
    std::cout << "compteur : " << data_cpt << std::endl;

    fclose(fp);
  }

  void blackPointDetection() {
    double min_r = INFINITY;
    double min_g = INFINITY;
    double min_b = INFINITY;

    // RED GET MIN
    for (int i = 1; i < height; i += 2)
      for (int j = 1; j < width; j += 2)
        min_r = std::min(min_r, data[i * width + j]);

    // GREEN GET MIN
    for (int i = 1; i < height; i += 1)
      for (int j = i % 2 + 1; j < width; j += 2)
        min_g = std::min(min_g, data[i * width + j]);

    // BLUE GET MIN
    for (int i = 0; i < height; i += 2)
      for (int j = 0; j < width; j += 2)
        min_b = std::min(min_b, data[i * width + j]);

    std::cout << "red min = " << min_r << std::endl;
    std::cout << "green min = " << min_g << std::endl;
    std::cout << "blue min = " << min_b << std::endl;

    // RED SET
    for (int i = 1; i < height; i += 2)
      for (int j = 1; j < width; j += 2) data[i * width + j] -= min_r;

    // GREEN SET
    for (int i = 1; i < height; i += 1)
      for (int j = i % 2 + 1; j < width; j += 2) data[i * width + j] -= min_g;

    // BLUE SET
    for (int i = 0; i < height; i += 2)
      for (int j = 0; j < width; j += 2) data[i * width + j] -= min_b;
  }

 public:
  int width;
  int height;
  double *data;
};
