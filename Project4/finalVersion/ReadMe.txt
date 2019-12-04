/*
 * Project 4
 *
 *  Last Edit: Dec 4, 2019
 *      Author: Junxiang Wen
 */


The goal of this project is to implement a simple file system on top of a virtual disk. To this end,
Implement a library of functions that offers a set of basic file system calls (such as
open, read, write, ...) to applications. The file data and file system meta-information will be
stored on a virtual disk. This virtual disk is actually a single file that is stored on the "real" file
system provided by the Linux operating system. That is, you are basically implementing your file
system on top of the Linux file system. 


Befor mount a disk:
        mkDisk + disk_name:           creat a disk with name disk_name
        oDisk + disk_name:            Open a disk with name disk_name
        cDisk + disk_name:            Close a disk with name disk_name
        mkFS + disk_name:             Creat file System for a disk with name disk_name
        mouFS + disk_name:            mount a file system for a disk with name disk_name

After mount a disk:
        umouFS + disk_name:           unmount a file system for a disk with name disk_name
        mkFile + file_name:           creat a file in file system with name file_name
        mkFile + directory_name:      creat a directory file in file system with name directory_name

After creat a file:
        opFile + file_name:           open a file with name file_name
        trFile + num1 + length:       truncated a file with fd(File descriptor) to be truncated to length bytes
        cFile + fd:                   close a file with fd(File descriptor)
        deFile + file_name:           delete a file or directory with name file_name
        getSize + fd:                 get the size of a file with fd(File descriptor)
        reFile + fd + length:         read a file for length byte with fd(File descriptor) (length = 0 to read the whole file)
        wrFile + fd:                  write a file with fd(File descriptor)
        setSeek + fd + offset:        set the offset of the file  with fd(File descriptor) 
        setSeek + fd:                 set the offset of the file  to the end of file with fd(File descriptor) 

Extra command
        quit:                         quit the progarm
        help:                         show this instruction again
        ls:                           list all file name in current directory
        pwd:                          list the current path


