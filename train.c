#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//lista dublu inlantuita pentru coada
typedef struct Queue {
  struct node * head;
  struct node * tail;
  int len;
}
Queue;

typedef struct vagon {
  void * data;
  struct vagon * next;
  struct vagon * prev;
}
vagon;

//lista circulara pentru trenulet
typedef struct Train {
  int len;
  vagon * sentinel; //locomotiva trenului
  vagon * vagon_mecanic; //retin unde se afla mecanicul
  Queue * coada_de_comenzi;
}
Train;

typedef struct node {
  char * comanda;
  struct node * next;
  struct node * prev;
}
node;

//initializez trenul (locomotiva + primul vagon "#")
void init(Train ** tren) {
  *tren = (Train *) malloc(sizeof(Train));
  (* tren) -> sentinel = (vagon *) malloc(sizeof(vagon));
  vagon * vagon_initial = (vagon *) malloc(sizeof(vagon));
  vagon_initial -> data = malloc(sizeof(char));
  *((char *) vagon_initial -> data) = '#'; //aici se afla mecanicul la inceput
  (* tren) -> sentinel -> data = NULL;
  (* tren) -> sentinel -> next = vagon_initial;
  (* tren) -> sentinel -> prev = vagon_initial;
  vagon_initial -> next = (* tren) -> sentinel;
  vagon_initial -> prev = (* tren) -> sentinel;
  (* tren) -> vagon_mecanic = vagon_initial;
  (* tren) -> len = 1;
}

//eliberez toata memoria alocata pentru tren
void free_train(Train *tren) {
  vagon *node = NULL;
  vagon *aux = NULL;
  node = tren -> sentinel -> next;
  while (node != tren -> sentinel) {
    aux = node;
    node = node -> next;
    free(aux -> data);
    free(aux);
  }
  free(tren -> sentinel -> data);
  free(tren -> sentinel);
  free(tren);
}

//eliberez memoria alocata pentru coada
void free_queue(Queue *coada) {
  node *current = coada -> head;
  while (current != NULL) {
    node *temp = current;
    current = current -> next;
    free(temp -> comanda);
    free(temp);
  }
  free(coada);
}

//functie care muta mecanicul cu un vagon la stanga
//daca acesta se afla in primul vagon, ajunge in ultimul prin santinela
void move_left(Train *tren) {
  vagon *node = tren -> vagon_mecanic;
  if (node -> prev != tren -> sentinel) {
    tren -> vagon_mecanic = node -> prev;
  } else {
    tren -> vagon_mecanic = node -> prev -> prev;
  }
}

//functie care muta mecanicul cu un vagon la dreapta
//daca acesta se afla in ultimul includ un nou vagon in tren
//inscrpitionat cu "#"
void move_right(Train *tren) {
  vagon *node = tren -> vagon_mecanic;
  if (node -> next != tren -> sentinel) {
    tren -> vagon_mecanic = node -> next;
  } else if (node -> next == tren -> sentinel) {
    vagon *vagon_nou = (vagon *) malloc(sizeof(vagon));
    vagon_nou -> data = malloc(sizeof(char));
    *((char *) vagon_nou -> data) = '#';
    node -> next = vagon_nou;
    vagon_nou -> prev = node;
    vagon_nou -> next = tren -> sentinel;
    tren -> sentinel -> prev = vagon_nou;
    tren -> vagon_mecanic = vagon_nou;
  }
}

//schimba caracterul din vagonul unde se afla mecanicul
void write(Train * tren, char caracter) {
  *((char *) tren -> vagon_mecanic -> data) = caracter;
}

//sterge vagonul unde se afla mecanicul care se muta cu o pozitie la stanga
void clear_cell(Train *tren) {
  if (tren -> len == 1) {
    vagon *node = tren -> vagon_mecanic;
    free(node -> data);
    free(node);
    vagon *vagon_initial = (vagon *) malloc(sizeof(vagon));
    vagon_initial -> data = malloc(sizeof(char));
    *((char *) vagon_initial -> data) = '#'; //aici e mecanicul la inceput
    tren -> sentinel -> data = NULL;
    tren -> sentinel -> next = vagon_initial;
    tren -> sentinel -> prev = vagon_initial;
    vagon_initial -> next = tren -> sentinel;
    vagon_initial -> prev = tren -> sentinel;
    tren -> vagon_mecanic = vagon_initial;
    tren -> len = 1;
  } else if (tren -> len > 1) {
    if (tren -> vagon_mecanic == tren -> sentinel -> next) {
      vagon *node = tren -> vagon_mecanic;
      tren -> sentinel -> next = node -> next;
      node -> next -> prev = tren -> sentinel;
      tren -> vagon_mecanic = tren -> sentinel -> prev;
      free(node -> data);
      free(node);
    } else if (tren -> vagon_mecanic != tren -> sentinel -> next) {
      vagon *node = tren -> vagon_mecanic;
      node -> prev -> next = node -> next;
      node -> next -> prev = node -> prev;
      tren -> vagon_mecanic = node -> prev;
      free(node -> data);
      free(node);
    }
    tren -> len--;  //lungimea trenului scade odata cu pierderea unui vagon
  }
}

