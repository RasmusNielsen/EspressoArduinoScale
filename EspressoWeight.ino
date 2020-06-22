//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <HX711.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define NAVBAR_X_COORD 57 // X position for navbar locataion

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HX711 scale;

const int LeftButtonPin = 6;
const int CenterButtonPin = 7;
const int RightButtonPin = 5;

int LeftButtonState = 0; 
int CenterButtonState = 0; 
int RightButtonState = 0; 

int brewTime = 27;
int targetGrams = 31;

float readingfromscale = 0;

int currentScreen = 1;
// 1 = Menu     2 = Brew    3 = Time settings     4 = Tare Done      5 = End

int menuItem = 1;  

int countdown = 100;

void setup()
{
  //initiate buttons
  pinMode(LeftButtonPin, INPUT);
  pinMode(CenterButtonPin, INPUT);
  pinMode(RightButtonPin, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for OLED
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  drawScreens();
  display.display();

  // listen on serial for scale HX711
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Calibrate the scale
  scale.set_scale(1860); 
  scale.tare();


}

void loop()
{
 //read button states
  LeftButtonState = digitalRead(LeftButtonPin);
  CenterButtonState = digitalRead(CenterButtonPin);
  RightButtonState = digitalRead(RightButtonPin);

  buttonLogic();

  if (scale.is_ready()) {
    //long reading = scale.read();
    //display.setTextSize(1);
    //display.setCursor(90,24);
    //display.print(scale.get_units(10));
    //display.display();    
    //readingfromscale = scale.get_units(10);
    //delay(200);

  countdown = countdown-1;


if (countdown == 0) {
  countdown = 100;
  long reading = scale.read();
  display.setTextSize(1);
  if (menuItem == 2){ display.setTextColor(BLACK);} else {display.setTextColor(WHITE);}
  display.setCursor(90,24);
  display.print(scale.get_units(10));
  display.display();    
  //Serial.print("Reset");
}
    
  }    
  
  delay(50);
}

void buttonLogic(){
   //if on menu and left is pressed
  if (LeftButtonState == HIGH && currentScreen == 1) {
      if (menuItem == 1){
        menuItem = 3;
      } else {
        menuItem = menuItem-1;
      }
      
      display.clearDisplay();
      drawScreens();
      display.display();
  }

  //if on menu and center is pressed
  if (CenterButtonState == HIGH && currentScreen == 1) {
   if (menuItem == 3){
        menuItem = 1;
     } else {
       menuItem = menuItem+1;
     }
     
      display.clearDisplay();
      drawScreens();
      display.display();
  }

  // if on brew menu and pressed
  if (RightButtonState == HIGH && menuItem == 1) {
      currentScreen = 2;
      display.clearDisplay();
      drawScreens();
      display.display();
  }

  // if on tare menu and pressed
  if (RightButtonState == HIGH && menuItem == 2) {
      scale.tare();
      display.clearDisplay();
      drawScreens();
      display.display();
  }
}

void drawScreens(){
    // SCREEN MENU
  if (currentScreen == 1){
    
  if (menuItem == 1){
      drawMainMenu(1);
      drawNavMenu();
  }
  
  if (menuItem == 2){
      drawMainMenu(2);
      drawNavMenu();
  }
  
  if (menuItem == 3){
      drawMainMenu(3);
      drawNavMenu();
  }
}
  
  // SCREEN BREWING
  if (currentScreen == 2){
    display.setCursor(1,1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Brewing");
    display.setTextSize(1);
    display.setCursor(110,6);
    display.print(targetGrams);
  }

  // SCREED TARE DONE
  if (currentScreen == 4){
    display.setCursor(1,1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Done");
  }
}

void readFromScale(){
}

void Brew(){

}

void drawMainMenu(int no){

    if (no == 1){
      display.fillRect(0,0, SCREEN_WIDTH, 16, WHITE);
    }

    if (no == 2){
      display.fillRect(0,19, SCREEN_WIDTH, 16, WHITE);
    }

    if (no == 3){
      display.fillRect(0,38, SCREEN_WIDTH, 16, WHITE);
    }

    
    display.setTextSize(2);
    if (no == 1){ display.setTextColor(BLACK);} else {display.setTextColor(WHITE);}
    display.setCursor(1,1);
    display.print("Brew");

    display.setTextSize(2);
    if (no == 2){ display.setTextColor(BLACK);} else {display.setTextColor(WHITE);}
    display.setCursor(1,20);
    display.print("Tare");

    display.setTextSize(2);
    if (no == 3){ display.setTextColor(BLACK);} else {display.setTextColor(WHITE);}
    display.setCursor(1,39);
    display.print("Time");    
}

void drawNavMenu(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,NAVBAR_X_COORD);
    display.print("UP");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(45,NAVBAR_X_COORD);
    display.print("DOWN");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(90,NAVBAR_X_COORD);
    display.print("SELECT");    
}
