#include "../object_list.h"
#include "../logger.h"

#include <assert.h>

int main(int argv, char** argc){
  
  LOG("Object list test :");
  object_list_t* ol = create_object_list();

  DEBUG_OBJLST(ol);

  free_object_list(ol);
  return 0;
}
