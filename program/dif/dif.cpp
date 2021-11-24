#include "dif.h"

#define CHECK_ERROR_(name, type_error, ret)                                             \
        do{                                                                             \
            if ((name) == NULL)                                                         \
            {                                                                           \
                printf ("\nERROR in function : %s \n"                                   \
                        "%s have %s on line %d\n", __func__,                            \
                        #name, type_error, __LINE__);                                   \
                return ret;                                                             \
            }                                                                           \
        } while (0)

#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

static int is_eq (double value_fr, double value_sec);

static int make_tree_recursion (Tree* tree_node, char* buffer, int* ammount, const int ammount_symb);

static int solve_const_exp_recurse (Tree* tree_node);

static int optim_exp_tree_recurse (Tree* tree_node);

static int set_value (Tree* tree_node, char* buffer, int* ammount);

static int write_report_recurse (Tree* tree_node, FILE* teh_output);

static int print_node (Tree* tree_node, FILE* teh_output);

/*static Tree* take_dif_tree_recurse (Tree* tree_node);*/

static int low_priority (const node_t value);

static Tree* dif_tree_node (Tree* tree_node);

static Tree* copy_tree_node_recurse (Tree* new_tree_node, Tree* tree_node);

static Tree* New_Node (bool it_const, const node_t value, Tree* left_ptr, Tree* right_ptr);

static int const_opt_remake_node (Tree* tree_node, const node_t var);

static int exp_opt_remake_node (Tree** tree_node, Tree* tree_side);

char* make_tree_from_library (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "NULL ADRESS", NULL);

    int ammount_symb = 0; 

    char* buffer = copy_in_buffer_from_file (&ammount_symb); 

    int ammount = 0;

    double value = buffer[ammount];

    /*for (int i = 0; i < ammount_symb; i++)
    {printf ("%c", buffer[i]);}
    printf ("\n");*/

    tree_push(tree_node, IN, 0, CONST);
    ammount = 1;

    make_tree_recursion (tree_node, buffer, &ammount, ammount_symb);

    return buffer;
}

char* copy_in_buffer_from_file (int* amount_symb)
{
    CHECK_ERROR_(amount_symb, "NULL_ADRESS", NULL);

    char* buffer = NULL; 
   
    FILE* input_file = fopen ("../../input/math_expression.txt", "r");
    CHECK_ERROR_(input_file, "NOT OPENED", NULL);

    buffer = remove_trash_and_copy_in_buffer (amount_symb, input_file);

    if (ferror(input_file))
    {
        printf ("ERROR in function : %s \n"
                "reading file falled\n", __func__);
        return NULL;
    }
    fclose (input_file);

    return buffer;
}

static int make_tree_recursion (Tree* tree_node, char* buffer, int* ammount, const int ammount_symb)
{
    bool was_left = 0;
    bool was_right = 0;

    for (;*ammount < ammount_symb;)
    {
        if ((buffer[*ammount] == '(') && (was_left != 1))
        {
            *ammount = *ammount + 1;
            tree_push(tree_node, LEFT, 0, CONST);
            make_tree_recursion (tree_node->left_ptr, buffer, ammount, ammount_symb);
            was_left = 1;
        }

        if ((buffer[*ammount] == '(') && (was_right != 1))
        {
            *ammount = *ammount + 1;
            tree_push(tree_node, RIGHT, 0, CONST);
            make_tree_recursion (tree_node->right_ptr, buffer, ammount, ammount_symb);
            was_right = 1;
        }
        
        if (buffer[*ammount] == ')')
        {
            *ammount = *ammount + 1;
            return 0;
        }

        if ((buffer[*ammount] == ')') || (buffer[*ammount] == '('))
        {
            printf ("INVALID EXPRESSION\n");
            return 0;
        }

        set_value (tree_node, buffer, ammount);

        if ((buffer[*ammount] != ')') && (buffer[*ammount] != '('))
        {
            printf ("INVALID EXPRESSION\n");
            return -1;
        }
    }
    return 0;
}

