#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 300
#define MAX_FIELDS 5

struct clip *build_a_lst();
struct clip *append();
int find_length();
void print_lst();
void split_line();

struct clip {
  int views;
  char *title;
  char *user;
  char *upload_time;
  char *duration;
  struct clip *next;
} *head;

int main(int argc, char **argv) {
  int n;
  head = build_a_lst(*(argv+1));
  n = find_length(head);
  printf("%d clips\n",n);
  print_lst(head);		/* prints the table */

  return 0;
}

struct clip *build_a_lst(char *fn) {
  FILE *fp;
  struct clip *hp;
  char *fields[MAX_FIELDS];
  char line[LINE_LENGTH];
  hp=NULL;
  
  //open fn
  fp = fopen(fn, "r");
  if(fp == NULL) printf("Cannot open file");

  // while no more lines
  else{
  	//skip first line (headers)
  	if((fgets(line, LINE_LENGTH,fp)) != NULL) {}
	while(!feof(fp)){
		// read a line
		if((fgets(line, LINE_LENGTH, fp)) != NULL){
			// split the line into five substrings/int and store them in a struct
			split_line(fields, line);
			hp=append(hp,fields);
		}
	}
  }
  return hp;
}

/* fields will have five values stored upon return */
void split_line(char **fields,char *line) {
  char *token, *delim;
  delim = ",\n";
  
  token = strtok(line, delim);
  do {
    *fields++ = token;
  } while (token = strtok(NULL, delim));
}

/* set five values into a clip, insert a clip at the of the list */
struct clip *append(struct clip *hp,char **five) {
  struct clip *cp,*tp;

  tp = (struct clip *) malloc(sizeof(struct clip));
  tp->views = atoi(five[4]); 
  tp->title = malloc(strlen(five[0]));
  tp->user = malloc(strlen(five[1]));
  tp->upload_time = malloc(strlen(five[2]));
  tp->duration = malloc(strlen(five[3]));
  
  tp->next = NULL;
  
  strcpy(tp->title, five[0]);
  strcpy(tp->user, five[1]);
  strcpy(tp->upload_time, five[2]);
  strcpy(tp->duration, five[3]);

  if (!hp) {
    return tp;
  }
  cp = hp;
  while (cp->next) cp = cp->next;
  cp->next = tp;

  return hp;
}

void print_lst(struct clip *cp) {
  FILE *fpp;
  fpp = fopen("newOutput.csv", "w+");
  fprintf(fpp,"%s,%s,%s,%s,%s\n","Views", "Title", "User", "Upload Time", "Duration");
  while (cp) {
     fprintf(fpp,"%d,%s,%s,%s,%s\n",cp->views,cp->title,cp->user,cp->upload_time,cp->duration);

     cp = cp->next;
  }
  fclose(fpp);
  printf("Success: CSV was created.\n");
}

int find_length(struct clip *cp) {
  int len = 0;
  while (cp) {
    ++len;
    cp = cp->next;
  }

  return len;
}
/* end */
