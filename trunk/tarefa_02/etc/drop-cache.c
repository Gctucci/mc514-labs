/*
 * drop VM cache for OS experiments.
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
//    int i, arg_start = 1, count = 100, drop_caches = 1;
//    double tmin = DBL_MAX, tmax = 0, ttotal = 0;

    FILE *f = fopen("/proc/sys/vm/drop_caches", "w");
    if (!f) {
        perror("could not open for write: /proc/sys/vm/drop_caches");
        _exit(1);
    }
    fputs("1\n", f);
    fclose(f);
	
    return 0;
}
