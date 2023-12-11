#ifndef __HAL_ADC12__
#define __HAL_ADC12__

typedef struct {
    union{
        unsigned char R;
        struct {
            unsigned char ADCrdy:1; // Bit=1 wenn Daten übertragen wurden
            unsigned char dummy:7;
        }B;
    }Status;
    unsigned short ADCBuffer[4];
    unsigned short SensorLeft;
    unsigned short SensorRight;
    unsigned short SensorFront;
    unsigned short VBat;
} ADC12Com;
ADC12Com ADC12Data;

void HAL_ADC_Init(void);

#endif /* __HAL_ADC12__ */