//se sterge fiecare vagon din tren, care ajunge la starea initiala
void clear_all(Train *tren) {
  vagon *current = tren -> sentinel -> next;
  while (current != tren -> sentinel) {
    vagon *node = current -> next;
    free(current -> data);
    free(current);
    current = node;
  }
  tren -> sentinel -> next = tren -> sentinel;
  tren -> sentinel -> prev = tren -> sentinel;
  free(tren -> sentinel -> data);
  vagon *vagon_initial = (vagon *) malloc(sizeof(vagon));
  vagon_initial -> data = malloc(sizeof(char));
  *((char *) vagon_initial -> data) = '#'; 
  tren -> sentinel -> next = vagon_initial;
  tren -> sentinel -> prev = vagon_initial;
  vagon_initial -> next = tren -> sentinel;
  vagon_initial -> prev = tren -> sentinel;
  tren -> vagon_mecanic = vagon_initial;
  tren -> len = 1;
}

//se insereaza un nou vagon cu o inscriptie data pe vagonul imediat in stanga
//mecanicului. daca mecanicul se afla in primul vagon se afiseaza "ERROR"
void insert_left(FILE *output_file, Train *tren, char caracter) {
  vagon *node = tren -> vagon_mecanic;
  vagon *nod_nou = (vagon *) malloc(sizeof(vagon));
  nod_nou -> data = malloc(sizeof(char));
  *((char *) nod_nou -> data) = caracter;
  nod_nou -> next = NULL;
  nod_nou -> prev = NULL;
  if (node -> prev == tren -> sentinel) {
    fprintf(output_file, "ERROR\n");
    free(nod_nou -> data);
    free(nod_nou);
    return;
  } else {
    node -> prev -> next = nod_nou;
    nod_nou -> prev = node -> prev;
    node -> prev = nod_nou;
    nod_nou -> next = node;
    tren -> vagon_mecanic = nod_nou;
  }
  tren -> len++;
}

//se insereaza un nou vagon in dreapta mecanicului
void insert_right(Train *tren, char caracter) {
  vagon *node = tren -> vagon_mecanic;
  vagon *nod_nou = (vagon *) malloc(sizeof(vagon));
  nod_nou -> data = malloc(sizeof(char));
  *((char *) nod_nou -> data) = caracter;
  nod_nou -> next = NULL;
  nod_nou -> prev = NULL;
  if (node -> next == tren -> sentinel) {
    node -> next = nod_nou;
    nod_nou -> prev = node;
    nod_nou -> next = tren -> sentinel;
    tren -> sentinel -> prev = nod_nou;
    tren -> vagon_mecanic = nod_nou;
  } else {
    node -> next -> prev = nod_nou;
    nod_nou -> next = node -> next;
    node -> next = nod_nou;
    nod_nou -> prev = node;
    tren -> vagon_mecanic = nod_nou;
  }
  tren -> len++;
}

//se afiseaza starea in care se afla trenuletul. mecanicul apare intre | |
void show(FILE *output_file, Train *tren) {
  vagon *current = tren -> sentinel -> next;
  vagon *node = tren -> vagon_mecanic;
  while (current != tren -> sentinel) {
    if (current == node) {
      fprintf(output_file, "|%c|", *((char *) current -> data));
    } else {
      fprintf(output_file, "%c", *((char *) current -> data));
    }
    current = current -> next;
  }
  fprintf(output_file, "\n");
}

//se afiseaza doar inscriptia vagonului in care se afla mecanicul
void show_current(FILE *output_file, Train *tren) {
  vagon * current = tren -> vagon_mecanic;
  fprintf(output_file, "%c", *((char* ) current -> data));
  fprintf(output_file, "\n");
}

