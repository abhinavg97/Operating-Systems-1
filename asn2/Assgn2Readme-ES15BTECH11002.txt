Executing the Program 

primes-th :

gcc Assgn2-Primes-th-ES15BTECH11002.c -pthread

primes-proc :

gcc Assgn2-Primes-proc-ES15BTECH11002.c -lrt

Running the program:

time ./a.out n k

n is the number below which primes need to be evaluated
k is the number of processes / threads

time command gives us user,system,and real time for the execution of the program