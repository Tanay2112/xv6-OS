#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"





int
getcmd(char *buf, int nbuf)
{
  //printf(2, "login: ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  
    
  return 0;
}


int checkpasswd(int fd, char *user, char *passwd){
  int i, n, c,l;
  char ipasswd[30];
  char iuser[30];
  char buf[1024];

  l = c = 0;
 
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n;) {

      if(l == 0){	
      	while(i < n && buf[i] != ':' )iuser[c++] = buf[i++];
      	if(i == n) break; 
      	iuser[c] = '\0';
      	i++;
      }
      while(i <n && buf[i] != ':')ipasswd[l++] = buf[i++];
      if(i == n) break;
      ipasswd[l] = '\0';
      c = 0;
      l = 0;
     
      if(!strcmp(user,iuser) && !strcmp(passwd,ipasswd))
      	return 1;
      while(i <n && buf[i++] != '\n');

    }
 }
 
 return 0;
 
}
int main(int argc, char *argv[]){
	//static char buf[100];
	static char user[30];
	static char passwd[30];
	static char nuser[30];
	static char npasswd[30];
	int fd;
	mkdir("home");

	//this part the os will know the pre user and pre passwd
	
	for(;;){
		printf(1, "user: ");
		while(getcmd(user, sizeof(user)) >= 0){
		
			break;
		}
		
		printf(1, "passwd: ");
		while(getcmd(passwd, sizeof(passwd)) >= 0){
			
			//strcpy(passwd,strlen(buf));
			
			break;
		}
		int i = 0;
		while( i< (strlen(user)-1)){
			nuser[i] = user[i];
			i++;
		}
		int j = 0;
		while( j< (strlen(passwd)-1)){
			npasswd[j] = passwd[j];
			j++;
		}
			
		if((fd = open("psswd", 0)) < 0){
			printf(1, "psswd: cannot open %s\n", argv[1]);
			exit();
		}
		
		
		if( checkpasswd(fd,nuser,npasswd)){
			
			printf(1,"welcome %s\n",nuser);
			strcpy( argv[1], nuser);
			argv[1] = nuser;
			argv[2]= 0;
			exec("sh", argv);
			
		}else{
			printf(1,"wrong user or password\n");
		}
	 close(fd);
	 
	 exit();

  }
  
}
