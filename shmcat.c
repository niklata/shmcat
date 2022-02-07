/*
 * Copyright (c) 2014 Nicholas J. Kain
 * SPDX-License-Identifier: MIT
 * Prints the contents of a SysV shm segment.
 *
 * Build: gcc -O2 -std=gnu99 shmcat.c -o shmcat
 * Usage: shmcat <shmid>
 *
 * Get the shmid by using 'ipcs -m'.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Need to specify a shmid as first argument.\n");
        return -1;
    }
    char *eptr;
    long shmidl = strtol(argv[1], &eptr, 0);
    if (((shmidl == LONG_MIN || shmidl == LONG_MAX) && errno == ERANGE)
        || eptr == argv[1]) {
        printf("shmid out of range or not a number: '%s'\n", argv[1]);
        return -1;
    }
    if (shmidl < 0 || shmidl > INT_MAX) {
        printf("shmid out of range: '%lu'\n", shmidl);
        return -1;
    }
    int shmid = shmidl;
    struct shmid_ds ds;
    int err = shmctl(shmid, IPC_STAT, &ds);
    if (err < 0) {
        printf("shmctl() failed: %s\n", strerror(errno));
        return -1;
    }
    size_t shm_bytes = ds.shm_segsz;
    printf("shmid=%i bytes=%zu\n-=-=-=-=-=-\n", shmid, shm_bytes);
    void *s = shmat(shmid, NULL, SHM_RDONLY);
    if (s == (void *)-1) {
        printf("shmat() failed: %s\n", strerror(errno));
        return -1;
    }
    unsigned char sbuf[32];
    memset(sbuf, 0, sizeof sbuf);
    size_t si = 0;
    printf("\n%8.8zX  ", si);
    for (si = 0; si < shm_bytes; ++si) {
        size_t sis = si % 16;
        if (si && si % 16 == 0) {
            printf(" ");
            for (size_t i = 0; i < 16; ++i) {
                printf("%c", isprint(sbuf[i]) ? sbuf[i] : '.');
            }
            memset(sbuf, 0, sizeof sbuf);
            printf("\n%8.8zX  ", si);
        }
        sbuf[sis] = ((char *)s)[si];
        printf("%2.2X ", sbuf[sis]);
    }
    if (si) {
        size_t sis = si % 16;
        sis = sis ? sis : 16;
        for (size_t i = sis; i < 16; ++i)
            printf("   ");
        printf(" ");
        for (size_t i = 0; i < sis; ++i) {
            printf("%c", isprint(sbuf[i]) ? sbuf[i] : '.');
        }
        memset(sbuf, 0, sizeof sbuf);
        printf("\n");
    }
    printf("\n");

    err = shmdt(s);
    if (err < 0) {
        printf("shmdt() failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

