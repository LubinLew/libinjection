#include <stdio.h>
#include <libinjection_sqli.h>

int main(int argc, const char* argv[])
{
    int          issqli;
    sqli_state_t state;
    const char  *input;

    if (argc < 2) {
        input = "-1' and 1=1 union/* foo */select load_file('/etc/passwd')--";
    }
    else {
        input = argv[1];
    }
    size_t slen = strlen(input);

    /* in real-world, you would url-decode the input, etc */
    libinjection_sqli_init(&state, input, slen, FLAG_NONE);
    issqli = libinjection_is_sqli(&state);
    if (issqli) {
        fprintf(stderr, "sqli detected with fingerprint of '%s'\n", state.fingerprint);
    }
    return issqli;
}
