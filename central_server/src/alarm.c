#include "alarm.h"

int presence_alarm = 0;
int smoke_alarm = 0;

int get_presece_alarm() {
    return presence_alarm;
}

int get_smoke_alarm() {
    return smoke_alarm;
}

int invert_presece_alarm() {
    presence_alarm = !presence_alarm;
    return presence_alarm;
}

int invert_smoke_alarm() {
    smoke_alarm = !smoke_alarm;
    return smoke_alarm;
}