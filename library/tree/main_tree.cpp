#include "tree.h"

#define CHECK_ERROR_(name, type_error, ret)                                             \
        do{                                                                             \
            if (name == NULL)                                                           \
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

    char name [] = "LITTLE SISTER";
    
    tree_push (tree_node, IN, name );
    tree_push (tree_node, LEFT, name );
    tree_push (tree_node, RIGHT, name );

    tree_push (tree_node->left_ptr, LEFT, name );
    tree_push (tree_node->left_ptr, RIGHT, name );

    tree_push (tree_node->right_ptr, LEFT, name );
    tree_push (tree_node->right_ptr, RIGHT, name );

    tree_dump_visual (tree_node);
    //tree_dump_text (tree);

    tree_dtor (&tree_node);
    
    return 0;
}

#undef CHECK_ERROR_