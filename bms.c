#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMPASS "mairala" // admin password
#define BASEACN 100 // starting account number


/* global subject structure */
struct account {

    char name[40];
    int ac;
    char type[10];
    double bal;
};

int ACC_NO; // global var, stores last account number
char LOGIN_TYPE; // global var, stores current login type (user/admin)


/* adds an account to file */
void addAc (void) {

    system("cls");
    puts("\n\n\n\tADD ACCOUNT\n\t===========");
    struct account acc;
    printf("\n\n\t Name: ");
    fflush(stdin);
    gets(acc.name);
    acc.ac = ++ACC_NO;
    printf("\t Account # %d\n", acc.ac);
    printf("\t Account Type: ");
    fflush(stdin);
    gets(acc.type);
    acc.bal = 0.0;
    printf("\t Balance: BDT %.2lf\n", acc.bal);
    FILE *fp;
    fp = fopen("masterfile.txt", "a");
    if (fp==NULL)
    {
        system("cls");
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    fprintf(fp, "%s %d %s %.2lf\n",
        acc.name, acc.ac, acc.type, acc.bal);
    fclose(fp);
    puts("\n\n\tAccount added successfully.");
    getch();
}


/* edits or deletes a filed account */
void editAc (char ch) {

    system("cls");
    FILE *src, *dest;
    src = fopen("masterfile.txt", "r");
    dest = fopen("temp.txt", "w");
    if (src==NULL || dest==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    int ac, flag=0;
    struct account acc;
    switch (ch)
    {
        case 'E':
            puts("\n\n\n\tEDIT ACCOUNT\n\t============");
            printf("\n\n\t Enter the Ac# of the account to be edited: ");
            break;
        case 'D':
            puts("\n\n\n\tDELETE ACCOUNT\n\t==============");
            printf("\n\n\t Enter the Ac# of the account to be deleted: ");
            break;
    }
    scanf("%d", &ac);
    int check_ac = 0; // to eliminate duplicate record reading
    while (!feof(src)) // feof() returns 1 if end of file
    {
        fscanf(src, "%s %d %s %lf",
            acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.ac==ac)
        {
            switch (ch)
            {
                case 'E':
                    printf("\n\n\t Name: ");
                    fflush(stdin);
                    gets(acc.name);
                    printf("\t Account # %d\n", acc.ac);
                    printf("\t Account Type: ");
                    fflush(stdin);
                    gets(acc.type);
                    printf("\t Balance: BDT %.2lf\n", acc.bal);
                    fprintf(dest, "%s %d %s %.2lf\n",
                        acc.name, acc.ac, acc.type, acc.bal);
                    break;
                case 'D':
                    break;
            }
            flag = 1;
        }
        else
            fprintf(dest, "%s %d %s %.2lf\n",
                acc.name, acc.ac, acc.type, acc.bal);
    }
    if (!flag)
        puts("\n\n\tACCOUNT DOES NOT EXIST.");
    else if (ch=='E')
        puts("\n\n\tAccount edited successfully.");
    else if (ch=='D')
        puts("\n\n\tAccount deleted.");
    fclose(src);
    fclose(dest);
    remove("masterfile.txt");
    getch();
    rename("temp.txt", "masterfile.txt");
}


/* view accounts */
void view_account(void) {

    system("cls");
    FILE *fp;
    fp = fopen("masterfile.txt", "r");
    if (fp==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    puts("\n\n\n\tACCOUNT DETAILS\n\t===============");
    struct account acc;
    int check_ac = 0, ac = 0;
    if (LOGIN_TYPE == 'U')
    {
        printf("\n\n\t Enter the account number: ");
        scanf("%d", &ac);
        puts("");
    }
    while(!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf",
            acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (LOGIN_TYPE == 'A')
        {
            printf("\n\n\t Name: %s", acc.name);
            printf("\n\t Account # %d", acc.ac);
            printf("\n\t Account Type: %s", acc.type);
            printf("\n\t Balance: BDT %.2lf", acc.bal);
        }
        else if (LOGIN_TYPE == 'U')
        {
            if (ac == acc.ac)
            {
                printf("\n\n\t Name: %s", acc.name);
                printf("\n\t Account # %d", acc.ac);
                printf("\n\t Account Type: %s", acc.type);
                printf("\n\t Balance: BDT %.2lf", acc.bal);
            }
        }
    }
    getch();
}


/* prints out total bank balance */
void totalAcBal (void) {

    system("cls");
    FILE *fp;
    fp = fopen("masterfile.txt", "r");
    if (fp==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    puts("\n\n\n\tTOTAL BANK BALANCE\n\t==================");
    struct account acc;
    double tot_bal = 0.0;
    int check_ac = 0;
    while (!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf",
            acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        tot_bal += acc.bal;
    }
    printf("\n\n\t The total bank balance stands at BDT %.2lf.\n", tot_bal);
    getch();
    fclose(fp);
}


/* displays the account(s) with the max balance */
void maxAc (void) {

    system("cls");
    FILE *fp;
    fp = fopen("masterfile.txt", "r");
    if (fp==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    puts("\n\n\n\tMAXIMUM BALANCE HOLDER\n\t======================");
    struct account acc;
    double maxbal;
    if (fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal)==4)
        maxbal = acc.bal;
    else
    {
        puts("\n\n\n\tERROR: NO ACCOUNTS EXIST.");
        getch();
        return;
    }
    int check_ac = acc.ac;
    while (!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.bal>=maxbal) maxbal = acc.bal;
    }
    rewind(fp); // for reading the file from the beginning
    check_ac = 0;
    while(!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.bal==maxbal)
        {
            printf("\n\n\t Name: %s", acc.name);
            printf("\n\t Account # %d", acc.ac);
            printf("\n\t Account Type: %s", acc.type);
            printf("\n\t Balance: BDT %.2lf", acc.bal);
        }
    }
    getch();
    fclose(fp);
}


/* displays the account(s) with the min balance */
void minAc (void) {

    system("cls");
    FILE *fp;
    fp = fopen("masterfile.txt", "r");
    if (fp==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    puts("\n\n\n\tMINIMUM BALANCE HOLDER\n\t======================");
    struct account acc;
    double minbal;
    if (fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal)==4)
        minbal = acc.bal;
    else
    {
        puts("\n\n\n\tERROR: NO ACCOUNTS EXIST.");
        getch();
        return;
    }
    int check_ac = acc.ac;
    while (!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.bal<minbal) minbal = acc.bal;
    }
    rewind(fp);
    check_ac = 0;
    while(!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.bal==minbal)
        {
            printf("\n\n\t Name: %s", acc.name);
            printf("\n\t Account # %d", acc.ac);
            printf("\n\t Account Type: %s", acc.type);
            printf("\n\t Balance: BDT %.2lf", acc.bal);
        }
    }
    getch();
    fclose(fp);
}


