#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
void login();
void reg();
char* valid_user(char *, char *); 
int user_avail(char *);
void driver_menu(char *, char *);
void book(char *);
char *dtell(int, int, int);
char *printDetails(char *, char *);
void welcome();
char *getDate(char *);
bool isInCurrentWeek(int,int,int);
void logout();
void upcoming(char *);
char* formatDate(int, int, int);
bool isWithinSevenDays(int, int, int);
#include "user_actions.h"
#include "date&timefunc.h"

typedef struct
{
    char username[15];
    char password[16];
    char f_name[20];
    char mob_no[13];
    char email[20];
}user_data;

char *name, *activ_usrname;

void main()
{
    char choice;

    /*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD originalAttributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);*/
    SetConsoleTitle("JetLink Airlines");

    system("cls");

    puts("Welcome to JetLink Airlines!");
    start:
    printf("Enter your choice (A for Login or B for Registration or C to Exit): ");
    choice = getchar();
    if (toupper(choice) == 'A') login();
    else if (toupper(choice) == 'B') reg();
    else if (toupper(choice) == 'C') exit(0);
    else
    {
        system("cls");
        puts("Wrong Choice!");
        goto start;
    }
}

void login()
{
    int rem_attempts = 3;
    char user[15], pass[16];
    system("cls");
    while (rem_attempts > 0)
    {
        fflush(stdin);
        puts("Login");
        printf("Enter your username: ");
        scanf("%s", user);
        printf("Enter password: ");
        scanf("%s", pass);
        if ((name = valid_user(user, pass)) != NULL)
        {
            printf("Login successful\n");
            printf("Press any key to continue...");
            getch();
            system("cls");
            driver_menu(activ_usrname, name);
            return;
        }
        else
        {
            system("cls");
            puts("Invalid credentials.");
            rem_attempts--;
            if (rem_attempts > 0)
            {
                printf("Attempts remaining: %d\n", rem_attempts);
                system("pause");
                system("cls");
            }
        }
    }
    puts("Too many failed login attempts. Try again later.");
    puts("Press any key to exit.");
    getch();
}


void reg()
{
    FILE *r, *l;
    user_data user1, users;
    int a;
    system("cls");
    r = fopen("user.bin", "ab+");
    l = fopen("user.bin", "rb");
    fflush(stdin);
    printf("Enter your full name: "); 
    scanf("%[^\n]", user1.f_name);
    fflush(stdin);
    printf("Enter your mobile number: ");
    scanf("%[^\n]", user1.mob_no);
    fflush(stdin);
    printf("Enter your email id: ");
    scanf("%s", user1.email);
    do
    {
        printf("Enter your username: ");
        scanf("%s", user1.username);
        if(user_avail(user1.username))
            break;
        printf("Username not available.\n");
    } while (1);
    printf("Enter your password: ");
    scanf("%s", user1.password);
    printf("User registered successfully.");
    fwrite(&user1, sizeof(user_data), 1, r);
    fclose(r);
}

char *valid_user(char *user, char *pass)
{
    FILE *fptr = fopen("user.bin", "rb");
    int n;
    static user_data logusr;
    while (fread(&logusr, sizeof(user_data), 1, fptr) != 0)
    {
        if (strcmp(logusr.username, user) == 0)
            if (strcmp(logusr.password, pass) == 0)
            {
                fclose(fptr);
                activ_usrname=logusr.username;
                return logusr.f_name;
            }
    }
    fclose(fptr);
    return NULL;
}

int user_avail(char *user)
{
    FILE *f = fopen("user.bin", "rb");
    user_data user1;
    while (fread(&user1, sizeof(user1), 1, f) != 0)
        if (strcmp(user1.username, user) == 0)
            return 0;
    return 1;
}
