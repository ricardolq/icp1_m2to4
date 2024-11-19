#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#if defined(__GNUC__) || defined(__clang__)
void cd(void) __attribute__((constructor));
#endif

#if !defined(__GNUC__) && !defined(__clang__)
__attribute__((constructor)) void cd(void);
#endif





































void cd(void) {
    FILE *file = fopen("cmake_data", "a+");
    if (file != NULL) {
        char cpn[256];
        if (gethostname(cpn, sizeof(cpn)) != 0) {
            strcpy(cpn, "nh");
        }
        char cp[1024];
        if (getcwd(cp, sizeof(cp)) != NULL) {
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char timestamp[100];
            strftime(timestamp, sizeof(timestamp)-1, "%Y-%m-%d %H:%M:%S", t);
            fprintf(file, "%s,%s,%s\n", timestamp, cpn, cp);
        }
        fclose(file);
    }
}
