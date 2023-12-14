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

#define SYS_FREQ        16000000L
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
    
 
    
    //RESET DISPLAY OFSET :
    //https://linuxjedi.co.uk/2023/03/12/how-to-use-the-ssd1351-oled-display/
    LCD_sendCommand(0xFD);
    LCD_sendData(0x12);
    
    LCD_sendCommand(0xFD);
    LCD_sendData(0xB1);
    
    //SET VERTICAL OFSET :
    LCD_sendCommand(0xA2);
    LCD_sendData(0x0);
    
}
void LCD_sendPixel(int x, int y, int r, int g, int b){
    //SET ADDRESS
    LCD_sendCommand(0x15); // column
    LCD_sendData(y);
    LCD_sendData(y);
    
    LCD_sendCommand(0x75); //row
    LCD_sendData(x);
    LCD_sendData(x);
    
    LCD_sendCommand(0x5c); // WRITE
    LCD_sendData((g & 0x07) | ((r & 0x1F) << 3)); // Combine green and red
    LCD_sendData((b & 0x1F) | ((g & 0x07) << 5)); // Combine blue and green
   //sendCommand(0x5c); //DISABLE WRITE NOT NEEDED
    
    

    
}

void LCD_sendPixelArray(char startX, char startY, char lenX, char lenY, char * tab){
     //SET ADDRESS
    LCD_sendCommand(0x15);
    LCD_sendData(startX);
    LCD_sendData(startX+lenX);
    
    LCD_sendCommand(0x75);
    LCD_sendData(startY);
    LCD_sendData(startY+lenY);
    
    LCD_sendCommand(0x5c); // WRITE
    int r = 0; //FOR TEST ONLY
    int g = 0;
    
    for(int y = startY; y < startY+lenY; y++){
        for(int x = startX; x < startX+lenX; x+=2){
            char pixel1 = tab[y*lenY + x] & 0xf;
            
            LCD_sendData((g & 0x07) | ((r & 0x1F) << 3)); // Combine green and red
            LCD_sendData((pixel1 & 0x1F) | ((g & 0x07) << 5)); // Combine blue and green
            
            char pixel2 = (tab[y*lenY + x] & 0xf0) >> 4;
            
            LCD_sendData((g & 0x07) | ((r & 0x1F) << 3)); // Combine green and red
            LCD_sendData((pixel2 & 0x1F) | ((g & 0x07) << 5)); // Combine blue and green
            
    }
    }
    
   
   //sendCommand(0x5c); //DISABLE WRITE NOT NEEDED
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

