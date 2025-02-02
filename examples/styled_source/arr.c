#include < stdio.h >
#include " stdio.h "

static int x = 0;

enum tag {
    A = 0,
    B,
    C,
    D,
    E
};

enum tag_b { A = 0, B, C, D, E };

typedef struct {
    int a;
    int b;
    int c;
} A_t;

typedef struct {
    int a;
    int b;
    int c;
} A_t;

A_t f = {1, 2, 3};
\
A_t f = {
    .a = 0, .b = 1,
    .c = 5
};

struct {
    int a;
    int b;
    int c;
} A_t f = {1, 2, 3};

int numberes[] = {1, 2, 3, 4, 5, 6, 7};
int numberes[] = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
};
int numberes[] = {
    [a] = 1, [b] = 2,
    [c] = 2
};
int numberes[] = {[a] = 1, [b] = 2, [c] = 2};

int a = 5; // a equal five

int a, b, c;

// comment
void foo(int a, int b) { x = a + b; }
// comment
int bar(
    int a,
    int b, int c,
    int d
) {
    return a * b
    + /* comment */ c * d;
}

int main(int argc, char **argv) {
    // declaration, init_declarator
    int a = 11;
    int const b = 13;

    int a = a + b;

    int x = a > b ? 5 : 0;
    int x = a > b ? 5 : 0;
    int x = a > b ? 5
        : 0;
    int x = a > b ? 5 : 0;
    int x = a > b ? : ;

    int x = a > b
        ? 5
        : 0;
    int x = a > b
        ? 5 : 0;
    int x = a > b
        ? 5
        : 0;
    alice++;
    ++alice;
    bob--;
    --bob;

    /*  block comment
    *  indentation
    is ... */

    /*  block comment
     *  indentation
        is ... */

    (void)x;   /*  block comment
    *  indentation
    is ... */

    /* comment */
    // comment
    int const **c = &a;
    int *const *d = &a;
    const char chr[] = "Hello world!";
    int num[3] = {0};

    foo(a, b);
    x = bar(
        a,
        b, c
        d
    );
    {
        int A = 0;
        bar(a, b, c);
    }
    {
        int A = 0;
        bar(a, b, c);
    }
    { int A = 5; }
    int s = sizeof(A);
    #define FOO 5 == n

    /* comment */
    (void)a;
    char a = (char)A;
    char const *a = (char const *)A;
    const char *a = (const char *)A;
    char const **a = (char const **)A;
    const char **a = (const char **)A;
    #ifdef _WIN32
    /* comment */

    #else

    /* comment */

    #endif

    #if
    /* comment */ 0
    (void)a;
    char a = (char)A;
    char const *a = (char const *)A;
    const char *a = (const char *)A;
    char const **a = (char const **)A;
    const char **a = (const char **)A;

    #else
    (void)a;
    char a = (char)A;
    char const *a = (char const *)A;
    const char *a = (const char *)A;
    char const **a = (char const **)A;
    const char **a = (const char **)A;

    #endif
    if (a == b) { return true; }
    if (a == b) {
        return true;
    }
    if (a == b) r = 8;
    if (a | b)
        return result;
    else
        return 0;
    if (a | b) {
        return result;
    } else {
        return 0;
    }

    if (a == 0
    | b == 4 & c == 5
    | f == 1) {
        printf("give me five");
    }

    if (A) {
        foo();
    } else if (B) {
        bar();
    } else {
        normal();
    }

    if (A) {
        foo();
    } else
        /* comment */ if (B) {
        bar();
    } else {
        normal();
    }

    if (A) {
        foo();
    } else if (B) { bar(); } else { normal(); }

    if (A) { foo(); } else if (B) {
        bar();
    } else {
        normal();
    }

    for (size_t i = 0; i < max; i++) { printf("i => %d\n", i); }
    for (
        size_t i = 0;
        i < max;
        i++
    ) { printf("i => %d\n", i); }
    for (
        size_t i = 0;
        i < max;
        i++
    ) {
        printf("i => %d\n", i);
    }
    for (size_t i = 0; i < max; i++)
        printf("i => %d\n", i);
    for (size_t i = 0; i < max; i++) printf("i => %d\n", i);

    for (size_t i = 0; i < max; i++)
        if (a == b)
            printf("i => %d\n", i);

    do { a++; } while (a < 10);
    do {
        a++;
    } while (a < 10);

    while (a < 10) { a++; }
    while (a < 10) {
        a++;
    }
    while (a < 10)
        a++;
    while (a < 10) a++;

    switch (state) {
        int a;
    case a:
        break;
    case c: {
        break; }

    case d: {
        for (size_t i = 0; i < max; i++) {
            a++;
        }
        {
            a++;
        }
        } break;
    case e: // comment
        {
        }
        break;
    default:
        break;
    }
    switch (state) {
        int a;
    case a:
        return;
    case c:
        return;
    case d: {
        for (size_t i = 0; i < max; i++) {
            a++;
        }
        {
            a++;
        }
        } return a + b;
    default:
        return;
    }
}
