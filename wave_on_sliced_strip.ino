#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
// generates advencing wave - the power is function of the index - the same pixel will always be with the same power 

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
#define PIN_1           5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       13
#define NUMPIXELS2      20
#define NUMPIXELS3      14

#define POWER 20 // the vase power of the leds

#define UNCONNECTED_ANALOG_PIN 0// for random seed

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS+NUMPIXELS2+NUMPIXELS3, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS+NUMPIXELS2+NUMPIXELS3, PIN_1, NEO_GRB + NEO_KHZ800);


int max_pix;
int fire_pix[6][6]; // [line][color_in_line]

void setup() {
  Serial.begin(9600);  
    pixels.begin(); // This initializes the NeoPixel library.
    pixels2.begin(); // This initializes the NeoPixel library.
    max_pix = max(NUMPIXELS,NUMPIXELS2);
    max_pix = max(max_pix,NUMPIXELS3);
    randomSeed(analogRead(UNCONNECTED_ANALOG_PIN));


}
/////////////////////////////////////////////////////////////////
void loop() {
/////////////////////////////////////////////////////////////////  
   startGradiant(1,0,20,10); // gradiant without powered led startGradiant(int color_jump, int power_jump,int delayval, int num_of_cycles)
//  startGradiant(0,1,5,10); // up and down powered led
//  startFire(50,10000); //startFire(delay_time, time_left)
   startWings(60,10000,17); //startWings(int delay_time, int time_left, int num_of_colors)

}

/////////////////////////////////////////////////////////////////
void startWings(int delay_time, int time_left, int num_of_colors) {
/////////////////////////////////////////////////////////////////  
   int j=0;
   clearAll();   
   while (time_left > 0) {     
    setWingLine(getColor(j,POWER,num_of_colors),NUMPIXELS,0);
    setWingLine(getColor(j+1,POWER,num_of_colors),NUMPIXELS2,NUMPIXELS);
    setWingLine(getColor(j+2,POWER,num_of_colors),NUMPIXELS3,NUMPIXELS+NUMPIXELS2);
    delay(delay_time);
    time_left -= delay_time;
    showPixels();
    j++;
  }
    
}

/////////////////////////////////////////////////////////////////
void setWingLine(uint32_t color, int num_of_pixels, int start_pixel) {
/////////////////////////////////////////////////////////////////  
    //Serial.println(start_pixel);
    //Serial.println(num_of_pixels);
    for (int i = start_pixel ; i<start_pixel+num_of_pixels; i++) {
      //Serial.println(i);
      addPixel(i,color ,1); //FIRST STRIP
      addPixel(NUMPIXELS+NUMPIXELS2+NUMPIXELS3-i,color ,2); //SECOND STRIP
    }
}

/////////////////////////////////////////////////////////////////
void startFire(int delay_time, int time_left) {
/////////////////////////////////////////////////////////////////  
  while (time_left > 0) {
    clearAll();
    // STRIP 1
      //LINE 1
    int base_pix = 0;
    int max_led = NUMPIXELS;
    addFireLine(max_led,fire_pix[0],base_pix,1,1);
    
      //LINE 2
    base_pix = NUMPIXELS+NUMPIXELS2-1;
    max_led = NUMPIXELS2;
    addFireLine(max_led,fire_pix[1],base_pix,-1,1);
    
      //LINE 3  
    base_pix = NUMPIXELS+NUMPIXELS2;
    max_led = NUMPIXELS3;
  //  Serial.println(fire_pix[2][0]); 
    addFireLine(max_led,fire_pix[2],base_pix,1,1);
  
  
    // STRIP 2
      //LINE 1
    base_pix = 0;
    max_led = NUMPIXELS;
    addFireLine(max_led,fire_pix[0],base_pix,1,2);
    
      //LINE 2
    base_pix = NUMPIXELS+NUMPIXELS2-1;
    max_led = NUMPIXELS2;
    addFireLine(max_led,fire_pix[1],base_pix,-1,2);
    
      //LINE 3  
    base_pix = NUMPIXELS+NUMPIXELS2;
    max_led = NUMPIXELS3;
    addFireLine(max_led,fire_pix[2],base_pix,1,2);
  
    showPixels();   
    delay(delay_time);
    time_left -= delay_time;
  }
}
/////////////////////////////////////////////////////////////////
void addFireLine(int max_led,int * fire_pix,int base_pix,int dir,int strip) {
/////////////////////////////////////////////////////////////////  
  addFireColor(pixels.Color(0,0,POWER),0,max_led/3,fire_pix[0],base_pix,dir,strip); // blue
  fire_pix[1] = max(fire_pix[0],fire_pix[1]);
  addFireColor(pixels.Color(0,0.7*POWER,0.3*POWER),fire_pix[0],2*max_led/3,fire_pix[1],base_pix,dir,strip);  // turqiz
  fire_pix[2] = max(fire_pix[1],fire_pix[2]);  
  addFireColor(pixels.Color(0.5*POWER,0.5*POWER,0),fire_pix[1],max_led,fire_pix[2],base_pix,dir,strip); //yellow
  fire_pix[3] = max(fire_pix[2],fire_pix[3]);  
  addFireColor(pixels.Color(POWER,0,0),fire_pix[2],max_led,fire_pix[3],base_pix,dir,strip); //red

}
/////////////////////////////////////////////////////////////////
void addFireColor(uint32_t color, int start,int max_pix,int &fire_pix,int base_pix,int dir,int strip) {
/////////////////////////////////////////////////////////////////  
  long randNumber;
  randNumber = random(-1,2);
//  Serial.println(randNumber); 
  int color_end = fire_pix+randNumber;
//  if (dir == -1) {
  //  Serial.println(color_end); 
 // }
  if (color_end<start) {
      color_end = start+1;
  }
  if (color_end>max_pix) {
    color_end = max_pix;
  }
 // Serial.println(max_pix); 
  fire_pix=color_end;
//  color_end = color_end + base_pix;
//  start = start + base_pix;
  for (int i = start; i < color_end ; i++) { 
    int index;    
    if (base_pix != 0) {
      if (dir == -1) {
        index = base_pix-i;
      } else {
        index = base_pix+i;
      }
    } else {
      index = i;      
    }
    addPixel(index, color, strip);
  }
}

