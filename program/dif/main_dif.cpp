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

int main ()
{
    Tree *tree_node = {};

    tree_ctor (&tree_node);

    make_tree_from_library (tree_node);

    tree_node = take_dif_tree (tree_node);
    
    solve_const_exp_tree (tree_node);

    optim_exp_tree (tree_node);

    /*FILE* teh_output = fopen ("../../output/dif.teh", "w");
    CHECK_ERROR_(teh_output, "NULL ADRESS", -1);

    write_report_title (teh_output);
    write_report (tree_node, teh_output);
    write_report_conclusion (teh_output);

    if (ferror(teh_output))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    
    fclose (teh_output);*/

    //tree_dtor(&(tree_node->left_ptr));

    //tree_node = copy_tree_node (tree_node);
    
    tree_dump_visual (tree_node);

    tree_dtor (&tree_node);
    
    return 0;
}

#undef CHECK_ERROR_