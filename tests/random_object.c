/* A test program for key bindings list
 */

#include "../def.h"
#include "../load_objects.h"
#include "../object_list.h"
#include "../logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <math.h> // for fmod

#define NB 5000

int main(int argv, char** argc){
  int i=0;

  int nb_au_dessus = 0;

  srand ( time(NULL) );

  printf("You should run this program from the xAdventure top directory.");
  object_list_t*ol = load_objects();
  
  printf("Find total = %f\n", ol->find_total);
  for (i=0; i<NB; i++){
    double r=(double)rand();
    double val = fmod(r, ol->find_total);
    if (val > 9.4|| val < 0.5){
      printf("\n %d: val = %f", i, val);
    }
    if (val>ol->find_total) printf("<========================\n");
  }
  printf("\n");

  
  unload_objects(ol);
  ol = NULL;
  return 0;
}
