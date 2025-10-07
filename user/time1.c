#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
	//this makes sure user does enter at least the 
	//one argument needed
	if (argc<2) {
	  printf("Usage: time1 command[args...]\n");
	  exit(1);
	}
	int start = uptime(); //this is to record the start time

	int pid = fork();
	if (pid <0){
	  printf("fork has failed\n");
	  exit(1);
	}

	if (pid == 0){
	  //the child will run the command
	  exec(argv[1], &argv[1]);
	  exit(1);
	}
	else{
	  //this process wait for child to finish then 
	  //calls uptime again to obtain the end time
	  //the parent will wait on the child
	 // wait(0);
	  //int end = uptime(); //to record the end time
	  //printf("elapsed time: %d ticks\n", end-start);
	//TASK 3
	struct rusage ru;
	int status = 0;
	wait2(&status,&ru);
	int end = uptime();

	int elapsed = end - start;
	int cpu = ru.cputime;
	int cpu_usage = 0;
	if (elapsed >0)
	  cpu_usage = cpu * 100 / elapsed;
	printf("elapsed time: %d ticks, cpu: %d ticks, %dCPU\n", elapsed, cpu, cpu_usage);
	exit(0);
	}
}

