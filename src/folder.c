/**
 * @Author: Remi Gastaldi <gastal_r>
 * @Date:   2017-11-26T15:57:47+01:00
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-26T15:58:14+01:00
 */


#include    "pam.h"


char  *get_folder_path(const char *username)
{
  char  *path = 0;

  size_t needed = snprintf(NULL, 0, "/home/%s/secure_data-rw", username) + 1;
  path = malloc(needed);
  snprintf(path, needed, "/home/%s/secure_data-rw", username);

  return (path);
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
