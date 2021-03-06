// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"


struct node *generate_list(){
  int len = sizeof(struct node);
  int i, j;

  struct node *head, *tmp, *act;
  char* tmp2;

  for (j = 0; j < 10; j++){
    tmp = (struct node*) malloc(len);
    //printf("tmp add = %p\n", tmp);
    tmp->next = NULL;
    tmp2 = (char*) tmp;

    for(i = sizeof(struct node*); i < len; i++){
        if (i%2)
          *(tmp2+i) = 0;
        else
          *(tmp2+i) = (j+1)%10;
        //printf("struct %i : byte %i = %i\n", j, i, *(tmp2+i));
        //printf("%i", *(tmp2+i));
    }

    if (j == 0){
      head = tmp;
    }
    else{
      (act)->next = tmp;
    }

    act = tmp;
  }

  act->next = NULL;

  /*struct node * run = head;
  while(run)
  {
    printf("run : %p\n", run);
    run = run->next;
  }*/

  return head;
}

void test_empty_list(){
  set_test_metadata("filter", _("Test the return value of the function when null pointer is given as arg"), 1);

  struct node* ret = (struct node*) 0x42;

  SANDBOX_BEGIN;
  ret = pair_filter(NULL);
  SANDBOX_END;

  CU_ASSERT_PTR_NULL(ret);
  if (ret)
    push_info_msg(_("Wrong returned value when passing NULL arg"));
}

void test_one_elem(){
  set_test_metadata("filter", _("Test the return value of the function when the list is one element long"), 1);

  struct node* ret = (struct node*) 0x42;

  struct node* list = malloc(sizeof(struct node));
  list->next = NULL;
  /*list->fifo = NULL;
  list->parent = '\0';*/

  SANDBOX_BEGIN;
  ret = pair_filter(list);
  SANDBOX_END;

  CU_ASSERT_PTR_NOT_NULL(ret);
  if (!ret){
    push_info_msg(_("NULL was returned for a one element list"));
    return;
  }

  CU_ASSERT_PTR_NULL(ret->next);
  if (memcmp(list, ret, sizeof(struct node))){
      CU_FAIL("The function produced a wrong list");
      push_info_msg(_("The function produced a wrong list"));
  }
}

void test_two_elem(){
  set_test_metadata("filter", _("Test the return value of the function when the list is two element long"), 1);

  struct node* ret = (struct node*) 0x42;

  struct node* list = malloc(sizeof(struct node));
  list->fifo = NULL;
  list->parent = '\0';

  list->next = malloc(sizeof(struct node));
  list->next->next = NULL;
  list->next->fifo = NULL;
  list->next->parent = '\0';

  SANDBOX_BEGIN;
  ret = pair_filter(list);
  SANDBOX_END;

  CU_ASSERT_PTR_NOT_NULL(ret);
  if (!ret){
    push_info_msg(_("NULL was returned for a two element list"));
    return;
  }

  CU_ASSERT_PTR_NULL(ret->next);
  list->next = NULL;
  if (memcmp(list, ret, sizeof(struct node))){
      CU_FAIL("The function produced a wrong list");
      push_info_msg(_("The function produced a wrong list"));
  }
}

void test_pair_filter(){
  set_test_metadata("filter", _("Test if the function returns a valid linked list in normal case"), 1);
  int len = sizeof(struct node);

  struct node* head = generate_list(), *ret;
  //printf("%p\n", (head));

  SANDBOX_BEGIN;
  ret = pair_filter(head);
  SANDBOX_END;

  if (!ret){
    CU_FAIL("NULL returned value");
    push_info_msg(_("NULL returned value"));
    return;
  }

  //printf("ret %p\n", ret);

  struct node *run1 = ret, *run2 = head;
  int cmp;
  while(!run1){
    cmp = memcmp((const void*) run1, (const void*) run2, len);
    if (cmp != 0){
      CU_FAIL("The function produced a wrong list");
      push_info_msg(_("The function produced a wrong list"));
      return;
    }

    run1 = run1->next;
    run2 = run2->next;
    if (!run2->next)
      break;
    run2 = run2->next;
  }

  CU_PASS("The produced list match");
}


int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_pair_filter, test_empty_list, test_one_elem, test_two_elem);
}
