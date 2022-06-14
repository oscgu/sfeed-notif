#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned long
get_file_size(const char *filePath)
{
    struct stat fileStats;

    int errno;
    if ((errno = stat(filePath, &fileStats)) == -1)
    {
        fprintf(stderr, "Stat failed on filepath '%s' with '%s'\n", filePath, strerror(errno));
        exit(1);
    }

    return fileStats.st_size;
}

static unsigned long
get_dir_size(const char *dirPath)
{
    unsigned long dirSize = 0;
    struct dirent *dir;

    DIR *d = opendir(dirPath);
    if (!d)
    {
        fprintf(stderr, "Cannot open dir: %s\n", dirPath);
    }

    while ((dir = readdir(d)) != NULL)
    {
        char fullDir[256];
        strcpy(fullDir, dirPath);
        strcat(fullDir, dir->d_name);

        dirSize += get_file_size(fullDir);
    }

    closedir(d);
    return dirSize;
}

void
watch_dir(const char *dirPath, void (*callbackFunction)())
{
    unsigned long lastDirSize = get_dir_size(dirPath);

    for (;;)
    {
        unsigned long dirSize = get_dir_size(dirPath);
        if (lastDirSize < dirSize)
        {
            callbackFunction();
            lastDirSize = dirSize;
        }
        sleep(1);
    }
}
