#ifndef INDENTATION_H
#define INDENTATION_H

#include "tree_sitter/api.h"
#include "OStr.h"
#include "ooo_job.h"

//void Indentation_remove(OStr *B, OStr *A);
void indentation_remove_runner( TSNode node, OOO_Job *job); 




#endif
