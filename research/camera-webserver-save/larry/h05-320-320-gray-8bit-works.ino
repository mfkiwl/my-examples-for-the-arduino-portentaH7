/*
  Portenta - TestSDCARD

  The sketch shows how to mount an SDCARD and list its content.
  then add a file.

  Note: SD Card seems finicky, takes a few times to load the filesystem

  The circuit:
   - Portenta H7 + Vision Shield
   or
   - Portenta H7 + Portenta Breakout

  This example code is in the public domain.
*/

#include "camera.h"

#ifdef ARDUINO_NICLA_VISION
  #include "gc2145.h"
  GC2145 galaxyCore;
  Camera cam(galaxyCore);
  #define IMAGE_MODE CAMERA_RGB565
#else
  #include "himax.h"
  HM01B0 himax;
  Camera cam(himax);
  #define IMAGE_MODE CAMERA_GRAYSCALE
#endif

/*
Other buffer instantiation options:
  FrameBuffer fb(0x30000000);
  FrameBuffer fb(320,240,2);
*/
FrameBuffer fb;



#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");


#include <PNGenc.h>

PNG png; // static instance of the PNG encoder class
FILE *myPngFile;

// don't forget to set the ucOut[] to be able to handle the PNG image size
#define PNG_WIDTH 320  //160  96
#define PNG_HEIGHT 320 // 120  96

// don't forget to setup the camera with the correct resolution
#define CAM_WIDTH 320
#define CAM_HEIGHT 320


// 86 86 works
uint8_t ucPal[768] = {0,0,0,0,255,0}; // black, green
uint8_t ucAlphaPal[256] = {0,255}; // first color (black) is fully transparent
//uint8_t ucOut[4096];
//uint8_t ucOut[8192];
//uint8_t ucOut[10240];  // works for 96x96
//uint8_t ucOut[20480];  // compiles
//uint8_t ucOut[30720];     // compiles
//uint8_t ucOut[40960];   // compiles  
uint8_t ucOut[81920];

// could use heap or Portenta SDRAM
static uint8_t frame_buffer[CAM_WIDTH*CAM_HEIGHT] __attribute__((aligned(32)));
//static uint8_t frame_buffer[160*120] __attribute__((aligned(32)));





void makePNG() {
  long l;
  l = micros();

int myTime = millis()/1000; // seconds since the sketch began
   char myPngFileName[] = "fs/myFolder6/my02----------.png";   // "fs/" needs to be there, think fileSystem

  int myExtensionLength = 3;  // .txt = 3 .json = 4
  // does anyone understand why the  + '0' is needed below???
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 10] = myTime/10000000 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 9] = myTime/1000000 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 8] = myTime/100000 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 7] = myTime/10000 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 6] = myTime/1000 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 5] = myTime/100 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 4] = myTime/10 % 10 + '0';
  myPngFileName[sizeof(myPngFileName)- myExtensionLength - 3] = myTime % 10 + '0';
  // Can make a new file but can't make a new folder



  
   myPngFile = fopen(myPngFileName, "w");          // "a" for append (add to file), "w" write, "r" read ?? 
  int rc, iDataSize, x, y;
  uint8_t ucLine[PNG_WIDTH];




  rc = png.open(ucOut, sizeof(ucOut));

  if (rc == PNG_SUCCESS) {


       // rc = png.encodeBegin(WIDTH, HEIGHT, PNG_PIXEL_INDEXED, 8, ucPal, 3);
        rc = png.encodeBegin(PNG_WIDTH, PNG_HEIGHT, PNG_PIXEL_GRAYSCALE, 8, ucPal, 3);
        png.setAlphaPalette(ucAlphaPal);
        if (rc == PNG_SUCCESS) {


           if (cam.grabFrame(fb, 3000) == 0) {
              //Serial.write(fb.getBuffer(), cam.frameSize());


              // transfer camera image to PNG class
              for (int y=0; y < PNG_HEIGHT && rc == PNG_SUCCESS; y++){    
                 memset(ucLine, 0, PNG_WIDTH); // zero the storage location
                 int yMap = map(y, 0, PNG_HEIGHT, 0, CAM_HEIGHT);
                 
                 for (int x=0; x < PNG_WIDTH && rc == PNG_SUCCESS; x++){       
                    int xMap = map(x, 0, PNG_WIDTH, 0, CAM_WIDTH);
                    uint8_t myGRAY1 = frame_buffer[(yMap * CAM_WIDTH) + xMap];   
                    ucLine[x] = myGRAY1;  
                } 
                rc = png.addLine(ucLine);
              } 


           }  




          
            iDataSize = png.close();

            
           // fwrite(ucOut,1, 3039, myPngFile);
            fwrite(ucOut,1, iDataSize, myPngFile);
           // fwrite(ucOut,1, sizeof(ucOut), myPngFile);
                        
            fclose(myPngFile);


            Serial.print("Printing the raw data");
           // Serial.write(ucOut, sizeof(ucOut));
           // Serial.write(ucOut, sizeof(ucOut));
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();

            Serial.print("Printing info:");

            Serial.println(sizeof(ucOut));
            Serial.println("  , file name: ");
            Serial.println(myPngFileName);
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            
            l = micros() - l;
            Serial.print(iDataSize);
            Serial.print(" bytes of data written to file in ");
            Serial.print((int)l);
            Serial.print(" us\n");
         }        
  } else {
    Serial.println("Failed to create PNG");
  }
}    /* end makePNG()  */




