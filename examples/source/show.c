////////////////////////////////////////////////////////////////////////////////
//                                   ooo
//                     Shows the way of code fromating.
// 
// - most of the time there is a onlie and a  multiline format solution
// - see the ooo formated version of source/show.c under styled_source/show.c
//
// - todo on binary expression ?
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                           struct specifier
////////////////////////////////////////////////////////////////////////////////

struct myStruct { char *chr;  unsigned int size; };

struct myStruct_1 { 
        char *chr;  
        unsigned int size; 
        unsigned int x;
        unsigned int y; unsigned int length;
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
        .length= 20, 
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
        switch(a) {
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
        switch(a) {
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
