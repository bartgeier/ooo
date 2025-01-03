////////////////////////////////////////////////////////////////////////////////
//                                   ooo
//                     Shows the way of code fromating.
//
// - Mostly there is a single-line or multi-line style format
// - see the ooo formated version of source/show.c under styled_source/show.c
//
// - todo on binary expression ?
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                           struct specifier
////////////////////////////////////////////////////////////////////////////////

struct myStruct {
    char *chr;
    unsigned int size;
};

struct myStruct_1 {
    char *chr;
    unsigned int size;
    unsigned int x;
    unsigned int y;
    unsigned int length;
    unsigned int height;
};

////////////////////////////////////////////////////////////////////////////////
//                        struct init declaration
////////////////////////////////////////////////////////////////////////////////

struct myStruct s = {"Greetings from ooo!", 19};

struct myStruct_1 s_1 = {
    .chr = "Greetings from ooo!",
    .size = 19,
    .x = 0,
    .y = 0,
    .length = 20,
    .height = 1
};
struct myStruct_1 s_2 = {
    "Greetings from ooo!", 19,
    0, 0,
    20, 1
};

////////////////////////////////////////////////////////////////////////////////
//                           enum specifier
////////////////////////////////////////////////////////////////////////////////

enum myEnum { STOP, CLOCKWISE, COUNTERWISE };

enum myEnunm_1 {
    NEGATIV = -1,
    NEUTRAL = 0,
    POSITIV = 1
};

////////////////////////////////////////////////////////////////////////////////
//                         array init declaration
////////////////////////////////////////////////////////////////////////////////

int myArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char hello[] = {
    "██   ██ ███████ ██      ██       ██████  \n"
    "██   ██ ██      ██      ██      ██    ██ \n"
    "███████ █████   ██      ██      ██    ██ \n"
    "██   ██ ██      ██      ██      ██    ██ \n"
    "██   ██ ███████ ███████ ███████  ██████  \n"
};

////////////////////////////////////////////////////////////////////////////////
//                           binary expression
////////////////////////////////////////////////////////////////////////////////

void binary_expression(void) {
    int a = 1, b = 2, c = 3, d = 4, e = 5, x = 6, y = 7;
    x = b * c + d * e;

    y = b * c
    + d * e;
}

////////////////////////////////////////////////////////////////////////////////
//                     if statement without curly braces
////////////////////////////////////////////////////////////////////////////////

void if_statement(void) {
    int a = 1, b = 2, c = 3;
    if (a > b) c = a; else c = b;

    if (a > b)
        c = a;
    else
        c = b;
}

////////////////////////////////////////////////////////////////////////////////
//                      if statement with curly braces
////////////////////////////////////////////////////////////////////////////////

void if_statement_with_curly_braces(void) {
    int a = 1, b = 2, c = 3;
    if (a > b) { c = a; } else { c = b; }

    if (a > b) {
        c = a;
    } else {
        c = b;
    }
}

////////////////////////////////////////////////////////////////////////////////
//                         conditional expression
////////////////////////////////////////////////////////////////////////////////

void conditional_expression(void) {
    int a = 1, b = 2, c = 3;
    c = (a > b) ? a : b;

    c = (a > b)
        ? a : b;

    c = (a > b)
        ? a
        : b;
}

////////////////////////////////////////////////////////////////////////////////
//                            switch statement
////////////////////////////////////////////////////////////////////////////////

void switch_statement(void) {
    int a = 1, b = 2, c = 3, x;
    switch (a) {
        int y;
    case 0:
        x = a;
        break;
    case 1:
        x = b;
        break;
    case 2:
        x = c;
        break;
    case 3:
    case 4:
        x = 11;
        break;
    default:
        x = 0;
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//                 switch statement and curly braces
////////////////////////////////////////////////////////////////////////////////

void switch_statement_with_curly_brace(void) {
    int a = 1, b = 2, c = 3, x;
    switch (a) {
        int y;
    case 0: {
        int y = a;
        } break;
    case 1: {
        {
            x = b;
        }
        {
            x = b;
        }
        } break;
    case 2: {
        x = c;
        } break;
    default: {
        x = 0;
        break; }
    }
}

////////////////////////////////////////////////////////////////////////////////
//               comment indentation
//               c block comment inside a c++ comment
////////////////////////////////////////////////////////////////////////////////

void comment_indentation(void) {
    /* abcd
           abcd
    */
}

void comment_from_c_style_to_Cpp_style(void) {
    // nested /* comment */ Foo

    y = b * c
    * /* nested // comment // Bar */ d * e;
}

////////////////////////////////////////////////////////////////////////////////
//               macro
////////////////////////////////////////////////////////////////////////////////

#define BAR

#define X(name) name

#define NUMBER 345

#define FOO    \
    int a = 3; \
    b = c + a; \
    ;          \
    put(d)

#define FOO int a = 3; \
b = c + a;             \
;                      \
put(d)

////////////////////////////////////////////////////////////////////////////////
//               macro
//               expression
////////////////////////////////////////////////////////////////////////////////

#define ADD(a, b) ((a) + (b))

#define ADD(a, b) \
    ((a) + (b))

////////////////////////////////////////////////////////////////////////////////
//               macro
//               call expression
////////////////////////////////////////////////////////////////////////////////

#define ADD(a, b) add((a), (b))

#define ADD(a, b) \
    add((a), (b))

#define min(a, b) ((a) < (b) ? (a) : (b))

////////////////////////////////////////////////////////////////////////////////
//               macro
//               GCC macro statement expression
//               https://youtu.be/4DS5E5tgxIA?si=MhdEZI9ggf4GMHzZ
////////////////////////////////////////////////////////////////////////////////

#define find_max(array, length) ({           \
    typeof(array[0]) current_max = array[0]; \
    for (int i = 1; i < length; i++)         \
        if (array[i] > current_max)          \
            current_max = array[i];          \
    current_max;                             \
})

#define find_max(array, length)                  \
    ({                                           \
        typeof(array[0]) current_max = array[0]; \
        for (int i = 1; i < length; i++)         \
            if (array[i] > current_max)          \
                current_max = array[i];          \
        current_max;                             \
    })

////////////////////////////////////////////////////////////////////////////////
//               macro statement
//               do { ... } while(0)
////////////////////////////////////////////////////////////////////////////////

#define MORE(a, b, c) do { (c) = (a) + (b); } while (0)

#define MORE(a, b, c) do { \
    (c) = (a) + (b);       \
} while (0)

#define MORE(a, b, c)    \
    do {                 \
        (c) = (a) + (b); \
    } while (0)

#define MORE(a, b, c)    \
    do {                 \
        (c) = (a) + (b); \
    } while (0)


////////////////////////////////////////////////////////////////////////////////
//                           x macro
////////////////////////////////////////////////////////////////////////////////

#define LIST_OF_THINGS \
    X(TABLE)           \
    X(DRAWER)          \
    X(CAR)             \
    X(BOOK)            \
    X(TRAIN)

#define X(name) name,
typedef enum {
    LIST_OF_THINGS
} Things;

#undef X
#define X(name) [name]   =    #name  ,
char *thing_strings[] = {
    LIST_OF_THINGS
};

////////////////////////////////////////////////////////////////////////////////
//             macro template function
//             The Tc function... interface does not format
//             but it displays what the author has written.
////////////////////////////////////////////////////////////////////////////////

#define template_add(Ta, Tb, Tc)     \
    Tc add_##Ta##_##Tb(Ta a, Tb b) { \
        if ((Tc)a == (Tc)b) {        \
            return (Tc)a;            \
        }                            \
        return (Tc)(a + b);          \
    }
