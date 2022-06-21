#include <stdio.h>
#include <security/pam_appl.h>

static struct pam_conv conv = {
    .conv=NULL,
    .appdata_ptr=NULL,
};

int main() {
    printf("PAM test app started\n");

    pam_handle_t * pamh = NULL;
    int retval = 0;
    retval = pam_start("minigame", NULL, &conv, &pamh);
    if (retval != PAM_SUCCESS) {
        printf("PAM start error code %d\n", retval);
        printf("%s\n", pam_strerror(pamh, retval));
        return 1;
    }

    retval = pam_authenticate(pamh, 0);
    if (retval == PAM_SUCCESS) {
        printf("Auth successful\n");
    } else {
        printf("Auth failed code %d\n", retval);
        printf("%s\n", pam_strerror(pamh, retval));
        return 2;
    }

    retval = pam_end(pamh, 0);
    if (retval != PAM_SUCCESS) {
        printf("PAM end failed code %d", retval);
        printf("%s\n", pam_strerror(pamh, retval));
        return 3;
    }

    return 0;
}
