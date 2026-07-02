#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

#define SOURCE_DIR "source"
#define BACKUP_DIR "backup"
#define LOG_FILE "sync.log"

static FILE *log_fp;

static void log_event(const char *message) {
    time_t now = time(NULL);
    char stamp[32];
    strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log_fp, "[%s] %s\n", stamp, message);
    fflush(log_fp);
}

static int needs_sync(const char *src_path, const char *dst_path) {
    struct stat src_stat, dst_stat;

    if (stat(src_path, &src_stat) != 0)
        return 0;
    if (stat(dst_path, &dst_stat) != 0)
        return 1;

    return src_stat.st_mtime > dst_stat.st_mtime;
}

static int copy_file(const char *src_path, const char *dst_path) {
    FILE *src = fopen(src_path, "rb");
    if (!src)
        return -1;

    FILE *dst = fopen(dst_path, "wb");
    if (!dst) {
        fclose(src);
        return -1;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
        fwrite(buffer, 1, bytes, dst);

    fclose(src);
    fclose(dst);
    return 0;
}

static int sync_directory(const char *src_dir, const char *dst_dir) {
    DIR *dir = opendir(src_dir);
    if (!dir) {
        fprintf(stderr, "Cannot open source directory: %s\n", src_dir);
        return -1;
    }

    struct dirent *entry;
    int synced = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char src_path[512], dst_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst_dir, entry->d_name);

        struct stat st;
        if (stat(src_path, &st) != 0 || !S_ISREG(st.st_mode))
            continue;

        if (needs_sync(src_path, dst_path)) {
            char message[1200];
            if (copy_file(src_path, dst_path) == 0) {
                snprintf(message, sizeof(message), "Synced %s -> %s", src_path, dst_path);
                synced++;
            } else {
                snprintf(message, sizeof(message), "Failed to sync %s", src_path);
            }
            log_event(message);
            printf("%s\n", message);
        }
    }

    closedir(dir);
    return synced;
}

int main(void) {
    log_fp = fopen(LOG_FILE, "a");
    if (!log_fp) {
        fprintf(stderr, "Cannot open log file: %s\n", LOG_FILE);
        return 1;
    }

    mkdir(BACKUP_DIR, 0755);

    log_event("Sync run started");
    printf("Starting file sync: %s -> %s\n", SOURCE_DIR, BACKUP_DIR);

    int synced = sync_directory(SOURCE_DIR, BACKUP_DIR);

    char summary[128];
    snprintf(summary, sizeof(summary), "Sync run finished, %d file(s) synced", synced);
    log_event(summary);
    printf("%s\n", summary);

    fclose(log_fp);
    return 0;
}
