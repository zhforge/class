#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	pid_t pid;

	pid = fork();

	if (pid < 0) { // error occurred
		fprintf(stderr, "Fork failed\n");
		return 1;
	}
	else if (pid == 0) { // child process
		printf("Child process started. ");

		if (argc == 1) { // no arguments
			printf("No arguments provided, terminating child...\n");
			exit(0);
		}
		else if (argc == 2) { // one argument
			printf("One argument provided, calling execlp()...\n");
			execlp(argv[1], argv[1], NULL); // gotta repeat it for some reason
		}
		else if (argc > 2) { // multiple arguments
			// select the passed arguments, and argv's terminating null pointer
			char *new_args[argc];
			for (int i = 0; i < argc; i++) {
				new_args[i] = argv[i+1];
			}

			printf("Multiple arguments provided, calling execvp()...\n");
			execvp(argv[1], new_args);
		}
	}
	else { // parent process
		int status;
		printf("Parent process started, waiting for process ID#%d\n", pid);
		
		// wait for child process to finish
		if (waitpid(pid, &status, 0) == -1) {
			printf("waitpid() failed\n");
			return 1;
		}

		printf("Parent process resumed. ");

		if (WIFEXITED(status)) {
			int exit_code = WEXITSTATUS(status);
			printf("Child process exited with code %d. ", exit_code);
		}
		else {
			printf("Child process terminated abnormally. ");
		}

		printf("Now terminating parent...\n");
	}
	return 0;
}
