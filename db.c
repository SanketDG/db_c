#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    // primary key
    unsigned int key;
    // first name
    char fname[16];
    // last name
    char lname[16];
    // age
    unsigned int age;

} person_record;

int update_record(char *filename, int append_flag)
{
    int fd;

    if(append_flag == 0)
        fd = open(filename, O_CREAT | O_RDWR, 0644);
    else
        fd = open(filename, O_CREAT | O_RDWR | O_APPEND, 0644);

    if(fd == -1)
        perror("open_record");

    return fd;
}

void close_record(int fd)
{
    close(fd);
}

int insert_record(int fd, person_record *record)
{
    int ret;

    ret = write(fd, record, sizeof(person_record));
    return ret;
}

int get_record(int fd, person_record *record, int key)
{
    int ret;

    while ((ret = read(fd, record, sizeof(person_record))) != -1)
    {
        if(ret == 0)
        {
            memset(record, 0, sizeof(person_record));
            break;
            return ret;
        }
        else if(key == record->key)
            return ret;
    }
    memset(record, 0, sizeof(person_record));
    return ret;
}

int delete_record(int fd, int key)
{
    int ret;
    person_record record;

    off_t pos;

    pos = lseek(fd, 0, SEEK_SET);

    while ((ret = read(fd, &record, sizeof(person_record))) != -1)
    {
        if(ret == 0)
        {
            return ret;
        }
        else if(key == record.key) {
            lseek(fd, pos, SEEK_SET);
            record.key = 0;
            ret = write(fd, &record, sizeof(person_record));
            return ret;
        }
        pos = lseek(fd, 0, SEEK_CUR);
    }
    return ret;
}

int main(int argc, char *argv[])
{

    int fd;
    person_record rec;

    /* fd = open_record("data1"); */

    if(argc > 1)
    {
        // insert data

        if(argc > 5 && !strcmp(argv[1], "insert"))
        {
            fd = update_record("data1", 1);
            rec.key = atoi(argv[2]);
            strcpy(rec.fname, argv[3]);
            strcpy(rec.lname, argv[4]);
            rec.age = atoi(argv[5]);

            insert_record(fd, &rec);
        }

        /* delete data */

        if(argc > 2 && !strcmp(argv[1], "delete"))
        {
            fd = update_record("data1", 0);
            delete_record(fd, atoi(argv[2]));
        }

        /* print data */

        if(argc > 2 && !strcmp(argv[1], "print"))
        {
            fd = update_record("data1", 1);

            get_record(fd, &rec, atoi(argv[2]));

            printf("key = %d\n", rec.key);
            printf("First = %s\n", rec.fname);
            printf("Last = %s\n", rec.lname);
            printf("Age = %d\n", rec.age);
        }
    }

    close_record(fd);
    return 0;
}