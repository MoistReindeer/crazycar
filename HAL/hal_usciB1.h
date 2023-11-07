#ifndef _HAL_USBCI_B1_
#define _HAL_USBCI_B1_

#define UCLK 100000

typedef struct {
    struct{
        unsigned char TxSuc; // Bit=1 wenn Daten übertragen wurden
    }Status;
    struct {
        unsigned char len; // Länge der Daten in Bytes die übertragen werden
        unsigned char cnt; // Index auf momentan übertragene Daten
        unsigned char Data[256]; // Tx Daten Array
    }TxData;
    struct {
        unsigned char len; // Länge der empfangenen Daten
        unsigned char Data[256]; // Rx Daten Array
    }RxData;
}USCIB1_SPICom;

void HAL_USCIB1_Init(void);
void HAL_USCIB1_Transmit(void);

#endif /* _HAL_USBCI_B1_ */
