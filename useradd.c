#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
     
 }
 
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

int writeid(void){
	int fd;
	char buf[20];
	char buf2[20];
	int r;
	int cd;
	
	if((fd = open("/userid", O_CREATE | O_RDWR)) < 0){
		printf(1, "cannot open userid \n");
		return 0;
		
	}	
	read(fd,buf,sizeof(buf));
	close(fd);
	unlink("/userid");
	
	if((cd = open("/userid", O_CREATE | O_RDWR)) < 0){
		printf(1, "cannot open userid \n");
		return 0;
		
	}	
	
	r = atoi(buf);
	r++;
	itoa(r,buf2);
	write(cd,buf2,strlen(buf2));
	close(cd);
	
	return r;
}

int
getcmd(char *buf, int nbuf)
{
  printf(1, "new passwd: ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  
    
  return 0;
}
int writeonfile( char *buf){
	int fd;
	int w;
	char readbuf [1024]; //assuming you save 10 commands
	
	if((fd = open("/psswd", O_CREATE | O_RDWR)) < 0){
		printf(1, "cannot open psswd \n");
		return 0;
		//exit();
	}	
	else{
		read(fd,readbuf,sizeof(readbuf)); //this allows to write at the end
		if((w = write(fd, buf, strlen(buf)))> 0){
			printf(1,"New user added\n");
		};
		
		
		close(fd);
		
		return 1;
		//exit();
	}
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
     	 	printf(1,"User already exist\n");
      	return 1;
      }
      while(i <n && buf[i++] != '\n');

    }
  }
  close(fd);
  printf(1, "User do not exist\n");
  return 0;
}

int user_add(  char *user, char *passwd){
	char newuser[80];
	
	int fd;
	
	
	
	char readbuf [1024]; 
	
	
	
	if((fd = open("/psswd",O_CREATE | O_RDWR))  < 0){
		printf(1, "cannot open psswd, function add_user \n");
		return 0;
		
	}
	else{
		read(fd,readbuf,sizeof(readbuf));
		//int count =0;
		//int i = 0;
		int id;
		char tmp[1];
		id = writeid();
		itoa(id,tmp);
	
		strcpy(newuser,user);
		strcpy(newuser+strlen(newuser),":");
		strcpy(newuser+strlen(newuser),passwd);
		strcpy(newuser+strlen(newuser),":");
		strcpy(newuser+strlen(newuser),tmp);
		strcpy(newuser+strlen(newuser),":");
		strcpy(newuser+strlen(newuser),"/home/");
		strcpy(newuser+strlen(newuser),user);
	  strcpy(newuser+strlen(newuser),"\n");
		
		//printf(1,"fd value%d",fd);
	
		//w = write(fd, newuser, srtlen(newuser));
		
		//printf(1,"newuser = %s",newuser);
		writeonfile(newuser);
		close(fd);
		return 1;
		//exit();
	}
}
 




int main(int argc, char *argv[]){
	static char passwd[30];
	static char npasswd[30];
	
	
	
	
	if(argc >= 2){
		if(!user_exist(argv[1])){
			while(getcmd(passwd, sizeof(passwd)) >= 0){
			break;
		}
		//printf(1,"pass %s",passwd);
		int j=0;
		while( j< (strlen(passwd)-1)){
			npasswd[j] = passwd[j];
			j++;
		}
			user_add(argv[1],npasswd);
		}
	}else{
		printf(1,"please write the new user name next time\n");
	}
	exit();
}
