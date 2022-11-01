#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_LEN 1000


void list_dir(char *dirPath, char *permissions, int size)
{
    DIR *dir;
    struct dirent *dirEntry;
    struct stat inode;
    char name[MAX_PATH_LEN];

    lstat(dirPath, &inode);

    if (!S_ISDIR(inode.st_mode))
    {
        printf("%s\n", dirPath);
        exit(7);
    }
    printf("SUCCESS\n");

    dir = opendir(dirPath);
    if (dir == 0)
    {
        printf("ERROR\ninvalid directory path\n");
        exit(4);
    }

    while ((dirEntry = readdir(dir)) != 0)
    {

        snprintf(name, MAX_PATH_LEN, "%s/%s", dirPath, dirEntry->d_name);

        if (strcmp(permissions, "") != 0)
        {
            lstat(name, &inode);
            int rights[9];
            for (int i = 0; i < 9; i++)
            {
                if (permissions[i] != '-')
                {
                    rights[i] = 1;
                }
                else
                    rights[i] = 0;
            }
            int checked = 1;
            for (int i = 0; i < 9; i++)
            {
                switch (i)
                {
                case 0:
                    if (rights[i] == 1)
                    {
                        if (!(S_IRUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IRUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 1:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 2:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 3:
                    if (rights[i] == 1)
                    {
                        if (!(S_IRGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IRGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 4:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 5:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 6:
                    if (rights[i] == 1)
                    {
                        if (!(S_IROTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IROTH & inode.st_mode)
                        checked = 0;
                    break;

                case 7:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWOTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWOTH & inode.st_mode)
                        checked = 0;
                    break;

                case 8:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXOTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXOTH & inode.st_mode)
                        checked = 0;
                    break;
                }
            }
            if (checked)
            {
                if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
                    printf("%s\n", name);
            }
        }
        else if (size >= 0)
        {
            lstat(name, &inode);
            if (S_ISREG(inode.st_mode) && inode.st_size > size)
            {
                printf("%s\n", name);
            }
        }
        else if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
            printf("%s\n", name);
    }

    closedir(dir);
}

void recursive_list_dir(char *dirPath, int first, char *permissions, int size)
{
    DIR *dir;
    struct dirent *dirEntry;
    struct stat inode;
    char name[MAX_PATH_LEN];

    lstat(dirPath, &inode);

    if (!S_ISDIR(inode.st_mode))
    {
        printf("%s\n", dirPath);
        exit(7);
    }
    if (first)
        printf("SUCCESS\n");

    dir = opendir(dirPath);
    if (dir == 0)
    {
        printf("%s\n", dirPath);
        printf("ERROR\ninvalid directory path\n");
        exit(4);
    }

    while ((dirEntry = readdir(dir)) != 0)
    {

        snprintf(name, MAX_PATH_LEN, "%s/%s", dirPath, dirEntry->d_name);

        lstat(name, &inode);
        if (S_ISDIR(inode.st_mode) && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
        {
            recursive_list_dir(name, 0, permissions, size);
        }
        if (strcmp(permissions, "") != 0)
        {
            stat(name, &inode);
            int rights[9];
            for (int i = 0; i < 9; i++)
            {
                if (permissions[i] != '-')
                {
                    rights[i] = 1;
                }
                else
                    rights[i] = 0;
            }
            int checked = 1;
            for (int i = 0; i < 9; i++)
            {
                switch (i)
                {
                case 0:
                    if (rights[i] == 1)
                    {
                        if (!(S_IRUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IRUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 1:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 2:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXUSR & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXUSR & inode.st_mode)
                        checked = 0;
                    break;

                case 3:
                    if (rights[i] == 1)
                    {
                        if (!(S_IRGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IRGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 4:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 5:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXGRP & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXGRP & inode.st_mode)
                        checked = 0;
                    break;

                case 6:
                    if (rights[i] == 1)
                    {
                        if (!(S_IROTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IROTH & inode.st_mode)
                        checked = 0;
                    break;

                case 7:
                    if (rights[i] == 1)
                    {
                        if (!(S_IWOTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IWOTH & inode.st_mode)
                        checked = 0;
                    break;

                case 8:
                    if (rights[i] == 1)
                    {
                        if (!(S_IXOTH & inode.st_mode))
                            checked = 0;
                    }
                    else if (S_IXOTH & inode.st_mode)
                        checked = 0;
                    break;
                }
            }
            if (checked)
            {
                if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
                    printf("%s\n", name);
            }
        }
        else if (size != -1)
        {
            stat(name, &inode);
            if (S_ISREG(inode.st_mode) && inode.st_size > size)
            {
                printf("%s\n", name);
            }
        }
        else if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
            printf("%s\n", name);
    }

    closedir(dir);
}

int validType(unsigned char type){
    int ok38 = 0, ok22 = 0, ok34 = 0, ok67 = 0;

    if(type == 38)
        ok38 = 1;
    if(type == 22)
        ok22 = 1;
    if(type == 34)
        ok34 = 1;
    if(type == 67)
        ok67 = 1;
    
    return (ok38 || ok22 || ok34 || ok67);
}

void parse(char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        printf("File not open\n");
        close(fd);
        return;
    }

    char magic[] = "0000";
    lseek(fd,-4,SEEK_END);
    read(fd,magic,4);
    if (strcmp("x1YX", magic) != 0)
    {
        printf("ERROR\nwrong magic");
        close(fd);
        return;
    }

    short header_size = 0;
    lseek(fd, -6, SEEK_END);
    read(fd, &header_size, 2);

    lseek(fd, -header_size, SEEK_END);
    u_int8_t version = 0;
    read(fd, &version, 1);
    if (version < 78 || version > 187)
    {
        printf("ERROR\nwrong version");
        close(fd);
        return;
    }

    u_int8_t no_of_sections = 0;
    read(fd, &no_of_sections, 1);
    if(no_of_sections < 8 || no_of_sections > 16){
        printf("ERROR\nwrong sect_nr");
        close(fd);
        return;
    }

    u_int8_t type = 0;
    for(int section = 0; section < no_of_sections; section++){
        lseek(fd, 20, SEEK_CUR);
        read(fd, &type, 1);
        if(!validType(type)){
            printf("ERROR\nwrong sect_types");
            close(fd);
            return;
        }
        lseek(fd, 8, SEEK_CUR);
    }

    char* name = (char*)malloc(5*sizeof(int)+1);
    int size = 0, offset = 0;
    lseek(fd, -header_size, SEEK_END);
    read(fd, &version, 1);
    read(fd, &no_of_sections, 1);
    printf("SUCCESS\nversion=%d\nnr_sections=%d\n", version, no_of_sections);
    for(int section = 0; section < no_of_sections; section++){
        strcpy(name,"");
        read(fd, name, 20);
        read(fd, &type, 1);
        read(fd, &offset, 4);
        read(fd, &size, 4);
        printf("section%d: %s %d %d\n", section+1, name, type, size);
    }
    free(name);
    close(fd);
    return;
}

int main(int argc, char **argv)
{
    char *perm = (char *)malloc(9);
    int size = -1;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("83579\n");
        }
        if (argc >= 3)
        {
            if (strcmp(argv[1], "list") == 0)
            {
                char path[MAX_PATH_LEN] = "";
                for (int i = 5; i < strlen(argv[argc - 1]); i++)
                {
                    path[i - 5] = argv[argc - 1][i];
                }
                if (argc >= 4)
                {
                    if (argc == 5)
                    {
                        if (argv[2][0] == 'p')
                        {
                            strcpy(perm, argv[2] + 12);
                        }
                        else if (argv[3][0] == 'p')
                        {
                            strcpy(perm, argv[3] + 12);
                        }
                        else if (argv[2][0] == 's')
                        {
                            size = atoi(argv[2] + 13);
                        }
                        else if (argv[3][0] == 's')
                        {
                            size = atoi(argv[3] + 13);
                        }
                    }
                    else
                    {
                        if (argv[2][0] == 'p')
                        {
                            strcpy(perm, argv[2] + 12);
                        }
                        else if (argv[2][0] == 's')
                        {
                            size = atoi(argv[2] + 13);
                        }
                    }
                    if (strcmp(argv[2], "recursive") == 0 || strcmp(argv[3], "recursive") == 0)
                    {
                        recursive_list_dir(path, 1, perm, size);
                    }
                    else
                        list_dir(path, perm, size);
                }
                else
                    list_dir(path, perm, size);
            }
            else
            {
                if (argc == 3)
                {
                    if (strcmp(argv[1], "parse") == 0 || strcmp(argv[2], "parse") == 0)
                    {
                        if (strcmp(argv[1], "parse") == 0)
                        {
                            char* pathp;
                            pathp = strtok(argv[2],"=");
                            pathp = strtok(NULL,"=");
                            parse(pathp); //argv[2]+5
                        }
                        else
                        {
                            char* pathp;
                            pathp = strtok(argv[2],"=");
                            pathp = strtok(NULL,"=");
                            parse(pathp);
                        }
                    }
                }
            }
        }
    }
    free(perm);
    return 0;
}
