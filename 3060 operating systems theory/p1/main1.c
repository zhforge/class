#include <stdio.h>
#include "account.h"

int main(int argc, char *argv[]) {
  Account a1, a2;
  account_init(&a1, 1, 1000, "Hermione Granger");
  account_init(&a2, 2, 500, "Jack Swallow");
  account_deposit(&a1, 500);
  account_withdraw(&a2, 50);
  account_withdraw(&a1, 100);
  account_transfer(&a2, &a1, 25);
  account_print(&a1);
  account_print(&a2);
}
