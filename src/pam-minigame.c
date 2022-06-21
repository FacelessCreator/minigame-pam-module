#include <stdio.h>
#include <security/pam_modules.h>

int pam_sm_authenticate(pam_handle_t * pamh, int flags, int argc, const char **argv) {
    printf("PAM minigame module is alife\n");
    return PAM_SUCCESS;
}
