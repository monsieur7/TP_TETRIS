
/*
 * File:   newmain.c
 * Author: vd407744
 *
 * Created on 14 décembre 2023, 09:12
 */

#include "mcc_generated_files/system.h"

#include "xc.h"
#include "time.h"
#include "stdlib.h"
//#include "mcc_generated_files/pin_manager.h"
#include "LCD.h"
#include <stdio.h>
#include <stdint.h>




#define FCY 8000000

/***************/
#define WIDTH 12
#define HEIGHT 12
/***************/

#define MASK_32 0xB4BCD35C
#define MASK_31 0x7A5BC2E3

/*
 * DEFINITION DES CUSTOMS CHAR
 */

char flagTimer1;
int randSeed = 0;
char tab[WIDTH * HEIGHT ] = {0};
char tabMergedForDisplay[WIDTH * HEIGHT] = {0};
char fallingPiece[WIDTH*HEIGHT] = {0}; // Tableau de la pièce qui tombe

int ligne[4][4] = { {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}};
int carre[4][4] = {{0, 0, 0, 0},
                    {0, 2, 2, 0},
                    {0, 2, 2, 0},
                    {0, 0, 0, 0}};
int L[4][4] = {{0, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 3, 3, 0},
                {0, 0, 0, 0}};
int LInverse[4][4] = {{0, 0, 0, 0},
                        {0, 0, 0, 4},
                        {0, 4, 4, 4},
                        {0, 0, 0, 0}};
int T[4][4] = {{0, 0, 0, 0},
                {0, 5, 0, 0},
                {5, 5, 5, 0},
                {0, 0, 0, 0}};
int zigzag[4][4] = {{0, 0, 0, 0},
                    {6, 6, 0, 0},
                    {0, 6, 6, 0},
                    {0, 0, 0, 0}};
int zigzagInverse[4][4] = {{0, 0, 0, 0},
                            {0, 7, 7, 0},
                            {7, 7, 0, 0},
                            {0, 0, 0, 0}};

short score = 0;
char pieceJustGotPlaced = 0;


typedef unsigned int uint;
uint lfsr32, lfsr31;

//declaration
char getValue(char x, char y);
void setValue(char x, char y, char value);
char checkFullLine(char line);
void endOfPlacement();
void deleteLine(char line);
char isTheGameLost();
void mergeTabsForDisplay();
void descente();
int* pieceAleatoire(void);
void transposition(int piece[4][4]);
void reverseCounterClockWise(int piece[4][4]);
void reverseClockWise(int piece[4][4]);
void translationGauche(int piece[4][4]);
void translationDroite(int piece[4][4]);
void copy(int startX, int startY, int piece[4][4]);
char get_random(void);

void copy(int startX, int startY, int piece[4][4]){
    for(int x = startX; x < startX+4; x++){
         for(int y = startY; y < startY+4; y++){
             fallingPiece[y*WIDTH+x] = piece[y-startY][x-startX];
        }
    }
}
int* pieceAleatoire(void)
{
    char nbRandom = get_random();
    char piece = nbRandom % 7;
        switch (piece) {
        case 0:
            //affichePiece(ligne);
            return ligne;
            break;

        case 1:
            //affichePiece(carre);
            return carre;
            break;

        case 2:
            //affichePiece(L);
            return L;
            break;

        case 3:
            //affichePiece(LInverse);
            return LInverse;
            break;

        case 4:
            //affichePiece(T);
            return T;
            break;

        case 5:
            //affichePiece(zigzagInverse);
            return zigzagInverse;
            break;

        case 6:
            //affichePiece(zigzag);
            return zigzag;
            break;

        default:
            break;
    }
}

void transposition(int piece[4][4])
{
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 4; ++j) {
            int temp = piece[i][j];
            piece[i][j] = piece[j][i];
            piece[j][i] = temp;
        }
    }
}

void reverseCounterClockWise(int piece[4][4]) 
{ 
    int temp;

    // Inverser les lignes
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            // Échanger les éléments entre les lignes opposées
            temp = piece[i][j];
            piece[i][j] = piece[3 - i][j];
            piece[3 - i][j] = temp;
        }
    } 
} 

void reverseClockWise(int piece[4][4]) 
{ 
    int temp;

    // Inverser les colonnes
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            // Échanger les éléments entre les colonnes opposées
            temp = piece[i][j];
            piece[i][j] = piece[i][3 - j];
            piece[i][3 - j] = temp;
        }
    }
} 

/*Une translation à gauche consiste à faire une transposition de matrice puis d'inverser les lignes*/
void translationGauche(int piece[4][4])
{
    transposition(piece);
    reverseCounterClockWise(piece);
}

/*Une translation à droite consiste à faire une transposition de matrice puis d'inverser les colonnes*/
void translationDroite(int piece[4][4])
{
    transposition(piece);
    reverseClockWise(piece);
}

//verifier collison entre pieces
bool collision(int piece1[4][4], int piece2[4][4])
{
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 4; ++j) {
            if (piece1[i][j] == 1 && piece2[i][j] == 1)
            {
                return true;
            }
        }
    }
    return false;
}


