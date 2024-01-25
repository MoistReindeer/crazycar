#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_adc12.h>
#include <DL/driver_conversion.h>

// DistanceTable DistanceTable;
extern ConversionData ConvertedData;
extern ADC12Com ADC12Data;

void Fetch_Distance() {
    int idf, idr, idl;
    idf = (int)(ADC12Data.SensorFront >> LUT_FACTOR);
    idr = (int)(ADC12Data.SensorRight >> LUT_FACTOR);
    idl = (int)(ADC12Data.SensorLeft >> LUT_FACTOR);

    ConvertedData.Distance.front = DistanceTable.ir_front[idf];
    ConvertedData.Distance.right = DistanceTable.ir_right[idr];
    ConvertedData.Distance.left = DistanceTable.ir_left[idl];
}
