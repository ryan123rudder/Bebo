// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Arduino_GFX_Library.h>
#include <Adafruit_FT6206.h>
#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_ILI9341.h>     // Hardware-specific library
#include <SD_MMC.h>
#include "MjpegClass.h" // Used to split gifs frame-wise

#define NORMAL_SPEED

#define MJPEG_FOLDER       "/videos" // cannot be root!
#define MJPEG_OUTPUT_SIZE  (960 * 360 * 2)      // memory for a output image frame
#define MJPEG_BUFFER_SIZE (MJPEG_OUTPUT_SIZE / 5) // memory for a single JPEG frame
#define MJPEG_LOOPS        0

Arduino_XCA9554SWSPI *expander = new Arduino_XCA9554SWSPI(
    PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI,
    &Wire, 0x3F);
    
// Panel Parameters
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
    TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5,
    TFT_G0, TFT_G1, TFT_G2, TFT_G3, TFT_G4, TFT_G5,
    TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5,
    1 /* hsync_polarity */, 20 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 20 /* hsync_back_porch */,
    1 /* vsync_polarity */, 20 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */
    //,1, 30000000
    );

// Display parameters
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  /* 3.7" 240x960 bar display*/
    360 /* width */, 960 /* height */, rgbpanel, 1 /* rotation */, true /* auto_flush */,
    expander, GFX_NOT_DEFINED /* RST */, HD371001C40_init_operations, sizeof(HD371001C40_init_operations));

static MjpegClass mjpeg;
File mjpegFile, video_dir;
uint8_t *mjpeg_buf;
uint16_t *output_buf;

unsigned long total_show_video = 0;

void setup()
{  
  Serial.begin(115200);
  Wire.setClock(1000000); // speed up I2C 

  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }

  gfx->fillScreen(WHITE);

  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH); //Backlight on

  // Configure filesystem on SD
  SD_MMC.setPins(SCK, MOSI /* CMD/MOSI */, MISO /* D0/MISO */, A0 /* D1 */, A1 /* D2 */, SS /* D3/CS */); // quad MMC!

  // Mount SD filesystem
  while (!SD_MMC.begin("/root", true))
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
    delay(1000);
  }
  Serial.println("Found SD Card");

  // Open filesystem
  video_dir = SD_MMC.open(MJPEG_FOLDER);
  if (!video_dir || !video_dir.isDirectory()){
     Serial.println("Failed to open " MJPEG_FOLDER " directory");
     while (1) delay(100);
  }
  Serial.println("Opened Directory");

  // Allocate decoding buffer
  mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
  if (!mjpeg_buf) {
    Serial.println(F("mjpeg_buf malloc failed!"));
    while (1) delay(100);
  }
  Serial.println("Allocated decoding buffer");

  // Allocate output buffer
  output_buf = (uint16_t *)heap_caps_aligned_alloc(16, MJPEG_OUTPUT_SIZE, MALLOC_CAP_8BIT);
  if (!output_buf) {
    Serial.println(F("output_buf malloc failed!"));
    while (1) delay(100);
  }
  Serial.println("Allocated output buffer");
}

void loop()
{
  /* variables */
  int total_frames = 0;
  unsigned long total_read_video = 0;
  unsigned long total_decode_video = 0;
  unsigned long start_ms, curr_ms;
  uint8_t check_UI_count = 0;
  int16_t x = -1, y = -1, w = -1, h = -1;
  total_show_video = 0;

  if (mjpegFile) mjpegFile.close();
  Serial.println("looking for a file...");

  if (!video_dir || !video_dir.isDirectory()){
     Serial.println("Failed to open " MJPEG_FOLDER " directory");
     while (1) delay(100);
  }

  // look for first mjpeg file in next gif
  while ((mjpegFile = video_dir.openNextFile()) != 0) {
    if (!mjpegFile.isDirectory()) {
      Serial.print("  FILE: ");
      Serial.print(mjpegFile.name());
      Serial.print("  SIZE: ");
      Serial.println(mjpegFile.size());
      if ((strstr(mjpegFile.name(), ".mjpeg") != 0) || (strstr(mjpegFile.name(), ".MJPEG") != 0)) {
        Serial.println("   <---- found a video!");
        break;
      }
    }
    if (mjpegFile) mjpegFile.close();
  }

  if (!mjpegFile || mjpegFile.isDirectory())
  {
    Serial.println(F("ERROR: Failed to find a MJPEG file for reading, resetting..."));
    //gfx->println(F("ERROR: Failed to find a MJPEG file for reading"));

    // We kept getting hard crashes when trying to rewindDirectory or close/open dir
    // so we're just going to do a softreset
    esp_sleep_enable_timer_wakeup(1000);
    esp_deep_sleep_start(); 
  }

  bool done_looping = false;
  while (!done_looping) {
    mjpegFile.seek(0);
    total_frames = 0;
    total_read_video = 0;
    total_decode_video = 0;
    total_show_video = 0;

    Serial.println(F("MJPEG start"));
  
    start_ms = millis();
    curr_ms = millis();
    if (! mjpeg.setup(&mjpegFile, mjpeg_buf, output_buf, MJPEG_OUTPUT_SIZE, true /* useBigEndian */)) {
       Serial.println("mjpeg.setup() failed");
       while (1) delay(100);
    }
  
    while (mjpegFile.available() && mjpeg.readMjpegBuf())
    {
      // Read video
      total_read_video += millis() - curr_ms;
      curr_ms = millis();

      // Play video
      mjpeg.decodeJpg();
      total_decode_video += millis() - curr_ms;
      curr_ms = millis();

      // Get image parameters
      if (x == -1) {
        w = mjpeg.getWidth();
        h = mjpeg.getHeight();
        x = (w > gfx->width()) ? 0 : ((gfx->width() - w) / 2);
        y = (h > gfx->height()) ? 0 : ((gfx->height() - h) / 2);
      }
      gfx->draw16bitBeRGBBitmap(x, y, output_buf, w, h); // Draw the frame to the output buffer
      total_show_video += millis() - curr_ms;

      curr_ms = millis();
      total_frames++;
      check_UI_count++;
      if (check_UI_count >= 5) {
        check_UI_count = 0;
      }
    }
    int time_used = millis() - start_ms;
    Serial.println(F("MJPEG end"));
    
    float fps = 1000.0 * total_frames / time_used;
    total_decode_video -= total_show_video;
    Serial.printf("Total frames: %d\n", total_frames);
    Serial.printf("Time used: %d ms\n", time_used);
    Serial.printf("Average FPS: %0.1f\n", fps);
    Serial.printf("Read MJPEG: %lu ms (%0.1f %%)\n", total_read_video, 100.0 * total_read_video / time_used);
    Serial.printf("Decode video: %lu ms (%0.1f %%)\n", total_decode_video, 100.0 * total_decode_video / time_used);
    Serial.printf("Show video: %lu ms (%0.1f %%)\n", total_show_video, 100.0 * total_show_video / time_used);
  }
}

/**
* Deprecated. Displays the text parameter as a face on the display. E.G. ^ u ^
**/
void makeFace(char face[]){
  gfx->fillScreen(BLACK);
  gfx->setCursor(120, 50);
      gfx->setTextColor(0x7a7a);
      gfx->setTextSize(18 /* x scale */, 18 /* y scale */, 2 /* pixel_margin */);
  gfx->println(face);
}
