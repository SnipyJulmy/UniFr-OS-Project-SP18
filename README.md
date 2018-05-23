# UniFr-OS-Project-SP18

## Authors
* Michaël Papinutto
* Sami Veillard
* Sylvain Julmy

## Database operations
- `ls` display the content of the database
- `add <key> <value>` add the specified *value*, related to the specified *key*, to the database
- `add <value>` add the specified *value* to the database and return the associated key 
- `read_k <key>` return the value of the associated *key*
- `read_v <value>` return a key for the associated *value*
- `rm_k <key>` delete the database entry specified by *key*
- `rm_v <value>` delete a database entry specified by *value*
 
- `update <key> <value>` update the value specified by *key*, if *key* don't exist, the behavior is similar to `add <key> <value>` 
- `q` terminate the server process

## Project specifications

The goal of the Mini-Project Lab is to implement a simple In-Memory Key-Value store with simultaneous access via TCP in Linux.

“Key-value (KV) stores use the associative array (also known as a map or dictionary) as their fundamental data model. In this model, data is represented as a collection of key-value pairs, such that each possible key appears at most once in the collection.” [Wik 15](http://en.wikipedia.org/wiki/NoSQL#Key-value_store)

Race conditions e.g. due to badly synchronized database accesses are still an issue in real use cases as of today. [Fra 15](http://josipfranjkovic.blogspot.ch/2015/04/race-conditions-on-facebook.html)

### Deliverables

* A runnable code with a makefile and testing scripts.
* A short two pages documentation per group.
* Preparation of a final presentation per group (3min/Person).
* One page report per participant (Approach chosen, Problems encountered, Lessons learned.).

### Minimal feature set

* Ability to write keys with values through TCP sockets.
* Ability to read values through TCP sockets by providing a key. Graceful fallback (error
message) if key is nonexistent.
* Use multiple threads.
* Simultaneous safe access of the readers and the writers.
* Provide an automated testing in form of another program or script. (Showing all the mentioned functionalities including simultaneous and mixed read/write requests.)

### Implementation decision

* Read or Write priority: Which requests have priority, writes or reads?
* Data structure Size: How many entries can the KV store have? Can the size grow?
* Simultaneous access: How many connections to allow simultaneously?
* Synchronization: How to synchronize the threads?

### Hints
* [Semaphores](http://web.cs.wpi.edu/~claypool/courses/3013-B01/samples/share-sem.c)
* [Fork with Semaphores](http://cboard.cprogramming.com/c-programming/67202-simple-short-fork-join-parallelization.html#post477394)
