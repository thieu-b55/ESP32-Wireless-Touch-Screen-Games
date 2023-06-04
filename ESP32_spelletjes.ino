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
bool juiste_volgorde_bool = false;
bool connect_4_bool = false;
bool ingevuld_bool = false;
bool spel_gedaan_bool = false;
bool kleuren_ontvangen_bool = false;
bool score_ontvangen_bool = false;
bool spel_bezig_bool = false;
bool rij_ontvangen_bool = false;
bool veld_gezet_bool = false;

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


int mijn_punten_int = 0;
int tegenspeler_punten_int = 0;
int mijn_random_int;
int ontvangen_random_int;
int ontvangen_positie_int;
int kleur_int;
int spel_int;
int aantal_kleuren_int = 3;
int ontvangen_kleur_int = 0;
int score_int = 0;
int ontvangen_score_int;
int cirkel_kleur_int;
int cirkel_waarde_int;
int rij_int;
int veld_array[10];
int veld_4_array[25];
int kleur_rnd_array[20];
int kleur_array[6] = {0x0000, 0x867d, 0xf800, 0xffe0, 0x07e0, 0xf81f};
int veld_connect_array[43];

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
                            {7, 13, 19, 25},
                            {2, 8, 14, 20},
                            {6, 12, 18, 24},
                            {5, 9, 13, 17},
                            {9, 13, 17, 21},
                            {10, 14, 18, 22},
                            {4, 8, 12, 16}};

int score_connect_array[69][4] = {{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7},
                                  {8, 9, 10, 11}, {9, 10, 11, 12}, {10, 11, 12, 13}, {11, 12, 13, 14},
                                  {15, 16, 17, 18}, {16, 17, 18, 19}, {17, 18, 19, 20}, {18, 19, 20, 21},
                                  {22, 23, 24, 25}, {23, 24, 25, 26}, {24, 25, 26, 27}, {25, 26, 27, 28},
                                  {29, 30, 31, 32}, {30, 31, 32, 33}, {31, 32, 33, 34}, {32, 33, 34, 35},
                                  {36, 37, 38, 39}, {37, 38, 39, 40}, {38, 39, 40, 41}, {39, 40, 41, 42},
                                  {1, 8, 15, 22}, {8, 15, 22, 29}, {15, 22, 29, 36},
                                  {2, 9, 16, 23}, {9, 16, 23, 30}, {16, 23, 30, 37},
                                  {3, 10, 17, 24}, {10, 17, 24, 31}, {17, 24, 31, 38},
                                  {4, 11, 18, 25}, {11, 18, 25, 32}, {18, 25, 32, 39},
                                  {5, 12, 19, 26}, {12, 19, 26, 33}, {19, 26, 33, 40},
                                  {6, 13, 20, 27}, {13, 20, 27, 34}, {20, 27, 34, 41},
                                  {7, 14, 21, 28}, {14, 21, 28, 35}, {21, 28, 35, 42},
                                  {15, 23, 31, 39},
                                  {8, 16, 24, 32}, {16, 24, 32, 40},
                                  {1, 9, 17, 25}, {9, 17, 25, 33}, {17, 25, 33, 41},
                                  {2, 10, 18, 26}, {10, 18, 26, 34}, {18, 26, 34, 42},
                                  {3, 11, 19, 27}, {11, 19, 27, 35},
                                  {4, 12, 20, 28},
                                  {4, 10, 16, 22},
                                  {5, 11, 17, 23}, {11, 17, 23, 29},
                                  {6, 12, 18, 24}, {12, 18, 24, 30}, {18, 24, 30, 36},
                                  {7, 13, 19, 25}, {13, 19, 25, 31}, {19, 25, 31, 37},
                                  {14, 20, 26, 32}, {20, 26, 32, 38},
                                  {21, 27, 33, 39}};
                         
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

int kleurvak_array[6][2] = {{0, 0},
                            {0, 0},
                            {160, 0},
                            {80, 80},
                            {0, 160},
                            {160, 160}};

int touch_coor_kleur_array[6][4] = {{0, 0, 0, 0},
                                    {0, 80, 0, 80},
                                    {159, 240, 0, 80},
                                    {79, 160, 79, 160},
                                    {0, 80, 159, 240},
                                    {159, 240, 159, 240}};

