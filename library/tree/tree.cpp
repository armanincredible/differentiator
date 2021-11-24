#include "tree.h"

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

#define _TREE_VERIFICATE_(name)                         \
        do{                                             \
            if (tree_verificate (name) == -1)           \
            {                                           \
                return -1;                              \
            }                                           \
        }while(0)

static int write_tree_element_vis (Tree* tree_node, FILE* output_file);

static int write_tree_element_text (Tree* tree_node, FILE* output_file);

static int tree_ver_recurs (Tree* tree_node);

static int is_eq (double value_fr, double value_sec);

int tree_verificate (Tree* tree_node)
{
    CHECK_ERROR_(tree_node, "HAVE ZERO ADRESS", -1);

    if (tree_ver_recurs (tree_node) == -1)
    {
        return -1;
    }
    
    return 0;
}

#define tree_ver_(arg)                                                      \
        do                                                                  \
        {                                                                   \
            Tree* node_ptr = tree_node->arg;                                \
            if (node_ptr != NULL)                                           \
            {                                                               \
                if (node_ptr->level - tree_node->level <= 0)                \
                {                                                           \
                    printf ("TREE HAVE CYCLE ERROR\n");                     \
                    return -1;                                              \
                }                                                           \
                if (node_ptr->pre_ptr != tree_node)                         \
                {                                                           \
                    printf ("TREE HAVE PARRENT CONNECT ERROR\n");           \
                    return -1;                                              \
                }                                                           \
                if (tree_ver_recurs(node_ptr) == -1)                        \
                {                                                           \
                    return -1;                                              \
                }                                                           \
            }                                                               \
        } while (0)
        
static int tree_ver_recurs (Tree* tree_node)
{
    /*tree_ver_(left_ptr);
    tree_ver_(right_ptr);*/
    return 0;
}

#undef tree_ver_

int tree_ctor (Tree** tree_node)
{
    CHECK_ERROR_(tree_node, "NULL ADRESS", -1);

    *tree_node = (Tree*) calloc (1, sizeof (Tree));
    CHECK_ERROR_(*tree_node, "NULL ADRESS (NOT FREE MEMORY)", -1);

    Tree* node_ptr = *tree_node;

    node_ptr->left_ptr =  NULL;
    node_ptr->right_ptr = NULL;
    node_ptr->pre_ptr = NULL;

    node_ptr->level = 0;

    node_ptr->value.data = NULL;
    node_ptr->value.type = 0;

    return 0;
}

#define _tree_dtor_elem_(name)                                      \
        do {                                                        \
            if ((node_ptr)->name != NULL)                           \
            {                                                       \
                tree_dtor (&((node_ptr)->name));                    \
            }                                                       \
        }while (0)

int tree_dtor (Tree** tree_node)
{
    CHECK_ERROR_(tree_node, "NULL ADRESS", -1);

    Tree* node_ptr = *tree_node;

    _TREE_VERIFICATE_(node_ptr);

    _tree_dtor_elem_(left_ptr);

    _tree_dtor_elem_(right_ptr);

    node_ptr->level = 0;
    
    node_ptr->value.type = 0;
    node_ptr->value.data = NULL;

    node_ptr->left_ptr = NULL;
    node_ptr->right_ptr = NULL;
    
    node_ptr->pre_ptr = NULL;/////////////////////////////////////////////////////////CHECK IT

    free (*(tree_node));    
    //*tree_node = NULL;

    return 0;
}

#undef _tree_dtor_elem_

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
        
int tree_push (Tree* tree_node, int side, node_t name, bool const_status)
{
    if (tree_node == NULL)
    {
        CHECK_ERROR_(tree_node, "NULL ADRESS", -1);
    }

    int type_node_value = 0;
    
    if (const_status == 0)
    {
        if (is_eq (name, PLUS) || is_eq (name, MINUS) || (is_eq (name, MULTIPLICATION)) || (is_eq (name, DIVISION)) || (is_eq (name, POWER)))
        {
            type_node_value = OPERATION;
        }
        else if (is_eq (name, VARIABLE))
        {
            type_node_value = VAR_X;
        }
        else if ((is_eq (name, SIN)) || (is_eq (name, COS)))
        {
            type_node_value = FUNCT;
        }
    }
    else
    {
        type_node_value = CONST_VALUE;
    }

    if (side == IN)
    {
        tree_node->value.data = name;
        tree_node->value.type = type_node_value;
    }

    if (side == LEFT)
    {
        Tree* node_ptr = tree_node->left_ptr;

        tree_ctor (&(node_ptr));

        node_ptr->value.data = name;
        node_ptr->level = tree_node->level + 1;
        node_ptr->pre_ptr = tree_node;

        node_ptr->value.type = type_node_value;

        tree_node->left_ptr = node_ptr;
    }
    
    if (side == RIGHT)
    {
        Tree* node_ptr = tree_node->right_ptr;

        tree_ctor (&(node_ptr));

        node_ptr->value.data = name;
        node_ptr->level = tree_node->level + 1;
        node_ptr->pre_ptr = tree_node;

        node_ptr->value.type = type_node_value;

        tree_node->right_ptr = node_ptr;
    }

    return 0;
}

