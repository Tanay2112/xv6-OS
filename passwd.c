#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
getcmd(char *buf, int nbuf, char *msg)
{
  printf(1,"%s",msg);
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  
    
  return 0;
}

int user_exist( char *user){
  int i, n, c,l;
  int fd;
  char iuser[30];
  char buf[1024];

  fd = open("/psswd", O_CREATE | O_RDWR);
  l = c = 0;
 
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n;) {

      if(l == 0){	
      	while(i < n && buf[i] != ':' )iuser[c++] = buf[i++];
      	if(i == n) break; 
      	iuser[c] = '\0';
      	i++;
      }
     
      if(i == n) break;

      c = 0;
      l = 0;
     
      if(!strcmp(user,iuser) ){
      	close(fd);
     	 	//printf(1,"User already exist\n");
      	return 1;
      }
      while(i <n && buf[i++] != '\n');

    }
  }
  close(fd);
  printf(1, "User do not exist\n");
  return 0;
}

int changepasswd( char *user, char *passwd, char *npass){
  int fd;
	int i, n, c,l;
  char ipasswd[30];
  char iuser[30];
  char buf[1024];
  char buf2[1024];
  char line[100];

  l = c = 0;
  
  if((fd = open("/psswd", O_CREATE | O_RDWR)) < 0){
			printf(1, "cannot open psswd \n");
			return 0;
			//exit();
		}
 
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n;) {

      if(l == 0){	
      	while(i < n && buf[i] != ':' )iuser[c++] = buf[i++];
      	if(i == n) break; 
      	iuser[c] = '\0';
      	
      	i++;
      }
      
    strcpy(line,iuser);
    line[strlen(line)]=':';
     while(i <n && buf[i] != ':')ipasswd[l++] = buf[i++];
      if(i == n) break;
      ipasswd[l] = '\0';
      c = 0;
      l = 0;
     
      if(!strcmp(user,iuser)){
      		if(!strcmp(passwd,ipasswd)){
 
      		strcpy(line+(strlen(line)), npass);
      	
      		}else{
      			printf(1,"passwords dont match \n");
      			strcpy(line+(strlen(line)), ipasswd);
      		}
      		
      }else{
      	strcpy(line+(strlen(line)), ipasswd);
      }
      while(i <n && buf[i] != '\n'){
      	line[strlen(line)]=buf[i++];
      }
      //i++;
      line[strlen(line)]=buf[i++];
      //printf(1,"line: %s\n",line);
      strcpy(buf2 + strlen(buf2),line);
     
      
      memset(line, 0, 100);
    }
    buf2[strlen(buf2)]='\0';
    //printf(1,"%s\n",buf2);
  }
  close(fd);
  unlink("/psswd");
  int cd;
  if((cd = open("/psswd", O_CREATE | O_RDWR)) < 0){
			printf(1, "cannot open psswd \n");
			return 0;
	}
		
	if((write(cd, buf2, strlen(buf2)))> 0){
		printf(1,"Password changed\n");
	};
	close(cd);
  return 0;
}

int main(int argc, char *argv[]){
	
	char oldpss[30];
	char newpss[30];
	
	char msg[20];
	strcpy(msg,"Old password: ");
	char msg1[20];
	strcpy(msg1,"New password: ");
	
	if(argc < 2){
		printf(1,"Usage: passwd program needs a user in the first argument\n");
		exit();
	}
	
	if(user_exist(argv[1])){
		while(getcmd(oldpss, sizeof(oldpss),msg) >= 0){
				break;
			}
			oldpss[strlen(oldpss)-1]='\0';
		while(getcmd(newpss, sizeof(newpss),msg1) >= 0){
				break;
			}
			newpss[strlen(newpss)-1]='\0';
		changepasswd( argv[1],oldpss,newpss);
	}
	
	exit();
}
