#ifndef TREEHEADER
#define TREEHEADER

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 

typedef double node_t;

const double EPS = 10e-6;

enum Opeartors
{
    PLUS = '+',
    MINUS = '-',
    MULTIPLICATION = '*',
    DIVISION = '/',
    POWER = '^',
    VARIABLE = 'x',
    SIN = 's',
    COS = 'c'
};

enum Const_true
{
    NO_CONST,
    CONST
};

enum Type_Node
{
    CONST_VALUE,
    OPERATION,
    FUNCT,
    VAR_X
};

struct Arg_node
{
    node_t data;
    int type;
};

struct Tree
{
    Arg_node value;
    Tree* left_ptr;
    Tree* right_ptr;
    Tree* pre_ptr;
    int level;
};

enum Orientation
{
    LEFT,
    RIGHT,
    IN
};

const int NAME_LENGTH = 16;

int tree_verificate (Tree* tree_node);

int tree_ctor (Tree** tree_node);

int tree_dtor (Tree** tree_node);

int tree_push (Tree* tree_node, int side, node_t name, const bool const_status);

int tree_dump_visual (Tree* tree_node);

//int tree_dump_text (Tree* tree_node);
/*
int tree_deletr (Tree* )
*/
//int tree_traverse ()


#endif