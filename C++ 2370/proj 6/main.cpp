#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <future>

using namespace std;

int grand_total = 0;
ofstream OFS;
mutex mtx;

int count_primes(int start_num) {
    // check every odd number by starting at 3, 5, 7, and 9 and incrementing by 8
    // (and including 2)
    bool is_prime;
    int local_total = 0;
    OFS.open("primes.dat", std::ios_base::app); // open output file in append mode

    if (start_num == 2) { // (2 is prime, include it then start at 3)
            local_total++;
            mtx.lock();
            grand_total++;
            OFS << start_num << endl;
            mtx.unlock();
            start_num++;
    }

    for (int i = start_num; i < 1000000; i += 8) {
        is_prime = true; // true until proven false
        for (int j = 3; j < i/2; j++) {
            // to determine if prime, see if i is divisible by any number from 3 to i/2
            // (it can't be 2 because no even numbers are checked, they're all not prime)
            if (i % j == 0) {
                is_prime = false;
            }
        }

        if (is_prime) {
            local_total++;
            mtx.lock();
            grand_total++;
            OFS << i << endl;
            mtx.unlock();
        }
    }
    OFS.close();
    return local_total;
}

int main() {
    future<int> th1 = async(count_primes, 2);
    future<int> th2 = async(count_primes, 5);
    future<int> th3 = async(count_primes, 7);
    future<int> th4 = async(count_primes, 9);

    int r1 = th1.get();
    int r2 = th2.get();
    int r3 = th3.get();
    int r4 = th4.get();

    cout << "Grand total primes found: " << grand_total << endl
         << "Primes found by first thread: " << r1 << endl
         << "Primes found by second thread: " << r2 << endl
         << "Primes found by third thread: " << r3 << endl
         << "Primes found by fourth thread: " << r4 << endl;
}