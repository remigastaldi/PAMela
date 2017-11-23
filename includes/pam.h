/**
* @Author: Matthias Prost <prost_m>
* @Date:   2017-11-23T20:10:54+01:00
* @Email:  matthias.porst@epitech.eu
* @Filename: pam.h
 * @Last modified by:   prost_m
 * @Last modified time: 2017-11-23T23:39:05+01:00
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

#endif
