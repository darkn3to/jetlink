typedef struct 
{
    char flightname[6], airline[10], origin[10], destination[15], departure[7], arrival[7], ttime[10], day[10];
}flightdetails;

typedef struct {
    char flightname[6], username[15], date[11];
}bkdflight;

typedef struct {
    char flightname[6], username[15], date[11];
}upc;

int dd, mm, yyyy, flag=0;
char dateString[11], userfullname[20], flightchoice[6];

void welcome() {
    printf("Welcome %s\t\t\t\t", userfullname);
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime); 
    strftime(dateString, sizeof(dateString), "%d/%m/%Y", localTime);
    printf("%s\n", dateString);
    printf("==============================================================\n");
}

void driver_menu(char *usrname, char *name) {                 //drives the whole thing.
    int act;
    strcpy(userfullname, name);
    printf("\n");
    while(1) {
        system("cls");
        welcome();
        printf("Choose your action:\n");
        printf("1. Book a Flight\n");
        printf("2. Upcoming Flights\n");
        printf("3. Logout\n");
        invalidaction:
        scanf("%d", &act);
    if (act==1) {
        printf("We operate in the following cities: \n");
        printf("1. Mumbai(BOM), Maharashtra\n");
        printf("2. New Delhi(DEL), Delhi\n");
        printf("3. Goa(GOI)\n");
        printf("4. Agra(AGR), Uttar Pradesh\n");
        printf("5. Udaipur(UDR), Rajasthan\n");
        printf("6. Indore\n");
        book(usrname);
    }
    else if (act==2) upcoming(usrname);
    else if (act==3) logout();
    else { printf("Select a valid action."); goto invalidaction; }
    }
}

void book(char *usrname) {                                //books the flight
    bkdflight bofl;
    char citychoice[15], *date, *dayweek;
    printf("Enter the city name for searching flights: ");
    scanf("%s", citychoice);
    retake:
    printf("Enter the departure date[dd mm yyyy]. Please note that advance bookings can only be made for a maximum of 7 days from the current date. \n");
    scanf("%d %d %d", &dd, &mm, &yyyy);

    if (isWithinSevenDays(dd, mm, yyyy)==false) {
        printf("Invalid date. Please enter a date within the current week.\n");
        goto retake;
    }
    system("cls");
    char *day=dtell(dd, mm-1, yyyy-1900);
    dayweek=printDetails(day, citychoice);
    /*if (dayweek!=NULL) date=getDate(dayweek);
    else strcpy(date, formatDate(dd, mm, yyyy));*/
    if (flag==1) date=getDate(dayweek);
    else date=formatDate(dd, mm, yyyy);
    strcpy(bofl.flightname, flightchoice);
    strcpy(bofl.username, usrname);
    strcpy(bofl.date, date);
    FILE *bof=fopen("bookedflight.bin", "ab+");
    fwrite(&bofl, sizeof(bkdflight), 1, bof);
    printf("Flight successfully booked for %s.\n", date);
    system("pause");
    fclose(bof);
}

char *printDetails(char *day, char *citychoice) {                   //prints the flight details when prompted
    static flightdetails fd;
    int results=0;
    char yesno;
    FILE *gd=fopen("flightdata.bin", "rb");
    welcome();
    printf("\n");
    printf("Showing flight results from Dehradun to %s on %d/%d/%d.\n", citychoice, dd, mm, yyyy);
    printf("Flight Name\tAirline\t\tOrigin\t\tDestination\tDeparture\tArrival\t\tTravel Time\n");
    while (fread(&fd, sizeof(flightdetails), 1, gd) != 0) {
        if (strcmpi(fd.day, day)==0 && strcmpi(fd.destination, citychoice)==0) {
            printf("%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\t\t%s\n", fd.flightname, fd.airline, fd.origin, fd.destination, fd.departure, fd.arrival, fd.ttime);
            results++;
        }
    }

    rewind(gd);
    if (results==0) {
        system("cls");
        welcome();
        printf("No flights are available from Dehradun to %s on %d/%d/%d.\n", citychoice, dd, mm, yyyy);
        getchar();
        printf("Would you like to search for flights to the same destination within the next 7 days? (Y/N): ");
        scanf("%c", &yesno);
        yesno=toupper(yesno);
        if (yesno=='Y') {
            printf("Showing flight results from Dehradun to %s within the same week.\n", citychoice);
            printf("Flight Name\tAirline\t\tOrigin\t\tDestination\tDeparture\tArrival\t\tTravel Time\t\tDay\n");
            while (fread(&fd, sizeof(flightdetails), 1, gd) != 0) {
                if (strcmpi(fd.destination, citychoice)==0) {
                    printf("%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", fd.flightname, fd.airline, fd.origin, fd.destination, fd.departure, fd.arrival, fd.ttime, fd.day);
                }
            }
            rewind(gd);
            printf("Please select a flight from the available options[flight name]: ");
            scanf("%s", flightchoice);
            while (fread(&fd, sizeof(flightdetails), 1, gd) != 0) {
            if (strcmpi(fd.flightname, flightchoice)==0) {flag=1; return fd.day;}
            }           
        }
        else if (yesno=='N') {
            system("cls");
            exit(0);
        } 
    }
    printf("Please select a flight from the available options[flight name]: ");
    scanf("%s", flightchoice);
    while (fread(&fd, sizeof(flightdetails), 1, gd) != 0) {
            if (strcmpi(fd.flightname, flightchoice)==0) return fd.day;
    }  
    fclose(gd);
    return NULL;
}

void logout() {
    printf("You have been logged out.\n");
    system("pause");
    exit(0);
}

void upcoming(char *usrname) {
    bkdflight upcoming;
    flightdetails gupd;
    int flights=0;
    char flightdate[11], flightname[6];
    FILE *up, *getupcomingdetails;
    up=fopen("bookedflight.bin", "rb");
    getupcomingdetails=fopen("flightdata.bin", "rb");
    printf("Flight Name\tAirline\t\tOrigin\t\tDestination\tDeparture\tArrival\t\tTravel Time\t\tDate\n");
    while (fread(&upcoming, sizeof(bkdflight), 1, up) != 0) {
            if (strcmpi(upcoming.username, usrname)==0)  {
                strcpy(flightdate, upcoming.date);
                strcpy(flightname, upcoming.flightname);
                rewind(getupcomingdetails);
                while (fread(&gupd, sizeof(flightdetails), 1, getupcomingdetails) != 0) {
                    if (strcmpi(gupd.flightname, flightname)==0) {
                        printf("%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", gupd.flightname, gupd.airline, gupd.origin, gupd.destination, gupd.departure, gupd.arrival, gupd.ttime, flightdate);
                    }
                }
                flights++;
            }
    }  
    if (flights==0) {
        system("cls");
        welcome();
        printf("No upcoming flights.\n");
    }
    system("pause");
    fclose(up);
    fclose(getupcomingdetails);
}
