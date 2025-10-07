struct rusage{
	unit cputime;
}

struct rusage;
int wait2(int *status, struct rusage *rusage);
