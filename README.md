# What is required of us in this project?
---
## The main idea is, message transmission using signals. Okay, how ı did this?
### First of all, ı learned what is a signal;
* Signals are asynchronous notifications sent by the OS kernel to a process (e.g., SIGINT when pressing Ctrl+C).
* When you are push a key combination something like ctrl+c, ctrl+v this sends 2 things to proccessor, 0 and 1, simply ı know this side of signals.
* Ctrl+C, is a SIGINT btw but try to understand it. 
* The OS have 2 signal functions, SIGUSR1 and SIGUSR2 this functions are returns 0 and 1, (SIGUSR1 => 0 and SIGUSR2 => 1)
* So the question is.. how used this functions?
---

*** Begining of the project ı just used a take_signal function, this function is taking the signals and processing it. It takes the string and uses one bit to one bit. 
---

### How does the signal transmission work?

**Server Side:**
* First, the server starts and prints its Process ID (PID)
* It sets up signal handlers using `sigaction()` for SIGUSR1 and SIGUSR2
* When a signal arrives, the handler function is triggered
* Each signal represents one bit:  SIGUSR1 = 0, SIGUSR2 = 1
* The server collects 8 bits (1 byte) to form a character
* After receiving a complete character, it prints it

**Client Side:**
* Takes the server's PID and the message to send as arguments
* Converts each character to binary (8 bits)
* Sends each bit as a signal to the server: 
  - If bit is 0 → send SIGUSR1
  - If bit is 1 → send SIGUSR2
* Uses `usleep()` between signals to prevent signal loss
* Waits for acknowledgment from server before sending next bit

---

### The Bit-by-Bit Transmission Process

Let's say we want to send the letter 'A': 
1. 'A' in ASCII = 65
2. 65 in binary = 01000001
3. Client sends:  SIGUSR1, SIGUSR2, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR2
4. Server receives and reconstructs:  01000001 → 'A'

---

### Key Challenges I Faced

**Problem 1: Signal Loss**
* Signals were being lost when sent too quickly
* **Solution:** Added `usleep()` delays between signals

**Problem 2: Bit Order**
* Initially sent bits in wrong order
* **Solution:** Implemented proper bit shifting (MSB to LSB or LSB to MSB)

**Problem 3: Synchronization**
* Client and server were getting out of sync
* **Solution:** Implemented acknowledgment system (bonus part)

---

### Project Structure

```
minitalk/
├── server. c          # Server implementation
├── client. c          # Client implementation
├── minitalk.h        # Header file with definitions
├── libft/            # Custom library functions
└── Makefile          # Compilation rules
```

---

### How to Use

**Compilation:**
```bash
make
```

**Running the Server:**
```bash
./server
# Server will display its PID, for example: Server PID: 12345
```

**Running the Client:**
```bash
./client [SERVER_PID] "Your message here"
# Example: ./client 12345 "Hello, 42!"
```

---

### Bonus Features

If you implemented bonuses, you can add: 

* ✅ Server acknowledges every received signal
* ✅ Unicode character support
* ✅ Multiple client handling

---

### What I Learned

* How UNIX signals work at a low level
* Process communication (IPC) without pipes or shared memory
* Binary manipulation and bit operations
* Importance of synchronization in asynchronous systems
* Signal handling and `sigaction()` vs `signal()`

---

### Useful Resources

* `man 2 kill` - Sending signals to processes
* `man 2 signal` - Signal handling
* `man 2 sigaction` - Advanced signal handling
* `man 3 usleep` - Microsecond delays

---

**Grade:** [Your grade here]  
**Project Duration:** [Your duration]  
**School:** 42

