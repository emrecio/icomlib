#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

struct foostruc {
	int elmo1; int elmo2; int elmo3;
	char elmo4[128]; char elmo5[32];
	float elmo6;
} elmo101;


class obj1 {
	public :
		obj1 ();
		~obj1();

	private:
		int foo1;
		char *foo2;
		float foo3;
};

obj1 :: obj1()
{
	elmo101.elmo1=10;
	elmo101.elmo2=21;
	elmo101.elmo3=32;
	strcpy(elmo101.elmo4, "elmo4 variable");
	strcpy(elmo101.elmo5, "elmo5 var");
	elmo101.elmo6=43.223;
}

obj1 :: ~obj1()
{
	fprintf(stderr, "destroying \n");
}

void main()
{
	obj1 elmofoo();

	int fd = open("./output.dat", O_CREAT | O_WRONLY);
	write(fd, &elmo101, sizeof(&elmo101));
	close(fd);
}
