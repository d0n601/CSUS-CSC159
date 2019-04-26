# CSUS CSC159
## Phase 8 Signal IPC II: Alpha Derby!


##### Coding Hinds
1. **(DONE)** Enter 'race' at a terminal prompt and 5 child processes will race
   across the PC screen, and results will be shown by the parent process.

2. **(DONE)** proc.c, check how demo runs:

   Aout is mostly the same as before except
   a. make a call PauseCall() before the loop to show its letter
   b. the sleep period in the loop is random, ranging from 5 to 24,
      use the return of RandCall() to achieve this

   ````
   UserProc
      once 'race' is entered (no longer looking for 'fork')
      loop 5 times {
           same code as before...
           (Fork, child calls ExecCall, parent shows child PID)
      }

      sleep for 3 seconds (so 5 children can get to be ready)
      KillCall(0, SIGGO) (send signal SIGGO to all 5 children)

      loop 5 times {
         exit_code = WaitCall(); // as before
         shows exit code as before (Itoa converts it)
         SHOW: CHILD ALPHABET (convert from exit_code)... ARRIVES!
      }
   ````
   
3. **(DONE)** OS needs an: unsigned rand = 0; (main.c/k-data.h)
   It will be seeded to sys_centi_sec when a 'n' key is caught _and_
   it is still 0.

4. **(DONE)** A new state and some new contants:
    ````
   PAUSE is a new process state
   PAUSE_CALL 58      // to pause a process
   KILL_CALL 59       // to kill is actually to send a signal
   RAND_CALL 60       // to get a random number
   SIGGO 4            // a new signal
   A_PRIME 6700417
   G2 2147483647      // 2G-1, % operator can't take a full 2G

5. **(DONE)** syscall.c/.h needs code of 3 new calls:
    ````
   void PauseCall(void)
   void KillCall(int pid, int sig_num)
   unsigned RandCall(void)

6. **(DONE)** k-entry.S/.h needs code of 3 new entries:
    ````
      PauseEntry
      KillEntry
      RandEntry

7. **(DONE)** In main.c, the 3 new entry addresses are added into intr_table,
   and Kernel() has 3 new cases.

8. **(DONE)** in k-sr.c/.h:
  
   ````
   void PauseSR(void)
      a. alter the state of the running process
      b. the running process is now NONE

   void KillSR(int pid, int sig_num)
      if pid is 0 {
         loop thru PCB array {
            look for ppid is the running process and state is PAUSE
            if found: a. update its state to ? b. enqueue it to ?

   unsigned RandSR(void)
      rand = run_pid * rand + A_PRIME
      rand %= G2
      return rand
    ````
