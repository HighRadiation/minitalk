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