static int set_value (Tree* tree_node, char* buffer, int* ammount)
{
    double value = 0;

    value = buffer[*ammount];

    bool it_const = 0;

    if (isdigit (buffer[*ammount]))
    {
        value = strtod (buffer + *ammount, NULL);
        it_const = 1;
    }
    
    tree_push (tree_node, IN, value, it_const);

    *ammount = *ammount + 1;
    while (isdigit (buffer[*ammount]))
    {
        *ammount = *ammount + 1;
    }

    return 0;
}

Tree* take_dif_tree (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "HAVE ZERO ADRESS", NULL);
    
    Tree* dif_node = dif_tree_node (tree_node);
    tree_dtor(&tree_node);

    return dif_node;
}


#define D_(node) dif_tree_node (node)

#define DL_ dif_tree_node (tree_node->left_ptr)

#define DR_ dif_tree_node (tree_node->right_ptr)

#define CL_ copy_tree_node (tree_node->left_ptr)

#define CR_ copy_tree_node (tree_node->right_ptr)

#define NEW_COS_(left_node) New_Node (NO_CONST, COS, left_node, NULL)

#define NEW_SIN_(left_node) New_Node (NO_CONST, SIN, left_node, NULL)

#define NEW_CONST_(value) New_Node (CONST, value, NULL, NULL)

#define NEW_POW_(left_node, right_node) New_Node (NO_CONST, POWER, left_node, right_node)

#define NEW_ADD_(left_node, right_node) New_Node (NO_CONST, PLUS, left_node, right_node)

#define NEW_SUB_(left_node, right_node) New_Node (NO_CONST, MINUS, left_node, right_node)

#define NEW_DIV_(left_node, right_node) New_Node (NO_CONST, DIVISION, left_node, right_node)

#define NEW_MUL_(left_node, right_node) New_Node (NO_CONST, MULTIPLICATION, left_node, right_node)

static Tree* dif_tree_node (Tree* tree_node)
{   
    if (tree_node->value.type == CONST_VALUE)
    {
        return NEW_CONST_(0);
    }
    if (tree_node->value.type == VAR_X)
    {
        return NEW_CONST_(1);
    }
    switch (int (tree_node->value.data))
    {
        case PLUS:
        {
            return NEW_ADD_(DL_, DR_);
        }
        case MULTIPLICATION:
        {
            return NEW_ADD_(NEW_MUL_(DL_, CR_), NEW_MUL_(CL_, DR_));
        }
        case DIVISION :
        {
            return NEW_DIV_(NEW_SUB_(NEW_MUL_(DL_, CR_), NEW_MUL_(CL_, DR_)), NEW_POW_(CR_, NEW_CONST_(2)));
        }
        case POWER :
        {
            return NEW_MUL_(NEW_MUL_(CR_, NEW_POW_( CL_, NEW_SUB_(CR_, NEW_CONST_(1)) )), DL_);
        }
        case SIN :
        {
            return NEW_MUL_(NEW_COS_(CL_), DL_);
        }
        case COS :
        {
            return NEW_MUL_(NEW_MUL_(NEW_SIN_(CL_), NEW_CONST_(-1)), DL_);
        }
        default:
        {
            return 0;
        }
    }
    return 0;
}

#undef NEW_CONST_

#undef NEW_ADD_
#undef NEW_SUB_
#undef NEW_MUL_
#undef NEW_POW_
#undef NEW_DIV_

#undef NEW_COS_
#undef NEW_SIN_

#undef CL_
#undef CR_

#undef D_L
#undef D_R
#undef D_

