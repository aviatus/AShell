#include "main.h"

void execmd(char **args)
{
    char *command = args[0];
    int pid = fork();
    if (pid == 0)
    {
        execvp(command, args);
        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(NULL);
    }
}

int main(void)
{
    char *prompt = "(AShell) $ ";
    char *full_command = NULL, *copy_command = NULL;
    size_t n = 0;
    ssize_t nchars_read;
    const char *delim = " \t\n";
    char **argv;
    int num_tokens = 0;
    char *token;
    int i = 0;

    while (1)
    {
        printf("%s", prompt);

        nchars_read = getline(&full_command, &n, stdin);
        if (nchars_read == -1 || strcmp(full_command, "exit\n") == 0)
        {
            break;
        }

        copy_command = malloc(sizeof(char) * nchars_read);
        if (copy_command == NULL)
        {
            perror("tsh: memory allocation error");
            break;
        }

        strcpy(copy_command, full_command);

        token = strtok(full_command, delim);
        while (token != NULL)
        {
            num_tokens++;
            token = strtok(NULL, delim);
        }
        num_tokens++;

        argv = malloc(sizeof(char *) * num_tokens);
        if (argv == NULL)
        {
            perror("tsh: memory allocation error");
            break;
        }

        token = strtok(copy_command, delim);
        for (i = 0; token != NULL; i++)
        {
            argv[i] = malloc(sizeof(char) * strlen(token) + 1);
            strcpy(argv[i], token);

            token = strtok(NULL, delim);
        }
        argv[i] = NULL;

        if (argv)
        {
            execmd(argv);
        }
    }

    free(argv);
    free(full_command);
    free(copy_command);
    return (0);
}
