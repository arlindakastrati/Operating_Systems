//Has the information for all input, output related functions.
#include <stdio.h>
//Provide exit function
#include <stdlib.h>
/*Provide one variable type, one macro, and various functions  for manipulating arrays of character*/ 
#include <string.h>
//Provide system calls: read, write and pipe 
#include <unistd.h>
//Provide the wait system call
#include <sys/types.h>
//Provide open and creat system calls for files
#include <fcntl.h>
int main(){
	/*The pid with which the process will be identified*/
	pid_t pid;
	/*Declaring pointer of FILE type*/
	FILE *fd;
	/*Two file descriptors used on pipe to read from and write to*/
	int fs[2];
	/*Where data from file will reside temporarily*/
	char buffer[30];
	/*Condition whether the pipe can be created or not*/
	if(pipe(fs) == -1){
		/*The message that will be displayed if the pipe cannot be created*/
		perror("Pipe failed!\n");
		exit(1);
	}
	/*If the fork id is less than 0 the fork can not be created*/
	if((pid = fork()) < 0) {
		perror("Fork failed!\n");
		exit(1);
	}
	/*If the process id is equal to 0, the child process is created*/
	if(pid == 0){
		/*Child process will close write end of the pipe since
		it is only going to use read end of it*/
		close(fs[1]);

		/*Check if the file is provided by the parent process*/
		if(fd == NULL) {
		    printf("Could not read file data.txt");
	     	return 1;
		}
		/*Child process reads from the pipe's read end in 120 character chunks
		 by	executing a read system call */
		if(read(fs[0], buffer, 120) <= 0 ) {
			perror("Child read failed!\n");
			exit(1);
		}
		/*Child process read data from parent process*/
		printf("\nChild is reading... \n %s\n", buffer);
		/*Once the reading is finished, 
		the process will close read end of the pipe and the output file*/
		close(fs[0]);
		exit(0);
	}
	else{
		/*Close the read end of the pipe since it will write on the pipe*/
		close(fs[0]);
		/*Open file and write in it*/
		fd = fopen("data.txt", "w");
		/*If no file is set in the fd variable, the message is displayed*/
		if(fd == NULL) {
		    printf("Could not oper file data.txt");
		    return 1;
		}
		printf("Parent is writing...\n");
		printf("Write something: \n");
		/*It reads a line from the specified stream and stores it into 
		the string pointed to by str.*/
		/*The data is taken from stdin up to a maximum of 30 characters and placed in the buffer*/
		fgets(buffer, 30, stdin);
		/*The data found in the buffer is placed in the file*/
		fputs(buffer, fd);
		/*Termination of a character string of buffer*/
		buffer[strlen(buffer)-1] = '\0';
		/*Parent process writes on pipe what it has read from the input file
		and if the data in the buffer is less than 120 bytes the message is displayed*/
		if(write(fs[1], buffer, 120) <= 0) {
			perror("Parent write failed!\n");
			exit(1);
		}
		/*Once the parent is done with copying data from input file to the pipe
		it will close the write end of the pipe and the input file*/
		close(fs[1]);
	}
}