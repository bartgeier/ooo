#define NOB_IMPLEMENTATION
#include "../nob.h"

#ifdef AMALGAMATE_TREE_SITTER_API_H
#define TREE_SITTER_API_IMPLEMENTATION
#include "tree_sitter_api.h"
#endif

//#define ARQ_IMPLEMENTATION
//#include "arq.h"

#define OSTR_IMPLEMENTATION
#include "OStr.h"
#undef OSTR_IMPLEMENTATION

#define OJOB_IMPLEMENTATION
#include "OJob.h"

#define REGEX_TRUNC_NODE_SPACE_IMPLEMENTATION
#include "Regex_truncNodeSpace.h"

#define REGEX_LINE_CONT_IMPLEMENTATION
#include "Regex_lineCont.h"

#define REGEX_TRUNC_COMMENT_SPACE_IMPLEMENTATION
#include "Regex_truncCommentSpace.h"

#define REGEX_SIGNED_COMMENT_IMPLEMENTATION
#include "Regex_signedComment.h"

#define REGEX_LINE_FEED_IMPLEMENTATION
#include "Regex_lineFeed.h"

#define REGEX_TAB_FILTER_IMPLEMENTATION
#include "Regex_tabFilter.h"

#define REGEX_LINE_UP_IMPLEMENTATION
#include "Regex_lineUp.h"

#define OARENA_IMPLEMENTATION
#include "OArena.h"

