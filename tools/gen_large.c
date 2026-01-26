#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_file(const char *filename, const char *name, int nb_noeuds,
                   int density_percent) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    perror("fopen");
    return;
  }

  // Header: N <nb> <oriente=0>
  fprintf(f, "N %d 0\n", nb_noeuds);

  // Nodes: V <id> <name> <type> <x> <y> <active>
  for (int i = 0; i < nb_noeuds; i++) {
    int type = rand() % 4; // 0=ROUTEUR, 1=COMMUTATEUR...
    float x = (float)(rand() % 1000);
    float y = (float)(rand() % 1000);
    fprintf(f, "V %d Node_%d %d %.2f %.2f 1\n", i, i, type, x, y);
  }

  // Edges: E <src> <dest> <cost> <lat> <bp> <fiab> <sec>
  for (int i = 0; i < nb_noeuds; i++) {
    for (int j = i + 1; j < nb_noeuds; j++) {
      if ((rand() % 100) < density_percent) {
        float cout = (float)(rand() % 20 + 1);
        float lat = (float)(rand() % 10 + 1);
        float bp = 1000.0f;
        float fiab = 0.9f + ((rand() % 10) / 100.0f); // 0.90 - 0.99
        int sec = (rand() % 3) + 1;                   // 1-3

        fprintf(f, "E %d %d %.2f %.2f %.2f %.2f %d\n", i, j, cout, lat, bp,
                fiab, sec);
      }
    }
    // Ensure connectivity ring/chain
    if (i < nb_noeuds - 1) {
      fprintf(f, "E %d %d 10.00 1.00 1000.00 0.99 1\n", i, i + 1);
    }
  }
  fclose(f);
  printf("Generated %s\n", filename);
}

int main() {
  srand(time(NULL));
  generate_file("data/topologies/medium/grid_25.txt", "Grid_25", 25,
                20); // Not a real grid but connected graph
  generate_file("data/topologies/large/sparse_500.txt", "Sparse_500", 500, 2);
  generate_file("data/topologies/large/dense_300.txt", "Dense_300", 300, 15);
  return 0;
}
