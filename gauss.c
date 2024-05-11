#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char id[2];
  unsigned int size[2];
  unsigned short reserved;
  unsigned short unused;
  unsigned int offset;
} BMPHeader;

typedef struct {
  unsigned int size;
  int width;
  int height;
  unsigned short coloPlanes;
  unsigned short bitsPerPixel;
  unsigned int compression;
  unsigned int imageSize;
  int xResolution;
  int yResolution;
  unsigned int colors;
  unsigned int importantColors;
} BMPInfoHeader;

int kernel[10][10] = {{1, 4, 7, 10, 11, 10, 7, 4, 1, 0},
                      {4, 16, 26, 33, 36, 33, 26, 16, 4, 0},
                      {7, 26, 41, 52, 57, 52, 41, 26, 7, 0},
                      {10, 33, 52, 66, 72, 66, 52, 33, 10, 0},
                      {11, 36, 57, 72, 79, 72, 57, 36, 11, 0},
                      {10, 33, 52, 66, 72, 66, 52, 33, 10, 0},
                      {7, 26, 41, 52, 57, 52, 41, 26, 7, 0},
                      {4, 16, 26, 33, 36, 33, 26, 16, 4, 0},
                      {1, 4, 7, 10, 11, 10, 7, 4, 1, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void gaussBlur(int w, int h, unsigned char *img) {
  int temp[w * h];

  for (int y = 1; y < h - 1; y++) {
    for (int x = 1; x < w - 1; x++) {
      float sum = 0.0;
      // Iterate kernel
      for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
          sum += kernel[k + 1][l + 1] * img[(y + k) * w + (x + l)];
        }
      }
      temp[y * w + x] = sum;
    }
  }

  for (int i = 0; i < w * h; i++) {
    img[i] = temp[i];
  }
}

int main(int argc, char *argv[]) {
  FILE *file = fopen("./imagen_referencia_640l.bmp", "rb");
  if (!file) {
    printf("Unable to open file.");
    return 1;
  }

  BMPHeader header;
  BMPInfoHeader infoHeader;

  fread(&header, sizeof(BMPHeader), 1, file);
  fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

  printf("Width: %d\n", infoHeader.width);
  printf("Height: %d\n", infoHeader.height);

  unsigned char *imageData = (unsigned char *)malloc(infoHeader.imageSize);
  fread(imageData, 1, infoHeader.imageSize, file);

  FILE *output_file = fopen("output.bmp", "wb");
  if (!output_file) {
    printf("Unable to create output file.");
    return 1;
  }

  fwrite(&header, sizeof(BMPHeader), 1, output_file);
  fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, output_file);

  gaussBlur(infoHeader.width, infoHeader.height, imageData);

  fwrite(imageData, 1, infoHeader.imageSize, output_file);

  free(imageData);
  fclose(file);
  fclose(output_file);

  return 0;
}