int tree_dump_visual (Tree* tree_node)
{
    _TREE_VERIFICATE_(tree_node);

    FILE* output_file = fopen ("../../output/tree_dump_vis.dot", "w");

    //FILE* output_file = fopen ("tree_dump.dot", "w");

    CHECK_ERROR_(output_file, "NOT CREATED", -1);

    fprintf (output_file, "\tdigraph dump_graph{\n"
                          "\trankdir=HR;\n");


    if (tree_node->value.type == OPERATION)
    {
        fprintf (output_file, "\tnode[color = \"red\",fontsize=14];\n");
        fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %c | <right> RIGHT :\\n%p\"];\n",
                                (void*)tree_node, (void*)tree_node->left_ptr, (char)tree_node->value.data, (void*)tree_node->right_ptr);
    }

    if (tree_node->value.type == VAR_X)
    {
        fprintf (output_file, "\tnode[color = \"darkgreen\",fontsize=14];\n");
        fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %c | <right> RIGHT :\\n%p\"];\n",
                                (void*)tree_node, (void*)tree_node->left_ptr, (char)tree_node->value.data, (void*)tree_node->right_ptr);
    }

    if (tree_node->value.type == CONST_VALUE)
    {
        fprintf (output_file, "\tnode[color = \"blue\",fontsize=14];\n");
        fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %lf | <right> RIGHT :\\n%p\"];\n",
                                (void*)tree_node, (void*)tree_node->left_ptr, tree_node->value.data, (void*)tree_node->right_ptr);
    }

    /*fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %s | <right> RIGHT :\\n%p\"];\n",
                             (void*)tree_node, (void*)tree_node->left_ptr, tree_node->value.data, (void*)tree_node->right_ptr);
*/

    write_tree_element_vis (tree_node, output_file);

    fprintf(output_file, "\t}");


    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    
    fclose (output_file);


    system ("\"C:/Program Files/Graphviz/bin/dot.exe\" -Tpng ../../output/tree_dump_vis.dot -o ../../output/tree_graph.png");


    return 0;
}

#define write_define_vis_(arg, side)                                                                                                      \
        do{                                                                                                                                     \
            if (tree_node->arg != NULL)                                                                                                         \
            {                                                                                                                                   \
                Tree* node_ptr = tree_node->arg;                                                                                                \
                if (node_ptr->value.type == OPERATION)                                                                                          \
                {                                                                                                                       \
                    fprintf (output_file, "\tnode[color = \"red\",fontsize=14];\n");                                                                    \
                    fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %c| <right> RIGHT :\\n%p\"];\n",       \
                                            (void*)node_ptr, (void*)node_ptr->left_ptr, (char) node_ptr->value.data, (void*)node_ptr->right_ptr);          \
                }                                                                                                                                   \
                if (node_ptr->value.type == CONST_VALUE)                                                                                                \
                {                                                                                                                                       \
                    fprintf (output_file, "\tnode[color = \"blue\",fontsize=14];\n");                                                                    \
                    fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %lf| <right> RIGHT :\\n%p\"];\n",       \
                                            (void*)node_ptr, (void*)node_ptr->left_ptr, node_ptr->value.data, (void*)node_ptr->right_ptr);          \
                }                                                                                                                                       \
                                                                                                                                                                \
                if (node_ptr->value.type == VAR_X)                                                                                          \
                {                                                                                                                       \
                    fprintf (output_file, "\tnode[color = \"darkgreen\",fontsize=14];\n");                                                                    \
                    fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %c| <right> RIGHT :\\n%p\"];\n",       \
                                            (void*)node_ptr, (void*)node_ptr->left_ptr, (char) node_ptr->value.data, (void*)node_ptr->right_ptr);          \
                }          \
                if (node_ptr->value.type == FUNCT)\
                {\
                    fprintf (output_file, "\tnode[color = \"darkmagenta\",fontsize=14];\n");                                                                    \
                    fprintf (output_file, "\tINFO%p [shape = record, label = \" <left> LEFT :\\n%p | <name> %c| <right> RIGHT :\\n%p\"];\n",       \
                                            (void*)node_ptr, (void*)node_ptr->left_ptr, (char) node_ptr->value.data, (void*)node_ptr->right_ptr);   \
                }\
                                                                                                                                                \
                fprintf (output_file, "\tINFO%p: <%s> -> INFO%p: <name>\n", (void*)tree_node, #side, (void*)node_ptr);  \
                fprintf (output_file, "\tINFO%p: <name> -> INFO%p: <%s>\n", (void*)node_ptr, (void*)node_ptr->pre_ptr, #side);  \
                write_tree_element_vis (node_ptr, output_file);                                                                                 \
            }                                                                                                                                   \
        }while(0)


static int write_tree_element_vis (Tree* tree_node, FILE* output_file)
{
    write_define_vis_(left_ptr, left);
    write_define_vis_(right_ptr, right);
    return 0;
}

#undef write_define_vis_

int tree_dump_text (Tree* tree_node)
{
    _TREE_VERIFICATE_(tree_node);

    FILE* output_file = fopen ("../../output/tree_dump_text.txt", "w");

    fprintf (output_file, "{\n");
    fprintf (output_file, "%s\n", tree_node->value.data);
    write_tree_element_text (tree_node, output_file);
    fprintf (output_file, "}\n");

    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    
    fclose (output_file);

    return 0;
}

#define _write_tree_elem_(name)                                             \
        do {                                                                \
            fprintf (output_file, "{\n");                                   \
            if (tree_node->name != NULL)                                    \
            {                                                               \
                fprintf (output_file, "%s\n", tree_node->name->value.data); \
                write_tree_element_text (tree_node->name, output_file);     \
            }                                                               \
            fprintf (output_file, "}\n");                                   \
        }while (0)

static int write_tree_element_text (Tree* tree_node, FILE* output_file)
{
    _write_tree_elem_(left_ptr);
    _write_tree_elem_(right_ptr);
    return 0;
}

#undef _write_tree_elem_

#undef CHECK_ERROR_

#undef _TREE_VERIFICATE_

#undef $