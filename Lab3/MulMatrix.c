#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <time.h>

void ClientSockeWork(void);
void readresult(void);

FILE *f;
FILE *out;
 int N=100;
int i,j;
int F=0;
int sock;
int mainpart;
int ostatok;
int count;
int h;
int id;
int keysem;
int *Result;

int main(int argc, char *argv[]) {

if (argc == 1)
{
  printf("Введите количество потокв\n");
  exit(1);
}
else
{
  F=atoi(argv[1]);
}
f=fopen("inp.txt","w");
//fscanf(f, "%d",&N);
fprintf(f, "%d\n", N);
srand( time( NULL ) );

int Matrix[N][N];
int Vector[N];
int Thread[F];
int num;
Result=(int*)malloc(N * sizeof(int));

for (i=0;i<N;i++)
   {
     num = rand() % 10 + 1;
     Vector[i]=num;
    fprintf(f, "%d ", num);
   }
  fprintf(f, "\n");
for(i=0;i<N;i++)
{
  for(j=0;j<N;j++)
   {
     num = rand() % 10 + 1;
    Matrix[i][j]= num;
    fprintf(f, "%d ", num);
   }
   fprintf(f, "\n");
}
fclose(f);
printf("End");
for (i=0; i<N;i++)
        printf("%d ",Vector[i]);

for (i=0; i<N;i++)
{
  for (j=0; j<N;j++)
        printf("%d ",Matrix[i][j]);
printf("\n");
}


int mainsocket;
struct sockaddr server;
 server.sa_family = AF_UNIX;
 strcpy(server.sa_data, "s.soc");


if ((mainsocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
       {
		perror("socket error");
		exit(1);
	}

if (bind(mainsocket, &server, sizeof(server)) == -1) {
		perror("bind error");
		exit(1);
	}

if (listen(mainsocket, F) == -1) {
		perror("listen error");
		exit(1);
	}

mainpart = N/F;
ostatok = N%F;
count=0;
int M=0;

//Запуск дополнительных потоков
for (i=0; i<F;i++){
  if (fork() == 0) {
    ClientSockeWork();
    exit(0);
  }
}

for (h=0;h<F;h++) 
{
    if ((sock = accept(mainsocket, NULL, NULL)) == -1) {
	//perror("accept error");
       // exit(1);
	//continue;
   }
   Thread[h]=sock;
  if (ostatok!=0) 
    {
      ostatok--;
      M=mainpart+1;
    }
    else M=mainpart;
   int a[M][N];
   for (i=count;i<count+M;i++)
     for (j=0; j<N;j++)
       a[i-count][j]=Matrix[i][j];
   int KolStrIndex[2]; KolStrIndex[0] = count; KolStrIndex[1]=M; 
   send(sock, KolStrIndex, 2*sizeof(int), 0);   
   send(sock, a, M*N*sizeof(int), 0);
   send(sock, Vector, N*sizeof(int), 0);
   count+=M;
   }
for (h=0;h<F;h++) 
{
        int res[N+2];
	recv( Thread[h], res, (N+2)*sizeof(int),0); 
        //printf("Индекс %d, Строк %d\n",res[0],res[1]);
        int c;
       // printf("От %d до %d\n",res[0],res[0]+res[1]);#include <time.h>
        for(c=0;c<res[1];c++)
        {
           Result[res[0]+c]=res[c+2];
         //  printf("Result %d %d\n",res[0]+c,res[c+2]);
        }
close(Thread[h]);
}

out=fopen("out.txt","w");
 int s;
 printf("Результат\n");
for (s=0;s<N;s++)
 fprintf(out,"%d ",Result[s]);
 remove("s.soc");
fclose(out);
return(0);
}


void ClientSockeWork()
{
int clientsocket;
int k,l;
struct sockaddr client;
 client.sa_family = AF_UNIX;
 strcpy(client.sa_data, "s.soc");

if ((clientsocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}

if (connect(clientsocket, &client, sizeof(client)) == -1) {
		perror("connect error");
		exit(1);
	}

   // send(clientsocket, message, sizeof(message), 0);
    int KolStrIndex[2];
    int index;
    int M;
    recv(clientsocket, KolStrIndex, 2*sizeof(int), 0);
    index=KolStrIndex[0]; M=KolStrIndex[1];
    int bufMat[M][N];
    recv(clientsocket, bufMat, M*N*sizeof(int), 0);
    int bufVec[N];
    recv(clientsocket, bufVec, N*sizeof(int), 0);
   
    int result[M+2]; result[0]=index; result[1]=M;
    for (k=0;k<M;k++)
     {
       result[k+2]=0;
       for (l=0;l<N;l++)
         result[k+2]+=bufMat[k][l]*bufVec[l];
     //  printf("%d: %d\n",M,result[k+2]);

     }
  send(clientsocket, result, (M+2)*sizeof(int), 0);
  send(clientsocket, result, (M+2)*sizeof(int), 0);
  close(clientsocket);
  exit(0);
}


