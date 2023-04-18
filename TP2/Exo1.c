#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int checkFile(char *file_name)
{
    if (fopen(file_name, "r") == NULL)
    {
        return -1;
    }
    return 0;
}

int checkFileContent(char *f1_name, char *f2_name)
{
    FILE *file1, *file2;
    long file_size;
    char *buff;
    file1 = fopen(f1_name, "r");
    if (file1 == NULL)
    {
        printf("Error opening file %s!\n", f1_name);
    }
    file2 = fopen(f2_name, "r");
    if (file2 == NULL)
    {
        printf("Error opening file %s!\n", f2_name);
    }
    /* FILE 1 */
    fseek(file1, 0, SEEK_END); // Move the file pointer to the end of the file
    file_size = ftell(file1);  // ftell to determine the size of the file
    rewind(file1);             // Move the pointer back to the beggining
    buff = (char *)malloc(sizeof(char) * (file_size + 1));
    fread(buff, 1, file_size, file1);
    buff[file_size] = '\0';
    printf("%s", buff);
    fclose(file1);
    free(buff);

    /* FILE 2 */
    fseek(file2, 0, SEEK_END); // Move the file pointer to the end of the file
    file_size = ftell(file2);  // ftell to determine the size of the file
    rewind(file2);             // Move the pointer back to the beggining
    buff = (char *)malloc(sizeof(char) * (file_size + 1));
    fread(buff, 1, file_size, file2);
    buff[file_size] = '\0';
    printf("%s", buff);
    fclose(file2);
    free(buff);
    
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ERROR nom fichier 1 et 2 manquant:\n./Exo nomfichier1 nomfichier2\n");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        fprintf(stderr, "ERROR nom du fichier 2 manquant:\n./Exo nomfichier1 nomfichier2\n");
        exit(EXIT_FAILURE);
    }
    if (checkFile(argv[1]) == -1)
    {
        fprintf(stderr, "%s does not exist. \n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (checkFile(argv[2]) == -1)
    {
        fprintf(stderr, "%s does not exist. \n", argv[2]);
        exit(EXIT_FAILURE);
    }
    checkFileContent(argv[1], argv[2]);
    exit(EXIT_SUCCESS);
}