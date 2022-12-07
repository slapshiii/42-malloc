#include "../ft_malloc.h"

void    *allocate(t_list** l, size_t s) {
    t_list  *cur = *l;
    void    *res = NULL;
    while (cur != NULL)
    {
        res = get_first_fit(cur)

    }
}

void    *allocate_large(t_list** l, size_t s) {

}