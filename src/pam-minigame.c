#include <stdio.h>
#include <unistd.h>
#include <security/pam_modules.h>
#include <sys/wait.h>

int play_game(const char * path) {
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) { // child
        char * args [] = {NULL};
        if(execv(path, args) == -1) {
            fprintf(stderr, "Execution of %s failed\n", path);
            return -2;
        }
    }
    int game_status;
    wait(&game_status);
    return game_status;
}

int pam_sm_authenticate(pam_handle_t * pamh, int flags, int argc, const char **argv) {
    if (argc == 0) {
        fprintf(stderr, "Game was not set. Aborting\n");
        return PAM_AUTH_ERR;
    } else {
        const char * game_path = argv[0];
        int game_result = play_game(game_path);
        if (game_result) {
            fprintf(stderr, "Game returned %d. Aborting\n", game_result);
            return PAM_AUTH_ERR;
        } else {
            return PAM_SUCCESS;
        }
    }
}
