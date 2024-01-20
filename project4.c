#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_COUNT 100

int main() {
    int fd[3][2];
    pid_t p1, p2, p3;
    int count[3] = {0};

    // create pipes
    for (int i = 0; i < 3; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // create child processes
    p1 = fork();
    if (p1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        // child process P1
        close(fd[0][0]); // close read end
        close(fd[1][1]); // close write end
        close(fd[2][1]); // close write end

        char *str1 = "Process P1: ";
        while (1) {
            // write to pipe P2
            write(fd[1][0], str1, sizeof(str1));
            count[0]++;

            // write to file
            FILE *file = fopen("process.log", "a");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(file, "%s", str1);
            fclose(file);

            // wait for P3 to print
            waitpid(p3, NULL, 0);

            // sleep for a random time
            srand(time(NULL));
            sleep(rand() % 3 + 1);
        }
    }

    p2 = fork();
    if (p2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p2 == 0) {
        // child process P2
        close(fd[0][0]); // close read end
        close(fd[0][1]); // close write end
        close(fd[2][1]); // close write end

        char *str2 = "Process P2: ";
        while (1) {
            // read from pipe P1
            char buf[MAX_COUNT];
            read(fd[0][0], buf, sizeof(buf));
            printf("%s", buf);
            fflush(stdout);
            count[1]++;

            // write to pipe P3
            write(fd[2][0], str2, sizeof(str2));
            count[1]++;

            // write to file
            FILE *file = fopen("process.log", "a");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(file, "%s", str2);
            fclose(file);

            // wait for P1 to print
            waitpid(p1, NULL, 0);

            // sleep for a random time
            srand(time(NULL));
            sleep(rand() % 3 + 1);
        }
    }

    p3 = fork();
    if (p3 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p3 == 0) {
        // child process P3
        close(fd[0][0]); // close read end
        close(fd[0][1]); // close write end
        close(fd[1][0]); // close read end

        char *str3 = "Process P3: ";
        while (1) {
            // read from pipe P2
            char buf[MAX_COUNT];
            read(fd[1][0], buf, sizeof(buf));
            printf("%s", buf);
            fflush(stdout);
            count[2]++;

            // write to file
            FILE *file = fopen("process.log", "a");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(file, "%s", str3);
            fclose(file);

            // sleep for a random time
            srand(time(NULL));
            sleep(rand() % 3 + 1);
        }
    }

    // parent process
    close(fd[0][0]); // close read end
    close(fd[0][1]); // close write end
    close(fd[1][0]); // close read end
    close(fd[1][1]); // close write end
    close(fd[2][0]); // close read end
    close(fd[2][1]); // close write end

    while (1) {
        // do nothing
    }

    return 0;
}