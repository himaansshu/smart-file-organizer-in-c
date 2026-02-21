#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILES 1000

struct FileInfo {
    char name[300];
    long size;
};

struct FileInfo files[MAX_FILES];
int fileCount = 0;


int isExtension(const char *filename, const char *ext) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return 0;
    return strcmp(dot, ext) == 0;
}

void createFolder(const char *basePath, const char *folderName) {
    char newPath[400];
    sprintf(newPath, "%s/%s", basePath, folderName);
    mkdir(newPath);
}

void moveFile(const char *basePath, const char *folder, const char *filename) {
    char oldPath[400], newPath[400];

    sprintf(oldPath, "%s/%s", basePath, filename);
    sprintf(newPath, "%s/%s/%s", basePath, folder, filename);

    rename(oldPath, newPath);
}

void storeFileInfo(const char *basePath, const char *filename) {
    char fullPath[400];
    struct stat st;

    sprintf(fullPath, "%s/%s", basePath, filename);

    if (stat(fullPath, &st) == 0 && fileCount < MAX_FILES) {
        strcpy(files[fileCount].name, filename);
        files[fileCount].size = st.st_size;
        fileCount++;
    }
}

void findDuplicates() {
    int found = 0;

    printf("\nDuplicate Report \n");

    for (int i = 0; i < fileCount; i++) {
        for (int j = i + 1; j < fileCount; j++) {

            if (files[i].size == files[j].size &&
                strcmp(files[i].name, files[j].name) != 0) {

                printf("Duplicate files found:\n");
                printf("→ %s\n", files[i].name);
                printf("→ %s\n\n", files[j].name);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No duplicates found.\n");
    }
}

int main() {
    char path[300];
    struct dirent *entry;
    DIR *dir;

    printf("Smart File Organizer By Himanshu Kumar\n");
    printf("Enter folder path to organize:\n");

    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = 0;

    dir = opendir(path);

    if (dir == NULL) {
        printf("Could not open directory.\n");
        return 1;
    }

    createFolder(path, "Images");
    createFolder(path, "Documents");
    createFolder(path, "Videos");
    createFolder(path, "Music");
    createFolder(path, "Others");

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (strcmp(entry->d_name, "Images") == 0 ||
            strcmp(entry->d_name, "Documents") == 0 ||
            strcmp(entry->d_name, "Videos") == 0 ||
            strcmp(entry->d_name, "Music") == 0 ||
            strcmp(entry->d_name, "Others") == 0) {
            continue;
        }

        storeFileInfo(path, entry->d_name);

        if (isExtension(entry->d_name, ".jpg") ||
            isExtension(entry->d_name, ".png") ||
            isExtension(entry->d_name, ".jpeg") ||
            isExtension(entry->d_name, ".gif")) {

            moveFile(path, "Images", entry->d_name);
        }

        else if (isExtension(entry->d_name, ".pdf") ||
                 isExtension(entry->d_name, ".txt") ||
                 isExtension(entry->d_name, ".doc") ||
                 isExtension(entry->d_name, ".docx")) {

            moveFile(path, "Documents", entry->d_name);
        }

        else if (isExtension(entry->d_name, ".mp4") ||
                 isExtension(entry->d_name, ".mkv") ||
                 isExtension(entry->d_name, ".avi")) {

            moveFile(path, "Videos", entry->d_name);
        }

        else if (isExtension(entry->d_name, ".mp3") ||
                 isExtension(entry->d_name, ".wav") ||
                 isExtension(entry->d_name, ".aac") ||
                 isExtension(entry->d_name, ".flac")) {

            moveFile(path, "Music", entry->d_name);
        }

        else {
            moveFile(path, "Others", entry->d_name);
        }
    }

    closedir(dir);

    printf("\nCongratulations | Files Organized Successfully.\n");
    
    findDuplicates();

      printf("Developed by Himanshu Kumar\n");

    return 0;
}