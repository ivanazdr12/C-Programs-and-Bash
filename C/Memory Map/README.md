INSTRUCTIONS:

Write a C program that prints a memory map. For example, executing "hello this is CS 288" will print a map such as the one shown below. 
"hello" is an executable file while "this is CS 288" are four parameters passed to the executable. Your source code is hello.c and hello 
is generated by gcc -o hello hello.c.

Do not use the example addresses below. Obtain your own memory addresses and draw them by executing your own program. The drawing doesn't
have to as fancy as below but somehow you need to be able to convey the memroy addresses and data.


Memory map filled with hexa decimals only:

       +------+------+------+------+ FFFFFFFF
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
       |      |      |      |00(\0)| 60000014
       +------+------+------+------+
       | 38(8)| 38(8)| 32(2)|00(\0)| 60000010
       +------+------+------+------+
       | 53(S)| 43(C)|00(\0)| 73(s)| 6000000C
       +------+------+------+------+
       | 69(i)|00(\0)| 73(s)| 69(i)| 60000008
       +------+------+------+------+
       | 68(h)| 74(t)|00(\0)| 6F(o)| 60000004
       +------+------+------+------+
       | 6C(l)| 6C(l)| 65(e)| 68(h)| 60000000
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
       |  00  |  00  |  00  |  00  | 5F22AC90
       +------+------+------+------+
argv+4 |  60  |  00  |  00  |  11  | 5F22AC8C
       +------+------+------+------+
argv+3 |  60  |  00  |  00  |  0E  | 5F22AC88
       +------+------+------+------+
argv+2 |  60  |  00  |  00  |  0B  | 5F22AC84
       +------+------+------+------+
argv+1 |  60  |  00  |  00  |  06  | 5F22AC80
       +------+------+------+------+
argv+0 |  60  |  00  |  00  |  00  | 5F22AC7C
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
argv   |  5F  |  22  |  AC  |  7C  | 5F22AC44
       +------+------+------+------+
argc   |  00  |  00  |  00  |  05  | 5F22AC40
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+
       +------+------+------+------+ 00000000
