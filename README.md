libinjection
============

SQL / SQLI tokenizer parser analyzer For C.



## LICENSE

Copyright (c) 2012-2016 Nick Galbreath

Licensed under the standard [BSD 3-Clause](http://opensource.org/licenses/BSD-3-Clause) open source
license. See [COPYING](https://github.com/client9/libinjection/blob/master/COPYING) for details.



## Example

```c
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include "libinjection.h"
#include "libinjection_sqli.h"

int main(int argc, const char* argv[])
{
    struct libinjection_sqli_state state;
    int issqli;

    const char* input = argv[1];
    size_t slen = strlen(input);

    /* in real-world, you would url-decode the input, etc */

    libinjection_sqli_init(&state, input, slen, FLAG_NONE);
    issqli = libinjection_is_sqli(&state);
    if (issqli) {
        fprintf(stderr, "sqli detected with fingerprint of '%s'\n", state.fingerprint);
    }
    return issqli;
}
```

```
$ gcc -Wall -Wextra examples.c libinjection_sqli.c
$ ./a.out "-1' and 1=1 union/* foo */select load_file('/etc/passwd')--"
sqli detected with fingerprint of 's&1UE'
```



## QUALITY AND DIAGNOSITICS

The continuous integration results at
https://travis-ci.org/client9/libinjection tests the following:

- [x] build and unit-tests under GCC
- [x] build and unit-tests under Clang
- [x] static analysis using [clang static analyzer](http://clang-analyzer.llvm.org)
- [x] static analysis using [cppcheck](https://github.com/danmar/cppcheck)
- [x] checks for memory errors using [valgrind](http://valgrind.org/)
- [x] code coverage online using [coveralls.io](https://coveralls.io/github/client9/libinjection)


