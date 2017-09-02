/* A test program for key bindings list
 */

#include "../def.h"
#include "../keybinding.h"

#include <assert.h>

int test(){
  return 0;
}

void add_5_binding(key_binding_list_t* , unsigned short*);
void add_10_binding(key_binding_list_t* , unsigned short* );
void add_50_binding(key_binding_list_t* , unsigned short* );


int main(int argv, char** argc){
  unsigned short idx = 1;

  // Creating
  key_binding_list_t* bl;
  bl = init_key_binding_list();

  // Ading key bindings
  add_50_binding(bl, &idx);


  // Debug
  debug_key_binding_list(bl);

  // Destroying
  destroy_key_binding_list(bl);
  bl = NULL;

  return 0;
}

void add_5_binding(key_binding_list_t* bl, unsigned short* idx){
  add_binding(bl, (*idx)++, &test);
  add_binding(bl, (*idx)++, &test);
  add_binding(bl, (*idx)++, &test);
  add_binding(bl, (*idx)++, &test);
  add_binding(bl, (*idx)++, &test);
}

void add_10_binding(key_binding_list_t* bl, unsigned short* idx){
  add_5_binding(bl, idx);
  add_5_binding(bl, idx);
}

void add_50_binding(key_binding_list_t* bl, unsigned short* idx){
  add_10_binding(bl, idx);
  add_10_binding(bl, idx);
  add_10_binding(bl, idx);
  add_10_binding(bl, idx);
  add_10_binding(bl, idx);
}