Tree* copy_tree_node (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "HAVE NULL ADRESS", NULL);

    Tree* new_tree_node = NULL;
    tree_ctor (&new_tree_node);

    new_tree_node->pre_ptr = tree_node->pre_ptr;
    new_tree_node->level = tree_node->level;

    if (tree_node->value.type == CONST_VALUE)
    {
        tree_push (new_tree_node, IN, tree_node->value.data, CONST);
    }
    else
    {
        tree_push (new_tree_node, IN, tree_node->value.data, NO_CONST);
    }

    copy_tree_node_recurse (new_tree_node, tree_node);

    return new_tree_node;
}

static Tree* copy_tree_node_recurse (Tree* new_tree_node, Tree* tree_node)
{
    if (tree_node->left_ptr != NULL)
    {
        if (tree_node->left_ptr->value.type == CONST_VALUE)
        {
            tree_push(new_tree_node, LEFT, tree_node->left_ptr->value.data, CONST);
        }
        else
        {
            tree_push(new_tree_node, LEFT, tree_node->left_ptr->value.data, NO_CONST);
        }

        copy_tree_node_recurse (new_tree_node->left_ptr, tree_node->left_ptr);
    }

    if (tree_node->right_ptr != NULL)
    {
        if (tree_node->right_ptr->value.type == CONST_VALUE)
        {
            tree_push(new_tree_node, RIGHT, tree_node->right_ptr->value.data, CONST);
        }
        else
        {
            tree_push(new_tree_node, RIGHT, tree_node->right_ptr->value.data, NO_CONST);
        }

        copy_tree_node_recurse (new_tree_node->right_ptr, tree_node->right_ptr);
    }

    return 0;
}

static Tree* New_Node (bool it_const, const node_t value, Tree* left_ptr, Tree* right_ptr)
{
    Tree* tree_node = NULL;
    tree_ctor (&tree_node);

    tree_push (tree_node, IN, value, it_const);

    Tree* new_left_ptr = NULL;
    Tree* new_right_ptr = NULL; 

    if (left_ptr != NULL)
    {
        new_left_ptr = copy_tree_node (left_ptr);
        new_left_ptr->pre_ptr = tree_node;

        tree_node->level = new_left_ptr->level - 1;
    }
    if (right_ptr != NULL)
    {
        new_right_ptr = copy_tree_node (right_ptr); 
        new_right_ptr->pre_ptr = tree_node;

        tree_node->level = new_right_ptr->level - 1;
    }

    //new_left_ptr->pre_ptr = tree_node;
    tree_node->left_ptr  =  new_left_ptr;
    tree_node->right_ptr =  new_right_ptr;
    
    return tree_node;
}


int solve_const_exp_tree (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "NULL_ADRESS", NULL);

    solve_const_exp_recurse (tree_node);

    return 0;
}

static int solve_const_exp_recurse (Tree* tree_node)
{
    if (tree_node == NULL)
    {
        return 0;
    }

    node_t type_node_val = tree_node->value.data;
    node_t type_node = tree_node->value.type;

    if (type_node == OPERATION)
    {
        solve_const_exp_recurse (tree_node->left_ptr);
        solve_const_exp_recurse (tree_node->right_ptr);

        node_t imm = 0;

        Tree* left_node = tree_node->left_ptr;
        Tree* right_node = tree_node->right_ptr;

        if ((tree_node->left_ptr != NULL)                           && 
            (tree_node->right_ptr != NULL)                          && 
            (tree_node->left_ptr->value.type == CONST_VALUE)        && 
            (tree_node->right_ptr->value.type == CONST_VALUE))
        {
            node_t left_node_val = left_node->value.data;
            node_t right_node_val = right_node->value.data;

            switch (int(type_node_val))
            {
                case PLUS:
                {
                    imm = left_node_val + right_node_val;
                    break;
                }
                case MINUS:
                {
                    imm = left_node_val - right_node_val;
                    break;
                }
                case MULTIPLICATION:
                {
                    imm = left_node_val * right_node_val;
                    break;
                }
                case DIVISION:
                {
                    imm = left_node_val / right_node_val;
                    break;
                }
                case POWER:
                {
                    imm = pow (left_node_val, right_node_val);
                }
                default:
                {
                    break;
                }
            }

            const_opt_remake_node (tree_node, imm);

            return 0;
        }
    }

    return 0;
}

