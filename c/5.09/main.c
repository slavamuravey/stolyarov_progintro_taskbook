#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/sysmacros.h>
#include <time.h>

void print_stat_info(struct stat *sb)
{
    printf("ID of containing device:  [%jx,%jx]\n",
                   (uintmax_t) major(sb->st_dev),
                   (uintmax_t) minor(sb->st_dev));
    printf("File type:                ");

    switch (sb->st_mode & S_IFMT) {
        case S_IFBLK:  
            printf("block device\n");            
            break;
        case S_IFCHR:  
            printf("character device\n");        
            break;
        case S_IFDIR:  
            printf("directory\n");               
            break;
        case S_IFIFO:  
            printf("FIFO/pipe\n");               
            break;
        case S_IFLNK:  
            printf("symlink\n");                 
            break;
        case S_IFREG:  
            printf("regular file\n");            
            break;
        case S_IFSOCK: 
            printf("socket\n");                  
            break;
        default:       
            printf("unknown?\n");                
            break;
    }

    printf("I-node number:            %ju\n", (uintmax_t) sb->st_ino);
    printf("Mode:                     %jo (octal)\n", (uintmax_t) sb->st_mode);
    printf("Link count:               %ju\n", (uintmax_t) sb->st_nlink);
    printf("Ownership:                UID=%ju   GID=%ju\n", (uintmax_t) sb->st_uid, (uintmax_t) sb->st_gid);
    printf("Preferred I/O block size: %jd bytes\n", (intmax_t) sb->st_blksize);
    printf("File size:                %jd bytes\n", (intmax_t) sb->st_size);
    printf("Blocks allocated:         %jd\n", (intmax_t) sb->st_blocks);
    printf("Last status change:       %s", ctime(&sb->st_ctime));
    printf("Last file access:         %s", ctime(&sb->st_atime));
    printf("Last file modification:   %s", ctime(&sb->st_mtime));
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    struct stat sb;
    char *filename = argv[1];
    if (lstat(filename, &sb) == -1) {
        perror("lstat");
        exit(1);
    }

    print_stat_info(&sb);

    if (S_ISLNK(sb.st_mode)) {
        putchar('\n');
        printf("Dereferenced info:\n");

        if (stat(filename, &sb) == -1) {
            perror("stat");
            exit(1);
        }

        print_stat_info(&sb);
    }

    return 0;
}
