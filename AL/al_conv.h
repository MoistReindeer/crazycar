#ifndef _DRIVER_CONVERSION_H_
#define _DRIVER_CONVERSION_H_

#define LUT_FACTOR 8
#define TABLE_SIZE 512

extern const unsigned short ir_front[TABLE_SIZE];
extern const unsigned short ir_right[TABLE_SIZE];
extern const unsigned short ir_left[TABLE_SIZE];

extern const unsigned short front_speed_tbl[212];

typedef struct {
    struct {
        unsigned short front;
        unsigned short left;
        unsigned short right;
        unsigned char done; // 1 when done converting distance, 0 when not done converting
    } Distance;

    unsigned short speed;
    unsigned short velocity_dd;
    unsigned short battery_voltage;
}ConversionData;

extern ConversionData ConvertedData;

void Fetch_Distance();

#endif /* _DRIVER_CONVERSION_H_ */
