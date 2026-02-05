# What is required of us in this project?

---

## The main idea is message transmission using signals. Okay, how did I do this?

### First of all, I learned what a signal is;

* Signals are basically notifications sent by the OS to a process.
* You know when you press `Ctrl+C`? You are actually sending a signal called **SIGINT**.
* The OS gives us two user-defined signals: **SIGUSR1** and **SIGUSR2**.
* These signals don't carry data by themselves, so I created a logic: **I treated SIGUSR1 as '0' and SIGUSR2 as '1'.**
* So the question is... how do we use this to send text?

---

*** At the beginning of the project, I created a signal handler function. This function catches the signals and processes them bit by bit to reconstruct the message. ***

---

### How does the signal transmission work?

**Server Side:**

* First, the server starts and prints its Process ID (PID).
* It sets up signal handlers using `sigaction` (which is safer than `signal`).
* It waits for signals.
* **The Logic:**
* If it gets SIGUSR1 → it writes a 0 bit.
* If it gets SIGUSR2 → it writes a 1 bit.


* The server collects 8 bits (1 byte) to form a character.
* When the character is ready, it prints it.

**Client Side:**

* Takes the server's PID and the message as arguments.
* Converts every character into binary (8 bits).
* Sends each bit as a signal to the server:
* If the bit is 0 → send SIGUSR1
* If the bit is 1 → send SIGUSR2


* **Crucial Step:** I used `usleep()` (or a confirmation signal) between sends to prevent the server from getting overwhelmed.

---

### The Bit-by-Bit Transmission Process

Let's say we want to send the letter 'A':

1. 'A' in ASCII = 65
2. 65 in binary = `01000001`
3. Client sends: `SIGUSR1`, `SIGUSR2`, `SIGUSR1`, `SIGUSR1`, `SIGUSR1`, `SIGUSR1`, `SIGUSR1`, `SIGUSR2`.
4. Server receives them, puts the bits together: `01000001` → 'A'.

---

### Key Challenges I Faced

**Problem 1: Signal Loss**

* Signals were being lost when sent too quickly because the server couldn't catch up.
* **Solution:** I added delays (`usleep`) between signals to give the server time.

**Problem 2: Bit Order**

* My binary conversion was messy at first.
* **Solution:** I learned to use bitwise operations (shifting `>>` and `<<`) to send bits from the first to the last correctly.

---

### Project Structure

```
minitalk/
├── server.c          # The listener
├── client.c          # The sender
├── minitalk.h        # Definitions
├── libft/            # My custom library
└── Makefile          # Compilation

```

---

### What I Learned

* How UNIX signals actually work (it's not just magic).
* Sending data without standard tools like pipes.
* Binary manipulation (0s and 1s) and bitwise operations.
* The importance of timing and synchronization between two programs.
