# shmcat

Copyright (c) 2014-2017 Nicholas J. Kain.

License: Two-clause BSD.

## Introduction

Prints the contents of a SysV shm segment.

## Motivation

SysV shared memory can be used as a persistent store of state that lives
in a separate namespace from the filesystem and the process table.  Thus,
it is not uncommon for it to be used as a covert channel for malware.

It would be useful to be able to examine the contents of shm segments,
but unfortunately there is no standard tool to do so.  shm segments can
easily be listed using the POSIX ipcs utility, but there is no standard
program to dump the contents of a shm segment given a shmid.

shmcat is a very simple program written to fill this gap and make it
easy to analyze the contents of shm segments.

## Building

`gcc -O2 -std=gnu99 shmcat.c -o shmcat`

## Install

Copy to wherever you like.  A good place might be `/usr/local/sbin`.

## Usage

First, find an interesting shmid.  To get a list of all shmids on
the system:

`ipcs -m`

Now, given the <shmid>:

`shmcat <shmid>`

The output should be very similar to that of a hex editor.  It can of
course be redirected to a file using standard unix shell facilities.

