libinjection
============

SQL / SQLI tokenizer parser analyzer For C.

## Example

```c
#include <stdio.h>
#include <strings.h>
#include <errno.h>

#include <libinjection.h>
#include <libinjection_sqli.h>

int main(int argc, const char* argv[])
{
    int          issqli;
    sqli_state_t state;

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

## LICENSE

Copyright (c) 2012-2016 Nick Galbreath ([BSD 3-Clause](http://opensource.org/licenses/BSD-3-Clause))



## Reference

[SQL注入与libinjection分析](https://blog.csdn.net/lqy971966/article/details/106857168/)

[SQL简单语义分析概述](https://zhuanlan.zhihu.com/p/44292411)

[SQL语义分析指纹识别](https://zhuanlan.zhihu.com/p/44537204)

[如何绕过WAF/NGWAF的libinjection实现SQL注入](https://www.anquanke.com/post/id/86097)

[Github libinjection库研究总结 - 码农教程](http://www.manongjc.com/detail/15-njsryzzujhnneor.html)


