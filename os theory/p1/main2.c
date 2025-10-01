#include <string.h>
#include <stdio.h>
#include "account.h"

#define MAX_ACCOUNTS (5)
#define MAX_COMMAND_SIZE (100)

int main(int argc, char *argv[]) {
	char buffer[MAX_COMMAND_SIZE];	// input line buffer
	int running = 1;
	Account account_db[MAX_ACCOUNTS];
	int curr_acct_amt = 0;
	char temp_str1[MAX_COMMAND_SIZE];	// for parsing command arguments
	char temp_str2[MAX_COMMAND_SIZE];
	int temp_int1;
	int temp_int2;
	int temp_int3;
	int temp_int4;
	Account new_acct;
	int i;
	

	while (running) {
		fgets(buffer, MAX_COMMAND_SIZE, stdin);

		switch (buffer[0]) {

		case 'n':	// new
			if (curr_acct_amt == MAX_ACCOUNTS) {	// prevent overflow
				printf("Maximum number of accounts. No more can be created.\n\n");
				break;
			}

			sscanf(buffer, "new %d %s %s", &temp_int1, temp_str1, temp_str2);	// separate args
			if (temp_int1 < 0) {	// check for invalid balance
				printf("Invalid starting balance.\n\n");
				break;
			}
			strcat(temp_str1, " ");
			strcat(temp_str1, temp_str2);	// assemble full name

			account_init(&new_acct, curr_acct_amt+1, temp_int1, temp_str1);
			account_db[curr_acct_amt] = new_acct;
			curr_acct_amt++;

			printf("New account: %d %s $%d\n\n", curr_acct_amt, temp_str1, temp_int1);
			
			break;

		case 'd':	// deposit
			sscanf(buffer, "deposit %d %d", &temp_int1, &temp_int2);
			if (temp_int1 > curr_acct_amt) {	// invalid account id
				printf("Invalid account number: %d\n\n", temp_int1);
				break;
			}

			temp_int3 = account_deposit(&account_db[temp_int1-1], temp_int2);
			if (temp_int3 < 0) {	// invalid deposit
				account_print(&account_db[temp_int1-1]);
				printf("Invalid deposit amount: $%d\n\n", temp_int2);
				break;
			}
			else {	// valid deposit
				printf("Deposit Amount: $%d Owner: %s New Balance: $%d\n\n", temp_int2, account_db[temp_int1-1].owner, account_db[temp_int1-1].balance);
			}

			break;
		case 'w':	// withdraw
			sscanf(buffer, "withdraw %d %d", &temp_int1, &temp_int2);
			if (temp_int1 > curr_acct_amt) {	// invalid account id
				printf("Invalid account number: %d\n\n", temp_int1);
				break;
			}

			temp_int3 = account_withdraw(&account_db[temp_int1-1], temp_int2);
			if (temp_int3 < 0) {	// invalid withdrawal
				account_print(&account_db[temp_int1-1]);
				printf("Invalid withdrawal amount: $%d\n\n", temp_int2);
				break;
			}
			else {	// valid withdrawal
				printf("Withrdawal Amount: $%d Owner: %s New Balance: $%d\n\n", temp_int2, account_db[temp_int1-1].owner, account_db[temp_int1-1].balance);
			}

			break;
		case 't':	// transfer
			sscanf(buffer, "transfer %d %d %d", &temp_int1, &temp_int2, &temp_int3);
			if (temp_int1 > curr_acct_amt) {	//invalid account id
				printf("Invalid account number: %d\n\n", temp_int1);
				break;
			}
			if (temp_int2 > curr_acct_amt) {	//invalid account id
				printf("Invalid account number: %d\n\n", temp_int2);
				break;
			}

			temp_int4 = account_transfer(&account_db[temp_int1-1], &account_db[temp_int2-1], temp_int3);
			if (temp_int4 < 0) {	// invalid transfer
				printf("Invalid transfer amount: $%d\n\n", temp_int3);
				break;
			}
			else {	// valid transfer
				printf("Transfer Amount: $%d\n", temp_int3);
				printf("From: %s New Balance: $%d\n", account_db[temp_int1-1].owner, account_db[temp_int1-1].balance);
				printf("To: %s New Balance: $%d\n\n", account_db[temp_int2-1].owner, account_db[temp_int2-1].balance);
			}

			break;
		case 'p':	// print
			printf("ACCOUNTS\n");
			for (i = 0; i < curr_acct_amt; i++) {
				account_print(&account_db[i]);
			}
			printf("\n");

			break;
		case 'q':	// quit
			printf("Goodbye\n\n");

			running = 0;
			break;

	}
	}
	return 0;
}
