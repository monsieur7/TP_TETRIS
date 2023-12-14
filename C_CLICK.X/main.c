/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ1024GA610
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
//PIN
//R/W :P25 : RB10 FAIT
//CS : P14 : RG9 FAIT
//RST : P95 :RF3 FAIT
//EN : P67 : RD12 FAIT
//SDI : P12 : MOSI :RG8 FAIT
//SDO : P11 : MISO : RG7 FAIT
//SCK : P10 : SCK : RG6 FAIT
//D/C : P72 : COMMAND / DATA :RD0 FAIT

#include "mcc_generated_files/system.h"
#include "LCD.h"

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    LCD_INIT();
    LCD_Clear();
    char array[12*12] = {0};
    for(char y = 0; y < 12; y++){
        for(char x = 0; x < 12; x++){
                array[y*12+x] = 0xcc;
            
        }
    }
    while (1)
    {
        LCD_sendPixelArray(10, 10, 12, 12, array);
     /*for(int x = 50; x < 96; x++){
        for(int y = 50; y < 96; y++){
            LCD_sendPixel(x, y, 0xf, 0xf, 0xf);
        }
    }
      * */
        
    
     //LCD_resetXY();
        // Add your application code
    }

    return 1;
}
/**
 End of File
*/

