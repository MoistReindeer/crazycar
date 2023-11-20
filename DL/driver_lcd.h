#ifndef _DRIVER_LCD_H_
#define _DRIVER_LCD_H_

// Einstellungen für die LCD Initialisierung
#define LCD_RESET_FG 0xE2
#define LCD_BIAS 0xA3
#define ADC_SEL_NORMAL 0xA0
#define COMMON_REVERSE 0xC8
#define RES_RATIO 0x24
#define ELEC_VOL_MODE 0x81
#define ELEC_VOL_VALUE 0x0F // Kontrast Einstellung
#define POWER_CONT 0x2F
#define DISPLAY_ON 0xAF

#define LCD_DATA (P8OUT=(P8OUT | LCD_DATACMD))
#define LCD_COMMAND (P8OUT=(P8OUT & ~LCD_DATACMD))
#define LCD_MAX_COLM 128

#define SET_PAGE 0xB0
#define LAST_PAGE 0xB7

#define MSB_COL_ADDR 0x10
#define LSB_COL_ADDR 0x00

#define ALL_POINTS_ON 0xA5
#define ALL_POINTS_OFF 0xA4

void Driver_LCD_Init(void);
void Driver_LCD_SetPosition(unsigned char page, unsigned char col);
void Driver_LCD_Clear(void);
void Driver_LCD_WriteCommand(unsigned char *data, unsigned char len);
void Driver_LCD_WriteText(unsigned char *text, unsigned char len, unsigned char page, unsigned char col);
void Driver_LCD_WriteUInt(unsigned int num, unsigned char page, unsigned char col);
void Driver_LCD_Test();

#endif /*_DRIVER_LCD_H_*/
