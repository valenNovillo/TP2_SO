#include "include/time.h"

void getTime(char * time){
    char secS[3], minS[3], hourS[3];
    int sec = getSeconds();
    int min = getMinutes();
    int hour = getHours();

    if (hour <= 3){
        hour = 21 + hour;
    } else{
        hour -= 3;
    }

    intToString(sec, secS);
    intToString(min, minS);
    intToString(hour, hourS);

    if (hour < 10){
        time[0] = '0';
        time[1] = hourS[0];
    } else{
        time[0] = hourS[0];
        time[1] = hourS[1];
    }
    time[2] = ':';
    if (min < 10){
        time[3] = '0';
        time[4] = minS[0];
    } else{
        time[3] = minS[0];
        time[4] = minS[1];
    }
    time[5] = ':';
    if (sec < 10){
        time[6] = '0';
        time[7] = secS[0];
    } else{
        time[6] = secS[0];
        time[7] = secS[1];
    }
    time[8] = 0;
}