/////////////////////////////////////////////////////////////////
void clearAll() {
/////////////////////////////////////////////////////////////////  
  for (int i=0; i<NUMPIXELS+NUMPIXELS2+NUMPIXELS3; i++) {
    addPixel(i,pixels.Color(0,0,0),0);
  }
  
}
/////////////////////////////////////////////////////////////////
// startGradiant
// color_jump - each iteration the color will move this amount of pixels
// power_jump - each iteration the led with more power will move this amount of pixels
// delayval - the delay between to iterations
// num_of_cycel - number of cycles that the function will run
/////////////////////////////////////////////////////////////////
void startGradiant(int color_jump, int power_jump,int delayval, int num_of_cycles) {
/////////////////////////////////////////////////////////////////
  int color_i =0;
  int power_i = 0;
  int color_add =color_jump;
  int power_add =power_jump;
  for (int a =1 ; a<2*max_pix*num_of_cycles;a++) { // number of cycles it will run 
//    Serial.println(power_i);
    //    Serial.println(max_pix);
    color_i = color_i +color_add;
    power_i = power_i + power_add;
    if (color_i >= max_pix) {
        color_add = -color_jump;
    } else if (color_i <= 0) {
        color_add = color_jump;
    }
    if (power_i > max_pix) {
      power_add = -power_jump;
    } else if (power_i <= 0) {
      power_add = power_jump;
    }
    gradiant(color_i,power_i,0,0);
    int delay_val;
    if (a%max_pix >= max_pix-5) {
      delay_val = 1.5*delayval;
    } else {
      delay_val = delayval;
    }
    delay(delay_val); // Delay for a period of time (in milliseconds).
//    Serial.print(power_i);
//    Serial.print(" ");
//    Serial.println(color_i);
  }
}
/////////////////////////////////////////////////////////////////
// gradiant:
//   color_i - color changing from the index base color (for color movement)
//   power_i - the led index to be whith more power
//   strip - 0 - both strips; 1 - strip 1; 2 - strip 2
//   line  - 0 - all the lines ; 1 - line 1; 2 - line 2; 3- line 3 
/////////////////////////////////////////////////////////////////
void gradiant(int color_i,int power_i,int strip,int line) { 
/////////////////////////////////////////////////////////////////  
 for(int i=0;i<max_pix;i++){
  //  int power = sin(PI*(float)((i%WAVE_WIDTH)+1)/(WAVE_WIDTH+1))*50;
    int power = POWER;
 //   Serial.println(i);
    int power_tmp;
    if (i==power_i+1 || i==power_i-1) {
      power_tmp=power+40;
   //   Serial.println(i);
    } else if (i==power_i) {
      power_tmp=power+80;      
    } else {
      power_tmp=power;
    }

    // FIRST LINE
    if (i<NUMPIXELS && line == 1 || line == 0) {
      addPixel(i, getColor(i+color_i,power_tmp,max_pix),strip);
    }
 
    // SECOND LINE
    int index= NUMPIXELS+NUMPIXELS2-1-i;
    if (index>=NUMPIXELS && line == 2 || line == 0) {
	  addPixel(index, getColor(i+color_i,power_tmp,max_pix),strip);
    }
 

    // THIRD LINE   
    if (line == 3 || line == 0) {
      index= NUMPIXELS+NUMPIXELS2+i;
      addPixel(index, getColor(i+color_i,power_tmp,max_pix),strip);
    } 
  }
  showPixels();   

}

/////////////////////////////////////////////////////////////////
uint32_t getColor(int i, int power, int max_pix) {
/////////////////////////////////////////////////////////////////
  i = i%max_pix;
  if (i<max_pix/3) {
       int p = i*3*power/max_pix;
//         Serial.print(p);         
//         Serial.println(power);                               
       return pixels.Color(power-p,p,0);
  } else if (i <2*max_pix/3) {
      // int p = i*3*power/(2*max_pix);
       int p = (i-max_pix/3)*3*power/max_pix;
       //Serial.println(power);
       //Serial.println(p);
       return pixels.Color(0,power-p,p);
  } else {
       //int p = i*power/max_pix;
       int p = (i-2*max_pix/3)*3*power/max_pix;
       return pixels.Color(p,0,power-p); 
  }
}

/////////////////////////////////////////////////////////////////
void showPixels() {
/////////////////////////////////////////////////////////////////  
   pixels.show(); // This sends the updated pixel color to the hardware.
   pixels2.show(); // This sends the updated pixel color to the hardware.
}

/////////////////////////////////////////////////////////////////
void addPixel(int i, uint32_t color,int strip) {
/////////////////////////////////////////////////////////////////  
   switch (strip) {
     case 0:
          pixels.setPixelColor(i, color); 
          pixels2.setPixelColor(i, color); 
          break;
     case 1:
          pixels.setPixelColor(i, color); 
          break;
     case 2:
          pixels2.setPixelColor(i, color); 
          break;      
   }
}
