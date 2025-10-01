#include <string.h>

#define MAX_NAME_LEN (100)

typedef struct {
  int id;
  int balance;
  char owner[MAX_NAME_LEN]; // USED FOR A STRING
} Account;

int account_init(Account *acct_p, int id, int balance, char *owner);
int account_deposit(Account *acct_p, int amount);
int account_withdraw(Account *acct_p, int amount);
int account_transfer(Account *from_p, Account *to_p, int amount);
int account_print(Account *acct_p);
