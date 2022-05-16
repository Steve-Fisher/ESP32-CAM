/*************************************************************************************************************************************************
 *  TITLE: Time Lapse Imaging Using The ESP32-CAM board
 *  This sketch wakes the ESP32-CAM board at a pre-determined interval, captures an image, saves it to the microSD card and then puts it to sleep.
 *  This loop continues indefinitely and the time interval can be changed. The microSD card needs to be formatted with the FAT32 file system.
 *
 *  By Frenoy Osburn
 *  YouTube Video: https://youtu.be/u7cYWQiltuM
 *  BnBe Post: https://www.bitsnblobs.com/time-lapse-camera-using-the-esp32-cam
 *************************************************************************************************************************************************/

#include "esp_camera.h"
#include "SD_MMC.h"
#include "EEPROM.h"

#define ID_ADDRESS            0x00
#define COUNT_ADDRESS         0x01
#define ID_BYTE               0xAA
#define EEPROM_SIZE           0x0F

#define TIME_TO_SLEEP  60            //time ESP32 will go to sleep (in seconds)
#define uS_TO_S_FACTOR 1000000ULL   //conversion factor for micro seconds to seconds */

uint16_t nextImageNumber = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting...");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  //init with high specs to pre-allocate larger buffers
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  SD_MMC.begin("/sdcard", true);
  uint8_t cardType = SD_MMC.cardType();
  
  if(cardType == CARD_NONE)
  {
    Serial.println("SD failed to mount");
    return;
  }
  
  //initialize camera
  esp_err_t err = esp_camera_init(&config);
  delay(3000);  // Essential to give the camera time to properly initialise.  Otherwise the white balance is all wrong!
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //initialize EEPROM & get file number
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("Failed to initialise EEPROM"); 
    Serial.println("Exiting now"); 
    while(1);   //wait here as something is not right
  }
  
  /*ERASE EEPROM BYTES START*/
  /*
  Serial.println("Erasing EEPROM...");
  for(int i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, 0xFF);
    EEPROM.commit();
    delay(20);
  }
  Serial.println("Erased");
  while(1);
  */
  /*ERASE EEPROM BYTES END*/  

  if(EEPROM.read(ID_ADDRESS) != ID_BYTE)    //there will not be a valid picture number
  {
    Serial.println("Initializing ID byte & restarting picture count");
    nextImageNumber = 0;
    EEPROM.write(ID_ADDRESS, ID_BYTE);  
    EEPROM.commit(); 
  }
  else                                      //obtain next picture number
  {
    EEPROM.get(COUNT_ADDRESS, nextImageNumber);
    nextImageNumber +=  1;    
    Serial.print("Next image number:");
    Serial.println(nextImageNumber);
  }

  // take new image
  camera_fb_t * fb = NULL;
  //obtain camera frame buffer
  fb = esp_camera_fb_get();
  if (!fb) 
  {
    Serial.println("Camera capture failed");
    Serial.println("Exiting now"); 
    while(1);   //wait here as something is not right
  }

  //save to SD card
  //generate file path
  String path = "/IMG" + String(nextImageNumber) + ".jpg";
    
  fs::FS &fs = SD_MMC;

  //create new file
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file)
  {
    Serial.println("Failed to create file");
    Serial.println("Exiting now"); 
    while(1);   //wait here as something is not right    
  } 
  else 
  {
    file.write(fb->buf, fb->len); 
    EEPROM.put(COUNT_ADDRESS, nextImageNumber);
    EEPROM.commit();
  }
  file.close();

  //return camera frame buffer
  esp_camera_fb_return(fb);
  Serial.printf("Image saved: %s\n", path.c_str());

  delay(500);
  Serial.println("Entering deep sleep mode");
  Serial.flush(); 

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  
}

void loop() 
{


}
