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

    tree_node = dif_and_report (tree_node);

    tree_dump_visual (tree_node);

    tree_dtor (&tree_node);
    
    return 0;
}

#undef CHECK_ERROR_