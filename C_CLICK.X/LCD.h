/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include <xc.h> // include processor files - each processor file is guarded.  

#define SYS_FREQ        8000000L
#define FCY             (SYS_FREQ/2)
#include <libpic30.h>  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

//RGB MAP SHENANIGANS :
// Define a structure to hold the RGB color components
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGBColor;

// Function to map a 4-bit color to a 6-bit RGB color
RGBColor map4BitTo6BitColor(uint8_t color4Bit) {
    // Scale the 4-bit color to a 6-bit value (0-63)

    // Extract RGB components from the 6-bit value
    RGBColor result;
    result.r = (color4Bit& 0x3)*10;          // Bits 5-3
    result.g = ((color4Bit&0xc)>>2)*10; // Bits 2-1
    result.b = ((color4Bit&0xc)>>2)*10; // Bit 0 shifted to 4

    return result;
}

void LCD_sendCommand(int command){
    
    CS_SetLow();
    D_C_SetLow();
    SPI1_Exchange8bit(command);
    __delay_us(5);
    CS_SetHigh();
}

void LCD_sendData(int data){
    CS_SetLow();
    D_C_SetHigh();
    SPI1_Exchange8bit(data);
    __delay_us(5);
    CS_SetHigh();
}
 
void LCD_INIT(){
    R_W_SetLow(); //SPI MODE
    __delay_ms(2);
    RST_SetLow(); //RESET
    __delay_us(5);
    RST_SetHigh();
    __delay_ms(2);
    EN_SetHigh();
    __delay_ms(250);

    //AFh : enable
    LCD_sendCommand(0xAF);
    //REFRESH RATE :
    LCD_sendCommand(0xB3);
    LCD_sendData(1);
    
 
     // Unlock display and turn off
    LCD_sendCommand(0xFD);
    LCD_sendData(0x12);

    LCD_sendCommand(0xFD); //UNLOCK
    LCD_sendData(0xB1);

    // Setup SSD1351
    LCD_sendCommand(0xA0); // Set Remap and Color Depth
    LCD_sendData(0x74);     // Default: 0x74

    LCD_sendCommand(0xA1); // Set Display Start Line
    LCD_sendData(0x80);     // Default: 0x80

    LCD_sendCommand(0xA2); // Set Display Offset
    LCD_sendData(0x20);     // Default: 0x20

    //LCD_sendCommand(0xA4); // Set Display Mode to Normal
    //LCD_sendCommand(0xAF); // Display ON
    
    //SET MUX RATIO
    
    LCD_sendCommand(0xCA);
    LCD_sendData(95);
}
void LCD_sendPixel(char x, char y, RGBColor rgb){
    //SET ADDRESS (not needed defined in sendArray)
    //LCD_sendCommand(0x5c); // WRITE
    LCD_sendData((rgb.g & 0x07) | ((rgb.r & 0x1F) << 3)); // Combine green and red
    LCD_sendData((rgb.b & 0x1F) | ((rgb.g & 0x07) << 5)); // Combine blue and green
   //sendCommand(0x5c); //DISABLE WRITE NOT NEEDED
    
    

    
}

void LCD_sendPixelArray(char lenX, char lenY, char * tab){
    LCD_sendCommand(0x15); // column
    LCD_sendData(0x10);
    LCD_sendData(0x10+lenX*8);
    
    LCD_sendCommand(0x75); //row
    LCD_sendData(0);
    LCD_sendData(0+lenY*8);
    LCD_sendCommand(0x5c); // WRITE

    for (char y = 0; y < lenY; y++) {
        for (char x = 0; x < lenX; x++) {

            char pixel1 = tab[y * lenY + x];

            // Map 4-bit color to 6-bit RGB
            RGBColor color1 = map4BitTo6BitColor(pixel1);
             LCD_sendCommand(0x15); // column
            LCD_sendData(0x10+x*8);
            LCD_sendData(0x10+x*8+8);
    
            LCD_sendCommand(0x75); //row
            LCD_sendData(0+y*8);
            LCD_sendData(0+y*8+8);
            LCD_sendCommand(0x5c); // WRITE
            for(char i = 0; i < 8; i++){
                for(char j = 0; j < 8; j++){
                    LCD_sendPixel((x * 8) + i, (y * 8) + j, color1);
                }
            }

           
        }
    }
}
void LCD_Clear(){
    LCD_sendCommand(0x5c); // WRITE
    for(char x = 0; x < 127; x++){
        for(char y = 0; y < 127; y++){
            LCD_sendData(0);
            LCD_sendData(0);
        }
    }
}
LCD_resetXY(){
    
     //SET ADDRESS
    LCD_sendCommand(0x15); // column
    LCD_sendData(0);
    LCD_sendData(127);
    
    LCD_sendCommand(0x75); //row
    LCD_sendData(0);
    LCD_sendData(127);
}
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

