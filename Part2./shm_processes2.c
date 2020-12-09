#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  BankProcess(int []);

int  main(int  argc, char *argv[]) {
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
     int i;
     int BankAccount;
     int Turn;
     int Deposit;

/*     if (argc != 5) {
          printf("Use: %s #1 #2 #3 #4\n", argv[0]);
          exit(1);
     }
*/
     ShmID = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");
   
     BankAccount = 0;
     Turn = 0;
     Deposit = 0;
  
     ShmPTR[0] = BankAccount;
     ShmPTR[1] = Turn;
  
     printf("Server is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          BankProcess(ShmPTR);
          wait(&status);
          exit(0);
     }
     else {
          for(i = 0; i < 25; i++) {
            // Sleep some random amount of time between 0 - 5 seconds
            sleep(rand() % 5); 
            // After waking up, copy the value in BankAccount to a local variable account
            if (ShmPTR[1] == 0) {
              if(ShmPTR[0] <= 100) {
                Deposit = rand() % 100;
                if (Deposit % 2 == 0) {
                  ShmPTR[0] = ShmPTR[0] + Deposit;
                  printf("Dear old Dad: Deposits $%d / Balance = $%d\n", Deposit, ShmPTR[0]);
                }
                else {
                  printf("Dear old Dad: Doesn't have any money to give\n");
                }
                ShmPTR[1] = ShmPTR[1];
              }           
            }
         }
     }

     wait(&status);
     exit(0);
}

void  BankProcess(int  SharedMem[]) { 
              int balance;
              int i;
              for(i = 0; i < 25; i++) { 
                  sleep(rand() % 5);
                  if(SharedMem[1] == 1) {
                    balance = rand() % 50;
                    printf("Poor Student needs $%d\n", balance);
                    if (balance <= SharedMem[0]) {
                      SharedMem[0] = SharedMem[0] - balance;
                      printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, SharedMem[0]);
                    }
                    else {
                      printf("Poor Student: Not Enough Cash ($%d)\n", SharedMem[0]);
                    }
                  SharedMem[1] = 0;
                }
            }
  exit(0);
}