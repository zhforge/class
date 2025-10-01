#include <string.h>
#include <stdio.h>
#include "account.h"


int account_init(Account *acct_p, int id, int balance, char *owner) {
	if (balance < 0)
		return -1;
	acct_p->id = id;
	acct_p->balance = balance;
	strcpy(acct_p->owner, owner);
	return 0;
}

int account_deposit(Account *acct_p, int amount) {
	if (amount < 0)
		return -1;
	acct_p->balance += amount;
	return 0;
}

int account_withdraw(Account *acct_p, int amount) {
	if (amount < 0 || acct_p->balance < amount)
		return -1;
	acct_p->balance -= amount;
	return 0;
}

int account_transfer(Account *from_p, Account *to_p, int amount) {
	if (amount < 0 || from_p->balance < amount)
		return -1;
	from_p->balance -= amount;
	to_p->balance += amount;
	return 0;
}

int account_print(Account *acct_p) {
	printf("%d %s $%d\n", acct_p->id, acct_p->owner, acct_p->balance);
	return 0;
}
