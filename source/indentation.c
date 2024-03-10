#include "indentation.h"
#include "ooo_job.h"
#include "ooo_treesitter_symbol_ids.h"
#include "ooo_runner.h"

void Indentation_remove(OOO_Job *job, size_t i_column, size_t start_idx, size_t end_idx) {
        OStr *A = &job->source;
        OStr *B = &job->sink;

        bool indentation = false;
        size_t column = i_column;
        size_t x = job->sink.size;
        for (size_t i = start_idx; i < end_idx; i++) {
                if (A->at[i] == ' ' & column == 0) {
                       indentation = true; 
                } 
                if ((A->at[i] != ' ') | !indentation) {
                        indentation = false; 
                        B->at[x++] = A->at[i];
                }
                column = (A->at[i] == '\n') ? 0 :  column + 1;
        }
        B->at[x] = 0;
        B->size = x;
}

void indentation_remove_runner(
        TSNode node,
        OOO_Job *job
) {
        TSSymbol me = ooo(node);
        size_t last_end_idx = job->cursor.idx;
        size_t column = job->cursor.column;
        size_t start_idx = OStrCursor_move_to_point(
                        &job->cursor, 
                        &job->source, 
                        ts_node_start_point(node)
        );
        Indentation_remove(job, column, last_end_idx, start_idx);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                indentation_remove_runner(child, job);
        }
        last_end_idx = job->cursor.idx;
        size_t end_idx = OStrCursor_move_to_point(
                        &job->cursor, 
                        &job->source, 
                        ts_node_end_point(node)
        );
        for (size_t i = last_end_idx; i < end_idx; i++) {
                OStr_append_chr(&job->sink, job->source.at[i]);
        }
}
