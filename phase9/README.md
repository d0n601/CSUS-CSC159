# CSUS CSC159
## Phase 9 Wide Open Virtual Space
##### Coding Hinds

k-type.h
   PCB has a new 'int main_table' that must be filled out in
   NewProcSR, ForkSR, and ExecSR.
   

**(done)** main.c: 
   Declare 'int kernel_main_table' (prototype in k-data.h).
   Initialize kernel_main_table = get_cr3() during bootstrap.
   And, before Loader(...), do set_cr3(pcb...) (two places).

```
NewProcSR
   set the new pcb[..].main_table = kernel_main_table

ExitSR
   set_cr3(kernel_main_table) when run_pid exits successfully

WaitSR
   use set_cr3() to switch to child's virtual space in order to access
   its trapframe for exit code, and switch back to run_pid's space
   (test this by adding SleepCall(1500) after KillCall(..) in UserProc)
```

**(done)** New constants:
```
   PAGE_NUM 100                       // up the page number from 20
   M256 0x10000000                    // virtual space starts 256MB
   G1_1 0x3fffffff                    // virtual space ends 1G-1B
   V_TF (G1_1 - sizeof(int [2]) - sizeof(trapframe_t) + 1) // virtual TF locale
   MASK10 0x003ff000                  // mask out others except 2nd 10 bits
   PRESENT 0x1                        // flag indicating page is Present
   RW 0x2                             // flag indicating page is R/W-able
```

ExecSR revisited:
```
   void ExecSR(int code, int arg) {
      int i, j, pages[5], *p, entry;
      trapframe_t *q;
      enum {MAIN_TABLE, CODE_TABLE, STACK_TABLE, CODE_PAGE, STACK_PAGE};
```
   1. allocate 5 RAM pages by
      loop page_user array, find each, put index in pages[5]
      if not getting 5 indices -> show Panic msg, return
      set page_user[] to so they're now used by run_pid
      calculate their addresses (put in/re-use pages[] which had indices)

   2. build code page (use addr already calculated in pages[CODE_PAGE])
      (call MemCpy to copy from code similar to as before)

   3. build stack page (addr is pages[STACK_PAGE])
      A. Bzero it
      B. put arg on top (set p, lower it, and write to where it points)
      C. skip 4 bytes (not used, just lower p)
      D. build trapframe by
         a. set q to p (needs typecasting of course)
         b. lower q (by one whole trapframe_t space)
         c. set q->efl and q->cs as before
         d. but set q->eip to virtual addr, the constant M256
      
   4. build addr-trans main table
      A. Bzero it
      B. MemCpy from kernel_main_table to duplicate the 1st 4 entries (each size int)
      C. get entry # = leftmost 10 bits in virtual addr M256
      D. set the content of this entry to the addr of code table bitwise-OR-ed with
         the PRESENT and RW flags
      E. get entry # from leftmost 10 bits in virtual addr G1_1 (or V_TF, same)
      F. set the content of this entry to the addr of stack table bitwise-OR-ed with
         the PRESENT and RW flags

   5. build code table (subtable)
      A. Bzero it
      B. get entry # = bits 11~20 from left (2nd 10 bits) in virtual addr M256 (use MASK10)
      C. set the content of this entry to the addr of code page bitwise-OR-ed with
         the PRESENT and RW flags

   6. build stack table (subtable)
      A. Bzero it
      B. get entry # = bits 11~20 from left (2nd 10 bits) in virtual addr G1_1 (use MASK10)
      C. set the content of this entry to the addr of stack page bitwise-OR-ed with
         the PRESENT and RW flags

   7. set the main_table in the PCB of run_pid to the addr of the main table
      set the trapframe_p in the PCB of run_pid to V_TF (typecast of course)
