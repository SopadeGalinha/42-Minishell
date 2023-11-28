#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// code vault studies

#define PROCESS_NUM 3  // Adjust the number of processes based on your needs

void execute_commands(int argc, char* argv[])
{
    int	pipes[PROCESS_NUM][2];
    int i;

	i = 0;
	// Create all pipes
    while (i < PROCESS_NUM) {
        if (pipe(pipes[i]) == -1) {
            perror("Error with creating pipe");
            return;
        }
        i++;
    }

    i = 0;
    while (i < argc - 1) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Error with creating process");
            return ;
        }

        if (pid == 0) {
            // Child process
            if (i != 0) {
                // Redirect input for all processes except the first one
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }

            // Redirect output for all processes except the last one
            if (i != argc - 2) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            // Close all pipes in the child process
            int j;
            for (j = 0; j < argc - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Parse the command
            char* args[4];  // Adjust the size based on your needs
            args[0] = "/bin/sh";
            args[1] = "-c";
            args[2] = argv[i + 1];
            args[3] = NULL;

            // Execute the command
            if (execve("/bin/sh", args, NULL) == -1) {
                perror("Error executing command");
                return;
            }
        }

        i++;
    }

    // Close all pipes in the parent process
    for (i = 0; i < argc - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    i = 0;
    while (i < argc - 1) {
        wait(NULL);
        i++;
    }
}

typedef struct s_pipes
{
	char						**cmds;
	struct s_pipes				*next;
}					t_pipes;

int main(int argc, char* argv[], char* envp[])
{
	t_pipes	head;

    execute_commands(argc, argv);
    return 0;
}
