/**
* @Author: Remi Gastaldi <gastal_r>
* @Date:   2017-11-23T17:38:39+01:00
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-26T12:15:08+01:00
*/

#include "pam.h"
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
//
// void  check_folder_permission(char *path, const char *username)
// {
//   struct stat info;
//   sleep(1);
//   if (stat("/home/pamela/secure_data-rw", &info) != -1)
//     return;
//     printf("===========\n");
//     return;
//   struct passwd *pw = getpwuid(info.st_uid);
//   struct group  *gr = getgrgid(info.st_gid);
//
//   struct passwd *pwd = getpwnam(username);
//
//   printf("==============\n");
//   printf("%s %s\n", pw->pw_name, gr->gr_name);
//   // if (pwd->pw_uid == pw->pw_name && pwd->pw_gid == gr->gr_name)
//   //   return;
//   chown("/home/pamela/secure_data-rw", 1000, 1000);
// }

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int retval = 0;
  const char* username = 0;
  char  *path = 0;

  retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;

  path = get_folder_path(username);
  if (strcmp(username, "root") != 0)
  {
    path = get_folder_path(username);
    if (access(path, F_OK) == -1)
    {
      if (create_folder(username) == -1)
      {
        free (path);
        return (PAM_SUCCESS);
      }
    }
    mount_container(username);
    own_folder(username);
    free(path);
  }

  return(PAM_SUCCESS);
}

void  umount_container(const char *username)
{
  char *cmd = 0;
  char *path = get_folder_path(username);

  size_t needed = snprintf(NULL, 0, "umount %s", path) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "umount %s", path);
  system(cmd);
  free(cmd);
  free(path);
}

void  close_container(const char *username)
{
  char *cmd = 0;

  size_t needed = snprintf(NULL, 0, "cryptsetup luksClose %s_secure_data", username) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "cryptsetup luksClose %s_secure_data", username);
  system(cmd);
  free(cmd);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int retval;
  const char* username;
  char *path = NULL;

  retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;

  if (strcmp(username, "root") != 0)
  {
    path = get_folder_path(username);
    umount_container(username);
    close_container(username);
    free(path);
  }
  return(PAM_SUCCESS);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_IGNORE);
}

char  *get_folder_path(const char *username)
{
  char  *path = 0;

  size_t needed = snprintf(NULL, 0, "/home/%s/secure_data-rw", username) + 1;
  path = malloc(needed);
  snprintf(path, needed, "/home/%s/secure_data-rw", username);

  return (path);
}

char  *get_encrypted_file_path(const char *username)
{
  char  *path = 0;

  size_t needed = snprintf(NULL, 0, "/home/%s/secure_data-rw.encrypted", username) + 1;
  path = malloc(needed);
  snprintf(path, needed, "/home/%s/secure_data-rw.encrypted", username);

  return (path);
}

void open_container(const char *username, const char *password)
{
  size_t needed = 0;
  char  *cmd = 0;
  char *path = get_encrypted_file_path(username);

  needed = snprintf(NULL, 0, "echo -e \"%s\" | cryptsetup luksOpen %s %s_secure_data",
                    password, path, username) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "echo -e \"%s\" | cryptsetup luksOpen %s %s_secure_data",
                    password, path, username);
  printf("open=%s\n", cmd);
  system(cmd);
  free(cmd);
  free(path);
}

void  mount_container(const char *username)
{
  char *cmd = 0;
  char  *path = get_folder_path(username);

  size_t needed = snprintf(NULL, 0, "mount /dev/mapper/%s_secure_data %s", username, path) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "mount /dev/mapper/%s_secure_data %s", username, path);
  system(cmd);
  free(cmd);
  free(path);
}

void create_container(const char *username)
{
  char *cmd = 0;
  char *path = get_encrypted_file_path(username);

  size_t needed = snprintf(NULL, 0, "%s%s", "dd if=/dev/zero bs=1M count=500 of=", path) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "%s%s", "dd if=/dev/zero bs=1M count=500 of=", path);
  system(cmd);
  free(cmd);
  free(path);
}

void  encrypt_container(const char *username, const char *password)
{
  char *cmd = 0;
  char *path = get_encrypted_file_path(username);

  size_t needed = snprintf(NULL, 0, "echo -e \"%s\" | cryptsetup luksFormat %s", password, path) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "echo -e \"%s\" | cryptsetup luksFormat %s", password, path);
  printf("format=%s\n", cmd);
  system(cmd);
  free(cmd);
  free(path);
}

void  format_container(const char *username)
{
  char *cmd = 0;

  size_t needed = snprintf(NULL, 0, "mkfs.ext4 /dev/mapper/%s_secure_data", username) + 1;
  cmd = malloc(needed);
  snprintf(cmd, needed, "mkfs.ext4 /dev/mapper/%s_secure_data", username);
  system(cmd);
  free(cmd);
}

int  create_folder(const char *username)
{
  char *path = get_folder_path(username);

  int result = mkdir(path, 0755);
  free(path);

  return (result);
}

void  own_folder(const char *username)
{
  char *path = get_folder_path(username);
  struct passwd *pwd = getpwnam(username);

  chown(path, pwd->pw_uid, pwd->pw_gid);
  free(path);
}
/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int retval = 0;
  const char* username = 0;
  const void *pass = 0;
  char  *e_path = 0;

  retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;

  if (strcmp(username, "root") != 0)
  {
    e_path = get_encrypted_file_path(username);
    pam_get_item(pamh, PAM_AUTHTOK, &pass);
    if (access(e_path, F_OK) != -1)
    {
      open_container(username, pass);
      // mount_container(username);
      // own_folder(username);
      // mount_container(username);
    }
    else
    {
      create_container(username);
      encrypt_container(username, pass);
      open_container(username, pass);
      format_container(username);
    }
    free(e_path);
  }
  return PAM_SUCCESS;
}

/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_IGNORE);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const void *pass;
  char *path;
  const char* username = 0;

  int retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;

  pam_get_item(pamh, PAM_AUTHTOK, &pass);
  if (flags == PAM_UPDATE_AUTHTOK)
  {
    printf("PASSWORD: %s\n", (char *)pass);
  }
  else
    return PAM_SUCCESS;

  umount_container(username);
  close_container(username);

  path = get_encrypted_file_path(username);
  remove(path);

  create_container(username);
  encrypt_container(username, pass);
  open_container(username, pass);
  format_container(username);

  free(path);
  return PAM_SUCCESS;
}