void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, HIGH);
  
  delay(5000); // time to connect serial
  Serial.println("15 seconds to connect serial monitor if needed");
  delay(5000);
  Serial.println("10 seconds to connect serial monitor if needed");
  delay(5000);
  Serial.println("5 seconds to connect serial monitor if needed");
  delay(5000);
  //while (!Serial);  // blocking call
 
  

  Serial.println("Mounting Camera...");
  // if (!cam.begin(CAMERA_R320x240, IMAGE_MODE, 30)) {
  //if (!cam.begin(CAMERA_R160x120, CAMERA_GRAYSCALE, 30)) {
  if (!cam.begin(CAMERA_R320x320, CAMERA_GRAYSCALE, 30)) {
    Serial.println("No Camera, make sure portenta Vision shield is connected");
  }

    // set the frame_buffer to the static uint8_t
    fb.setBuffer(frame_buffer); 

  
  Serial.println("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
     Serial.println("No SD Card filesystem found, please check SD Card on computer and manually format if needed.");
     // int err = fs.reformat(&block_device);  // seriously don't want to format your good data
  }

}




void loop() {
 
  // Make a folder if needed
  mkdir("fs/myFolder6",0777);                     // 0777 full access permissions linux style 
   
  char myFileName[] = "fs/myFolder6/test2.txt";   // "fs/" needs to be there, think fileSystem
 
  FILE *myFile = fopen(myFileName, "w");          // "a" for append (add to file), "w" write, "r" read ??
 
     Serial.println(myFileName);
     fprintf(myFile,"Test how cool this is \r\n");
     fprintf(myFile,"Also this line \r\n");
   
  fclose(myFile);

  Serial.println("------------------------- Done Writing file --------------------------------");
  delay(10);
   
  unsigned char c; 
  FILE *fp = fopen(myFileName, "r");              // "r" read only
     while (!feof(fp)){                           // while not end of file
        c=fgetc(fp);                              // get a character/byte from the file
        //printf("Read from file %02x\n\r",c);    // and show it in hex format
        Serial.print((char)c);                    // show it as a text character
     }
  fclose(fp); 
  Serial.println("------------------------- Done Showing file --------------------------------");
  Serial.println("------------------------- Making a PNG --------------------------------");

  digitalWrite(LEDB, LOW); 
  delay(500);
  digitalWrite(LEDB, HIGH);  
  delay(500);
  
  digitalWrite(LEDB, LOW); 
  delay(1000);
  digitalWrite(LEDB, HIGH); 
  delay(500);
  
  digitalWrite(LED_BUILTIN, LOW); 
  makePNG();
  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.println("------------------------- Done and waiting 10 seconds --------------------------------");
  delay(8000);   // wait a bit
  
}
