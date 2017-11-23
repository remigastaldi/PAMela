/**
* @Author: Remi Gastaldi <gastal_r>
* @Date:   2017-11-23T17:38:39+01:00
 * @Last modified by:   prost_m
 * @Last modified time: 2017-11-24T00:07:22+01:00
*/

#include "pam.h"

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  const void *pass;
  char *cmd;
  int retval;
  const char* pUsername;

  retval = pam_get_user(pamh, &pUsername, "Username: ");
  if (strcmp(pUsername, "root") != 0)
  {
    pam_get_item(pamh, PAM_AUTHTOK, &pass);
    cmd = strdup("echo ");
    cmd = my_strcat(cmd, (char *)pass);
    cmd = my_strcat(cmd, " | cryptsetup luksFormat -c aes -h sha256 ");
    cmd = my_strcat(cmd, "/home/");
    cmd = my_strcat(cmd, pUsername);
    cmd = my_strcat(cmd, "/secure_data-rw");
    system(cmd);
  }

  if (retval != PAM_SUCCESS) {
    return retval;
  }

  return(PAM_SUCCESS);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int retval;
  char *dir = NULL;
  char *mount = NULL;
  const char* pUsername;

  retval = pam_get_user(pamh, &pUsername, "Username: ");
  printf("Welcome %s\n", pUsername);
  if (strcmp(pUsername, "root") != 0)
  {
    dir = my_strcat(strdup("/home/"), pUsername);
    dir = my_strcat(dir, "/secure_data-rw");
    mount = my_strcat(strdup("mount -t ext4 "), dir);
    if (open(dir, O_DIRECTORY) == -1)
    {
      mkdir(dir, 0755);
      system(mount);
    }
    else
      system(mount);
    free(dir);
  }

  if (retval != PAM_SUCCESS) {
    return retval;
  }

  return PAM_SUCCESS;
}

/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  const void *pass;

  pam_get_item(pamh, PAM_AUTHTOK, &pass);
  if (flags == PAM_UPDATE_AUTHTOK)
    printf("PASSWORD: %s\n", (char *)pass);
  return PAM_SUCCESS;
}
