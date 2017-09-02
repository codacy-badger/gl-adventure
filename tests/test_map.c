/* A test program for key bindings list
 */

#include "../def.h"
#include "../map.h"

#include <assert.h>

int main(int argv, char** argc){
  map_t* map;
  map = init_map(false);
  debug_map(map);
  free_map(map);
  map = NULL;

  map = init_map(true);
  debug_map(map);
  free_map(map);
  map = NULL;

  return 0;
}
