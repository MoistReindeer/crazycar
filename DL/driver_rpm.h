#ifndef __DRIVER_RPM_H__
#define __DRIVER_RPM_H__

#define TA0_HZ      (1/0.5)
#define TA0_DIV     (SMCLK_FREQU/4/5)

unsigned int RPM_DISTANCE;

void RPM_Count_Init(void);

#endif /* __DRIVER_RPM_H__ */
