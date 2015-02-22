#include <pebble.h>
#include "alarm_wakeup.h"
  
void alarm_schedule_wakeup(Alarm *alarm) {
    bool some_active=false;
    alarm_cancel_wakeup(alarm);
    static int weekday_names[7];
    
    time_t timestamp = clock_to_timestamp(TODAY,alarm->hour,alarm->minute);
    
    int hour_out = alarm->hour;
    bool is_am = false;
    if (!clock_is_24h_style()) {
        convert_24_to_12(alarm->hour, &hour_out, &is_am);
        
        // draw active weekdays
        char weekday_state[10];
        snprintf(weekday_state,sizeof(weekday_state),"%c%c%c%c%c\n%c%c",
                 alarm->weekdays_active[1]?weekday_names[1]:'_',
                 alarm->weekdays_active[2]?weekday_names[2]:'_',
                 alarm->weekdays_active[3]?weekday_names[3]:'_',
                 alarm->weekdays_active[4]?weekday_names[4]:'_',
                 alarm->weekdays_active[5]?weekday_names[5]:'_',
                 alarm->weekdays_active[6]?weekday_names[6]:'_',
                 alarm->weekdays_active[0]?weekday_names[0]:'_');
        
    }
    
    if(alarm->enabled) {
        // Calculate time to wake up
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        
        
        // Check if we may schedule the alarm today
        int current_weekday = t->tm_wday;
        for(int i=0;i<7;i++)
        {
            if(alarm->weekdays_active[(i+current_weekday)%7])
            {
                if(i==0)
                    timestamp = clock_to_timestamp(TODAY,alarm->hour,alarm->minute);
                else
                    timestamp = clock_to_timestamp(((i+current_weekday)%7)+1,alarm->hour,alarm->minute);
                if((now-timestamp)>(60*60*24*7)) {
                    // we can not be more than a week away?
                    continue;
                }
                else {
                    some_active=true;
                    break;
                }
            }
        }
        if(some_active) {
            t = localtime(&timestamp);
            APP_LOG(APP_LOG_LEVEL_DEBUG,"Scheduled at %d.%d %d:%d",t->tm_mday, t->tm_mon+1,t->tm_hour,t->tm_min);
            alarm->alarm_id = wakeup_schedule(timestamp,0,true);
        }
        else //maybe we have missed a today event. this takes then place next week {
            if(alarm->weekdays_active[current_weekday]) {
                some_active=true;
                timestamp = clock_to_timestamp(TODAY,alarm->hour,alarm->minute);
                t = localtime(&timestamp);
                APP_LOG(APP_LOG_LEVEL_DEBUG,"Scheduled at %d.%d %d:%d",t->tm_mday, t->tm_mon+1,t->tm_hour,t->tm_min);
                alarm->alarm_id = wakeup_schedule(timestamp,0,true);
            }
            else // one-time alert (no weekdays active) {
                timestamp = clock_to_timestamp(TODAY,alarm->hour,alarm->minute);
        if((timestamp-now)>(60*60*24)) {
            timestamp = clock_to_timestamp(current_weekday+1,alarm->hour,alarm->minute);
            
            t = localtime(&timestamp);
            APP_LOG(APP_LOG_LEVEL_DEBUG,"Scheduled one-time event at %d.%d %d:%d",t->tm_mday, t->tm_mon+1,t->tm_hour,t->tm_min);
            alarm->alarm_id = wakeup_schedule(timestamp,0,true);
        }
    }
    if(some_active && alarm->alarm_id<0) {
        switch (alarm->alarm_id) {
            case E_RANGE:
                alarm->alarm_id = wakeup_schedule(timestamp-120,0,true);
                APP_LOG(APP_LOG_LEVEL_DEBUG,"Had to reschedule timer...");
                break;
            case E_OUT_OF_RESOURCES:
                // do some magic here...
                break;
            case E_INVALID_ARGUMENT:
                // should not happen
                break;
            default:
                break;
        }
    }
}

void alarm_cancel_wakeup(Alarm *alarm) {
    if (alarm->alarm_id <= 0) {
        return;
    }
    wakeup_cancel(alarm->alarm_id);
    alarm->alarm_id = -1;
}

void convert_24_to_12(int hour_in, int* hour_out, bool* am) {
    if(hour_in==0) {
        *hour_out=12;
        *am=true;
    }
    else if (hour_in==12) {
        *hour_out=12;
        *am=false;
    }
    else if (hour_in<12) {
        *hour_out = hour_in;
        *am=true;
    }
    else {
        *hour_out = hour_in-12;
        *am=false;
    }
}

bool alarm_has_description(Alarm *alarm) {
    return alarm->description[0];
}