int optim_exp_tree (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "NULL ADRESS", -1);

    optim_exp_tree_recurse (tree_node);

    return 0;
}


static int is_eq (double value_fr, double value_sec)
{
    if (fabs(value_fr - value_sec) <= EPS)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int optim_exp_tree_recurse (Tree* tree_node)
{
    if (tree_node->left_ptr != NULL)
    {
        optim_exp_tree_recurse (tree_node->left_ptr);
    }
    if (tree_node->right_ptr != NULL)
    {
        optim_exp_tree_recurse (tree_node->right_ptr);
    }

    if (tree_node->pre_ptr == NULL)
    {
        return 0;
    }
    Tree* pre_ptr = tree_node->pre_ptr;

    if (pre_ptr->value.type == OPERATION)
    {
        Tree* right_pre_ptr = pre_ptr->right_ptr;
        Tree* left_pre_ptr = pre_ptr->left_ptr;
        if ((right_pre_ptr == NULL) || (left_pre_ptr == NULL))
        {
            return 0;
        }

        node_t right_pre_val = right_pre_ptr->value.data;
        node_t left_pre_val = left_pre_ptr->value.data;

        switch ((int)pre_ptr->value.data)
        {
            case MULTIPLICATION:
            {
                if ((is_eq (left_pre_val, 0)) || (is_eq (right_pre_val, 0)))
                {
                    const_opt_remake_node (pre_ptr, 0);
                }
                if (is_eq (left_pre_val, 1))
                {
                    exp_opt_remake_node (&pre_ptr, right_pre_ptr);
                }
                if (is_eq (right_pre_val, 1))
                {
                    exp_opt_remake_node (&pre_ptr, left_pre_ptr);
                }
                break;
            }
            case MINUS:
            {
                if (is_eq (left_pre_val, 0))
                {
                    exp_opt_remake_node (&pre_ptr, right_pre_ptr);
                }
                if (is_eq (right_pre_val, 0))
                {
                    exp_opt_remake_node (&pre_ptr, left_pre_ptr);
                }
                break;
            }
            case PLUS:
            {
                if (is_eq (left_pre_val, 0))
                {
                    exp_opt_remake_node (&pre_ptr, right_pre_ptr);
                }
                if (is_eq (right_pre_val, 0))
                {
                    exp_opt_remake_node (&pre_ptr, left_pre_ptr);
                }
                break;
            }
            case DIVISION:
            {
                if (is_eq (right_pre_val, 0))
                {
                    printf ("MAN U ARE IDIOT ( U ARE DIVISION ON NULL");
                    return -1;
                }
                if (is_eq (right_pre_val, 1))
                {
                    exp_opt_remake_node (&pre_ptr, left_pre_ptr);
                }
                break;
            }
            case POWER:
            {
                if (is_eq (right_pre_val, 0))
                {
                    const_opt_remake_node (pre_ptr, 1);
                }
                if (is_eq (right_pre_val, 1))
                {
                    exp_opt_remake_node (&pre_ptr, left_pre_ptr);
                }
                if (is_eq (left_pre_val, 1))
                {
                    const_opt_remake_node (pre_ptr, 1);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return 0;
}

static int const_opt_remake_node (Tree* tree_node, const node_t var)
{
    tree_push (tree_node, IN, var, CONST);
    
    if (tree_node->left_ptr != NULL)
    {
        tree_dtor (&(tree_node->left_ptr));
    }
    if (tree_node->right_ptr != NULL)
    {
        tree_dtor (&(tree_node->right_ptr));
    }
    
    tree_node->left_ptr = NULL;
    tree_node->right_ptr = NULL;

    return 0;
}

static int exp_opt_remake_node (Tree** tree_node, Tree* tree_side)
{
    Tree* save_tree_node_ptr = *tree_node;
    Tree* save_pre_ptr = (*tree_node)->pre_ptr;
    Tree * new_tree_node = copy_tree_node (tree_side);
    
    new_tree_node->pre_ptr = save_pre_ptr;

    if ((*tree_node)->pre_ptr->left_ptr == *tree_node)
    {
        (*tree_node)->pre_ptr->left_ptr = new_tree_node;
    }
    if ((*tree_node)->pre_ptr->right_ptr == *tree_node)
    {
        (*tree_node)->pre_ptr->right_ptr = new_tree_node;
    }

    *tree_node = new_tree_node;

    if (save_tree_node_ptr->left_ptr != NULL)
    {
        tree_dtor (&(save_tree_node_ptr->left_ptr));
    }
    if (save_tree_node_ptr->right_ptr != NULL)
    {
        tree_dtor (&(save_tree_node_ptr->right_ptr));
    }

    return 0;
}

static int low_priority (const node_t value)
{
    if ((is_eq (value, PLUS)) || (is_eq(value, MINUS)))
    {
        return 1;
    }
    return 0;
}

int write_report (Tree* tree_node, FILE* teh_output)
{
    if (tree_verificate (tree_node) == -1)
    {
        return -1;
    }

    CHECK_ERROR_(teh_output, "NULL ADRESS", -1);

    write_report_recurse (tree_node, teh_output);

    return 0;
}

static int write_report_recurse (Tree* tree_node, FILE* teh_output)
{
    if (tree_node->left_ptr != NULL)
    {
        write_report_recurse (tree_node->left_ptr, teh_output);
    }
    if (tree_node->right_ptr != NULL)
    {
        write_report_recurse (tree_node->right_ptr, teh_output);
    }

    if (tree_node->value.type == OPERATION)
    {
        if (low_priority(tree_node->value.data))
        {
            fprintf (teh_output, "(");

            print_node (tree_node, teh_output);

            fprintf (teh_output, ")");
        }
        else
        {
            print_node (tree_node, teh_output);
        }
    }
    else
    {
        return 0;
    }
    
    return 0;
}

static int print_node (Tree* tree_node, FILE* teh_output)
{
    if (tree_node->left_ptr != NULL)
    {
        int left_type = tree_node->left_ptr->value.type;

        if (left_type == VAR_X)
        {
            fprintf (teh_output, "%c", (char) tree_node->left_ptr->value.data);
        }
        else if (left_type == CONST_VALUE)
        {
            fprintf (teh_output, "%d", tree_node->left_ptr->value.data);
        }
    }

    fprintf (teh_output, "%c", (char) tree_node->value.data);

    if (tree_node->right_ptr != NULL)
    {
        int right_type = tree_node->right_ptr->value.type;

        if (right_type == VAR_X)
        {
            fprintf (teh_output, "%c", (char) tree_node->right_ptr->value.data);
        }
        else if (right_type == CONST_VALUE)
        {
            fprintf (teh_output, "%d", tree_node->right_ptr->value.data);
        }
    }

    return 0;
}

int write_report_title (FILE* teh_output)
{
    CHECK_ERROR_(teh_output, "NULL ADRESS", -1);

    fprintf (teh_output, "\\documentclass[a4paper, 12pt]{article}\n"
                         "\\usepackage[left=1cm,right=1cm,top=1cm,bottom=3cm,bindingoffset=0cm]{geometry}\n"
                         "\\usepackage[T2A]{fontenc}\n"
                         "\\usepackage[utf8]{inputenc}\n"
                         "\\usepackage[english,russian]{babel}\n"
                         "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n"
                         "\\begin{document}\n"
                         "\\maketitle\n"
                         "\\textbf\n");

    return 0;

}

int write_report_conclusion (FILE* teh_output)
{
    return 0;
}



#undef CHECK_ERROR_

#undef s