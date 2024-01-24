#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define ID_WIDTH 60
#define VALUE_WIDTH 4

#define ERR_WRONG_FILE_DATA "Wrong data in the file.\n"
#define ERR_BROKEN_FILE "Broken file.\n"
#define ERR_WHOLE_BUFFER "Couldn't write whole buffer.\n"
#define ERR_TOO_FEW_ARGUMENTS "Too few arguments.\n"
#define ERR_MAX_ID_LEN "Maximum identifier length exceeded.\n"
#define ERR_COMMANDS "Available commands are: add, query, list.\n"

enum cmd {
    CMD_ADD,
    CMD_QUERY,
    CMD_LIST
};

void Write(int fd, const void *buf, size_t len)
{
    ssize_t n;
    n = write(fd, buf, len);
    if (n == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    if (n != len) {
        fprintf(stderr, ERR_WHOLE_BUFFER);
        exit(EXIT_FAILURE);
    }
}

off_t Lseek(int fd, off_t offset, int whence)
{
    off_t off_res;
    if ((off_res = lseek(fd, offset, whence)) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    return off_res;
}

struct record {
    char key[ID_WIDTH];
    long value;
};

int record_from_bytes(const char *bytes, struct record *r) 
{
    char value_str[VALUE_WIDTH + 1];
    long value;
    char *endptr;
    
    memset(r, 0, sizeof(*r));
    strncpy(r->key, bytes, ID_WIDTH);
    strncpy(value_str, bytes + ID_WIDTH, sizeof(value_str));
    value = strtol(value_str, &endptr, 10);
    if (*endptr != '\0') {
        return -1;
    }

    r->value = value;

    return 0;
}

struct records_array {
    size_t len;
    size_t cap;
    struct record *p;
};

struct records_array *records_array_create(size_t cap)
{
    struct records_array *ra = malloc(sizeof(struct records_array));
    ra->cap = cap;
    ra->len = 0;
    ra->p = malloc(sizeof(struct record) * ra->cap);

    return ra;
}

void records_array_append(struct records_array *ra, struct record *record)
{
    ra->len++;
    if (ra->len > ra->cap) {
        ra->cap *= 2;
        ra->p = realloc(ra->p, sizeof(struct record) * ra->cap);
    }

    ra->p[ra->len - 1] = *record;
}

struct db {
    int fd;
    struct records_array *ra;
};

struct db *db_init(char *filename) 
{
    struct db *db;
    int fd;
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    db = malloc(sizeof(struct db));
    db->fd = fd;
    db->ra = records_array_create(4);

    return db;
}

long db_query(struct db *db, const char *id) 
{
    char buf[ID_WIDTH + VALUE_WIDTH];
    ssize_t count;
    while ((count = read(db->fd, buf, sizeof(buf)))) {
        struct record r;
        
        if (count == -1) {
            if (errno != EINTR) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            continue;
        }
        
        if (count < sizeof(buf)) {
            fprintf(stderr, ERR_BROKEN_FILE);
            exit(EXIT_FAILURE);
        }

        if (strncmp(id, buf, ID_WIDTH)) {
            continue;
        }

        if (record_from_bytes(buf, &r) == -1) {
            fprintf(stderr, ERR_WRONG_FILE_DATA);
            exit(EXIT_FAILURE);
        }

        return r.value;
    }
    
    return 0;
}

void db_add(struct db *db, const char *id) 
{
    size_t len;
    long value;
    char value_str[VALUE_WIDTH + 1];
    memset(value_str, 0, sizeof(value_str));
    value = db_query(db, id);
    snprintf(value_str, VALUE_WIDTH, "%ld", value + 1);
    len = sizeof(value_str) - 1;

    if (value) {
        Lseek(db->fd, -VALUE_WIDTH, SEEK_CUR);
        Write(db->fd, value_str, len);
    } else {
        char buf[ID_WIDTH + VALUE_WIDTH];
        strncpy(buf, id, ID_WIDTH);
        strncpy(buf + ID_WIDTH, value_str, VALUE_WIDTH);
        Write(db->fd, buf, sizeof(buf));
    }
}

struct records_array *db_list(struct db *db) 
{
    char buf[ID_WIDTH + VALUE_WIDTH];
    ssize_t count;
    db->ra->len = 0;
    while ((count = read(db->fd, buf, sizeof(buf)))) {
        struct record r;
        if (count == -1) {
            if (errno != EINTR) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            continue;
        }
        
        if (count < sizeof(buf)) {
            fprintf(stderr, ERR_BROKEN_FILE);
            exit(EXIT_FAILURE);
        }

        if (record_from_bytes(buf, &r) == -1) {
            fprintf(stderr, ERR_WRONG_FILE_DATA);
            exit(EXIT_FAILURE);
        }

        records_array_append(db->ra, &r);
    }

    return db->ra;
}

void db_close(struct db *db) 
{
    close(db->fd);
    free(db->ra->p);
    free(db->ra);
    free(db);
}

void validate_id(const char *id)
{
    if (strlen(id) > ID_WIDTH - 1) {
        fprintf(stderr, ERR_MAX_ID_LEN);
        exit(EXIT_FAILURE);
    }
}

void validate_argc(int argc, int required_argc) 
{
    if (argc < required_argc) {
        fprintf(stderr, ERR_TOO_FEW_ARGUMENTS);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    enum cmd cmd;
    char *filename, *cmd_text, *id;
    struct db *db;
    long count;
    int i;
    struct records_array *ra;

    validate_argc(argc, 3);

    filename = argv[1];
    cmd_text = argv[2];

    if (!strcmp(cmd_text, "add")) {
        cmd = CMD_ADD;
    } else if (!strcmp(cmd_text, "query")) {
        cmd = CMD_QUERY;
    } else if (!strcmp(cmd_text, "list")) {
        cmd = CMD_LIST;
    } else {
        fprintf(stderr, ERR_COMMANDS);
        exit(EXIT_FAILURE);
    }

    db = db_init(filename);

    switch (cmd) {
    case CMD_ADD:
        validate_argc(argc, 4);
        id = argv[3];
        validate_id(id);
        db_add(db, id);
        
        break;
    case CMD_QUERY:
        validate_argc(argc, 4);
        id = argv[3];
        validate_id(id);
        count = db_query(db, id);
        printf("%ld\n", count);
        
        break;
    case CMD_LIST:
        ra = db_list(db);
        
        for (i = 0; i < ra->len; i++) {
            struct record record;
            record = ra->p[i];
            printf("%s %ld\n", record.key, record.value);
        }

        break;
    }

    db_close(db);

    return EXIT_SUCCESS;
}
