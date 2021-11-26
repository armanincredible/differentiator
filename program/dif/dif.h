#ifndef DIFHEADER
#define DIFHEADER

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 
#include "..\..\library\tree\tree.h"
#include "..\..\library\onegin\dora.h"
#include "..\..\library\stack\stack.h"
/*
enum Opeartors
{
    PLUS = '+',
    MINUS = '-',
    MULTIPLICATION = '*',
    DIVISION = '/',
    POWER = '^'
};

enum Type_Node
{
    CONST_VALUE,
    OPERATION
};
*/

Tree* dif_and_report (Tree* tree_node);

char* make_tree_from_library (Tree* tree_node);

char* copy_in_buffer_from_file (int* amount_str);

int solve_const_exp_tree (Tree* tree_node);

Tree* optim_exp_tree (Tree* tree_node);

Tree* take_dif_tree (Tree* tree_node);

Tree* copy_tree_node (Tree* tree_node);

int write_report (Tree* tree_node, FILE* teh_output);

int write_report_title (FILE* teh_output);

int write_report_conclusion (FILE* teh_output);


#endif
