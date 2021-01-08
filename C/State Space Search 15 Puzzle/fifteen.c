/******************************************************************************
 CS288 HW7 three search strategies: depth, breadth, and intelligent
 command example: command 16 numbers and search strategy

 fifteen 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0 15 {dfs|bfs|astar}
******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BF 4			/* Branching factor of the search tree */
#define N 4
#define NxN 16
#define DFS 1                   /* depth first search */
#define BFS 2                   /* breadth first search */
#define BEST 3                  /* best first search */
#define BB 4                    /* branch and bound */
#define ASTAR 5                 /* A* search */
#define UNKNOWN 9		/* unknown search strategy */

#define MAX_PATH 1000

#define DN 0			/* for finding path */
#define RT 1
#define UP 2
#define LT 3
#define UK 9

#define FVAL 0			/* f=g+h, keep it simple for now */
#define GVAL 1
#define HVAL 2
#define PATH 3			/* array index, how it got to this state */

#define TRUE 1
#define FALSE 0

int level,strategy;

int nodes_same(),str_compare(),count(),find_h();
void swap(),exit_proc(),print_a_node(),print_nodes();
int toggle_dir(), solvable();
void find_path(),print_path();
int path[MAX_PATH],path_buf[MAX_PATH];
void prep_dir_to_str(),dir_to_str();
char *dir_to_sdir[4] = {"Down", "Right", "Up", "Left"}, strategy_s[10];

struct node {
  int board[N+1][N];
  struct node *next;
};

struct node *find_parent();
struct node *start,*goal;
struct node *initialize(),*expand(),*merge(),*filter(),*move(),*prepend(),*append();
struct node *insert_node(),*check_list(),*goal_found();

