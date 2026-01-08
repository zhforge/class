#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int isPrime(int n) {
	// returns 1 if n is prime, 0 if not

	if (n == 1 || n == 0)
		return 0;

	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0)
			return 0;
	}

	return 1;
}

void getPrimes(int n, int *primes) {
	// adds all prime numbers from 2 to sqrt(n) into *primes
	
	int counter = 0;
	for (int i = 2; i <= sqrt(n); i++) {
		if (isPrime(i)) {
			primes[counter] = i;
			counter++;
		}
	}
	primes[counter] = -1;	// terminate with -1

	return;
}

void *getPrimeFactors(void *arg) {
	// update prime_factors at a given index with the prime factors of n
	int n = *(int*)arg;
	int prime_numbers[(int)sqrt(n)+1];	// slightly inefficient, but you can't really calculate # of primes from 1 to n
	getPrimes(n, prime_numbers);
	int *factors = malloc(sizeof(int)*(int)sqrt(n)+1);	// or # of prime factors
	int factors_counter = 0;
	
	for (int i = 0; prime_numbers[i] != -1; i++) {
		int p = prime_numbers[i];
		while (n % p == 0 && n != p) {
			factors[factors_counter] = p;
			factors_counter++;
			n = n / p;
		}
	}

	factors[factors_counter] = n;
	factors[factors_counter+1] = -1; // terminate factors with -1
	
	
	// TESTING
	/*
	for (int i = 0; i < n; i++)
		printf("%d\n", prime_numbers[i]);
	
	factors[0] = 1;
	factors[1] = 2;
	factors[2] = 3;
	*/

	return factors;
}

int main(int argc, char *argv[]) {
	const int num_inputs = argc-1;
	if (num_inputs == 0) {
		printf("No numbers given\n");
		return 1;
	}
	int inputs[num_inputs];
	for (int i = 1; i < argc; i++)
		inputs[i-1] = atoi(argv[i]);
	int *prime_factors[num_inputs];

	pthread_t threads[num_inputs];
	for (int i = 0; i < num_inputs; i++) {
		if (pthread_create(&threads[i], NULL, getPrimeFactors, (void *)&inputs[i]) != 0) {
			perror("pthread_create failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < num_inputs; i++) {
		void *thread_return_value;
		int j = pthread_join(threads[i], &thread_return_value);
		if (j != 0) {
			perror("pthread_join failed");
			exit(EXIT_FAILURE);
		}
		prime_factors[i] = (int*)thread_return_value;
	}

	for (int i = 0; i < num_inputs; i++) {
		printf("%d:", inputs[i]);
		int c = 0;
		while (prime_factors[i][c] != -1) {
			printf(" %d", prime_factors[i][c]);
			c++;
		}
		printf("\n");
	}

	for (int i = 0; i < num_inputs; i++) {
		free(prime_factors[i]);
	}

	
	// TESTING
	/*
	int h = 10;
	int *test = getPrimeFactors(&h);
	printf("%d %d %d %d %d\n", test[0], test[1], test[2], test[3], test[4]);
	*/
	

	return 0;
}