int connect_4_scherm_array[11][4] = {{35, 44, 35, 248},
                                     {69, 44, 69, 248},
                                     {103, 44, 103, 248},
                                     {137, 44, 137, 248},
                                     {171, 44, 171, 248},
                                     {205, 44, 205, 248},
                                     {0, 78, 239, 78},
                                     {0, 112, 239, 112},
                                     {0, 146, 239, 146},
                                     {0, 180, 239, 180},
                                     {0, 214, 239, 214}};

int connect_4_cirkels_array[7] = {17, 52, 86, 120, 154, 188, 222};

int touch_coor_connect_array[8][2] = {{0, 0},
                                      {0, 34},
                                      {33, 68},
                                      {67, 102},
                                      {101, 136},
                                      {135, 170},
                                      {169, 204},
                                      {203, 240}};

int cirkel_x_coor_array[8] = {0, 17, 52, 86, 120, 154, 188, 222};
                                      
int cirkel_y_coor_array[7] = {0, 61, 95, 129, 163, 197, 231};

uint16_t t_x;
uint16_t t_y; 
unsigned long touch_update_long;
unsigned long nieuw_spel_wacht_long;
unsigned long start_kleur_long;
unsigned long wachten_long;

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

/*
 * TIC TAC TOE
 */
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


/*
 * FOUR IN A ROW
 */

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

/*
 * TIC TAC TOE   FOUR IN A ROW
 */

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

/*
 * SIMON
 */

void juiste_volgorde_scherm(){
  tft.fillScreen(TFT_BLACK);
  tft.drawRoundRect(0, 0, 80, 80, 0, TFT_SKYBLUE);
  tft.drawRoundRect(160, 0, 80, 80, 0, TFT_RED);
  tft.drawRoundRect(80, 80, 80, 80, 0, TFT_YELLOW);
  tft.drawRoundRect(0, 160, 80, 80, 0, TFT_GREEN);
  tft.drawRoundRect(160, 160, 80, 80, 0, TFT_MAGENTA);
  tft.setTextFont(2);
  tft.setTextColor(TFT_RED);
  tft.setCursor(10, 270);
  tft.print(mijn_punten_int);
  tft.print(" - ");
  tft.print(tegenspeler_punten_int);
  tft.setTextFont(4);
  tft.setCursor(107, 270);
  tft.print(mijn_punten_int);
}

/*
 * CONNECT 4
 */

void connect_4_scherm(){
  tft.fillScreen(TFT_BLACK);
  for(int x = 0; x < 11; x ++){
    tft.drawLine(connect_4_scherm_array[x][0], connect_4_scherm_array[x][1], connect_4_scherm_array[x][2], connect_4_scherm_array[x][3], TFT_WHITE);
  }
  tft.setTextFont(2);
  tft.setTextColor(TFT_RED);
  tft.setCursor(10, 270);
  tft.print(mijn_punten_int);
  tft.print(" - ");
  tft.print(tegenspeler_punten_int);
  tft.setTextFont(4);
  tft.setCursor(107, 270);
  tft.print(mijn_punten_int);
  for(int x = 0; x < 43; x ++){
    veld_connect_array[x] = 0;
  }
}

void connect_4_cirkels(){
  tft.fillRoundRect(0, 0, 240, 34, 0, TFT_GREEN);
  for(int x = 0; x < 7; x ++){
    tft.fillCircle(connect_4_cirkels_array[x], 17, 12, cirkel_kleur_int);
  }
}

void controle_connect_4(){
  for(int x = 0; x < 69; x++){
    int totaal = veld_connect_array[score_connect_array[x][0]] + veld_connect_array[score_connect_array[x][1]] + veld_connect_array[score_connect_array[x][2]] + veld_connect_array[score_connect_array[x][3]];
    if(totaal == 12){
      int y = score_connect_array[x][0];
      int y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      int x_coor =  score_connect_array[x][0];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][1];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][1];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][2];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][2];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][3];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][3];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
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
      int y = score_connect_array[x][0];
      int y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      int x_coor =  score_connect_array[x][0];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][1];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][1];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][2];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][2];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
      y = score_connect_array[x][3];
      y_coor = 1;
      while(y > 7){
        y_coor ++;
        y -= 7;
      }
      x_coor =  score_connect_array[x][3];
      while(x_coor > 7){
        x_coor -= 7;
      }
      tft.fillCircle(cirkel_x_coor_array[x_coor], cirkel_y_coor_array[y_coor], 12, TFT_BLUE);
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
    spel_gedaan_bool = false;
    connect_4_scherm();
  }
}