int main(int argc,char **argv) {
  int iter,cnt=0,total=1,ocnt=0,ccnt=0;
  int perm;		/* perm=odd=no solution, perm=even=solvable */
  struct node *cp,*open,*closed,*succ,*tp;

  //get start time
  struct timeval  tv1, tv2;
  gettimeofday(&tv1, NULL);

  open=closed=succ=NULL;
  start=initialize(argc,argv);	/* init initial and goal states */
  if(!solvable(start)) return 1;

  open=start;
  iter=0;

  while (open) {
    printf("%d: open=%d + clsd=%d = total=%d\n",iter,ocnt,ccnt,ocnt+ccnt);
    ocnt=count(open);
    ccnt=count(closed);
    cp=open; open=open->next; cp->next=NULL; /* get the first node from open */
    succ = expand(cp);			     /* Find new successors */
    succ = filter(succ,open);		 /* Remove places already visited */
    succ = filter(succ,closed);	 /* Remove places already marked to visit */
    cnt=count(succ);
    total=total+cnt;
    if (succ) open=merge(succ,open,strategy); /* New open list */
    closed=append(cp,closed);		      /* New closed */
    if ((cp=goal_found(succ,goal))) break;
    ++iter;
  }
  printf("%s strategy: %d iterations %d nodes\n",strategy_s,iter+1,total);
  int *path, n;
  find_path(cp, closed, &path, &n);
  print_path(n, path);

  //end time
  gettimeofday(&tv2, NULL);
  printf("Execution time = %.6f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000);

  free(path);

  return 0;
} /* end of main */

inline int toggle_dir(int dir){
  return (dir + 2) & 0x3;
}

void print_path(int n,int *path){
  int i,p;
  //  for (i=0;i<n;i++) path[i] = path_buf[path_cnt-i-1];
  //  for (i=0;i<path_cnt;i++) printf("%d ",path[i]); printf("\n");
  //  printf("entering print_path: n=%d\n",n);

  ////  for (i=n-1;i>=0;i--) printf("%d ",*(path+i)); printf("\n");
  for (i=n-1;i>=0;i--) {
    p = *(path+i);
    if (i>0) printf("%s -> ",dir_to_sdir[p]);
    else printf("%s\n",dir_to_sdir[p]);
  }
  //  printf("exiting print_path\n");
}

void find_path(struct node *cp, struct node *cldp, int **path, int *n){
  int max_p = MAX_PATH;
  *path = (int *) malloc(max_p * sizeof(int));
  int *p = *path;
  while (!nodes_same(cp, start)) {
    if (p - *path >= max_p) {
      int max_p2 = max_p << 1;
      *path = (int *) realloc(*path, max_p2 * sizeof(int));
      p = *path + max_p;
      max_p = max_p2;
    }
    *p = cp->board[N][PATH];
    //parent
    cp = find_parent(cp, toggle_dir(*p++));  // increment p for next use
    for (struct node *tp = cldp; tp; tp = tp->next) {
      if (nodes_same(cp, tp)) {
        free(cp);
        cp = tp;
        break;
      }
    }
  }
  *n = p - *path;  //size on length path
}

//similar to expand, move based on direction
struct node *find_parent(struct node *cp,int prev_dir){
  // Find 0 in array
  int row, col;
  for (row = 0; row < N; ++row) {
    for (col = 0; col < N; ++col) {
      if (!cp->board[row][col]) break;
    }
    if (col < N) break;
  }
  switch (prev_dir) {
    case DN:
      return move(cp, row, col, row+1, col, DN);
    case RT:
      return move(cp, row, col, row, col+1, RT);
    case UP:
      return move(cp, row, col, row-1, col, UP);
    case LT:
      return move(cp, row, col, row, col-1, LT);
  }
}

// Expand: generate successors of the current node
struct node *expand(struct node *cp) {
  int i,j,k,cnt,row=0,col=j;
  struct node *succ,*tp;
  succ=NULL;

  /* check where 0 is. find indices i,j */
  for(i=0; i < N; i++){
    for(j=0; j < N; j++)
      if (cp->board[i][j]==0) break;
    if (j<N) break;
  }
  /* DOWN */
  if((i+1) < N){
    tp = move(cp,i,j,i+1,j,DN);
    succ = append(tp,succ);
  }
  /* RIGHT */
  if((j+1) < N){
    tp = move(cp,i,j,i,j+1,RT);
    succ = append(tp,succ);
  }
  /* UP */
  if((i-1) >= 0){
    tp = move(cp,i,j,i-1,j,UP);
    succ = append(tp,succ);
  }
  /* LEFT */
  if((j-1) >= 0){
    tp = move(cp,i,j,i,j-1,LT);
    succ = append(tp,succ);
  }
  return succ;
}

/* attach in the beginning */
struct node *prepend(struct node *tp,struct node *sp) {
  tp->next = sp;
  return tp;
}

/* attach at the end */
struct node *append(struct node *tp,struct node *sp) {
  tp->next = NULL;  // just to be sure
  if (!sp) {
    return tp;
  }
  struct node *cp = sp;
  while (cp->next)
    cp = cp->next;

  cp->next = tp;
  return sp;
}

void swap(struct node *cp,int i,int j,int k,int l){
  int tmp = cp->board[i][j];
  cp->board[i][j] = cp->board[k][l];
  cp->board[k][l] = tmp;
}

struct node *move(struct node *cp,int a,int b,int x,int y,int dir) {
  struct node *newp;
  int i,j,k,l,tmp;
  newp = (struct node *) malloc(sizeof(struct node));
  memcpy(newp, cp, sizeof(struct node));
  swap(newp, a, b, x, y);
  newp->board[N][GVAL] += 1;
  newp->board[N][HVAL] = find_h(newp->board, goal->board);
  newp->board[N][FVAL] = newp->board[N][GVAL] + newp->board[N][HVAL];
  newp->board[N][PATH] = dir;
  return newp;
}

struct node *goal_found(struct node *cp,struct node *gp){
  while (cp) {
    if (nodes_same(cp, gp)) return cp;
    cp = cp->next;
  }
  return NULL;
}

int count(struct node *cp) {
  int cnt=0;
  while (cp) {
    cnt++;
    cp = cp->next;
  }
  return cnt;
}

struct node *merge(struct node *succ,struct node *open,int flg) {
  struct node *csucc,*copen;

  if (flg == DFS) {	          /* attach in the front: succ -> ... -> open */
    if (!succ) return open;
    for (csucc = succ; csucc->next; csucc = csucc->next);
    csucc->next = open;
    open = succ;

  } else if (flg == BFS) {    /* attach at the end: open -> ... -> succ */
    if (!open) return succ;

    for (copen = open; copen->next; copen = copen->next);
    copen->next = succ;

  } else if (flg == BEST) {   /* Best first: sort on h value */


  } else if (flg == ASTAR) {  /* A* search: sort on f=g+h value */
    open = insert_node(succ, open, FVAL);
  }
  return open;
}

/* insert succ into open in ascending order of x value, where x is an array
   index: 0=f,1=g,h=2 of board[N][x]
 */
struct node *insert_node(struct node *succ,struct node *open,int x) {
  struct node *succ_next;
  while (succ) {
    succ_next = succ->next;

    if (!open || succ->board[N][x] <= open->board[N][x]) {
      succ->next = open;
      open = succ;
    } else {
      struct node *copen;
      for (copen = open; copen->next; copen = copen->next) {
        if (succ->board[N][x] <= copen->next->board[N][x]) {
          succ->next = copen->next;
          copen->next = succ;
          break;
        }
      }
      if (!copen->next) {
        copen->next = succ;
        succ->next = NULL;
      }
    }
    succ = succ_next;
  }
  return open;
}

int find_h(int current[N+1][N],int goalp[N+1][N]) {
  int h=0,i,j,k,l,done;
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      for (k = 0; k < N; ++k) {
        for (l = 0; l < N; ++l) {
          if (current[i][j] == goalp[k][l]) {
            h += abs(i - k) + abs(j - l);  // manhattan distance
            break;
          }
        }
        if (l < N) break;
      }
    }
  }
  return h;
}

