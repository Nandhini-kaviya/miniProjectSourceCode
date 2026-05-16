# Bank Account Management System Using C

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData {
    int acctNum;
    char lastName[15];
    char firstName[15];
    float balance;
};

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void searchRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    int choice;
    int password;

    printf("===== BANK ACCOUNT MANAGEMENT SYSTEM =====\n");
    printf("Enter Password: ");
    scanf("%d", &password);

    if(password != 1234)
    {
        printf("Wrong Password! Access Denied.\n");
        return 0;
    }

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            return 1;
        }

        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
        }
    }

    do
    {
        printf("\n===== BANK MENU =====\n");
        printf("1 - Display all accounts\n");
        printf("2 - Search account\n");
        printf("3 - Add new account\n");
        printf("4 - Update account\n");
        printf("5 - Delete account\n");
        printf("6 - Deposit\n");
        printf("7 - Withdraw\n");
        printf("9 - Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                textFile(cfPtr);
                break;

            case 2:
                searchRecord(cfPtr);
                break;

            case 3:
                newRecord(cfPtr);
                break;

            case 4:
                updateRecord(cfPtr);
                break;

            case 5:
                deleteRecord(cfPtr);
                break;

            case 6:
                depositMoney(cfPtr);
                break;

            case 7:
                withdrawMoney(cfPtr);
                break;

            case 9:
                printf("Program terminated successfully\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 9);

    fclose(cfPtr);

    return 0;
}

void textFile(FILE *readPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(readPtr);

    printf("\nAcct LastName FirstName Balance\n");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%d %-10s %-10s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

void searchRecord(FILE *fPtr)
{
    int accountNum;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("%d %s %s %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
    }
    else
    {
        printf("Account not found\n");
    }
}

void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    int accountNum;

    printf("Enter account number (1-100): ");
    scanf("%d", &accountNum);

    if(accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
    }
    else
    {
        client.acctNum = accountNum;

        printf("Enter lastname firstname balance:\n");
        scanf("%s%s%f", client.lastName, client.firstName, &client.balance);

        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account created successfully\n");
    }
}

void updateRecord(FILE *fPtr)
{
    int accountNum;
    float transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Current balance: %.2f\n", client.balance);
        printf("Enter amount (+/-): ");
        scanf("%f", &transaction);

        client.balance += transaction;

        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated balance: %.2f\n", client.balance);
    }
    else
    {
        printf("Account not found\n");
    }
}

void deleteRecord(FILE *fPtr)
{
    int accountNum;

    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
    }
    else
    {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        printf("Account deleted\n");
    }
}

void depositMoney(FILE *fPtr)
{
    int accountNum;
    float amount;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Enter deposit amount: ");
        scanf("%f", &amount);

        client.balance += amount;

        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Deposit successful\n");
    }
    else
    {
        printf("Account not found\n");
    }
}

void withdrawMoney(FILE *fPtr)
{
    int accountNum;
    float amount;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Enter withdraw amount: ");
        scanf("%f", &amount);

        if (amount > client.balance)
        {
            printf("Insufficient balance\n");
        }
        else
        {
            client.balance -= amount;

            fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);

            printf("Withdraw successful\n");
        }
    }
    else
    {
        printf("Account not found\n");
    }
}
```

## Features Included

* Create new account
* Display all accounts
* Search account
* Update balance
* Delete account
* Deposit money
* Withdraw money
* Password protection
* File handling using `credit.dat`
* Random access using `fseek()`
