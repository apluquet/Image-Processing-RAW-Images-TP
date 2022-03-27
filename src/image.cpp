/**
 * @file main.cpp
 * @author Adèle PLUQUET and Adrien ANTON LUDWIG
 * @brief Main file for raw decoder
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "src/image.h"

#include <fstream>

void Image::readRaw(const std::string &path, double *data) {
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

void Image::blackPointDetection() {
  double min_r = INFINITY;
  double min_g = INFINITY;
  double min_b = INFINITY;

  // RED GET MIN
  for (int i = 1; i < height; i += 2)
    for (int j = 1; j < width; j += 2)
      min_r = std::min(min_r, data[i * width + j]);

  // GREEN GET MIN
  for (int i = 0; i < height; i += 1)
    for (int j = (i + 1) % 2; j < width; j += 2)
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
  for (int i = 0; i < height; i += 1)
    for (int j = (i + 1) % 2; j < width; j += 2) data[i * width + j] -= min_g;

  // BLUE SET
  for (int i = 0; i < height; i += 2)
    for (int j = 0; j < width; j += 2) data[i * width + j] -= min_b;
}

double Image::getCross(int i, int j) {
  double mean = 0;
  int cpt = 0;
  if (i > 0) {
    mean += data[(i - 1) * width + j];
    cpt++;
  }
  if (i < height - 1) {
    mean += data[(i + 1) * width + j];
    cpt++;
  }
  if (j > 0) {
    mean += data[i * width + j - 1];
    cpt++;
  }
  if (j < width - 1) {
    mean += data[i * width + j + 1];
    cpt++;
  }
  return mean / cpt;
}

double Image::getDiag(int i, int j) {
  double mean = 0;
  int cpt = 0;

  if (i > 0 && j > 0) {
    mean += data[(i - 1) * width + j - 1];
    cpt++;
  }
  if (i < height - 1 && j > 0) {
    mean += data[(i + 1) * width + j - 1];
    cpt++;
  }
  if (i > 0 && j < width - 1) {
    mean += data[(i - 1) * width + j + 1];
    cpt++;
  }
  if (i < height - 1 && j < width - 1) {
    mean += data[(i + 1) * width + j + 1];
    cpt++;
  }
  return mean / cpt;
}

double Image::getLine(int i, int j) {
  double mean = 0;
  int cpt = 0;
  if (i > 0) {
    mean += data[(i - 1) * width + j];
    cpt++;
  }
  if (i < height - 1) {
    mean += data[(i + 1) * width + j];
    cpt++;
  }
  return mean / cpt;
}

double Image::getCol(int i, int j) {
  double mean = 0;
  int cpt = 0;

  if (j > 0) {
    mean += data[i * width + j - 1];
    cpt++;
  }
  if (j < width - 1) {
    mean += data[i * width + j + 1];
    cpt++;
  }
  return mean / cpt;
}

void Image::demosaicing() {
  // RED
  for (int i = 1; i < height - 1; i += 2)
    for (int j = 1; j < width - 1; j += 2) {
      r[i * width + j] = data[i * width + j];
      g[i * width + j] = getCross(i, j);
      b[i * width + j] = getDiag(i, j);
    }

  // GREEN
  for (int i = 0; i < height - 1; i += 1)
    for (int j = (i + 1) % 2; j < width - 1; j += 2) {
      g[i * width + j] = data[i * width + j];
      if (i % 2) {
        r[i * width + j] = getLine(i, j);
        b[i * width + j] = getCol(i, j);
      } else {
        r[i * width + j] = getCol(i, j);
        b[i * width + j] = getLine(i, j);
      }
    }

  // BLUE
  for (int i = 0; i < height - 1; i += 2)
    for (int j = 0; j < width - 1; j += 2) {
      r[i * width + j] = getDiag(i, j);
      g[i * width + j] = getCross(i, j);
      b[i * width + j] = data[i * width + j];
    }
}

void Image::toPpm() {
  {
    std::ofstream ofs("mySuperImage.ppm", std::ofstream::out);
    ofs << "P3 " << width << " " << height << " 255" << std::endl;
    for (std::size_t i = 0; i < height; i++) {
      for (std::size_t j = 0; j < width; j++)
        ofs << static_cast<int>((r[i * width + j] / maxR) * 255) << " "
            << static_cast<int>((g[i * width + j] / maxG) * 255) << " "
            << static_cast<int>((b[i * width + j] / maxB) * 255) << " ";
      ofs << std::endl;
    }
    ofs.close();
  }
  std::cout << "Write image : success !" << std::endl;
}
