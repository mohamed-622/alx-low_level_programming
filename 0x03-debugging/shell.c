#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int string_length(const char *str);
int string_compare(const char *str1, const char *str2);
void string_copy(char *dest, const char *src);

/**
 * main - The main function that implements the simple shell.
 * Takes command, executes it, promptes for the next command.
 * Return: 0 on successful execution
 */
int main(void)
{
	char command[BUFFER_SIZE];
	ssize_t readBytes;
	pid_t pid;
	int status;

	while (1)
	{
		write(STDOUT_FILENO, "$ ", 2);

		readBytes = read(STDIN_FILENO, command, BUFFER_SIZE);
		if (readBytes == -1)
		{
			write(STDERR_FILENO, "read", 4);
			_exit(EXIT_FAILURE);
		}
		else if (readBytes == 0)
		{
			break;
		}

		command[readBytes - 1] = '\0';

		if (string_compare(command, "exit") == 0)
		{
			break;
		}

		pid = fork();

		if (pid == -1)
		{
			write(STDERR_FILENO, "fork", 4);
			_exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			char *args[4];

			args[0] = "/bin/sh";
			args[1] = "-c";
			args[2] = malloc(string_length(command) + 1);
			if (args[2] == NULL)
			{
				perror("malloc");
				_exit(EXIT_FAILURE);
			}
			string_copy(args[2], command);
			args[3] = NULL;

			execve("/bin/sh", args, NULL);
			write(STDERR_FILENO, "execve", 6);
			_exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}

	return (0);
}

/**
 * string_length - Calculates the length of a string.
 * @str: The input string
 * Return: The length of the string
 */
int string_length(const char *str)
{
	int length = 0;

	while (str[length] != '\0')
	{
		length++;
	}
	return (length);
}

/**
 * string_compare - Compares two strings.
 * @str1: The first string
 * @str2: The second string
 * Return: 0 if strings are equal, + if str1 > str2, - if str1 < str2
 */
int string_compare(const char *str1, const char *str2)
{
	int i = 0;

	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
		{
			return (str1[i] - str2[i]);
		}
		i++;
	}
	return (str1[i] - str2[i]);
}

/**
 * string_copy - Copies a string.
 * @dest: The destination string
 * @src: The source string
 */
void string_copy(char *dest, const char *src)
{
	int i = 0;

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
