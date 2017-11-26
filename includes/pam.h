/**
* @Author: Matthias Prost <prost_m>
* @Date:   2017-11-23T20:10:54+01:00
* @Email:  matthias.porst@epitech.eu
* @Filename: pam.h
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-26T16:00:03+01:00
*/

#ifndef _PAM_H_
#define _PAM_H_

#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

char  *my_strcat(char *dest, const char *src);

void  umount_container(const char *username);
void  mount_container(const char *username);
void  close_container(const char *username);
void  open_container(const char *username, const char *password);
void  create_container(const char *username);
void  encrypt_container(const char *username, const char *password);
void  format_container(const char *username);

char  *get_encrypted_file_path(const char *username);
char  *get_folder_path(const char *username);

int   create_folder(const char *username);
void  own_folder(const char *username);

#endif
