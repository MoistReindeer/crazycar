#ifndef _HAL_USBCI_B1_
#define _HAL_USBCI_B1_

#define UCLK 100000

#define LCD_CS_LOW  P8OUT &= ~LCD_SPI_CS;
#define LCD_CS_HIGH  P8OUT |= LCD_SPI_CS;

typedef struct {
    struct{
        unsigned char TxSuc; // Bit=1 wenn Daten �bertragen wurden
    }Status;
    struct {
        unsigned char len; // L�nge der Daten in Bytes die �bertragen werden
        unsigned char cnt; // Index auf momentan �bertragene Daten
        unsigned char Data[256]; // Tx Daten Array
    }TxData;
    struct {
        unsigned char len; // L�nge der empfangenen Daten
        unsigned char Data[256]; // Rx Daten Array
    }RxData;
}USCIB1_SPICom;

void HAL_USCIB1_Init(void);
void HAL_USCIB1_Transmit(unsigned char len);

#endif /* _HAL_USBCI_B1_ */