//incepand cu vagonul unde se afla mecanicul se cauta circular un sir de
//caractere. daca este gasit, mecanicul se muta pe prima poz a sirului
void search(FILE *output_file, Train *tren, char *sir_de_cautat) {
  vagon *current = tren -> vagon_mecanic;
  vagon *stop = tren -> vagon_mecanic;
  int gasit = 0;
  do {
    int ok = 1;
    vagon *aux = current;
    char *sir_aux = sir_de_cautat;
    //compar fiecare caracter din sir cu vagoane. daca un vagon se potriveste
    //continui cu urmatorul caracter din sir pana cand se termina, cand ok=1
    while ( *sir_aux != '\0') {
      if ((aux == stop && * ((char *) aux -> data) != *sir_aux) ||
       * ((char *) aux -> data) != *sir_aux) {
        ok = 0;
        break;
      }
      if (aux -> next == tren -> sentinel) {
        aux = aux -> next -> next;
        sir_aux++;
      } else {
        aux = aux -> next;
        sir_aux++;
      }
    }
    if (ok == 1 && *sir_aux == '\0') {
      tren -> vagon_mecanic = current;
      gasit = 1;
      break;
    }
    if (current -> next == tren -> sentinel) {
      current = current -> next -> next;
    } else {
      current = current -> next;
    }
  } while (current != stop);
  if (gasit == 0)
    fprintf(output_file, "ERROR\n");
}

//search_right realizeaza compararea incepand cu pozitia mecanicului, la
//dreapta, pana cand se ajunge la locomotiva
void search_right(FILE *output_file, Train *tren, char *sir_de_cautat) {
  vagon *current = tren -> vagon_mecanic;
  int gasit = 0;
  do {
    int ok = 1;
    vagon *aux = current;
    char *sir_aux = sir_de_cautat;
    while ( * sir_aux != '\0') {
      if (aux == tren -> sentinel || *((char *) aux -> data) != *sir_aux) {
        ok = 0;
        break;
      }
      aux = aux -> next;
      sir_aux++;
    }
    if (ok == 1 && *sir_aux == '\0') {
      tren -> vagon_mecanic = aux -> prev;
      gasit = 1;
      break;
    }
    current = current -> next;
  } while (current -> next != tren -> sentinel -> next);
  if (gasit == 0)
    fprintf(output_file, "ERROR\n");
}

//search_left realizeaza cautarea in stanga mecanicului pana la intalnirea
//locomotivei. ambele functii search_left, search_right muta mecanicul
//pe ultimul caracter din sir. altfel, se afiseaza "ERROR"
void search_left(FILE *output_file, Train *tren, char *sir_de_cautat) {
  vagon *current = tren -> vagon_mecanic;
  int gasit = 0;
  do {
    int ok = 1;
    vagon *aux = current;
    char *sir_aux = sir_de_cautat;
    while ( * sir_aux != '\0') {
      if (aux == tren -> sentinel || *((char *) aux -> data) != *sir_aux) {
        ok = 0;
        break;
      }
      aux = aux -> prev;
      sir_aux++;
    }
    if (ok == 1 && *sir_aux == '\0') {
      tren -> vagon_mecanic = aux -> next;
      gasit = 1;
      break;
    }
    current = current -> prev;
  } while (current -> prev != tren -> sentinel -> prev);
  if (gasit == 0)
    fprintf(output_file, "ERROR\n");
}

//functia switch inverseaza ordinea comenzilor ce asteapta sa fie executate
void switch_queue(Queue *coada) {
  if (coada -> head == coada -> tail || coada -> len == 0) {
    return;
  }
  node *current = coada -> head;
  node *behind = NULL;
  node *next_nod = NULL;
  coada -> tail = coada -> head;
  while (current != NULL) {
    next_nod = current -> next;
    current -> next = behind;
    current -> prev = next_nod;
    behind = current;
    current = next_nod;
  }
  coada -> head = behind;
}

//functia command_queue este apelata atunci cand NU se primeste 
//comanda "EXECUTE". prin intermediul ei se adauga comenzi in coada
void command_queue(Queue *coada, char *command) {
  node *nod_nou = (node *) malloc(sizeof(node));
  if (nod_nou == NULL) {
    return;
  }
  char *temp = (char * ) malloc((strlen(command) + 1) * sizeof(char));
  strcpy(temp, command);
  if (temp == NULL) {
    free(nod_nou -> comanda);
    free(nod_nou);
    return;
  }
  nod_nou -> comanda = temp;
  nod_nou -> next = NULL;
  nod_nou -> prev = NULL;
  if (coada -> tail == NULL) {
    coada -> tail = nod_nou;
    coada -> head = nod_nou;
  } else {
    //trag tail in dreapta ca sa pastrez head pe prima pozitie a cozii
    coada -> tail -> next = nod_nou;
    nod_nou -> prev = coada -> tail;
    coada -> tail = nod_nou;
  }
  coada -> len++;
}

