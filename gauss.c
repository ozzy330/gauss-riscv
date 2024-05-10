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

void gaussBlur(int w, int h, unsigned char* img){
    for (int x  = 0; x < w; ++x) {
	for (int y = 0; y < h; ++y) {
	    if (x == 0) {
	    }
	    printf("PIXEL: %x ", img[x * h + y]);
	}
    }

}

int main(int argc, char* argv[]) {
	 FILE* file = fopen("./imagen_referencia_640l.bmp", "rb");
    if (!file) {
        printf("Unable to open file.");
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader infoHeader;

    // Read BMP header
    fread(&header, sizeof(BMPHeader), 1, file);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Display image information
    printf("Width: %d\n", infoHeader.width);
    printf("Height: %d\n", infoHeader.height);
    printf("Bit Depth: %d\n", infoHeader.bitsPerPixel);

    // Read image data
    unsigned char* imageData = (unsigned char*)malloc(infoHeader.imageSize);
    fread(imageData, 1, infoHeader.imageSize, file);

    // Process image data here
    gaussBlur(infoHeader.width, infoHeader.height, imageData);

    // Clean up
    free(imageData);
    fclose(file);

    return 0;
}
