/*
* Project1GoodVersion.c
*
* Created: 2/10/2020 2:25:47 PM
* Author : Ashton, Brad
*/
#include <avr/io.h>
// Main Code
int main(void){
  DDRB = 0b00000001;
  uint16_t index0;
  uint16_t index1;
  int halfp = 0;
  int half_period_map[25] = {3822, 3608, 3405, 3214, 3034, 2864, 2703, 2551, 2408,
  2273, 2145, 2025,
  1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 955};
  while(1){
    ADMUX = 0b01000101; // [0:3] determine input (0101 => potentiometer)
    ADCSRA = 0b11000111; // bits [0;2] ar div factors, not sure what they do really
    uint16_t adc = ADC; // gives value of voltage range 0x000 to 0x03ff, it is that
    range bc 3ff is 2^10-1
    char index = adc/41;
    halfp = half_period_map[index];
    PORTB = 0b00000001;
    index0 = halfp*7/6; // there are approx 7/6 loops per microsecond
      while(index0 > 0){
        index0--;
      }
    PORTB = 0b00000000;
    index1 = halfp*7/6;
    while (index1 > 0){
      index1--;
    }
  }
}
