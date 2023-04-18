#include <stdio.h>
#include <string.h>

#define BUFSIZE 1 << 20

int is_valid_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file) // file == NUll or file == 0
    {
        return 0;
    }
    fclose(file);
    return 1;
}

int compare_files(const char *_file1, const char *_file2)
{
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    FILE *file1 = fopen(_file1, "r");
    FILE *file2 = fopen(_file2, "r");
    if (!file1 || !file2)
    {
        return 0;
    }
    int line_num = 1;
    while (fgets(buf1, BUFSIZE, file1) && fgets(buf2, BUFSIZE, file2))
    {
        if (strcmp(buf1, buf2) != 0)
        {
            printf("Difference at line %d\n", line_num);
            printf("File 1: %s\nFile 2: %s\n", buf1, buf2);
            fclose(file1);
            fclose(file2);
            return 0;
        }
        line_num++;
    }
    fclose(file1);
    fclose(file2);
    return 1;
}

int main(int argc, char **argv)
{
    int opt;
    if (argc < 3)
    {
        printf("Usage : %s file1 file2 \n", argv[0]);
        return 1;
    }
    while ((opt = getopt(argc, argv, "r")) != -1)
    {
        switch (opt)
        {
        case 'r':
            printf("Option -r is set\n");
            break;

        default:
            printf("Usage: %s [-r]\n", argv[0]);
            return 1;
        }
    }
    const char *file1_name = argv[1];
    const char *file2_name = argv[2];

    if (!is_valid_file(file1_name) || !is_valid_file(file2_name))
    {
        printf("Error : file 1 or file 2 is not valid.\n");
        return 1;
    }

    if (compare_files(file1_name, file2_name))
    {
        printf("Files are identical");
    }
    else
    {
        printf("Files are different");
    }
    return 0;
}
