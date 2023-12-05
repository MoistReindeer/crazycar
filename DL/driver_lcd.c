#include <msp430.h>
#include <DL/driver_lcd.h>
#include <HAL/hal_usciB1.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_fonttable.h>

extern USCIB1_SPICom SPICom;

void Driver_LCD_Init(void) {
    LCD_BACKLIGHT_ON;
    P9OUT &= ~LCD_RESET; // Set Reset

    __delay_cycles(500000); // Wait to complete the reset
    P9OUT |= LCD_RESET; // Unset Reset

    unsigned char LCD_Init_Array[9] = { // Config array
        LCD_RESET_FG,
        LCD_BIAS,
        ADC_SEL_NORMAL,
        COMMON_REVERSE,
        RES_RATIO,
        ELEC_VOL_MODE,
        ELEC_VOL_VALUE,
        POWER_CONT,
        DISPLAY_ON};

    // Send config to the LCD
    Driver_LCD_WriteCommand(LCD_Init_Array , 9);
    while(SPICom.Status.TxSuc == 0);

    Driver_LCD_Clear(); // lcd alle pixel löschen
    while(SPICom.Status.TxSuc == 0);

}

void Driver_LCD_SetPosition(unsigned char page, unsigned char col) {
    unsigned char LCD_Pos_Array[3];
    unsigned char msb_col = (MSB_COL_ADDR | (col >> 4));    //0001 xxxx shiften zu MSB Wert
    unsigned char lsb_col = col & 0x0F;                     //0000 xxxx maskieren auf LSB Wert

    LCD_Pos_Array[0] = SET_PAGE + page;                     // Page-Wert muss als erstes gesetzt werden
    LCD_Pos_Array[1] = msb_col;                             // MSB Column Address
    LCD_Pos_Array[2] = lsb_col;                             // LSB Column Address

    // LCD_POS_Array Befehle an LCD senden
    Driver_LCD_WriteCommand(LCD_Pos_Array, 3);
    // Warten bis Daten übertragen wurden
    while(SPICom.Status.TxSuc == 0);
}

void Driver_LCD_Clear() {
    unsigned char LCD_Pos_Array[3];
    unsigned char i,j;
    LCD_Pos_Array[1] = MSB_COL_ADDR;
    LCD_Pos_Array[2] = LSB_COL_ADDR;

    for(j=0x00; (SET_PAGE+j) <= LAST_PAGE; j++) {
        LCD_Pos_Array[0] = SET_PAGE+j;
        Driver_LCD_WriteCommand(LCD_Pos_Array, 3);

        while(SPICom.Status.TxSuc == 0);
        LCD_DATA;

        for(i=0; i < LCD_MAX_COLM; i++) {
            SPICom.TxData.Data[i] = 0x00;
        }
        HAL_USCIB1_Transmit(LCD_MAX_COLM);
    }
}

void Driver_LCD_WriteCommand(unsigned char *data, unsigned char len) {
     unsigned char i;
     while(SPICom.Status.TxSuc == 0); // Wait for previous transmission to finish
     LCD_COMMAND; // Write a command

     for(i=0; i< len; i++) { // Copy data into TxData structure
         SPICom.TxData.Data[i] = *data;
         data++;
     }

     // Transmit the data
     HAL_USCIB1_Transmit(len);
}

void Driver_LCD_WriteText(unsigned char *text, unsigned char len, unsigned char page, unsigned char col) {
    unsigned char i,j;
    Driver_LCD_SetPosition(page, col);
    while(SPICom.Status.TxSuc == 0); // Wait for previous transmission to finish
    LCD_DATA;

    for(j=0x00; j < len; j++) {
        if (col + fonts_width_max >= LCD_MAX_COLM) {
            Driver_LCD_SetPosition(page++, 0);
            col = 0;
            while(SPICom.Status.TxSuc == 0);
            LCD_DATA;
        }
        for(i=0x00; i < fonts_width_max; i++) {
            SPICom.TxData.Data[i] = font_table[*text][i];
            col++;
        }
        HAL_USCIB1_Transmit(fonts_width_max);
        while(SPICom.Status.TxSuc == 0);
        text++;
    }
    //TODO - Overflow detection
}

void Driver_LCD_WriteUInt(unsigned int num, unsigned char page, unsigned char col) {
    unsigned char i, j, str[DIGITS], rem;
    Driver_LCD_SetPosition(page, col);
    while(SPICom.Status.TxSuc == 0); // Wait for previous transmission to finish
    LCD_DATA;

    for (j = 0; j < DIGITS; j++) {       // Clean the string array
        str[j] = 0x00;
    }

    i = 4;
    do {                                // Convert the numbers to string
        rem = num % 10;
        str[i] = rem + '0';             // Add the 0-Offset
        num /= 10;
        i--;
    } while (num != 0); // TODO - Clean leading empty space

    Driver_LCD_WriteText(str,DIGITS,page,col);
}

void Driver_LCD_Test() {
    unsigned char data[2] = {
        ALL_POINTS_ON,
        ALL_POINTS_OFF
    };

    Driver_LCD_WriteCommand(&data[0], 1);
    Driver_LCD_WriteCommand(&data[1], 1);

    Driver_LCD_WriteText("test", 4, 3, 18);
    Driver_LCD_Clear();

    Driver_LCD_WriteUInt(1234,0,0);
    Driver_LCD_Clear();

    Driver_LCD_WriteText("This is a sentence to write some text to the lcd because why not", 64, 0, 0);
    Driver_LCD_Clear();
}