void descente() {
    char isThereA1 = 0; // Flag to indicate if a '1' is found
    uint8_t preLigne1 = 0; // First line with '1'
    uint8_t derLigne1 = 0; // Last line with '1'

    for (int i = 0; i < WIDTH * HEIGHT; i += WIDTH) {
        int test = 0; // Variable to test if there is a '1' in the line

        // Check if there is at least one '1' in the falling piece
        for (int k = 0; k < WIDTH; k++) {
            test = test | fallingPiece[i + k];
        }

        if (test == 0) {
            // If there are no '1's in the line
            if (isThereA1) {
                // If there was a '1' in a previous line
                derLigne1 = (i - WIDTH) / WIDTH;
                break;
            }
        } else {
            // If there is at least one '1' in the line
            if (!isThereA1) {
                // If there was no '1' in a previous line
                isThereA1 = 1;
                preLigne1 = i / WIDTH;
            }
        }
    }

    // Move the falling piece down
    for (int j = preLigne1 * WIDTH; j <= derLigne1 * WIDTH + WIDTH - 1; j += 1) {
        if (tab[j + WIDTH] == 0 && (j + WIDTH) < HEIGHT * WIDTH) {
            // If there is an empty space below and we haven't reached the bottom
            if ((tab[j + WIDTH] || fallingPiece[j]) == 0) {
                // If both tab[j + WIDTH] and fallingPiece[j] are zero
                // Move the elements down one row
                for (int h = derLigne1; h > preLigne1; h--) {
                    fallingPiece[h * WIDTH + WIDTH - 1] = fallingPiece[(h - 1) * WIDTH + WIDTH - 1];
                }
                // Set the top row to zero
                for (int h = 0; h < WIDTH; h++) {
                    fallingPiece[preLigne1 * WIDTH + h] = 0;
                }
            }
        } else {
            // If there is a collision or we've reached the bottom
            isThereA1 = 0;
            pieceJustGotPlaced = 1;
            break;
        }
    }

    // If the piece got placed, update the main game array
    if (pieceJustGotPlaced) {
        for (int i = 0; i < HEIGHT * WIDTH; i++) {
            // Use the AND operator to erase the original blocks
            tab[i] = tab[i] & ~fallingPiece[i];
            fallingPiece[i] = 0;
        }
    }
}


char getValue(char x, char y)    //return the value like it was a 2d array
{
    return tab[y*WIDTH + x];
}

void setValue(char x, char y, char value)   //set the value like it was a 2d array
{
    tab[y*WIDTH + x] = value;
}

void mergeTabsForDisplay()     //allow the displaying of the piece and the board during the fall
{
    for(int i=0; i< WIDTH * HEIGHT; i++)
    {
        tabMergedForDisplay[i] = fallingPiece[i] | tab[i];
    }
}
        
void interruptTimer()   // set the game at 4fps
{
    flagTimer1 = 1;
}

int shift_lfsr(uint *lfsr, uint polynomial_mask)   //used for randomisation (clack box)
{
    int feedback;

    feedback = *lfsr & 1;
    *lfsr >>= 1;
    if(feedback == 1)
        *lfsr ^= polynomial_mask;
    return *lfsr;
}

void init_lfsrs(void)     //used for randomisation (clack box)
{
    lfsr32 = 0xABCDE; /* seed values */
    lfsr31 = 0x23456789;

}

char get_random(void)    //used for randomisation (clack box)
{
    shift_lfsr(&lfsr32, MASK_32);
    return(shift_lfsr(&lfsr32, MASK_32) ^ shift_lfsr(&lfsr31, MASK_31)) & 0xffff;
}

char checkFullLine(char line)      //return 1 if the given ligne is full
{
    char isLineFull = 1;
    for(char i = 0; i < WIDTH; i++)
    {
        if (getValue(i, line) == 0)
        {
            isLineFull = 0;
        }
    }
    return isLineFull;
}

void endOfPlacement()     // check if a line is full after the piece is placed
{
    for(char i = WIDTH - 1; i >= 0 ; i--)
    {
        if (checkFullLine(i))
        {
            deleteLine(i);
        }
    } 
}

void deleteLine(char line) {
    for (char i = line - 1; i >= 0 ; i--) {
        for (char j = 0; j < WIDTH; j++) {
            setValue(i, j, getValue(i-1, j));
        }
    } 
}

char isTheGameLost()    //check if a piece is placed in the buffer zone
{
    char lost = 0;
    for (char i = 0; i < 4; i++)
    {
        for (char j = 0; j < WIDTH; j++)
        {
            if( getValue(j, i) != 0)
            {
                lost = 1;
            }
        }
    }
    return lost;
}


int main(void) {
    SYSTEM_Initialize();
    LCD_INIT();
    LCD_Clear();
    TMR1_SetInterruptHandler(&interruptTimer);
    
    while(!RAND_BUTTON_GetValue());
    while(RAND_BUTTON_GetValue())
    {
        randSeed++;
    }
    
    randSeed%=4096;
    
    LATA = randSeed;
    
    lfsr32 = randSeed;
    lfsr31 = randSeed%97;
    
    char gameIsLost = 0;
    
     
    copy(WIDTH/2-2, 2, pieceAleatoire());
    init();
    
    while(!gameIsLost)
    {
        
        if (flagTimer1) // interrupt every 0.25s
        {
            mergeTabsForDisplay();
            descente();
            LCD_sendPixelArray(12, 12, tabMergedForDisplay);
            //affichage(tab);
            flagTimer1 = 0;
        }
        
        if(pieceJustGotPlaced)
        {
            pieceJustGotPlaced = 0;
            endOfPlacement();
            gameIsLost = isTheGameLost();
            //copy(WIDTH/2-2, 2, pieceAleatoire());
        }
    }
    return 1;
}




