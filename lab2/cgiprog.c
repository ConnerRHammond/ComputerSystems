#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    char * buf[1024]; 
    char * env = getenv("C"); 
    if(! env){
        fprintf(stderr,"Enviroment variable not found");
        fflush(stderr);
    }
    sprintf(env,"The query string is:%s\r\n",env);
    int len = (int)strlen(env);
    printf("Content-length:%d\r\n",len);
    printf("Content-type: text/plain\r\n\r\n%s",env);
    fflush(stdout);
}

// 1.
// USER         PID    PPID NLWP     LWP S CMD
// june9     988554  988169    1  988554 S echoserveri

// 2. One process and one thread since we are 
// not using concurrency it cannot run more than one thing at a time.

// 3.It receives the bytes from the other connections.

// 4. 
// USER         PID    PPID NLWP     LWP S CMD
// june9     993110  988169    1  993110 S echoserverp
// june9     993267  993110    1  993267 S echoserverp
// june9     993322  993110    1  993322 S echoserverp
// june9     993355  993110    1  993355 S echoserverp

// 5.There's 1 proces. One for parent for the server and 3 for each child
// connected to the server. Each process has one thread.

// 6.
// USER         PID    PPID NLWP     LWP S CMD
// june9     995008  988169    4  995008 S echoservert
// june9     995008  988169    4  995056 S echoservert
// june9     995008  988169    4  995078 S echoservert
// june9     995008  988169    4  995292 S echoservert

// 7.There's 1 process and  is running 4 threads a thread for each connection.

// 8.
// USER         PID    PPID NLWP     LWP S CMD
// june9     995950  988169    9  995950 S echoservert_pre
// june9     995950  988169    9  995951 S echoservert_pre
// june9     995950  988169    9  995952 S echoservert_pre
// june9     995950  988169    9  995953 S echoservert_pre
// june9     995950  988169    9  995954 S echoservert_pre
// june9     995950  988169    9  995955 S echoservert_pre
// june9     995950  988169    9  995956 S echoservert_pre
// june9     995950  988169    9  995957 S echoservert_pre
// june9     995950  988169    9  995958 S echoservert_pre

// 9. There is one process with 9 different threads.

// 10. 8 producer threads

// 11. 0 consumer threads

// 12. waiting for slots to be incremented

// 13. waiting for items

// 14. an item is added

// 15. Adding a connection/slot 

// 16. just one so far

// 17. Adding a connection/slot

// 18.It's always waiting on new connections new slots to be filled.

// 19.
// echoserverp
// echoservert
// echoservert_pre

// 20.echoservert_pre

// 21. Both the echoservert_pre and echoserverp 
// since they use threads that share data members

// 22. echoservert_pre

