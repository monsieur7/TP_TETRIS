/* 
 * File:   newmain.c
 * Author: tw286615
 *
 * Created on 14 décembre 2023, 09:07
 */
// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FOSCSEL
#pragma config FNOSC = OSCFDIV          // Oscillator Source Selection (Oscillator with Frequency Divider)
#pragma config PLLMODE = DISABLED       // PLL Mode Selection (No PLL used; PLLEN bit is not available)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFCN = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config SOSCSEL = ON             // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration bit (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler bits (1:32,768)
#pragma config FWPSA = PR128            // Watchdog Timer Prescaler bit (1:128)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bits (WDT Enabled)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config WDTCMX = WDTCLK          // WDT MUX Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = ON               // Brown Out Enable bit (Brown Out Enable Bit)
#pragma config LPCFG = OFF              // Low power regulator control (No Retention Sleep)
#pragma config DNVPEN = ENABLE          // Downside Voltage Protection Enable bit (Downside protection enabled using ZPBOR when BOR is inactive)

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config BTSWP = OFF              // BOOTSWP Disable (BOOTSWP instruction disabled)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config SOSCHP = ON              // SOSC High Power Enable bit (valid only when SOSCSEL = 1 (Enable SOSC high power mode (default))
#pragma config ALTVREF = ALTREFEN       // Alternate Voltage Reference Location Enable bit (VREF+ and CVREF+ on RA10, VREF- and CVREF- on RA9)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define FCY 8000000
#include <xc.h>
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <libpic30.h>
#include "print_lcd.h"
#include <time.h>

/*
 * variables globales
 */


int length = 2;
int width = 16;

/*
 * DEFINITION DES CUSTOMS CHAR
 */

char custom0[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

char custom1[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

char custom2[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

char custom3[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};


void init(){
    /*
     *Fonction permettant d'init le lcd et d'écrire les custom char dans la 
     * memoire
     * 
     */
    LCD_Initialize();
    
    customChar(&custom0, 0x00);
    customChar(&custom1, 0x01);
    customChar(&custom2, 0x02);
    customChar(&custom3, 0x03);
}

void arrayHandler(char passed_array[4][16], char result[2][16]) {
    /*
     *Fonction permettant de transformer la matrice 4 16 en 2 16
     * @param:
     * passed array : matrice 4 par 16 de char contenant les caracteres (0 a 7)
     * result : matrice 2 par 16 de char binaire
     * permet l'utilisation des caractères speciaux
     *
     */
    
    int i, j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            result[i][j] = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 16; j++) {
            if (passed_array[i][j] != 0) {
                if (i % 2 == 0) {
                    result[i / 2][j] += 1;
                } else {
                    result[i / 2][j] += 2;
                }
            }
        }
    }
}



void print_lcd(char matrice[2][16]){
    /*
     * fonction permettant l'affichage de la matrice de char sur le LCD
     * @param : 
     * matrice de char en 4 par 16
     * 
     * transformation en 2 par 16
     * 
     */   

    LCD_ClearScreen();
    int i;
    int j;
    
    for(i=0;i<length;i++){
        for(j=0;j<width;j++){
            switch(matrice[i][j]) {
                case 0:
                    LCD_PutChar(0);
                    break;
                case 1:
                    LCD_PutChar(1);
                    break;
                case 2:
                    LCD_PutChar(2);
                    break;
                case 3:
                    LCD_PutChar(3);
                    break;    
                default :
                    LCD_PutChar(0);
                    break;
            } 
        }
    }  
}


void affichage(char matrice[4][16]){
    /*
     *Fonction permettant d'appeler les fct arrayHandler et print_lcd
     * @param:
     * matrice de char 4 par 16
     */
    char result[2][16];
    
    arrayHandler(matrice, result);
    print_lcd(result);
    __delay_ms(200);
}

int main(int argc, char** argv) {

    init();
    
    //creation d'une matrice test
    //pas besoin de l'utiliser --> on utilise votre matrice (liste <3)
    char matrice[4][16] = {
        {0, 1, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
        {0, 6, 4, 1, 3, 5, 1, 7, 6, 2, 4, 1, 5, 3, 0, 0},
        {0, 3, 5, 7, 2, 6, 1, 4, 1, 3, 5, 7, 2, 6, 0, 0},
        {0, 2, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0},

    };
    

    
    while(1){
        affichage(matrice);
    }
    return 1;
}

