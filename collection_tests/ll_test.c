#include "../include/collections/ll.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include "collection_test_helper.h"
#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include <string.h>

int main(int argc, char *argv[]) {
    OBS_SETUP("Linked List")

    OBS_TEST_GROUP("LL_new && ll_new_node", {
        OBS_TEST("Create list and test properties", {
            LL list = ll_new("1");
            obs_test_strcmp(list->parent.name, "1");
            test_empty_list(list, ll);
            ll_free(list);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            LL_Node node = ll_new_node((Data){.int_data = 4}, INTEGER);
            obs_test_eq(node->data_tag, INTEGER);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.int_data, (long long)4);
            ll_free_node(node);

            // flt node
            node = ll_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test_eq(node->data_tag, FLOAT);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.flt_data, 5.9);
            ll_free_node(node);

            // str node
            node = ll_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test_eq(node->data_tag, STRING);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            ll_free_node(node);

            // any node
            int x = 9;
            node = ll_new_node((Data){.any_data = &x}, ANY);
            obs_test_eq(node->data_tag, ANY);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq((int*)node->data.any_data, &x);
            ll_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            LL_Node node = NEW_NODE(ll, 4);
            obs_test_eq(node->data_tag, INTEGER);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.int_data, (long long)4);
            ll_free_node(node);

            // flt node
            node = NEW_NODE(ll, 5.9);
            obs_test_eq(node->data_tag, FLOAT);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.flt_data, 5.9);
            ll_free_node(node);

            // str node
            node = NEW_NODE(ll, "Hello");
            obs_test_eq(node->data_tag, STRING);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            ll_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(ll, &x);
            obs_test_eq(node->data_tag, ANY);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq((int*)node->data.any_data, &x);
            ll_free_node(node);
        })
    });

    OBS_TEST_GROUP("LL_clear", {
        OBS_TEST("Clearing empty list", {
            LL list = ll_new("1");
            test_empty_list(list, ll);
            ll_clear(list);
            test_empty_list(list, ll);
            ll_free(list);
        })

        OBS_TEST("Clearing list with elements", {
            LL list = ll_new("2");
            long long *elements = ((long long[]){1, 2, 3, 4, 5});
            map_items(list, 5, elements, ll, ll_append);
            test_list(list, elements, ll);
            ll_clear(list);
            test_empty_list(list, ll);
            ll_free(list);
        })

        OBS_TEST("Clearing list then adding different elements", {
            LL list = ll_new("3");
            double *elements = ((double[]){5.0, 3.2, 100.5, 421.222, 1.022, 2.22, 0.2});
            map_items(list, 7, elements, ll, ll_append);
            test_list(list, elements, ll);
            ll_clear(list);
            test_empty_list(list, ll);
            char **new_elements = ((char*[]){"Hello", "World", "Wow"});
            map_items(list, 3, new_elements, ll, ll_append);
            test_strcmp_fakenode(list, new_elements);
            ll_clear(list);
            test_empty_list(list, ll);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_insert_after/before", {
        OBS_TEST("Inserting after on empty list", {
            LL list = ll_new("1");
            test_empty_list(list, ll);
            ll_insert_after(list, NEW_NODE(ll, 5), list->head);
            test_list(list, (long long[]){5}, ll);
            ll_free(list);
        })

        OBS_TEST("Inserting before on empty list", {
            LL list = ll_new("2");
            test_empty_list(list, ll);
            ll_insert_before(list, NEW_NODE(ll, "hello"), list->head);
            test_strcmp_fakenode(list, (char*[]){"hello"});
            ll_free(list);
        })

        OBS_TEST("Inserting after on single element list", {
            LL list = ll_new("3");
            ll_insert_after(list, NEW_NODE(ll, 5.4), list->head);
            test_list(list, ((double[]){5.4}), ll);
            ll_insert_after(list, NEW_NODE(ll, 2.2), list->head);
            test_list(list, ((double[]){5.4, 2.2}), ll);
            ll_free(list);
        })

        OBS_TEST("Inserting before on single element list", {
            LL list = ll_new("4");
            ll_insert_before(list, NEW_NODE(ll, 5.4), list->head);
            test_list(list, ((double[]){5.4}), ll);
            ll_insert_before(list, NEW_NODE(ll, 2.2), list->head);
            test_list(list, ((double[]){2.2, 5.4}), ll);
            ll_free(list);
        })

        OBS_TEST("Inserting after in middle of list", {
            LL list = ll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            ll_insert_after(list, NEW_NODE(ll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10});
            test_list(list, result, ll);
            ll_free(list);
        })

        OBS_TEST("Inserting after on last element", {
            LL list = ll_new("6");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            ll_insert_after(list, NEW_NODE(ll, 100), list->tail);
            long long *result = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100});
            test_list(list, result, ll);
            ll_free(list);
        })

        OBS_TEST("Inserting before in middle of list", {
            LL list = ll_new("7");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            ll_insert_before(list, NEW_NODE(ll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 100, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, ll);
            ll_free(list);
        })

        OBS_TEST("Inserting before on first element", {
            LL list = ll_new("8");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            ll_insert_before(list, NEW_NODE(ll, 100), list->head);
            long long *result = ((long long[]){100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, ll);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_remove_node", {
        OBS_TEST("Remove a NULL node from an empty list", {
            LL list = ll_new("1");
            obs_test_eq(ll_remove_node(list, NULL), NULL);
            ll_free(list);
        })

        OBS_TEST("Remove a non null node from an empty list", {
            LL list = ll_new("1");
            LL_Node n = NEW_NODE(ll, 5);
            obs_test_eq(ll_remove_node(list, n), NULL);
            ll_free_node(n);
            ll_free(list);
        })

        OBS_TEST("Remove a node from an non empty list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            ll_remove_node(list, list->head->next->next);
            long long *result = ((long long[]){1, 2, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, ll);
            ll_free(list);
        })

        OBS_TEST("Remove the only node from an non empty list", {
            LL list = ll_new("4");
            LL_Node original = NEW_NODE(ll, 1);
            ll_insert_after(list, original, list->head);
            LL_Node n = ll_remove_node(list, list->head);
            test_empty_list(list, ll);
            obs_test_eq(original, n);
            ll_free(list);
            ll_free_node(n);
        })

        OBS_TEST("Remove first and last then the rest (from forward to back)", {
            LL list = ll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            // remove front
            ll_free_node(ll_remove_node(list, list->head));
            long long *result1 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result1, ll);
            // remove back
            ll_free_node(ll_remove_node(list, list->tail));
            long long *result2 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9});
            test_list(list, result2, ll);
            // remove rest
            while (!ll_is_empty(list)) {
                ll_free_node(ll_remove_node(list, list->head));
            }
            test_empty_list(list, ll);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_is_empty", {
        OBS_TEST("Empty list", {
            LL list = ll_new("1");
            obs_test_true(ll_is_empty(list));
            ll_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = ll_new("2");
            ll_append(list, NEW_NODE(ll, 2));
            obs_test_false(ll_is_empty(list));
            ll_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            obs_test_false(ll_is_empty(list));
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_find_prev/next", {
        OBS_TEST("Previous/Next of first element", {
            LL list = ll_new("1");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            obs_test_eq(ll_find_prev(list, list->head), NULL);
            obs_test_eq(ll_find_next(list->head)->data.int_data, (long long)2);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of last element", {
            LL list = ll_new("2");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            obs_test_eq(ll_find_prev(list, list->tail)->data.int_data, (long long)9);
            obs_test_eq(ll_find_next(list->tail), NULL);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of null element", {
            LL list = ll_new("3");
            obs_test_eq(ll_find_prev(list, NULL), NULL);
            obs_test_eq(ll_find_next(NULL), NULL);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of middle elements", {
            LL list = ll_new("4");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            obs_test_eq(ll_find_prev(list, list->head->next->next), list->head->next);
            obs_test_eq(ll_find_next(list->head->next->next->next), list->head->next->next->next->next);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_length", {
        OBS_TEST("Empty list", {
            LL list = ll_new("1");
            obs_test_eq(ll_length(list), (int)0);
            ll_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = ll_new("2");
            ll_push(list, NEW_NODE(ll, 2));
            obs_test_eq(ll_length(list), (int)1);
            ll_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, ll, ll_append);
            test_list(list, items, ll);
            obs_test_eq(ll_length(list), (int)10);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_push/pop", {
        OBS_TEST("Push onto empty list, then pop it", {
            LL list = ll_new("1");
            ll_push(list, NEW_NODE(ll, 4));
            obs_test_eq(ll_length(list), (int)1);
            obs_test_eq(list->head->data.int_data, (long long)4);
            LL_Node n = ll_pop(list);
            obs_test_eq(n->data.int_data, (long long)4);
            ll_free_node(n);
            obs_test_eq(ll_length(list), (int)0);
            ll_free(list);
        })

        OBS_TEST("Pop empty list", {
            LL list = ll_new("2");
            obs_test_eq(ll_pop(list), NULL);
            ll_free(list);
        })

        OBS_TEST("Push multiple then pop them", {
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            long long *result = ((long long[]){5, 4, 2, 9, 5, 2, 1});
            LL list = ll_new("3");
            for (int i = 0; i < 7; i++) {
                ll_push(list, NEW_NODE(ll, elements[i]));
            }
            test_list(list, result, ll);
            for (int i = 0; i < 7; i++) {
                LL_Node n = ll_pop(list);
                obs_test_eq(n->data.int_data, result[i]);
                ll_free_node(n);
            }
            obs_test_eq(ll_length(list), (int)0);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_append", {
        OBS_TEST("Append to empty list", {
            LL list = ll_new("1");
            ll_append(list, NEW_NODE(ll, 10));
            obs_test_eq(ll_length(list), (int)1);
            obs_test_eq(list->head->data.int_data, (long long)10);
            ll_free(list);
        })

        OBS_TEST("Append to non-empty list", {
            LL list = ll_new("1");
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            map_items(list, 7, elements, ll, ll_append);
            test_list(list, elements, ll);
            ll_append(list, NEW_NODE(ll, 10));
            obs_test_eq(ll_length(list), (int)8);
            obs_test_eq(list->tail->data.int_data, (long long)10);
            ll_free(list);
        })
    })

    OBS_REPORT
}
