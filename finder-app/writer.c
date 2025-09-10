// File: writer.c
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char ** argv) {
    openlog(NULL, 0, LOG_USER);

    if (argc < 3) {
        syslog(LOG_ERR, "Usage: ./writer (writefile) (writestr)\n");
        closelog();
        return 1;
    }

    FILE * outfile = fopen(*(argv + 1), "w");
    if (outfile == NULL) {
        syslog(LOG_ERR, "ERROR: Could not open file \"%s\"\n", *(argv + 1));
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s\n", *(argv + 2), *(argv + 1));
    fprintf(outfile, "%s\n", *(argv + 2));

    fclose(outfile);
    closelog();
    return 0;
}