/* a=b=x[N][N] */
int nodes_same(struct node *xp,struct node *yp) {
  for (int row = 0; row < N; ++row) {
    if(memcmp(xp->board[row], yp->board[row], N * sizeof(int))) return FALSE;
  }
  return TRUE;
}



/******************************************************************************
  Check succ against open and closed. Remove those succ nodes that are in open or closed.
******************************************************************************/
struct node *filter_num();
struct node *filter(struct node *succ,struct node *hp){
  while (hp) {
    succ = filter_num(succ, hp);
    hp = hp->next;
  }

  return succ;
}

struct node *filter_num(struct node *succ, struct node *val) {
  struct node *tmp;
  while (succ && nodes_same(succ, val)) {
    tmp = succ->next;
    free(succ);
    succ = tmp;
  }
  if (succ) {
    for (struct node *csucc = succ; csucc->next;) {
      if (nodes_same(csucc->next, val)) {
        tmp = csucc->next->next;
        free(csucc->next);
        csucc->next = tmp;
      } else {
        csucc = csucc->next;
      }
    }
  }
  return succ;
}

void print_nodes(struct node *cp,char name[20]) {
  int i;
  printf("%s:\n",name);
  while (cp) { print_a_node(cp); cp=cp->next; }
}

void print_a_node(struct node *np) {
  int i,j;
  for (i=0;i<N+1;i++) {
    for (j=0;j<N;j++) printf("%2d ",np->board[i][j]);
    printf("\n");
  }
  printf("\n");
}

//cnt=odd -> no solution, cnt=even=solvable
int solvable(struct node *cp) {
  int i,j,k=0,l,lst[NxN],total=0,blank_row;
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      lst[k++] = cp->board[i][j];
      if (!cp->board[i][j]) blank_row = i;
    }
  }
  for (k = 0; k < NxN; ++k) {
    for (l = k + 1; l < NxN; ++l) {
      if (lst[l] && lst[k] > lst[l]) total++;
    }
  }
  return (blank_row & 1) != (total & 1);
}

/* fif 0 1 2 4 5 6 3 8 9 10 7 12 13 14 11 15 astar */
struct node *initialize(int argc, char **argv){
  int i,j,k,npe,n,idx,gidx,inv;
   struct node *tp;

   tp=(struct node *) malloc(sizeof(struct node));
   idx = 1;
   for (j=0;j<N;j++)
     for (k=0;k<N;k++) tp->board[j][k]=atoi(argv[idx++]);
   for (k=0;k<N;k++) tp->board[N][k]=0;	/* set f,g,h of initial state to 0 */
   tp->next=NULL;
   start=tp;

   printf("init state: \n");
   print_a_node(start);

   tp=(struct node *) malloc(sizeof(struct node));
   gidx = 1;
   for (j=0;j<N;j++)
     for (k=0;k<N;k++) tp->board[j][k] = gidx++;
   tp->board[N-1][N-1] = 0;		/* empty tile=0 */
   for (k=0;k<N;k++) tp->board[N][k]=0;	/* set f,g,h of goal state to 0 */
   tp->next=NULL;
   goal=tp;

   printf("goal state: \n");
   print_a_node(goal);

   strncpy(strategy_s,argv[idx], 10);
   if (strcmp(strategy_s,"dfs")==0) strategy=DFS;
   else if (strcmp(strategy_s,"bfs")==0) strategy = BFS;
   else if (strcmp(strategy_s,"best")==0) strategy=BEST;
   else if (strcmp(strategy_s,"bb")==0) strategy=BB;
   else if (strcmp(strategy_s,"astar")==0) strategy=ASTAR;
   else strategy=UNKNOWN;
   printf("strategy=%s\n",strategy_s);

   return start;
}

void exit_proc(char *msg){
   printf("Error: %s\n",msg);
   exit(1);
}

/*****************************************************************************
 End of file: fif.c. Fifteen Puzzle, Sequential A* 1 processor version.
*****************************************************************************/