//se executa comenzile
void execute_commands(FILE *output_file, Train *tren, Queue *coada) {
  if (coada -> head == NULL) {
    return;
  } else {
    if (strncmp(coada -> head -> comanda, "WRITE", 5) == 0) {
      char caracter = coada -> head -> comanda[6];
      write(tren, caracter);
    } else if (strcmp(coada -> head -> comanda, "MOVE_RIGHT") == 0) {
      move_right(tren);
    } else if (strcmp(coada -> head -> comanda, "MOVE_LEFT") == 0) {
      move_left(tren);
    } else if (strcmp(coada -> head -> comanda, "CLEAR_CELL") == 0) {
      clear_cell(tren);
    } else if (strcmp(coada -> head -> comanda, "CLEAR_ALL") == 0) {
      clear_all(tren);
    } else if (strncmp(coada -> head -> comanda, "INSERT_R", 8) == 0) {
      char caracter = coada -> head -> comanda[13];
      insert_right(tren, caracter);
    } else if (strncmp(coada -> head -> comanda, "INSERT_L", 8) == 0) {
      char caracter = coada -> head -> comanda[12];
      insert_left(output_file, tren, caracter);
    }
    else if (strncmp(coada -> head -> comanda, "SEARCH", 6) == 0 && 
    (coada -> head -> comanda)[6] != '_') {
      char *copie_comanda;
      copie_comanda = (char * ) malloc((strlen(coada -> head -> comanda)
       - 7 + 1) * sizeof(char));
      strcpy(copie_comanda, coada -> head -> comanda + 7);
      search(output_file, tren, copie_comanda);
      free(copie_comanda);
    } else if (strncmp(coada -> head -> comanda, "SEARCH_L", 8) == 0) {
      char *copie_comanda;
      copie_comanda = (char * ) malloc((strlen(coada -> head -> comanda)
       - 12 + 1) * sizeof(char));
      strcpy(copie_comanda, coada -> head -> comanda + 12);
      search_left(output_file, tren, copie_comanda);
      free(copie_comanda);
    } else if (strncmp(coada -> head -> comanda, "SEARCH_R", 8) == 0) {
      char *copie_comanda;
      copie_comanda = (char * ) malloc((strlen(coada -> head -> comanda)
       - 13 + 1) * sizeof(char));
      strcpy(copie_comanda, coada -> head -> comanda + 13);
      search_right(output_file, tren, copie_comanda);
      free(copie_comanda);
    }
  }
}

int main() {
  Train *tren;
  //fisierele unde se primesc, afiseaza datele 
  FILE *inp_file;
  FILE *output_file;
  init( & tren);
  int n, i;
  char linie[100];
  Queue *coada = (Queue *) malloc(sizeof(Queue));
  coada -> head = NULL;
  coada -> tail = NULL;
  coada -> len = 0;
  inp_file = fopen("example.in", "r");
  if (inp_file == NULL) {
    printf("ERROR\n");
    exit(-1);
  }
  output_file = fopen("example.out", "w");
  if (output_file == NULL) {
    printf("ERROR\n");
    exit(-1);
  }
  fscanf(inp_file, "%d", & n); //nr de operatii
  fgets(linie, sizeof(linie), inp_file);
  for (i = 2; i <= n + 1; i++) {
    fgets(linie, sizeof(linie), inp_file); // citeste o linie din tema1.in
    linie[strcspn(linie, "\n")] = '\0'; // elimin newline de la final
    //comenzile show, show_current, switch se realizeaza odata ce sunt primite
    if (strcmp(linie, "SHOW_CURRENT") == 0) {
      show_current(output_file, tren);
    } else if (strcmp(linie, "SHOW") == 0 && linie[4] != '_') {
      show(output_file, tren);
    } else if (strcmp(linie, "EXECUTE") == 0) {
      execute_commands(output_file, tren, coada);
      node * aux = coada -> head;
      if (coada -> head == coada -> tail) {
        coada -> head = NULL;
        coada -> tail = NULL;
      } else {
        coada -> head = coada -> head -> next;
      }
      free(aux -> comanda);
      free(aux);
      coada -> len--;
    } else if (strcmp(linie, "SWITCH") == 0) {
      switch_queue(coada);
    } else if (linie[0] < '0' || linie[0] > '9') {
    //strdup necesita eliberarea memoriei
      char * command = strdup(linie);
      command_queue(coada, command);
      free(command);
    }
  }
  //se inchid fisierele si se elibereaza memorie
  fclose(inp_file);
  fclose(output_file);
  free_train(tren);
  free_queue(coada);
  return 0;
}