void ingevuld_connect_4(){
  ingevuld_bool = true;
  for(int x = 1; x < 43; x++){
    if(veld_connect_array[x] == 0){
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
    connect_4_scherm();
  }
}

/*
 * ALL GAMES
 */

void enable_touch_int(){
  attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
  detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
  attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
  int_bool = false;
}

void uitsturen(){
  mesh.sendBroadcast(zend_string);
}

void receivedCallback( uint32_t from, String &msg ){
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
    if(spel_int == 93){
      rij_int = msg.toInt();
      rij_ontvangen_bool = true;
    }
  }
  if((spel_int == 92) && (!ik_begin_bool) && (!spel_bezig_bool)){
    for(int x = 0; x < 15; x++){
      kleur_rnd_array[x] = msg.substring(x, x +1).toInt();
    }
    kleuren_ontvangen_bool = true;
  }
  if((spel_int == 92) && (spel_bezig_bool)){
    ontvangen_score_int = msg.toInt();
    score_ontvangen_bool = true;
  }
}

void newConnectionCallback(uint32_t nodeId){
}

void changedConnectionCallback(){
  verbinding_bool = true;
}

void nodeTimeAdjustedCallback(int32_t offset){
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
  tft.print("ESP32");
  tft.setCursor(10, 100);
  tft.print("SPELLETJES");
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 140);
  tft.print("Wachten op verbinding");
  tft.setCursor(10, 300);
  tft.print("thieu-b55   mei 2023");
  while(!verbinding_bool){
    mesh.update();
  }
  mijn_random_int = esp_random();
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
    tft.setCursor(10, 20);
    tft.print("TIC-TAC-TOE");
    tft.setCursor(10, 60);
    tft.print("Vier Op Een Rij");
    tft.setCursor(10, 100);
    tft.print("Juiste Volgorde");
    tft.setCursor(10, 140);
    tft.print("Connect 4");
   
    while((!tic_tac_toe_bool) && (!vier_op_een_rij_bool) && (!juiste_volgorde_bool) && (!connect_4_bool)){
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
      if((t_x > 0) && (t_x < 239) && (t_y > 0) && (t_y < 40)){
        tic_tac_toe_bool = true;
        spel_keuze_bool = false;
        spel_int = 90;
        zend_string = String(spel_int);
        uitsturen();
        tic_tac_toe_scherm();
        enable_touch_int();
      }
      if((t_x > 0) && (t_x < 239) && (t_y > 39) && (t_y < 80)){
        vier_op_een_rij_bool = true;
        spel_keuze_bool = false;
        spel_int = 91;
        zend_string = String(spel_int);
        uitsturen();
        vier_op_een_rij_scherm();
        enable_touch_int();
      }
      if((t_x > 0) && (t_x < 239) && (t_y > 79) && (t_y < 120)){
        juiste_volgorde_bool = true;
        spel_keuze_bool = false;
        spel_int = 92;
        zend_string = String(spel_int);
        uitsturen();
        enable_touch_int();
      }
      if((t_x > 0) && (t_x < 239) && (t_y > 119) && (t_y < 160)){
        connect_4_bool = true;
        spel_keuze_bool = false;
        spel_int = 93;
        zend_string = String(spel_int);
        uitsturen();
        connect_4_scherm();
        cirkel_kleur_int = 0xf800;
        connect_4_cirkels();
        cirkel_waarde_int = 3;
        enable_touch_int();
      }
      if(((millis() - touch_update_long) > 500) && (int_bool)){
        enable_touch_int();
      }
      mesh.update();
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
      enable_touch_int();
    }
    if(spel_int == 91){
      vier_op_een_rij_scherm();
      enable_touch_int();
    }
    if(spel_int == 92){
      enable_touch_int();
    }
    if(spel_int == 93){
      connect_4_scherm();
      enable_touch_int();
    }
  }
  if(spel_int == 90){
    if((touch_int_bool == true) && (mijn_beurt_bool == true)){
      t_x = 0;
      t_y = 0; 
      int max_loop = 0;
      pressed = tft.getTouch(&t_x, &t_y, 40);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y, 40);
      }
      touch_int_bool = false;
      int_bool = true;
      touch_update_long = millis();
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
      pressed = tft.getTouch(&t_x, &t_y, 40);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y, 40);
      }
      touch_int_bool = false;
      int_bool = true;
      touch_update_long = millis();
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
  if(spel_int == 92){
    wachten_long = millis();
    while((millis() - wachten_long) < 1000){
      mesh.update();
    }
    juiste_volgorde_scherm();
    while(1){
      score_ontvangen_bool = false;
      kleuren_ontvangen_bool = false;
      if(ik_begin_bool){
        zend_string = "";
        for(int x = 0; x < 15; x++){
          int tmp_int = random(1, 6);
          kleur_rnd_array[x] = tmp_int;
          zend_string = zend_string + String(tmp_int);
        }
        spel_bezig_bool = true;
        uitsturen();
        wachten_long = millis();
        while((millis() - wachten_long) < 1000){
          mesh.update();
        }
      }
      if(!ik_begin_bool){
        while(!kleuren_ontvangen_bool){
          mesh.update();
        }
        spel_bezig_bool = true;
        wachten_long = millis();
        while((millis() - wachten_long) < 1000){
          mesh.update();
        }
      }
      for(int x = 0; x < aantal_kleuren_int; x ++){
        tft.fillRoundRect(kleurvak_array[kleur_rnd_array[x]][0], kleurvak_array[kleur_rnd_array[x]][1], 80, 80, 0, kleur_array[kleur_rnd_array[x]]);
        start_kleur_long = millis();
        while((millis() - start_kleur_long) < 1000){
          mesh.update();
        }
        tft.fillRoundRect(kleurvak_array[kleur_rnd_array[x]][0], kleurvak_array[kleur_rnd_array[x]][1], 80, 80, 0, TFT_BLACK);
        tft.drawRoundRect(kleurvak_array[kleur_rnd_array[x]][0], kleurvak_array[kleur_rnd_array[x]][1], 80, 80, 0, kleur_array[kleur_rnd_array[x]]);
        start_kleur_long = millis();
        while((millis() - start_kleur_long) < 500){
          mesh.update();
        }
      }
      int y = 0;
      while(y < aantal_kleuren_int){
        score_int = 1;
        if(touch_int_bool){
          t_x = 0;
          t_y = 0; 
          int max_loop = 0;
          pressed = tft.getTouch(&t_x, &t_y, 40);
          while((!pressed) && (max_loop < 15)){
            max_loop++;
            pressed = tft.getTouch(&t_x, &t_y, 40);
          }
          touch_int_bool = false;
          int_bool = true;
          touch_update_long = millis();
          for(int x = 1; x < 6; x ++){
            if((t_x > touch_coor_kleur_array[x][0]) && (t_x < touch_coor_kleur_array[x][1]) && (t_y > touch_coor_kleur_array[x][2]) && (t_y < touch_coor_kleur_array[x][3])){
              if(x == kleur_rnd_array[y]){
                tft.fillRoundRect(kleurvak_array[x][0], kleurvak_array[x][1], 80, 80, 0, kleur_array[x]);
                start_kleur_long = millis();
                while((millis() - start_kleur_long) < 250){
                  mesh.update();
                }
                tft.fillRoundRect(kleurvak_array[x][0], kleurvak_array[x][1], 80, 80, 0, TFT_BLACK);
                tft.drawRoundRect(kleurvak_array[x][0], kleurvak_array[x][1], 80, 80, 0, kleur_array[x]);
                y ++;
              }
              else{
                tft.fillScreen(TFT_RED);
                start_kleur_long = millis();
                while((millis() - start_kleur_long) < 250){
                  mesh.update();
                }
                tft.fillScreen(TFT_BLACK);
                juiste_volgorde_scherm();
                score_int = 0;
                y = aantal_kleuren_int;
                break;
              }
            }
          }
        }
        if(((millis() - touch_update_long) > 500) && (int_bool)){
          enable_touch_int();
        }
      }
      mijn_punten_int = mijn_punten_int + score_int;
      zend_string = String(score_int);
      if(ik_begin_bool){
        uitsturen();
        while(!score_ontvangen_bool){
          mesh.update();
        }
        tegenspeler_punten_int = tegenspeler_punten_int + ontvangen_score_int;
      }
      if(!ik_begin_bool){
        while(!score_ontvangen_bool){
          mesh.update();
        }
        tegenspeler_punten_int = tegenspeler_punten_int + ontvangen_score_int;
        uitsturen();
      }
      spel_bezig_bool = false;
      tft.fillScreen(TFT_WHITE);
      wachten_long = millis();
      while((millis() - wachten_long) < 250){
        mesh.update();
      }
      juiste_volgorde_scherm();
      if((mijn_punten_int >= ((aantal_kleuren_int -2) * 5)) && (tegenspeler_punten_int >= ((aantal_kleuren_int -2) * 5))){
        aantal_kleuren_int ++;
        if(aantal_kleuren_int > 15){
          aantal_kleuren_int = 15;
        }
      }
      mesh.update();
    }
  }
  if(spel_int == 93){
    if((touch_int_bool)  & (mijn_beurt_bool)){
      t_x = 0;
      t_y = 0; 
      int max_loop = 0;
      pressed = tft.getTouch(&t_x, &t_y, 40);
      while((!pressed) && (max_loop < 15)){
        max_loop++;
        pressed = tft.getTouch(&t_x, &t_y, 40);
        mesh.update();
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
      for(int x = 1; x < 8; x ++){
        if((t_x > touch_coor_connect_array[x][0]) && (t_x < touch_coor_connect_array[x][1]) && (t_y > 0) && (t_y < 34)){
          if(veld_connect_array[x] == 0){
            zend_string = String(x);
            uitsturen();
            int x_array_int = x;
            int y_array_int = 1;
            int veld_array_int = x;
            for(int i = 0; i < 6; i ++){
              tft.fillCircle(cirkel_x_coor_array[x_array_int], cirkel_y_coor_array[y_array_int], 12, cirkel_kleur_int);
              veld_array_int += 7;
              if((veld_connect_array[veld_array_int] == 0) && (veld_array_int < 43)){
                wachten_long = millis();
                while((millis() - wachten_long) < 250){
                  mesh.update();
                }
                tft.fillCircle(cirkel_x_coor_array[x_array_int], cirkel_y_coor_array[y_array_int], 12, TFT_BLACK);
                y_array_int ++;
              }
              else{
                if(ik_begin_bool){
                  cirkel_waarde_int = 3;
                }
                else{
                  cirkel_waarde_int = 5;
                }
                veld_connect_array[veld_array_int - 7] = cirkel_waarde_int;
                veld_gezet_bool = true;
                break;
              }
            }
          }
        }
      }
    }
    if(!mijn_beurt_bool){
      if(ik_begin_bool){
        cirkel_kleur_int = 0xffe0;
      }
      else{
        cirkel_kleur_int = 0xf800;
      }
      while(!rij_ontvangen_bool){
        mesh.update();
      }
      rij_ontvangen_bool = false;
      if((rij_int > 0) && (rij_int < 8)){
        int x_array_int = rij_int;
        int y_array_int = 1;
        int veld_array_int = rij_int;
        for(int i = 0; i < 6; i ++){
          tft.fillCircle(cirkel_x_coor_array[x_array_int], cirkel_y_coor_array[y_array_int], 12, cirkel_kleur_int);
          veld_array_int += 7;
          if((veld_connect_array[veld_array_int] == 0) && (veld_array_int < 43)){
            wachten_long = millis();
            while((millis() - wachten_long) < 250){
              mesh.update();
            }
            tft.fillCircle(cirkel_x_coor_array[x_array_int], cirkel_y_coor_array[y_array_int], 12, TFT_BLACK);
            y_array_int ++;
          }
          else{
            if(ik_begin_bool){
              cirkel_waarde_int = 5;
            }
            else{
              cirkel_waarde_int = 3;
            }
            veld_connect_array[veld_array_int - 7] = cirkel_waarde_int;
            veld_gezet_bool = true;
            break;
          }
        }
      }
    }
    if(veld_gezet_bool){
      tft.fillRoundRect(0, 0, 240, 34, 0, TFT_BLACK);
      veld_gezet_bool = false;
      mijn_beurt_bool = mijn_beurt_bool ^ 1;
      controle_connect_4();
      ingevuld_connect_4();
      if(mijn_beurt_bool){
        if(ik_begin_bool){
          cirkel_kleur_int = 0xf800;
          connect_4_cirkels();
        }
        else{
          cirkel_kleur_int = 0xffe0;
          connect_4_cirkels();
        }
      }
    }
  }
  if(((millis() - touch_update_long) > 500) && (int_bool)){
    enable_touch_int();
  }
  mesh.update();
}
