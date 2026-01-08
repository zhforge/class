#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE (10)

bool isPrime(int n) {
	if (n == 1 || n == 0)
		return false;

	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0)
			return false;
	}

	return true;
}

void getPrimes(int n, int *primes) {
	// adds all prime numbers from 2 to sqrt(n) into primes
	
	int counter = 0;
	for (int i = 2; i <= sqrt(n); i++) {
		if (isPrime(i)) {
			primes[counter] = i;
			counter++;
		}
	}
	primes[counter] = -1; // terminate with sentinel -1
}

void getPrimeFactors(int n, int *factors) {
	// adds all prime factors of n into factors
	
	int prime_numbers[(int)sqrt(n)+1]; // slightly inefficient but convenient approximation of prime number count from 1 to n
	getPrimes(n, prime_numbers);

	int factors_counter = 0;
	for (int i = 0; prime_numbers[i] != -1; i++) {
		int p = prime_numbers[i];
		while (n % p == 0 && n != p) { // check for prime divisibility, potentially repeated
			factors[factors_counter] = p;
			factors_counter++;
			n = n / p;
		}
	}
	factors[factors_counter] = n; // final leftover prime factor
	factors[factors_counter+1] = -1; // sentinel
}


// essentially stream or queue data structures
int in_buffer[BUFFER_SIZE];
int in_buffer_read = 0;
int in_buffer_write = 0;
bool in_buffer_empty = true;
bool in_buffer_full = false;
int out_buffer[BUFFER_SIZE][BUFFER_SIZE+2];
int out_buffer_read = 0;
int out_buffer_write = 0;
bool out_buffer_empty = true;
bool out_buffer_full = false;

bool finished = false;

pthread_mutex_t in_buffer_mtx;
pthread_mutex_t out_buffer_mtx;
pthread_cond_t in_buffer_empty_cv;
pthread_cond_t in_buffer_full_cv;
pthread_cond_t out_buffer_empty_cv;
pthread_cond_t out_buffer_full_cv;


void *Producer(void *arg) {
	while (true) {
		// read input number via in_buffer
		pthread_mutex_lock(&in_buffer_mtx);
		while (in_buffer_empty) {
			if (finished) pthread_exit(NULL);
			pthread_cond_wait(&in_buffer_empty_cv, &in_buffer_mtx);
		}
		
		int n = in_buffer[in_buffer_read];
		in_buffer_read = (in_buffer_read+1)%10;
		in_buffer_full = false;
		if (in_buffer_write == in_buffer_read)
			in_buffer_empty = true;
		
		pthread_mutex_unlock(&in_buffer_mtx);
		pthread_cond_signal(&in_buffer_full_cv);

		// get the number's prime factors
		int factors[BUFFER_SIZE+2]; // up to BUFFER_SIZE (10) factors, plus room for the sentinel, and the number itself at the end
		getPrimeFactors(n, factors);
		factors[BUFFER_SIZE+1] = n;

		// send the factors to consumber via out_buffer
		pthread_mutex_lock(&out_buffer_mtx);
		while (out_buffer_full) {
			pthread_cond_wait(&out_buffer_full_cv, &out_buffer_mtx);
		}
		
		memcpy(out_buffer[out_buffer_write], factors, sizeof factors);
		out_buffer_empty = false;
		out_buffer_write = (out_buffer_write+1)%10;
		if (out_buffer_write == out_buffer_read)
			out_buffer_full = true;
		
		pthread_mutex_unlock(&out_buffer_mtx);
		pthread_cond_signal(&out_buffer_empty_cv);
	}
}

void *Consumer(void *arg) {
	while (true) {
		// read number & factors via out_buffer
		pthread_mutex_lock(&out_buffer_mtx);
		while (out_buffer_empty) {
			if (finished) pthread_exit(NULL);
			pthread_cond_wait(&out_buffer_empty_cv, &out_buffer_mtx);
		}
		
		int factors[BUFFER_SIZE+2];
		memcpy(factors, out_buffer[out_buffer_read], sizeof factors);
		out_buffer_read = (out_buffer_read+1)%10;
		out_buffer_full = false;
		if (out_buffer_write == out_buffer_read)
			out_buffer_empty = true;
		
		pthread_mutex_unlock(&out_buffer_mtx);
		pthread_cond_signal(&out_buffer_full_cv);

		// output number & factors to the console
		printf("%d:", factors[BUFFER_SIZE+1]);
		for (int i = 0; factors[i] != -1; i++)
			printf(" %d", factors[i]);
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("No numbers given\n");
		return 1;
	}

	pthread_mutex_init(&in_buffer_mtx, NULL);
	pthread_mutex_init(&out_buffer_mtx, NULL);
	pthread_cond_init(&in_buffer_empty_cv, NULL);
	pthread_cond_init(&in_buffer_full_cv, NULL);
	pthread_cond_init(&out_buffer_empty_cv, NULL);
	pthread_cond_init(&out_buffer_full_cv, NULL);
	
	pthread_t prod;
	pthread_t cons;
	pthread_create(&prod, NULL, Producer, NULL);
	pthread_create(&cons, NULL, Consumer, NULL);


	// send all command-line args to the producer via in_buffer
	for (int i = 1; i < argc; i++) {
		int n = atoi(argv[i]);

		pthread_mutex_lock(&in_buffer_mtx);
		while (in_buffer_full) {
			pthread_cond_wait(&in_buffer_full_cv, &in_buffer_mtx);
		}
		
		in_buffer[in_buffer_write] = n;
		in_buffer_empty = false;
		in_buffer_write = (in_buffer_write+1)%10;
		if (in_buffer_write == in_buffer_read)
			in_buffer_full = true;
		
		pthread_mutex_unlock(&in_buffer_mtx);
		pthread_cond_signal(&in_buffer_empty_cv);
	}


	// wait for the buffers to be empty
	pthread_mutex_lock(&in_buffer_mtx);
	while (!in_buffer_empty) {
		pthread_cond_wait(&in_buffer_full_cv, &in_buffer_mtx);
	}
	pthread_mutex_unlock(&in_buffer_mtx);

	pthread_mutex_lock(&out_buffer_mtx);
	while (!out_buffer_empty) {
		pthread_cond_wait(&out_buffer_full_cv, &out_buffer_mtx);
	}
	pthread_mutex_unlock(&out_buffer_mtx);

	// signal threads to stop, terminate execution
	finished = true;
	pthread_cond_signal(&in_buffer_empty_cv);
	pthread_cond_signal(&out_buffer_empty_cv);
	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	return 0;
}
