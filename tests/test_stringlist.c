#include "../stringlist.h"

#include <assert.h>

int main(int argv, char** argc){
  string_list_t* sl;
  sl = create_string_list();
  assert(sl && "Cannot allocate string list memory");

  string_list_add(sl, "Item 1");
  string_list_add(sl, "Item 2");

  string_list_debug(sl);

  free_string_list(sl);
  sl = NULL;
  return 0;
}
