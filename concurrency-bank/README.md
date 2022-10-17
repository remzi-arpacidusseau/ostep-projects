
# **Mini-Project 2:**

# **A couple of transactions**

In this project, you will implement a banking system to execute some transactions on an account on behalf of a theoretical couple. You will be given their account balance, and the transactions each partner performed. You are to execute these transactions on the account given.

### Objectives

- Familiarize or re-familiarize yourself with the C programming language
- Use synchronization tools provided by the C POSIX library
- Apply concurrency to speed up task execution

### Summary of what gets turned in

- A single .c file named  **bank.c**.
- Each should compile successfully when compiled with the  **-Wall** , **-Werror, -lpthread, -lpt,** and **-O** flags

**Bank**

Bank is a small application that executes a series of banking transactions on behalf of a couple. It will take in three arguments from the command line: the starting account balance, a file containing the transactions made by the husband, and a file containing transactions made by the wife. Here is some example usage of the program:

``` bash
prompt ./bank 500 Husband.txt Wife.txt
```

Here, the bank program is to execute the transactions in Husband.txt and Wife.txt on their account, which has a starting balance of 500.

These transaction of the couple are to be executed concurrently.

To create the  **bank**  binary, you'll be creating a single source file,  **bank.c**. To compile this program, you will do the following:

``` bash
prompt> gcc -o bank bank.c -Wall -Werror -lpthread
```

Assume all transaction amounts are integers.

For example, the table below shows a series of concurrent requests on an account balance.

| Account balance | Husband transaction | Wife transaction |
| --- | --- | --- |
| 5000 |
| 5000 | Deposit(500) | Deposit(1000) |
| 6500 | Deposit(2000) | Withdraw(700) |
| 7800 | Withdraw(1500) | Deposit(200) |
| 6500 | Withdraw(750) | Withdraw(750) |
| 5000 |

For all cases in consideration, <text style="color:yellow">the account balance cannot be negative</text>. If a withdraw would result in a negative balance, the transaction request should be declined and dropped.

To ensure correctness, you will need to synchronize access to the couple's bank account. Say for example, if both the husband and the wife try to withdraw 500 from the account that contains 500 at the same time, you want to reject one of those transactions.

### Example of the contents of a file representing the transactions of a person

| deposit 500 <br> deposit 2000  <br>  withdraw 1500 <br> withdraw 750  <br>  |
| --- |

You'll need to learn how to use a few library routines from the C standard library (often called  **libc** ), well as the POSIX thread (Pthread) library for thread management to implement the source code for this program, which we'll assume is in a file called  **bank.c**.

All C code is automatically linked with the C library, which is full of useful functions you can call to implement your program. You will have to tell the compiler to link the pthread library to your code manually (hence, we pass the -lpthread, or -lpt) flag to the compiler. Learn more about the C library [here](https://en.wikipedia.org/wiki/C_standard_library) and perhaps [here](https://www-s.acm.illinois.edu/webmonkeys/book/c_guide/)[1](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-utilities#myfootnote1). Lelarn more about the Pthreads library [here](https://docs.oracle.com/cd/E19120-01/open.solaris/816-5137/tlib-1/index.html).

For this project, we recommend using the following routines to do file input and output: **fopen()**, **fgets()**, **getline()**, and **fclose()**, as well as **pthread\_create** and **pthread\_join** for managing threads. Whenever you use a new function like this, the first thing you should do is read about it -- how else will you learn to use it properly?

### Details

- Correct invocation should pass an integer and two files via the command line to the program; otherwise, the program should exit with exit code 1, printing only the message "bank: amount file1 file2" (with a newline).

- If any file cannot be accessed, the program should exit with exit code 1, printing the message "bank: cannot open file" (with a newline).

- Your assignment solution should implement the banking system using an appropriate synchronization tool to control access to the bank account balance.

- Your program should be able to process an <text style="color:red">infinite number</text> of transactions.

- Your program should handle the husband's and wife's transactions using two separate threads.

- Your program will be given an opening balance for the account before processing transactions.

- Transactions will be read from a text file separated by new lines Each transaction will be in the format:

<div align="center" style="color:red">
withdraw/deposit &lt;amount&gt;
</div>

- Your program should work <text style="color:red">even if some lines are empty</text>.

- Your program will output the starting balance **before** processing **all** transactions in the form below:

<div align="center" style="color:red">
Closing balance: A
</div>
<br>

- Your program will output the closing balance **after** processing **all** transactions in the form below:

<div align="center" style="color:red">
Closing balance: A
</div>
<br>

- Your program will output to the console the current bank balance after each transaction starting with the opening balance and ending with the closing balance after processing all transactions, <text style="color:red">strictly</text> in the format below:

<div align="center" style="color:red">
&lt;T&gt;: &lt;A&gt;, User: &lt;U&gt;, &lt;S | F&gt;
</div>
<br>
Where:

- &lt;T&gt;: Deposit | Withdraw
- &lt;A&gt;: int
- &lt;U&gt;: Wife | Husband
- &lt;S&gt;: Account balance after: int
- &lt;F&gt;: Transaction failed

### Example

Here's an elaborated example, _where the opening balance is 0_. The program was compiled and run with the arguments below:

``` bash
prompt gcc -o bank bank.c -Wall -Werror -lpthread
```

``` bash
prompt ./bank 0 Husband.txt Wife.txt
```

#### Contents of input files

| Wife.txt | Husband.txt |
| --- | --- |
| deposit 5000 <br> withdraw 5000 | withdraw 2500 <br> deposit 2000|

#### Possible output #1

``` txt
Opening balance: 0

Deposit: 5000, User: Wife, Account balance after: 5000

Withdraw: 2500, User: Husband, Account balance after: 2500

Withdraw: 5000, User: Wife, Transaction declined

Deposit: 2000, User: Husband, Account balance after: 4500

Closing balance: 4500
```

#### Possible output #2

``` txt
Opening balance: 0

Withdraw: 2500, User: Husband, Transaction declined

Deposit: 5000, User: Wife, Account balance after: 5000

Withdraw: 5000, User: Wife, Account balance after: 0

Deposit: 2000, User: Husband, Account balance after: 2000

Closing balance: 2000
```

#### Possible output #3

``` txt
Opening balance: 0

Deposit: 5000, User: Wife, Account balance after: 5000

Withdraw: 2500, User: Husband, Account balance after: 2500

Deposit: 2000, User: Husband, Account balance after: 4500

Withdraw: 5000, User: Wife, Transaction declined

Closing balance: 4500
```

Note that depending on the order in which the OS schedules threads, your output may vary.