/* displays the average balance of all accounts */
void avgBal (void) {

    system("cls");
    FILE *fp;
    fp = fopen("masterfile.txt", "r");
    if (fp==NULL)
    {
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    puts("\n\n\n\tAVERAGE BALANCE OF ACCOUNT HOLDERS\n\t==================================");
    struct account acc;
    int count=0, check_ac=0;
    double avg = 0.0;
    while (!feof(fp))
    {
        fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        avg += acc.bal;
        count++;
    }
    avg /= count;
    printf("\n\n\t The average balance of account holders stands at BDT %.2lf.\n", avg);
    getch();
    fclose(fp);
}


/* handles transactions */
void transaction (void) {

    system("cls");
    int ac, ch;
    double amt;
    puts("\n\n\n\tTRANSACTION\n\t===========");
    printf("\n\n\t Enter the account number: ");
    scanf("%d", &ac);
    do
    {
        system("cls");
        puts("\n\n\n\tTRANSACTION\n\t===========");
        puts("\n\n\t 1. Deposit\n\t 2. Withdraw\n\t 3. Cancel");
        printf("\n\t > ");
        scanf("%d", &ch);
    }
    while (ch!=1 && ch!=2 && ch!=3);
    if (ch==3) return;
    printf("\n\n\t Enter amount: BDT ");
    scanf("%lf", &amt);
    FILE *src, *dest;
    src = fopen("masterfile.txt", "r");
    dest = fopen("temp.txt", "w");
    if (src==NULL || dest==NULL)
    {
        system("cls");
        puts("\n\n\n\tERROR PROCESSING FILE.");
        getch();
        return;
    }
    struct account acc;
    double oldbal, newbal;
    int check_ac=0, flag=0;
    while (!feof(src))
    {
        fscanf(src, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
        if (acc.ac==check_ac) continue;
        else check_ac = acc.ac;
        if (acc.ac==ac)
        {
            oldbal = acc.bal;
            switch (ch)
            {   case 1:
                    acc.bal += amt;
                    break;
                case 2:
                    if (acc.bal < amt)
                        acc.bal = 0;
                    else
                        acc.bal = acc.bal - amt;
                    break;
            }
            newbal = acc.bal;
            flag = 1;
        }
        fprintf(dest, "%s %d %s %.2lf\n", acc.name, acc.ac, acc.type, acc.bal);
    }
    system("cls");
    puts("\n\n\n\tTRANSACTION\n\t===========");
    if (!flag)
        puts("\n\n\tTRANSACTION FAILED.");
    else
    {
        printf("\n\n\t Old balance: BDT %.2lf\n\t New balance: BDT %.2lf\n", oldbal, newbal);
        puts("\n\n\tTransaction successful.");
    }
    getch();
    fclose(src);
    fclose(dest);
    remove("masterfile.txt");
    rename("temp.txt", "masterfile.txt");

}


/* money exchange */
void moneyExchange(void) {

    float rate, initamt, finalamt;
    char incurr[4], outcurr[4];

    system("cls");
    puts("\n\n\n\tCURRENCY EXCHANGE\n\t=================");

    printf("\n\n\t Enter the initial currency code: ");
    fflush(stdin);
    gets(incurr);

    if (!stricmp(incurr, "USD"))
        rate = 77.78;
    else if (!stricmp(incurr, "CAD"))
        rate = 59.41;
    else if (!stricmp(incurr, "GBP"))
        rate = 121.96;
    else if (!stricmp(incurr, "BDT"))
        rate = 1.0;
    else if (!stricmp(incurr, "EUR"))
        rate = 85.83;
    else if (!stricmp(incurr, "INR"))
        rate = 1.19;
    else if (!stricmp(incurr, "KWD"))
        rate = 256.97;
    else if (!stricmp(incurr, "SAR"))
        rate = 20.73;
    else if (!stricmp(incurr, "AUD"))
        rate = 57.03;
    else {
        system("cls");
        printf("\n\n\n\tERROR: Currency not available!");
        getch();
        return;
    }

    printf("\n\t Enter the amount: %s ", incurr);
    scanf("%f", &initamt);

    initamt = initamt * rate;

    printf("\n\t Enter the final currency code: ");
    fflush(stdin);
    gets(outcurr);

    if (!stricmp(outcurr, "USD"))
        rate = 0.013;
    else if (!stricmp(outcurr, "CAD"))
        rate = 0.017;
    else if (!stricmp(outcurr, "GBP"))
        rate = 0.0082;
    else if (!stricmp(outcurr, "BDT"))
        rate = 1.0;
    else if (!stricmp(outcurr, "EUR"))
        rate = 0.012;
    else if (!stricmp(outcurr, "INR"))
        rate = 0.84;
    else if (!stricmp(outcurr, "KWD"))
        rate = 0.0039;
    else if (!stricmp(outcurr, "SAR"))
        rate = 0.048;
    else if (!stricmp(outcurr, "AUD"))
        rate = 0.018;
    else {
        system("cls");
        printf("\n\n\n\tERROR: Currency not available!");
        getch();
        return;
    }

    finalamt = initamt * rate;

    printf("\n\n\t Your final amount is %s %.2f.\n", outcurr, finalamt);
    getch();
}


/*  authorise via password, returns 0 if successful, 1 if failed. */
int auth(const char *PASSWORD, const int MAXPASSWORDCHAR) {

    int i = 0;
    char c;
    char input[MAXPASSWORDCHAR+1];

    while (1)
    {
        c = getch();
        if (c == '\b') // to delete asterisks
        {
            if (i < 0)
                continue;
            else if (i == 0)
                i = -1;
            else
            {
                printf("\b \b");
                i -= 2;
            }
        }
        else if (c == '\r') // to handle 'enter'
        {
            input[i] = '\0';
            break;
        }
        else if (i <= MAXPASSWORDCHAR) // to print asterisks
        {
            input[i] = c;
            printf("*");
        }
        if (i <= MAXPASSWORDCHAR) i++;
        else continue;
    }

    return stricmp(input, PASSWORD);
}


/* main menu for users */
void menu_user (void) {

    int op;
    do
    {
        system("cls");
        printf("\n\n\n\t%c%c%c%c%c WELCOME TO THE MAIN MENU %c%c%c%c%c\n\t====================================\n", 176, 176, 176, 176, 176, 176, 176, 176, 176, 176);
        puts("\n\n\t 1. View account details\n\t 2. Make a transaction");
        puts("\t 3. Carry out currency exchange\n\n\t 4. Exit");
        printf("\n\t > ");
        scanf("%d", &op);
        system("cls");
        switch (op)
        {
            case 1: view_account(); break;
            case 2: transaction(); break;
            case 3: moneyExchange(); break;
        }
    }
    while (op>=1 && op<4);
    system("cls");
    puts("\n\n\n\tThank you for using the system.");
    getch();
}


/* main menu for admin */
void menu_admin (void) {

    struct account acc;
    int op;
    do
    {
        FILE *fp;
        fp = fopen("masterfile.txt", "r");
        if (fp!=NULL)
        {
            while(!feof(fp))
                fscanf(fp, "%s %d %s %lf", acc.name, &acc.ac, acc.type, &acc.bal);
            ACC_NO = acc.ac;
        }
        else ACC_NO = BASEACN;
        fclose(fp);
        system("cls");
        printf("\n\n\n\t%c%c%c%c%c WELCOME TO THE MAIN MENU %c%c%c%c%c\n\t====================================\n", 176, 176, 176, 176, 176, 176, 176, 176, 176, 176);
        puts("\n\n\t 1. Add account\n\t 2. Edit account");
        puts("\t 3. Delete account\n\t 4. Make a transaction");
        puts("\t 5. View account details \n\t 6. Show total bank balance");
        puts("\t 7. Show the maximum balance holder\n\t 8. Show the minimum balance holder");
        puts("\t 9. Show the average bank balance");
        puts("\t 10. Carry out currency exchange\n\n\t 11. Exit");
        printf("\n\t > ");
        scanf("%d", &op);
        system("cls");
        switch (op)
        {
            case 1: addAc(); break;
            case 2: editAc('E'); break;
            case 3: editAc('D'); break;
            case 4: transaction(); break;
            case 5: view_account(); break;
            case 6: totalAcBal(); break;
            case 7: maxAc(); break;
            case 8: minAc(); break;
            case 9: avgBal(); break;
            case 10: moneyExchange();
        }
    }
    while (op>=1 && op<11);
    system("cls");
    puts("\n\n\n\tThank you for using the system.");
    getch();
}


/* main function */
int main(void) {

    system("color F1");

    do
    {
        system("cls");
        printf("\n\n\n\t%c%c%c%c%c WELCOME %c%c%c%c%c\n\t===================\n",
               176, 176, 176, 176, 176, 176, 176, 176, 176, 176);
        puts("\n\n\tChoose your login type:");
        printf("\n\t\tA. Administrator\n\t\tB. User\n\n\t> ");
        LOGIN_TYPE = getche();
        LOGIN_TYPE = toupper(LOGIN_TYPE);
    }
    while (LOGIN_TYPE != 'A' && LOGIN_TYPE != 'B');
    if (LOGIN_TYPE == 'B')
        LOGIN_TYPE = 'U';

    switch(LOGIN_TYPE)
    {
        case 'A':
            {
                int try=3;
                while ((try--)>0)
                {
                    system("cls");
                    printf("\n\n\n\t%c%c%c%c%c WELCOME %c%c%c%c%c\n\t===================\n",
                        176, 176, 176, 176, 176, 176, 176, 176, 176, 176);
                    printf("\n\n\t Enter password to continue: ");
                    if (auth(ADMPASS, 20) == 0)
                        break;
                    else
                    {
                        printf("\n\n\t WRONG PASSWORD. TRY AGAIN.");
                        getch();
                    }
                }
                if (try==-1)
                {
                    system("cls");
                    puts("\n\n\n\tACCESS DENIED. SYSTEM SHUTTING DOWN.");
                    getch();
                }
                else
                    menu_admin();
            }
            break;

        case 'U':
            menu_user();
    }

    return 0;
}

// end
