/*
* MIT License
*
* Copyright (c) 2023 thieu-b55
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include "FS.h"
#include "SD.h"
#include "painlessMesh.h"

TFT_eSPI tft = TFT_eSPI();

painlessMesh  mesh;

#define   MESH_PREFIX     "ESP32"
#define   MESH_PASSWORD   "ESP32_pswd"
#define   MESH_PORT        7777

#define TOUCH_INT 25
#define TFT_CS    15
#define TOUCH_CS  4
#define TFT_MISO  12
#define TFT_MOSI  13
#define TFT_SCK   14
#define TFT_DC    2
#define TFT_RST   -1 (EN ESP32)

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

int mijn_punten_int = 0;
int tegenspeler_punten_int = 0;
int mijn_random_int;
int ontvangen_random_int;
int ontvangen_positie_int;
int kleur_int;
int spel_int;
int veld_array[10];
int veld_4_array[25];

int score_array[8][3] = {{1, 2, 3},
                         {4, 5, 6},
                         {7, 8, 9},
                         {1, 5, 9},
                         {3, 5, 7},
                         {1, 4, 7},
                         {2, 5, 8},
                         {3, 6, 9}}; 
                         
int score_4_array[28][4] = {{1, 2, 3, 4},
                            {2, 3, 4, 5},
                            {6, 7, 8, 9},
                            {7, 8, 9, 10},
                            {11, 12, 13, 14},
                            {12, 13, 14, 15},
                            {16, 17, 18, 19},
                            {17, 18, 19, 20},
                            {21, 22, 23, 24},
                            {22, 23, 24, 25},
                            {1, 6, 11, 16},
                            {6, 11, 16, 21},
                            {2, 7, 12, 17},
                            {7, 12, 17, 22},
                            {3, 8, 13, 18},
                            {8, 13, 18, 23},
                            {4, 9, 14, 19},
                            {9, 14, 19, 24},
                            {5, 10, 15, 20},
                            {10, 15, 20, 25},
                            {1, 7, 13, 19},
                            {7, 13, 19, 26},
                            {2, 8, 14, 20},
                            {6, 12, 18, 24},
                            {5, 9, 13, 17},
                            {9, 13, 17, 21},
                            {10, 14, 18, 22},
                            {4, 8, 12, 16}};
                         
int positie_array[10][2] = {{0, 0},
                            {49, 29},
                            {129, 29},
                            {209, 29},
                            {49, 109},
                            {129, 109},
                            {209, 109},
                            {49, 189},
                            {129, 189},
                            {209, 189}};

int positie_4_array[26][2] = {{0, 0},
                              {33, 13},
                              {80, 13},
                              {128, 13},
                              {176, 13},
                              {224, 13},
                              {33, 61},
                              {80, 61},
                              {128, 61},
                              {176, 61},
                              {224, 61},
                              {33, 109},
                              {80, 109},
                              {128, 109},
                              {176, 109},
                              {224, 109},
                              {33, 157},
                              {80, 157},
                              {128, 157},
                              {176, 157},
                              {224, 157},
                              {33, 205},
                              {80, 205},
                              {128, 205},
                              {176, 205},
                              {224, 205}};
                            
int touch_coor_array[10][4] = {{0, 0, 0, 0},
                               {0, 80, 0, 80},
                               {79, 160, 0, 80},
                               {159, 240, 0, 80},
                               {0, 80, 79, 160},
                               {79, 160, 79, 160},
                               {159, 240, 79, 160},
                               {0, 80, 159, 240},
                               {79, 160, 159, 240},
                               {159, 240, 159, 240}};
                               
int touch_coor_4_array[26][4] = {{0, 0, 0, 0},
                                 {0, 48, 0, 48},
                                 {47, 96, 0, 48},
                                 {95, 144, 0, 48},
                                 {143, 192, 0, 48},
                                 {191, 240, 0, 48},
                                 {0, 48, 47, 96},
                                 {47, 96, 47, 96}, 
                                 {95, 144, 47, 96},
                                 {143, 192, 47, 96},
                                 {191, 240, 47, 96},
                                 {0, 48, 95, 144},
                                 {47, 96, 95, 144},
                                 {95, 144, 95, 144},
                                 {143, 192, 95, 144},
                                 {191, 240, 95, 144},
                                 {0, 48, 143, 192},
                                 {47, 96, 143, 192},
                                 {95, 144, 143, 192},
                                 {143, 192, 143, 192},
                                 {191, 240, 143, 192},
                                 {0, 48, 191, 240},
                                 {47, 96, 191, 240},
                                 {95, 144, 191, 240},
                                 {143, 192, 191, 240},
                                 {191, 240, 191, 240}};

uint16_t t_x;
uint16_t t_y; 
unsigned long touch_update_long;
unsigned long nieuw_spel_wacht_long;

bool pressed;
bool touch_int_bool = false;
bool int_bool = false;
bool verbinding_bool = false;
bool init_bool = true;
bool rnd_ontvangen_bool = false;
bool spel_int_ontvangen_bool = false;
bool ik_begin_bool = false;
bool mijn_beurt_bool = false;
bool spel_keuze_bool = false;
bool tic_tac_toe_bool = false;
bool vier_op_een_rij_bool = false;
bool ingevuld_bool = false;
bool spel_gedaan_bool = false;

bool x_bool_array[20][20] = {{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
                             {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, 
                             {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},
                             {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
                             {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                             {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                             {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                             {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
                             {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},
                             {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                             {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}};

bool nul_bool_array[20][20] = {{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                               {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, 
                               {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                               {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                               {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}};

String zend_string = "                        ";

void touch_calibrate(){
  uint16_t calData[5];
  uint8_t calDataOK = 0;
  if (!SPIFFS.begin()){
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }
  if (SPIFFS.exists(CALIBRATION_FILE)){
    if (REPEAT_CAL){
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else{
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f){
        if (f.readBytes((char *)calData, 14) == 14){
          calDataOK = 1;
        }
        f.close();
      }
    }
  }
  if (calDataOK && !REPEAT_CAL){
    tft.setTouch(calData);
  } 
  else{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();
    if (REPEAT_CAL){
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f){
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void touch(){
  detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
  touch_int_bool = true;
}

void dummy(){
}

void tic_tac_toe_scherm(){
  spel_gedaan_bool = false;
  kleur_int = 0xffff;
  tft.fillScreen(TFT_BLACK);
  tft.drawLine(79, 0, 79, 239, TFT_WHITE);
  tft.drawLine(159, 0, 159, 239, TFT_WHITE);
  tft.drawLine(0, 79, 239, 79, TFT_WHITE);
  tft.drawLine(0, 159, 239, 159, TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextColor(TFT_RED);
  tft.setCursor(10, 270);
  tft.print(mijn_punten_int);
  tft.print(" - ");
  tft.print(tegenspeler_punten_int);
  tft.setTextFont(4);
  tft.setCursor(107, 270);
  tft.print(mijn_punten_int);
  if(mijn_beurt_bool){
    tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
  }
  else{
    tft.fillRect(0, 309, 240, 10, TFT_BLACK);
  }
  if(ik_begin_bool){
    tft.setTextFont(4);
    tft.setCursor(200, 270);
    tft.setTextColor(TFT_GREEN);
    tft.print("O");
  }
  else{
    tft.setTextFont(4);
    tft.setCursor(200, 270);
    tft.setTextColor(TFT_GREEN);
    tft.print("X");
  }
  for(int x = 0; x < 10; x++){
    veld_array[x] = 0;
  }
}

void vier_op_een_rij_scherm(){
  spel_gedaan_bool = false;
  kleur_int = 0xffff;
  tft.fillScreen(TFT_BLACK);
  tft.drawLine(48, 0, 48, 239, TFT_WHITE);
  tft.drawLine(96, 0, 96, 239, TFT_WHITE);
  tft.drawLine(144, 0, 144, 239, TFT_WHITE);
  tft.drawLine(192, 0, 192, 239, TFT_WHITE);
  tft.drawLine(0, 48, 239, 48, TFT_WHITE);
  tft.drawLine(0, 96, 239, 96, TFT_WHITE);
  tft.drawLine(0, 144, 239, 144, TFT_WHITE);
  tft.drawLine(0, 192, 239, 192, TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextColor(TFT_RED);
  tft.setCursor(10, 270);
  tft.print(mijn_punten_int);
  tft.print(" - ");
  tft.print(tegenspeler_punten_int);
  tft.setTextFont(4);
  tft.setCursor(107, 270);
  tft.print(mijn_punten_int);
  if(mijn_beurt_bool){
    tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
  }
  else{
    tft.fillRect(0, 309, 240, 10, TFT_BLACK);
  }
  if(ik_begin_bool){
    tft.setTextFont(4);
    tft.setCursor(200, 270);
    tft.setTextColor(TFT_GREEN);
    tft.print("O");
  }
  else{
    tft.setTextFont(4);
    tft.setCursor(200, 270);
    tft.setTextColor(TFT_GREEN);
    tft.print("X");
  }
  for(int x = 0; x < 26; x++){
    veld_4_array[x] = 0;
  }
}

void teken_x(int x_int, int y_int){
  for(int tmp_x_int = x_int; tmp_x_int > x_int -20; tmp_x_int --){
    for(int tmp_y_int = y_int; tmp_y_int < y_int + 20; tmp_y_int ++){
      if(x_bool_array[x_int - tmp_x_int][tmp_y_int - y_int]){
        tft.drawPixel(tmp_x_int, tmp_y_int, kleur_int);
      }
    }
  }
}

void teken_o(int x_int, int y_int){
  for(int tmp_x_int = x_int; tmp_x_int > x_int -20; tmp_x_int --){
    for(int tmp_y_int = y_int; tmp_y_int < y_int + 20; tmp_y_int ++){
      if(nul_bool_array[x_int - tmp_x_int][tmp_y_int - y_int]){
        tft.drawPixel(tmp_x_int, tmp_y_int, kleur_int);
      }
    }
  }
}

void uitsturen(){
  mesh.sendBroadcast(zend_string);
}

void controle(){
  for(int x = 0; x < 8; x++){
    int totaal = veld_array[score_array[x][0]] + veld_array[score_array[x][1]] + veld_array[score_array[x][2]];
    if(totaal == 9){
      kleur_int = 0x001f;
      teken_o(positie_array[score_array[x][0]][0], positie_array[score_array[x][0]][1]); 
      teken_o(positie_array[score_array[x][1]][0], positie_array[score_array[x][1]][1]); 
      teken_o(positie_array[score_array[x][2]][0], positie_array[score_array[x][2]][1]); 
      spel_gedaan_bool = true;
      tft.fillRect(0, 260, 240, 59, TFT_BLACK);
      if(ik_begin_bool){
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(50, 270);
        tft.print("GEWONNEN");
        mijn_punten_int ++;
      }
      else{
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(70, 270);
        tft.print("JAMMER");
        tegenspeler_punten_int ++;
      }
    }
    if(totaal == 15){
      kleur_int = 0x001f;
      teken_x(positie_array[score_array[x][0]][0], positie_array[score_array[x][0]][1]); 
      teken_x(positie_array[score_array[x][1]][0], positie_array[score_array[x][1]][1]); 
      teken_x(positie_array[score_array[x][2]][0], positie_array[score_array[x][2]][1]); 
      spel_gedaan_bool = true;
      tft.fillRect(0, 260, 240, 59, TFT_BLACK);
      if(ik_begin_bool){
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(70, 270);
        tft.print("JAMMER");
        tegenspeler_punten_int ++;
      }
      else{
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(50, 270);
        tft.print("GEWONNEN");
        mijn_punten_int ++;
      }
    }
    if(spel_gedaan_bool){
      tft.setTextFont(2);
      tft.setCursor(35, 300);
      tft.print("nieuw spel over 5 seconden");
      nieuw_spel_wacht_long = millis();
      while((millis() - nieuw_spel_wacht_long) < 5000){
        mesh.update();
      }
      ik_begin_bool = ik_begin_bool ^ 1;
      if(ik_begin_bool){
        mijn_beurt_bool = true;
      }
      else{
        mijn_beurt_bool = false;
      }
      tic_tac_toe_scherm();
    }
  }
}

void controle_4(){
  for(int x = 0; x < 28; x++){
    int totaal = veld_4_array[score_4_array[x][0]] + veld_4_array[score_4_array[x][1]] + veld_4_array[score_4_array[x][2]] + veld_4_array[score_4_array[x][3]];
    if(totaal == 12){
      kleur_int = 0x001f;
      teken_o(positie_4_array[score_4_array[x][0]][0], positie_4_array[score_4_array[x][0]][1]); 
      teken_o(positie_4_array[score_4_array[x][1]][0], positie_4_array[score_4_array[x][1]][1]); 
      teken_o(positie_4_array[score_4_array[x][2]][0], positie_4_array[score_4_array[x][2]][1]); 
      teken_o(positie_4_array[score_4_array[x][3]][0], positie_4_array[score_4_array[x][3]][1]); 
      spel_gedaan_bool = true;
      tft.fillRect(0, 260, 240, 59, TFT_BLACK);
      if(ik_begin_bool){
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(50, 270);
        tft.print("GEWONNEN");
        mijn_punten_int ++;
      }
      else{
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(70, 270);
        tft.print("JAMMER");
        tegenspeler_punten_int ++;
      }
    }
    if(totaal == 20){
      kleur_int = 0x001f;
      teken_x(positie_4_array[score_4_array[x][0]][0], positie_4_array[score_4_array[x][0]][1]); 
      teken_x(positie_4_array[score_4_array[x][1]][0], positie_4_array[score_4_array[x][1]][1]); 
      teken_x(positie_4_array[score_4_array[x][2]][0], positie_4_array[score_4_array[x][2]][1]); 
      teken_x(positie_4_array[score_4_array[x][3]][0], positie_4_array[score_4_array[x][3]][1]); 
      spel_gedaan_bool = true;
      tft.fillRect(0, 260, 240, 59, TFT_BLACK);
      if(ik_begin_bool){
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(70, 270);
        tft.print("JAMMER");
        tegenspeler_punten_int ++;
      }
      else{
        tft.setTextColor(TFT_RED);
        tft.setTextFont(4);
        tft.setCursor(50, 270);
        tft.print("GEWONNEN");
        mijn_punten_int ++;
      }
    }
    if(spel_gedaan_bool){
      tft.setTextFont(2);
      tft.setCursor(35, 300);
      tft.print("nieuw spel over 5 seconden");
      nieuw_spel_wacht_long = millis();
      while((millis() - nieuw_spel_wacht_long) < 5000){
        mesh.update();
      }
      ik_begin_bool = ik_begin_bool ^ 1;
      if(ik_begin_bool){
        mijn_beurt_bool = true;
      }
      else{
        mijn_beurt_bool = false;
      }
      vier_op_een_rij_scherm();
    }
  }
}

void ingevuld(){
  ingevuld_bool = true;
  for(int x = 1; x < 10; x++){
    if(veld_array[x] == 0){
      ingevuld_bool = false;
    }
  }
  if(ingevuld_bool){
    tft.fillRect(0, 260, 240, 59, TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextFont(4);
    tft.setCursor(25, 270);
    tft.print("GEEN  WINNAAR");
    tft.setTextFont(2);
    tft.setCursor(35, 300);
    tft.print("nieuw spel over 5 seconden");
    nieuw_spel_wacht_long = millis();
    while((millis() - nieuw_spel_wacht_long) < 5000){
      mesh.update();
    }
    ik_begin_bool = ik_begin_bool ^ 1;
    if(ik_begin_bool){
      mijn_beurt_bool = true;
    }
    else{
      mijn_beurt_bool = false;
    }
    tic_tac_toe_scherm();
  }
}

void ingevuld_4(){
  ingevuld_bool = true;
  for(int x = 1; x < 26; x++){
    if(veld_4_array[x] == 0){
      ingevuld_bool = false;
    }
  }
  if(ingevuld_bool){
    tft.fillRect(0, 260, 240, 59, TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextFont(4);
    tft.setCursor(25, 270);
    tft.print("GEEN  WINNAAR");
    tft.setTextFont(2);
    tft.setCursor(35, 300);
    tft.print("nieuw spel over 5 seconden");
    nieuw_spel_wacht_long = millis();
    while((millis() - nieuw_spel_wacht_long) < 5000){
      mesh.update();
    }
    ik_begin_bool = ik_begin_bool ^ 1;
    if(ik_begin_bool){
      mijn_beurt_bool = true;
    }
    else{
      mijn_beurt_bool = false;
    }
    vier_op_een_rij_scherm();
  }
}

void receivedCallback( uint32_t from, String &msg ){
  Serial.println("bericht ontvangen");
  Serial.println(msg);
  if((init_bool) && (!rnd_ontvangen_bool)){
    rnd_ontvangen_bool = true;
    ontvangen_random_int = msg.toInt();
    zend_string = String(mijn_random_int);
    uitsturen();
  }
  if(spel_keuze_bool){
    spel_int = msg.toInt();
    if((spel_int > 89) && (spel_int < 100)){
      spel_int_ontvangen_bool = true;
    }
  }
  if(!init_bool){
    if(spel_int == 90){
      ontvangen_positie_int = msg.toInt();
      if((ontvangen_positie_int > 0) && (ontvangen_positie_int < 10)){
        if(ik_begin_bool){
          teken_x(positie_array[ontvangen_positie_int][0], positie_array[ontvangen_positie_int][1]); 
          veld_array[ontvangen_positie_int] = 5;
        }
        else{
          teken_o(positie_array[ontvangen_positie_int][0], positie_array[ontvangen_positie_int][1]); 
          veld_array[ontvangen_positie_int] = 3;
        }
        mijn_beurt_bool = mijn_beurt_bool ^ 1;
        if(mijn_beurt_bool){
          tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
        }
        else{
          tft.fillRect(0, 309, 240, 10, TFT_BLACK);
        }
        controle();
        if(!spel_gedaan_bool){
          ingevuld();
        }
      }
    }
    if(spel_int == 91){
      ontvangen_positie_int = msg.toInt();
      if((ontvangen_positie_int > 0) && (ontvangen_positie_int < 26)){
        if(ik_begin_bool){
          teken_x(positie_4_array[ontvangen_positie_int][0], positie_4_array[ontvangen_positie_int][1]); 
          veld_4_array[ontvangen_positie_int] = 5;
        }
        else{
          teken_o(positie_4_array[ontvangen_positie_int][0], positie_4_array[ontvangen_positie_int][1]); 
          veld_4_array[ontvangen_positie_int] = 3;
        }
        mijn_beurt_bool = mijn_beurt_bool ^ 1;
        if(mijn_beurt_bool){
          tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
        }
        else{
          tft.fillRect(0, 309, 240, 10, TFT_BLACK);
        }
        controle_4();
        if(!spel_gedaan_bool){
        ingevuld_4();
        }
      }
    }
  }
}

void newConnectionCallback(uint32_t nodeId){
  Serial.println("nieuwe verbinding");
}

void changedConnectionCallback(){
  Serial.println("veranderde verbinding");
  verbinding_bool = true;
}

void nodeTimeAdjustedCallback(int32_t offset){
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup(){
  delay(2500);
  Serial.begin(115200);
  pinMode(TOUCH_INT, INPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TOUCH_CS, HIGH);
  tft.init();
  tft.setRotation(2);
  touch_calibrate();
  mesh.setDebugMsgTypes( ERROR | STARTUP );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(4);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(10, 60);
  tft.print("TIC-TAC-TOE");
  tft.setCursor(10, 140);
  tft.print("4  OP  1  RIJ");
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 300);
  tft.print("thieu-b55   mei 2023");
  while(!verbinding_bool){
    mesh.update();
  }
  mijn_random_int = esp_random();
  while(mijn_random_int < 100){
    mijn_random_int = esp_random();
  }
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(4);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(90, 60);
  tft.print("Druk");
  tft.setCursor(38, 100);
  tft.print("op het scherm");
  tft.setCursor(15, 140);
  tft.print("om verder te gaan");
  attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
}

void loop(){
  while(init_bool){
    if((touch_int_bool) && (!rnd_ontvangen_bool)){
      touch_int_bool = false;
      zend_string = String(mijn_random_int);
      uitsturen();
    }
    if(rnd_ontvangen_bool){
      init_bool = false;
      spel_keuze_bool = true;
      if(mijn_random_int > ontvangen_random_int){
        ik_begin_bool = true;
        mijn_beurt_bool = true;
      }
    }
    if(!init_bool){
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
      detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
    }
    mesh.update();
  }
  if((ik_begin_bool) && (spel_keuze_bool)){
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 50);
    tft.print("Kies het spelletje");
    tft.setCursor(10, 100);
    tft.print("Tic Tac Toe");
    tft.setCursor(10, 150);
    tft.print("4 Op 1 Rij");
    while((!tic_tac_toe_bool) && (!vier_op_een_rij_bool)){
      while(!touch_int_bool){
        mesh.update();
      }
      touch_update_long = millis();
      t_x = 0;
      t_y = 0; 
      int max_loop = 0;
      pressed = tft.getTouch(&t_x, &t_y);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y);
      }
      if((t_x > 0) && (t_x < 239) && (t_y > 74) && (t_y < 125)){
        tic_tac_toe_bool = true;
        spel_keuze_bool = false;
        spel_int = 90;
        zend_string = String(spel_int);
        uitsturen();
      }
      if((t_x > 0) && (t_x < 239) && (t_y > 124) && (t_y < 175)){
        vier_op_een_rij_bool = true;
        spel_keuze_bool = false;
        spel_int = 91;
        zend_string = String(spel_int);
        uitsturen();
      }
      if(((millis() - touch_update_long) > 500) && (int_bool)){
        attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
        detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
        attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
        touch_int_bool = false;
      }
      mesh.update();
    }
    if(spel_int == 90){
      tic_tac_toe_scherm();
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
      detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
      touch_int_bool = false;
    }
    if(spel_int == 91){
      vier_op_een_rij_scherm();
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
      detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
      touch_int_bool = false;
    }
  }
  if((!ik_begin_bool) && (rnd_ontvangen_bool) && (spel_keuze_bool)){
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 50);
    tft.print("Effe geduld");
    tft.setCursor(0, 75);
    tft.print("Spelletje wordt gekozen");
    while(!spel_int_ontvangen_bool){
      mesh.update();
    }
    spel_keuze_bool = false;
    if(spel_int == 90){
      tic_tac_toe_scherm();
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
      detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
      touch_int_bool = false;
    }
    if(spel_int == 91){
      vier_op_een_rij_scherm();
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
      detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
      attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
      touch_int_bool = false;
    }
  }
  if(spel_int == 90){
    if((touch_int_bool == true) && (mijn_beurt_bool == true)){
      t_x = 0;
      t_y = 0; 
      int max_loop = 0;
      pressed = tft.getTouch(&t_x, &t_y, 30);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y, 30);
      }
      touch_int_bool = false;
      int_bool = true;
      touch_update_long = millis();
      /*
      Serial.print("x :");
      Serial.println(t_x);
      Serial.print("y :");
      Serial.println(t_y);
      */
      for(int x = 1; x < 10; x ++){
        if((t_x > touch_coor_array[x][0]) && (t_x < touch_coor_array[x][1]) && (t_y > touch_coor_array[x][2]) && (t_y < touch_coor_array[x][3])){
          if(veld_array[x] == 0){
            if(ik_begin_bool){
              teken_o(positie_array[x][0], positie_array[x][1]);
              veld_array[x] = 3;
            }
            else{
              teken_x(positie_array[x][0], positie_array[x][1]);
              veld_array[x] = 5;
            }
            zend_string = String(x);
            uitsturen();
            mijn_beurt_bool = mijn_beurt_bool ^ 1;
            if(mijn_beurt_bool){
              tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
            }
            else{
              tft.fillRect(0, 309, 240, 10, TFT_BLACK);
            }
            controle();
            if(!spel_gedaan_bool){
            ingevuld();
            }
          }
        }
      }
    }
  }
  if(spel_int == 91){
    if((touch_int_bool == true) && (mijn_beurt_bool == true)){
      t_x = 0;
      t_y = 0; 
      int max_loop = 0;
      pressed = tft.getTouch(&t_x, &t_y, 30);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y, 30);
      }
      touch_int_bool = false;
      int_bool = true;
      touch_update_long = millis();
      /*
      Serial.print("x :");
      Serial.println(t_x);
      Serial.print("y :");
      Serial.println(t_y);
      */
      for(int x = 1; x < 26; x ++){
        if((t_x > touch_coor_4_array[x][0]) && (t_x < touch_coor_4_array[x][1]) && (t_y > touch_coor_4_array[x][2]) && (t_y < touch_coor_4_array[x][3])){
          if(veld_4_array[x] == 0){
            if(ik_begin_bool){
              teken_o(positie_4_array[x][0], positie_4_array[x][1]);
              veld_4_array[x] = 3;
            }
            else{
              teken_x(positie_4_array[x][0], positie_4_array[x][1]);
              veld_4_array[x] = 5;
            }
            zend_string = String(x);
            uitsturen();
            mijn_beurt_bool = mijn_beurt_bool ^ 1;
            if(mijn_beurt_bool){
              tft.fillRect(0, 309, 240, 10, TFT_YELLOW);
            }
            else{
              tft.fillRect(0, 309, 240, 10, TFT_BLACK);
            }
            controle_4();
            if(!spel_gedaan_bool){
            ingevuld_4();
            }
          }
        }
      }
    }
  }
  if(((millis() - touch_update_long) > 500) && (int_bool)){
    attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
    detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
    attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
    int_bool = false;
  }
  mesh.update();
}
