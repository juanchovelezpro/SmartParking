
#include <Servo.h>

#include <Adafruit_MPR121.h>

#include <Wire.h>

#include <LiquidCrystal.h>


/*
 * Universidad ICESI 
 * Electronica & Laboratorio Semestre 18-2
 * Proyecto Electronica 
 * Juan Camilo Vélez Olaya - Daniel Alejandro Cerquera Castro
 * 
 * Parqueadero Inteligente - Smart Parking
 * 
 * Componentes:
 * 
 * 
 * 
 */
// Screen D4, D5, D6, D7, RS, E.
// RS, E , D4, D5, D6, D7 ---  LCD Screen 16x2
LiquidCrystal lcd(26, 27, 22, 23, 24, 25);

Adafruit_MPR121 sensor = Adafruit_MPR121();

uint16_t lasttouched = 0;
uint16_t currtouched = 0;


const int slots = 6;
int light = 42; // This pin turn on and turn off the light. (12 leds)
int red[slots];  // These arrays are to setup pins where leds will be.
int green[slots];
int Free = 0;
boolean Active[slots];   // These boolean arrays are to update the value Free.
boolean Desactive[slots];
int ldr = A0;
int power = 0;

Servo Entrance;
Servo Exit;


void setup() {

  Serial.begin(9600);
  lcd.begin(16,2);
  //credits();
  
  settingLedPins();  
  initSensorSlots();

  pinMode(ldr,INPUT);
  
  Entrance.attach(8);
  Exit.attach(9);

  Entrance.write(180);
  Exit.write(180);
  
  
  
}

void loop() {  

  refreshSlotsState();
  refreshApp();
  EntranceAndExit(); 

 power = analogRead(ldr);

 if(power < 400){

 digitalWrite(light, HIGH);
  
 }else{

 digitalWrite(light, LOW);
  
 }


 
}


 /*
   * The way like we connect our leds.
   * Example : Slot1 = RED --> Pin 30 ; GREEN --> 31 
   *           Slot2 = RED --> Pin 32 ; GREEN --> 33 
   * We connect our leds like this for every slot. 
   * 
   */

void settingLedPins(){

 

  // This loop refill the array red with integer values where we connect our led's.

  int h = 40;
  for(int i = 0; i<slots; i++){

  red[i] = h; 

  h -= 2;
    
  }

  // This loop refill the array green with integer values where we connect our led's. 
 
  int k = 41;
  for(int i = 0; i<slots; i++){

  green[i] = k;

  k -= 2;
    
  }

  // This loop declares every position of the arrays as an OUTPUT
  for(int i = 0; i<slots; i++){

  pinMode(red[i], OUTPUT);  
  pinMode(green[i], OUTPUT);
  
  }

  for(int i = 0; i<slots; i++){

  digitalWrite(green[i], HIGH);
    
  }

  //This loop refill values "true" in the array Desactive and "false" in the array Active

  for(int i = 0; i<slots; i++){

  Active[i] = false;
  Desactive[i] = true;
    
  }

  pinMode(light, OUTPUT);
 

 
  
}



void initSensorSlots(){

  if(!sensor.begin(0x5A)){

  Serial.println("Sensor not found");
  while(1); 
  }

  Serial.println("Sensor found!");
  sensor.setThreshholds(12, 8);
    
  
  
}

// This method only refresh all slots states.
void refreshSlotsState(){


 // Get the currently touched pads

  currtouched = sensor.touched();

        if (sensor.filteredData(0) < 170) {
      digitalWrite(red[0], HIGH);
      digitalWrite(green[0], LOW);
  
  }else{
   
      digitalWrite(red[0], LOW);
      digitalWrite(green[0], HIGH);
      
    }

      if (sensor.filteredData(1) < 170) {
      digitalWrite(red[1], HIGH);
      digitalWrite(green[1], LOW);
  
  }else{
   
      digitalWrite(red[1], LOW);
      digitalWrite(green[1], HIGH);
      
    }

    if (sensor.filteredData(2) < 200) {
      digitalWrite(red[2], HIGH);
      digitalWrite(green[2], LOW);
  
  }else{
   
      digitalWrite(red[2], LOW);
      digitalWrite(green[2], HIGH);
      
    }

          if (sensor.filteredData(3) < 195) {
      digitalWrite(red[3], HIGH);
      digitalWrite(green[3], LOW);
  
  }else{
   
      digitalWrite(red[3], LOW);
      digitalWrite(green[3], HIGH);
      
    }

          if (sensor.filteredData(4) < 175) {
      digitalWrite(red[4], HIGH);
      digitalWrite(green[4], LOW);
  
  }else{
   
      digitalWrite(red[4], LOW);
      digitalWrite(green[4], HIGH);
      
    }

   if (sensor.filteredData(5) < 175) {
      digitalWrite(red[5], HIGH);
      digitalWrite(green[5], LOW);
  
  }else{
   
      digitalWrite(red[5], LOW);
      digitalWrite(green[5], HIGH);
      
    }

  for (uint8_t i=0; i<6; i++) {


    // it if *is* touched and *wasnt* touched before, alert!

   // if ((currTouched & _BV(i)) && !(lastTouched & _BV(i)) ) {

    //  Serial.print(i); Serial.println(" touched");
     
     
    if(digitalRead(red[i]) == LOW && !Active[i]){

    Free++;
    Active[i] = true;
    Desactive[i] = false;
      
    }else if(digitalRead(red[i]) == HIGH && !Desactive[i]){

    Free--;
    Active[i] = false;
    Desactive[i] = true;
      
    }
 }

   lcd.setCursor(13,0);
   lcd.print(Free);
   
    
  lasttouched = currtouched; //  // reset our state

  // comment out this line for detailed data from the sensor!
  return;

    
  }
  



void refreshApp(){

  switch(Free){

  case 6:
  Serial.println("6");
  delay(500);
  break;
  
  case 5: 
  Serial.println("5");
  delay(500);
  break;

  case 4: 
  Serial.println("4");
  delay(500);
  break;

  case 3: 
  Serial.println("3");
  delay(500);
  break;

  case 2: 
  Serial.println("2");
  delay(500);
  break;

  case 1: 
  Serial.println("1");
  delay(500);
  break;

  case 0: 
  Serial.println("0");
  delay(500);
  break;
    
  }
  

  
}


void EntranceAndExit(){


  if(sensor.filteredData(8) < 140){

  Entrance.write(90);  
    
  }

  if(sensor.filteredData(9) < 145){

  Entrance.write(180);
    
  }

  if(sensor.filteredData(11) < 170){

  Exit.write(90);
    
  }
    
  if(sensor.filteredData(10) < 140){

  Exit.write(180);
    
  }

  
}


void credits(){

  lcd.setCursor(2,0);
  lcd.print("JUAN CAMILO");
  lcd.setCursor(2,1);
  lcd.print("VELEZ OLAYA");

  delay(3500);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("DANIEL ALEJANDRO");
  lcd.setCursor(0,1);
  lcd.print("CERQUERA CASTRO");
  
  delay(3500);

  lcd.clear();

   lcd.setCursor(3,0);
  lcd.print("PROYECTO DE");
  lcd.setCursor(3,1);
  lcd.print("ELECTRONICA");

  delay(2500);

  lcd.clear();
  
  lcd.setCursor(5,0);
  lcd.print("SMART");
  lcd.setCursor(4,1);
  lcd.print("PARKING");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DISPONIBLES: ");
  
  
}
