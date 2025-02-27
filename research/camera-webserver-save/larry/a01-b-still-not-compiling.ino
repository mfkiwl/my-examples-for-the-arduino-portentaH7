//
// PNG encoder test sketch
// written by Larry Bank
//
// Creates a 128x128x8-bpp image 'on-the-fly' and
// compresses it as a PNG file. The image is a green square with an X in the middle
// on a transparent background
//
// This program can run as either a memory-to-memory test for measuring
// performance or can write the PNG file to a micro-SD card
// Disable the macro below (WRITE_TO_SD) to use only memory
//

/*   ERROR MESSAGE

larry02-my-attempt:114:75: error: invalid conversion from 'int32_t (*)(PNGFILE*, uint8_t, int32_t) {aka long int (*)(png_file_tag*, unsigned char, long int)}' to 'int32_t (*)(PNGFILE*, uint8_t*, int32_t) {aka long int (*)(png_file_tag*, unsigned char*, long int)}' [-fpermissive]

   rc = png.open("fs/testimg.png", myOpen, myClose, myRead, myWrite, mySeek);

*/











#define File FILE   // so that Arduino Pro FILE class is used


#include <PNGenc.h>
PNG png; // static instance of the PNG encoder lass

// Add comment bars (//) in front of this macro to send the output to RAM only
#define WRITE_TO_SD

#ifdef WRITE_TO_SD
///#include <SD.h>
#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");


// Functions to access a file on the SD card
///File myfile;
File *myfile;

void * myOpen(const char *filename) {
  Serial.print("Attempting to open ");
  Serial.println(filename);
  // IMPORTANT!!! - don't use FILE_WRITE because it includes O_APPEND
  // this will cause the file to be written incorrectly
  // myfile = SD.open(filename, O_READ| O_WRITE | O_CREAT);
  myfile = fopen(filename, "w");   
  return &myfile;
}
void myClose(PNGFILE *handle) {
  File *f = (File *)handle->fHandle;
  ///f->close();
  fclose(f);
}
int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  File *f = (File *)handle->fHandle;
  ///return f->read(buffer, length);
  //return fread(buffer, length, f);
  return (int32_t)fread(buffer, 1, length, f);  // not sure why this even compiles
}
int32_t myWrite(PNGFILE *handle, uint8_t  buffer, int32_t length) {
///int32_t myWrite(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  File *f = (File *)handle->fHandle;
  ///return f->write(buffer, length);
  //return (int32_t)fprintf(f, (const char *)buffer, length);
  //return (int32_t)fwrite((const char *)buffer, length, 1, f);
  return fwrite((const char *)buffer, length, 1, f);
}
int32_t mySeek(PNGFILE *handle, int32_t position) {
  File *f = (File *)handle->fHandle;
  ///return f->seek(position);
  return (int32_t)fseek(f, 0, position); 
}
#endif // WRITE_TO_SD

#define WIDTH 128
#define HEIGHT 128

uint8_t ucPal[768] = {0,0,0,0,255,0}; // black, green
uint8_t ucAlphaPal[256] = {0,255}; // first color (black) is fully transparent

// Memory to hold the output file
#ifndef WRITE_TO_SD
uint8_t ucOut[4096];
#endif

void setup() {
int rc, iDataSize, x, y;
uint8_t ucLine[WIDTH];
long l;

  Serial.begin(115200);
  
  delay(5000); // time to connect serial
  Serial.println("15 seconds to connect serial monitor if needed");
  delay(5000);
  Serial.println("10 seconds to connect serial monitor if needed");
  delay(5000);
  Serial.println("5 seconds to connect serial monitor if needed");
  delay(5000);
  //while (!Serial);  // blocking call

#ifdef WRITE_TO_SD
 /// while (!SD.begin(32/*4*//*BUILTIN_SDCARD*/)) {
 ///   Serial.println("Unable to access SD Card");
 ///   delay(1000);
 /// }

   Serial.println("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
     Serial.println("No SD Card filesystem found, please check SD Card on computer and manually format if needed.");
     // int err = fs.reformat(&block_device);  // seriously don't want to format your good data
  }
  Serial.println("SD Card opened successfully");
#endif
  
  l = micros();
#ifdef WRITE_TO_SD
  ///rc = png.open("/testimg.png", myOpen, myClose, myRead, myWrite, mySeek);
  rc = png.open("fs/testimg.png", myOpen, myClose, myRead, myWrite, mySeek);
#else
  rc = png.open(ucOut, sizeof(ucOut));
#endif
  if (rc == PNG_SUCCESS) {
        rc = png.encodeBegin(WIDTH, HEIGHT, PNG_PIXEL_INDEXED, 8, ucPal, 3);
        png.setAlphaPalette(ucAlphaPal);
        if (rc == PNG_SUCCESS) {
            for (int y=0; y<HEIGHT && rc == PNG_SUCCESS; y++) {
              // prepare a line of image to create a red box with an x on a transparent background
              if (y==0 || y == HEIGHT-1) {
                memset(ucLine, 1, WIDTH); // top+bottom green lines 
              } else {
                memset(ucLine, 0, WIDTH);
                ucLine[0] = ucLine[WIDTH-1] = 1; // left/right border
                ucLine[y] = ucLine[WIDTH-1-y] = 1; // X in the middle
              }
                rc = png.addLine(ucLine);
            } // for y
            iDataSize = png.close();
            l = micros() - l;
            Serial.print(iDataSize);
            Serial.print(" bytes of data written to file in ");
            Serial.print((int)l);
            Serial.print(" us\n");
      }
  } else {
    Serial.println("Failed to create the file on the SD card!");
  }
} /* setup() */

void loop() {
  // nothing to do
} /* loop() */
