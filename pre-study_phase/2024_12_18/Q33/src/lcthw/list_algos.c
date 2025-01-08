#include "list_algos.h"
#include "dbg.h"

void swap_node(ListNode* a,ListNode *b){
    void *c = a->value;
    a->value = b->value;
    b->value = c;
}


List* merge_list(List* left, List* right,List_compare cmp){
    List* result = List_create();
    while(left->first && right->first){
        if(cmp(left->first->value,right->first->value) < 0){
            List_push(result,List_shift(left));
        } else {
            List_push(result,List_shift(right));
        }
    }
    while(left->first){
        List_push(result,List_shift(left));
    }
    while(right->first){
        List_push(result,List_shift(right));
    }
    return result;
}

void List_print(List *list){
    for(ListNode *nodeA = list->first;nodeA != NULL ; nodeA=nodeA->next){
        printf("%s -> ", (char*)nodeA->value);
    }
    printf("\n");
}


int List_bubble_sort(List *list, List_compare cmp)
{
    if(list->count < 2)
        return 0;
    
    for(ListNode *nodeA = list->first;nodeA != list->last ; nodeA=nodeA->next){
        for(ListNode *nodeB = nodeA->next;nodeB != NULL;nodeB=nodeB->next){
            if(cmp(nodeB->value,nodeA->value) < 1){
                swap_node(nodeA,nodeB);
            }
        }
    }
    return 0;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if(list->count<2) return list;
    int left = list->count/2;
    int right = list->count - left;
    List * left_list = List_create();
    List * right_list = List_create();
    for(int i=0;i<right;i++){
        List_push(right_list,List_pop(list));
    }
    left_list = list;
    return merge_list(List_merge_sort(left_list,cmp),List_merge_sort(right_list,cmp),cmp);
}
