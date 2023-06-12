char *dtell(int x, int y, int z) {                      //returns week day name corresponding to the date entered.
    struct tm timeinfo = {0};
    time_t time;
    timeinfo.tm_year=z;   
    timeinfo.tm_mon=y;      
    timeinfo.tm_mday=x;    
    time = mktime(&timeinfo);
    static char dayName[10];
    strftime(dayName, sizeof(dayName), "%A", &timeinfo);
    return dayName;
}

char *getDate(char* givenDayName) {                    /*takes a given day name as input and returns 
                                                        a date string in the format "dd/mm/yyyy" for 
                                                        that day within the current week.*/
    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);
    int currentWeekday = localTime->tm_wday;
    int givenWeekday = -1;
    if (strcasecmp(givenDayName, "Sunday") == 0) {
        givenWeekday = 0;
    } else if (strcasecmp(givenDayName, "Monday") == 0) {
        givenWeekday = 1;
    } else if (strcasecmp(givenDayName, "Tuesday") == 0) {
        givenWeekday = 2;
    } else if (strcasecmp(givenDayName, "Wednesday") == 0) {
        givenWeekday = 3;
    } else if (strcasecmp(givenDayName, "Thursday") == 0) {
        givenWeekday = 4;
    } else if (strcasecmp(givenDayName, "Friday") == 0) {
        givenWeekday = 5;
    } else if (strcasecmp(givenDayName, "Saturday") == 0) {
        givenWeekday = 6;
    }
    if (givenWeekday != -1) {
        int dayDifference = givenWeekday - currentWeekday;
        if (dayDifference < 0) {
            dayDifference += 7; 
        }
        localTime->tm_mday += dayDifference;
        mktime(localTime);
        static char dateString[11];
        strftime(dateString, sizeof(dateString), "%d/%m/%Y", localTime);
        return dateString;
    } else {
        return NULL;
    }
}

bool isWithinSevenDays(int userDay, int userMonth, int userYear) {
  time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    localTime->tm_hour = 0;
    localTime->tm_min = 0;
    localTime->tm_sec = 0;
    localTime->tm_isdst = -1; 
    struct tm userTime = {0};
    userTime.tm_year = userYear - 1900;
    userTime.tm_mon = userMonth - 1;
    userTime.tm_mday = userDay;
    userTime.tm_hour = 0;
    userTime.tm_min = 0;
    userTime.tm_sec = 0;
    userTime.tm_isdst = -1;  
    time_t currentUserTime = mktime(&userTime);
    time_t currentDateTime = mktime(localTime);
    double secondsInDay = 60 * 60 * 24;
    double difference = difftime(currentUserTime, currentDateTime);
    return (difference >= 0 && difference <= 7 * secondsInDay);
}

char* formatDate(int day, int month, int year) {
    char* datestr = malloc(sizeof(char) * 11);  
    sprintf(datestr, "%02d/%02d/%04d", day, month, year);
    return datestr;
}