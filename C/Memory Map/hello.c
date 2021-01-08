//Assignment 4 Memory Map 
#include <stdio.h>

int main(int argc, char *argv[]){
  printf("HEX:");
  printf("\n\t+----+----+----+----+ FFFFFFFFFFFF\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t| -- | %.2x | %x | %x | %p\n", argv[4][3], argv[4][2], argv[4][1], argv[4]);
  printf("\t| %x | %.2x | %x | %x | %p\n",  argv[4][0], argv[2][5], argv[2][4], argv[2][3], argv[3]);
  printf("\t| %.2x | %x | %x | %.2x | %p\n",  argv[1][7], argv[1][6], argv[1][5], argv[1][4], argv[2]);
  printf("\t| %x | %x | %x | %x | %p\n",  argv[1][3], argv[1][2], argv[1][1], argv[1][0], argv[1]);
  printf("\t| %.2x | %x | %x | %x | %p\n",  argv[0][7], argv[0][6], argv[0][5], argv[0][4], argv[0]);
  printf("\t| %x | %x | %x | %x | %p\n",  argv[0][3], argv[0][2], argv[0][1], argv[0][0], argv[0-1]);
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n\n");
  
  printf("ASCII:");
  printf("\n\t+----+----+----+----+ FFFFFFFFFFFF\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t| -- | %c   | %c  | %c  | %p\n",  argv[4][3], argv[4][2], argv[4][1], argv[4]);
  printf("\t| %c  | %c   | %c  | %c  | %p\n",  argv[4][0], argv[2][5], argv[2][4], argv[2][3], argv[3]);
  printf("\t| %c   | %c  | %c  | %c   | %p\n",  argv[1][7], argv[1][6], argv[1][5], argv[1][4], argv[2]);
  printf("\t| %c  | %c  | %c  | %c  | %p\n",  argv[1][3], argv[1][2], argv[1][1], argv[1][0], argv[1]);
  printf("\t| %c   | %c  | %c  | %c  | %p\n",  argv[0][7], argv[0][6], argv[0][5], argv[0][4], argv[0]);
  printf("\t| %c  | %c  | %c  | %c  | %p\n",  argv[0][3], argv[0][2], argv[0][1], argv[0][0], argv[0-1]);
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");

  if(argc > 1){
    for(int count=argc-1; count <= argc; count--){
      printf(" argv+%d | %p    | %p\n", count, argv[count], &argv[count]);
	if(count == 0){break;} 
    }
  }else{
    printf("No more arguments in command!\n");
   }

  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf(" argv   | %p    | %p\n", argv, &argv);
  printf(" argc   | %.14x    | %p\n", argc, &argc);
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+\n");
  printf("\t+----+----+----+----+ 00000000000000\n");

  return 0;

}

void strhex(char p[]){
	int count;
	for(count=0; count<p[count]; count++){};
	int printed=0;
	while(count != 0){
		printed++;
		printf("\t| %2x |", *&p[count]);
		count--;
	}
	printf("%p\n", p);
}
