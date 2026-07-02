#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BACKUP_FILE "backup_data.txt"
#define LOG_FILE "backup.log"

static void write_log(FILE *log, const char *message) {
    time_t now = time(NULL);
    char stamp[32];
    strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log, "[%s] %s\n", stamp, message);
}

static void create_backup_file(void) {
    FILE *fp = fopen(BACKUP_FILE, "w");
    if (!fp) {
        perror("fopen backup file");
        exit(1);
    }
    fprintf(fp, "record_id=1,status=ok\n");
    fprintf(fp, "record_id=2,status=ok\n");
    fprintf(fp, "record_id=3,status=ok\n");
    fclose(fp);
}

static void read_backup_file(void) {
    FILE *fp = fopen(BACKUP_FILE, "r");
    if (!fp) {
        perror("fopen backup file for reading");
        exit(1);
    }

    char line[128];
    printf("Backup file contents:\n");
    while (fgets(line, sizeof(line), fp))
        printf("  %s", line);

    fclose(fp);
}

int main(void) {
    FILE *log = fopen(LOG_FILE, "w");
    if (!log) {
        perror("fopen log file");
        return 1;
    }

    write_log(log, "Backup started");
    create_backup_file();
    write_log(log, "Backup file created and written");

    read_backup_file();
    write_log(log, "Backup file read back for verification");

    write_log(log, "Backup finished");
    fclose(log);

    printf("Backup complete. See %s and %s\n", BACKUP_FILE, LOG_FILE);
    return 0;
}
