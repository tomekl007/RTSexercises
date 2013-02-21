#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {


	//printf("Welcome to the QNX Momentics IDE\n");
		printf("Process Id PID: %d\n",getpid());
		printf("Parent Process Id PPID %d\n",getppid());
		printf("Group Processes Id PGID %d\n",getpgrp());
		printf("Session Id PGID %d\n",getsid());
		printf("User Id  %d\n",getuid());
		printf("Group Id  %d\n",getgid());
		printf("Process Envoriment %c\n", getenv("INCLUDE"));
		printf("Get Priority  %d\n", getprio());
	//	printf("Current Catalog  %d\n", getcwd()); //or sched_getparam()
		printf("Schedule Strategy  %d\n", sched_getscheduler());




	return EXIT_SUCCESS;
}
