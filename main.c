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
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"

#define SYS_FREQ        16000000L
#define FCY             (SYS_FREQ/2)
#include <libpic30.h>  

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    R_W_SetLow(); //SPI MODE
    EN_SetDigitalOutput(); // ENABLE LCD
    EN_SetHigh();
    __delay_ms(2);
    RST_SetLow(); //RESET
    __delay_us(5);
    RST_SetHigh();
    __delay_ms(2);
    //AFh : enable
    sendCommand(0xAF);
    //REFRESH RATE :
    sendCommand(0xB3);
    sendData(1);
    
    int i = 0;
    while (1)
    {
        for(int y = 0; y < 127; y++){
            for(int x = 0;  x < 127; x++){
                sendPixel(x, y, 0xf, 0x0, 0x0);
            }
        }
        // Add your application code
    }

    return 1;
}
/**
 End of File
*/

void sendPixel(int x, int y, int r, int g, int b){
    //SET ADDRESS
    sendCommand(0x15);
    sendData(x);
    sendData(x+1);
    
    sendCommand(0x75);
    sendData(y);
    sendData(y+1);
    
    sendCommand(0x5c); // WRITE
    sendData((g & 0x07) | ((r & 0x1F) << 3)); // Combine green and red
    sendData((b & 0x1F) | ((g & 0x07) << 5)); // Combine blue and green
   //sendCommand(0x5c); //DISABLE WRITE NOT NEEDED

    
}
void sendCommand(int command){
    CS_SetLow();
    D_C_SetLow();
    SPI1_Exchange8bit(command);
    __delay_us(5);
    CS_SetHigh();
}

void sendData(int data){
    CS_SetLow();
    D_C_SetHigh();
    SPI1_Exchange8bit(data);
    __delay_us(5);
    CS_SetHigh();
}
