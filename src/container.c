/**
 * @Author: Remi Gastaldi <gastal_r>
 * @Date:   2017-11-26T15:39:13+01:00
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-26T15:56:31+01:00
 */


#include  "pam.h"

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
