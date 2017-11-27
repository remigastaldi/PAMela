/**
* @Author: Remi Gastaldi <gastal_r>
* @Date:   2017-11-23T17:38:39+01:00
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-27T16:47:29+01:00
*/

#include "pam.h"
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void) flags;
  (void) argc;
  (void) argv;
  int retval = 0;
  const void * pass = 0;
  const char* username = 0;
  char  *path = 0;
  char  *e_path = 0;

  retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;


  path = get_folder_path(username);
  if (strcmp(username, "root") != 0)
  {
    pam_get_data(pamh, "pam_module_pass", &pass);
    e_path = get_encrypted_file_path(username);
    if (access(e_path, F_OK) != -1)
    {
      open_container(username, pass);
    }
    else
    {
      create_container(username);
      encrypt_container(username, pass);
      open_container(username, pass);
      format_container(username);
    }
    free(e_path);
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
    free((char*)pass);
    free(path);
  }

  return(PAM_SUCCESS);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void) flags;
  (void) argc;
  (void) argv;

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
  (void) pamh;
  (void) flags;
  (void) argc;
  (void) argv;

  return(PAM_IGNORE);
}

void cleanup(pam_handle_t *pamh, void *data, int error_status)
{
  (void) pamh;
  (void) data;
  (void) error_status;
  free(data);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void) flags;
  (void) argc;
  (void) argv;

  int retval = 0;
  const char* username = 0;
  const void *pass = 0;

  retval = pam_get_user(pamh, &username, "Username: ");
  if (retval != PAM_SUCCESS)
    return retval;

  if (strcmp(username, "root") != 0)
  {
    pam_get_item(pamh, PAM_AUTHTOK, &pass);
    pam_set_data(pamh, "pam_module_pass", strdup(pass), &cleanup);
  }
  return PAM_SUCCESS;
}

/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void) pamh;
  (void) flags;
  (void) argc;
  (void) argv;

  return(PAM_IGNORE);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void) argc;
  (void) argv;

  int			ret;
  const char 	*username;
  const char 	*password;
  const char 	*oldPassword;
  char *cmd;


  if (flags != PAM_UPDATE_AUTHTOK)
  {
    return (PAM_SUCCESS);
  }
  if ((ret = pam_get_user(pamh, &username, "Username: ")) != PAM_SUCCESS)
      return (ret);
  else if ((ret = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&password)) != PAM_SUCCESS)
      return (ret);
  if (password)
  {
   	if ((ret = pam_get_item(pamh, PAM_OLDAUTHTOK, (const void **)&oldPassword)) != PAM_SUCCESS)
   	  return (ret);
    asprintf(&cmd, "printf \"%s\n%s\n%s\n\" | cryptsetup luksAddKey /home/pamela/secure_data-rw.encrypted &", oldPassword, password, password);
    printf("addkey=%s\n", cmd);
    system(cmd);
  }
 return PAM_SUCCESS;
}
