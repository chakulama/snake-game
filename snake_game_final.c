#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> 



typedef struct
{char  nombre[80];
int no_socket;
}usuari;

typedef struct
{int  num;
usuari usuarios[20];
}listaU;

// to create the list of players who are playing with each other
typedef struct
{char player_1[100];
char player_2[100];
}plist;

typedef struct
{int  num;
plist plistas[20];
}plista_lista;


//gobal variable
listaU mlista;
plista_lista nlista;






int param[7];
pthread_t thread[7];
pthread_mutex_t lock;


//list of player in a game
/*void list_of_player( listaL L, char player1[100], char player2[100])
{ 
	
	
}*/
void construir_lista (listaU l, char mensaje[80])
	/* Construye un mensaje de texto con los nombre de los usuarios que
	hay en la lista l, separados por una coma */
{
	int i=1;
	strcpy (mensaje, l.usuarios[0].nombre);
	
	while (i<l.num)
	{
		sprintf (mensaje, "%s,%s",mensaje,l.usuarios[i].nombre);
		i=i+1;
	}
}


int buscar (char nombre[10], listaU l)
	/* Devuelve la posición que ocupa en la lista el usuario cuyo nombre se recibe
	* como parámetro. Si el usuario no está en la lista devuelve un -1 */
{
	int encontrado = 0;
	int i=0;
	while ((i<l.num) &&(!encontrado))
	{
		if (strcmp (l.usuarios[i].nombre, nombre)==0)
			encontrado = 1;
		if (!encontrado)
			i=i+1;
	}
	if (encontrado)
		return i;
	else
		return -1;
}




int eliminar (int i, listaU*l)//delete the user from list
{
	int j;
	if (i>=20)
		return -1;
	else
	{
		j=i;
		while (j< l->num-1)
		{
			l->usuarios[j]= l-> usuarios[j+1];
			j=j+1;
		}
		l->num=l->num-1;
		return 0;
	}
}



int adduser (listaU *l, char *nombre, int num_sock )
{
	if (l->num==20)
	{
		printf("Lista llena\n");
		
		return -1;
	}
	else
	{
		strcpy(l->usuarios[l->num].nombre, nombre);
		l->usuarios[l->num].no_socket = num_sock;
		
		l->num++;
		
		return 1;
	}
	
}

int Buscar_socket(char name [15], listaU l)
{
	int encontrado = 0;
	int i=0;
	while ((i<l.num) &&(!encontrado))
	{
		if (strcmp (l.usuarios[i].nombre, name)==0)
			 
			
			encontrado = 1;
		if (!encontrado)
			i=i+1;
	}
	if (encontrado)
		return l.usuarios[i].no_socket;
	else
		return -1;
	
}

int buscar_num(char name[100] ,plista_lista p )
{
	int encontrado = 0;
	int i=0;
	while ((i<p.num) &&(!encontrado))
	{
		if (strcmp (p.plistas[i].player_1,name)==0)
			encontrado = 1;
		if (!encontrado)
			i=i+1;
	}
	if (encontrado)
		return i;
	else
		return -1;
}




char buscar_persona (int sock_no,listaU l)
{
	char return_value[100];
	
	int encontrado = 0;
	int i=0;
	while ((i<l.num) &&(!encontrado))
	{
		
			if (l.usuarios[i].no_socket == sock_no)
		{
				encontrado = 1;
				
		}
			if (!encontrado)
				i=i+1;
		
			
	}
	if (encontrado==1)
	{
	
	return_value[100] = l.usuarios[i].nombre [100];
	printf("player__1 name :%s  ,sock_no %d\n",return_value[100] ,sock_no);
	
		return  return_value[100];
	}
	
	


}






void *AtenderCliente(void *sock)
{
	//Conector para acceder al servidor de bases de datos 
	MYSQL *conn;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	//int encontrado=0;
	//Creamos una conexión al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL)
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexión, indicando nuestras claves de acceso // al servidor de bases de datos (user,pass)
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "prototipo1",0, NULL, 0);
	
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	int *s;
	s=(int *) sock;
	int sock_conn= *s; //int sock_conn = *((int *) sock);
	
	
	
	
	int ret;
	int res;
	char buff[512];
	char buff2[512];
	char buff3[512];
	
	
	// Ahora recibimos su nombre, que dejamos en buff
	ret=read(sock_conn,buff, sizeof(buff));
	printf ("Recibido\n");
	
	// Tenemos que anadirle la marca de fin de string 
	// para que no escriba lo que hay despues en el buffer
	buff[ret]='\0'; //fin de linia
	char consulta[1000];
	int i = 0;
	char Nombre[100] ;
	char Password[100]  ;
	char player1[100]  ;
	char player2[100]  ;
	char res_1 [100];
	
	char player__1[100] ;
	//int res =0;
	
	
	char mensaje [80];
	char mensaje_2[80];
	char *p = strtok (buff, "/");
	int codigo =  atoi (p);
	
	if (codigo==1)//register
	{
		p = strtok (NULL, "/");
		strcpy(Nombre,p);
		p = strtok (NULL, "/");
		strcpy(Password,p);
		strcpy (consulta, "INSERT INTO jugadores VALUES ('");
		strcat (consulta, Nombre);
		strcat (consulta, "','");
		strcat (consulta, Password);
		strcat (consulta, "')");		
		printf("%s\n",consulta);
		
		// Ahora ya podemos realizar la inserción
		err = mysql_query(conn, consulta);
		if (err!=0)
		{
			sprintf(buff2,"1/2,error use another username");
			write(sock_conn,buff2,strlen(buff2));
			printf ("Error al introducir datos la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			//exit (1);
			
		} 
		
		if (err==0)
		{
		sprintf(buff2,"1/1,successfully signed up ");
		printf("successfully signed up\n");
		write(sock_conn,buff2,strlen(buff2));
		}
		printf("Disconnected\n");
		
	}
	
	if (codigo==3)//login 
	{
		p = strtok (NULL, "/");
		strcpy(Nombre,p);
		p = strtok (NULL, "/");
		strcpy(Password,p);
		strcpy (consulta, "SELECT Nombre,Password FROM jugadores WHERE Nombre ='");
		strcat (consulta, Nombre);
		strcat (consulta, "'AND Password ='");
		//concatenamos el nombre
		strcat (consulta, Password);		
		strcat (consulta, "'");
		
		printf("%s\n",consulta);
		err=mysql_query (conn, consulta);
		if (err!=0)
		{
			printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
			exit (1);
		} 
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		if (row == NULL)
		{
			printf ("No se han obtenido datos en la consulta\n");
			sprintf(buff2, "3/0,username o password incorrect");
			write(sock_conn,buff2,strlen(buff2));
			printf("Disconnected\n");
			
		}
		else
		{	
			int encon = adduser(&mlista, Nombre,sock_conn);
			if (encon==1)
				printf ("successfully added in the list\n");
			//printf(mlista);
			construir_lista (mlista, mensaje);	
			sprintf (buff2, "2/%d,%s,",mlista.num,mensaje);
			
			for (i=0; i<mlista.num; i++)
			{ 
				printf("envio al %d\n ",i );
				pthread_mutex_lock (&lock);
				write ( mlista.usuarios[i].no_socket, buff2,strlen(buff2));//enviar las listas de connectados
				pthread_mutex_unlock (&lock);
			
			}	
			
			
			int encontrado = 0;
			sprintf(buff2,"3/1,login successfully");
			write(sock_conn,buff2,strlen(buff2));

			while (encontrado == 0 )
			{
				
				ret=read(sock_conn,buff, sizeof(buff));
				printf ("Recibido\n");
				buff[ret]='\0'; //fin de linia
				
				char *p = strtok (buff, "/");
				int codigo =  atoi (p);
				
				switch (codigo)
				{
				
				case 4: //sign out and delete users from list 
					{encontrado = 1;
					p = strtok (NULL, "/");
					strcpy(Nombre,p);
					int p= buscar (Nombre, mlista);
					eliminar (p, &mlista);
					construir_lista (mlista, mensaje);
					sprintf (buff2, "4/%d,40,%s,",mlista.num, mensaje);
					//(buff2);
					write ( sock_conn, buff2,strlen(buff2));//send to the player who wants to disconnect
					
					
					sprintf (buff3, "4/%d,41,%s,",mlista.num, mensaje);
					for (i=0; i<=mlista.num; i++)
					{	
						pthread_mutex_lock (&lock);
						write ( param[i], buff3,strlen(buff3));//enviar las listas de connectados
						pthread_mutex_unlock (&lock);
					}
					//pthread_mutex_lock (&lock);
					//n--;
					//pthread_mutex_unlock (&lock);
					
					close(sock_conn);
					break;
				}
					
					
				case 5://winners
					{err=mysql_query (conn, "SELECT winner FROM partidas");
					if (err!=0)
					{
						printf ("Error al consultar datos de la base %u %s\n",
								mysql_errno(conn), mysql_error(conn));
						exit (1);
					} 
					resultado = mysql_store_result (conn);
					row = mysql_fetch_row (resultado);
					if
						(row  == NULL)
						printf (
								"No se han obtenido datos en la consulta\n"); 
					else
						while(row !=NULL)
					{ 
							strcat (mensaje_2,row[0]);
							
							sprintf (mensaje_2, "%s,",mensaje_2);
							//char msg [500]= createmessage(mensaje_2);
							//sprintf (mensaje_2,",", mensaje_2);
							// obtenemos la siguiente fila
							row = mysql_fetch_row (resultado);
					}
						
						
						sprintf (buff2,"5/%s", mensaje_2);
						//printf("list of winner : \n",buff2);
						write(sock_conn,buff2,strlen(buff2));
						break;
				}
						
						
						
						
				case 6://invite player
					
					{
					p = strtok (NULL, "/");
					strcpy(player2,p);
					p = strtok (NULL, "/");
					strcpy(player1,p);
					
					//printf("%s\n",Nombre);
					int no_socket_res = Buscar_socket (player2, mlista);
					//printf("%d,player1:%s,palyer2:%s\n",no_socket_res,player1,player2);					
					sprintf (buff2, "6/%s do you like to join?,%s,",player2,player1);
					write ( no_socket_res, buff2,strlen(buff2));
					
					break;
					}
				
				case 7://response from player2 to player1
				
					{
						//printf("yes from client 7\n");
					//int number= 0;
					
					p = strtok (NULL, "/");
					int res =  atoi (p);
					//strcpy(res_1,p);
					p = strtok (NULL, "/");
					strcpy(player2,p);
					p = strtok (NULL, "/");
					strcpy(player1,p);
					int no_socket_res = Buscar_socket (player1,mlista);
					printf("%d,player1:%s,palyer2:%s,result:%d\n",no_socket_res,player1,player2,res);
					//solo para ver los numero de socket respecto con nombre
					for(int i=0;i<mlista.num; i++)
					{
						printf("%s--%d\n",mlista.usuarios[i].nombre,mlista.usuarios[i].no_socket);
					}
					
					if(res == 1)
					{
						
						
						strcpy(nlista.plistas[nlista.num].player_1, player1);
						strcpy(nlista.plistas[nlista.num].player_2, player2);
						nlista.num++;
						 
						
						sprintf (buff2,"7/yes, wanna join,");
						printf("%s yes \n,",buff2);
						
						
						write ( no_socket_res, buff2,strlen(buff2));
					}
					//if(res_1 == "no")
					else
					{
						sprintf (buff2, "7/no, dont wanna join,");
						printf("%s no \n,",buff2);
						write ( no_socket_res, buff2,strlen(buff2));
					}
					
					break;
					}
					
					
					case 8://player 1 send its dice sum value to player 2
						{
						
						//printf("player1 socket number: %d\n", sock_conn);
						p = strtok (NULL, "/");
						int res =  atoi (p);//number of dice of player 1
						p = strtok (NULL, "/");
						strcpy(player2,p);
						int no_socket_res = Buscar_socket (player2, mlista);
						
						
						
						
						
						
						
						if (res == 36 )
						{
							encontrado=0;
							i=0;
							while ((i<mlista.num) &&(!encontrado))
							{
								
								if (mlista.usuarios[i].no_socket == sock_conn)
								{
									encontrado = 1;	
								}
								if (encontrado==0)
									i=i+1;					
							}
							
							if (encontrado == 1)
							
								//player__1[100] = mlista.usuarios[i].nombre;
								//printf("lama 2    player1 %s,\n,",mlista.usuarios[i].nombre);
							
							printf("worked1\n");
							strcpy (consulta, "INSERT INTO partidas VALUES ('NULL','NULL','NULL','");
							strcat (consulta, mlista.usuarios[i].nombre);
							strcat (consulta, "')");		
							printf("%s\n",consulta);
							mysql_query (conn, consulta);
							printf("worked2\n");
						}
						
					     //player__1[100] = buscar_persona(sock_conn,mlista);						
						//int pnum = buscar_num (player__1,nlista); 						
						//int no_socket_res1 = Buscar_socket (nlista.plistas[pnum].player_2, mlista);
						//printf("%s %d  sock%d\n",player__1,pnum,no_socket_res1);
						//printf("Message  to palyer 2(socket no = %d ) from palyer 1:: %s \n",no_socket_res,buff2);
						
						
						sprintf (buff2, "8/%d,",res);
						write ( no_socket_res, buff2,strlen(buff2));
			
					
						break;
					}
					
						
				case 9:////player 2 sends its dice sum value to player 1
				{
							p = strtok (NULL, "/");
							int res =  atoi (p);//number of dice of player 1
							p = strtok (NULL, "/");
							strcpy(player1,p);
							
							

							int no_socket_res = Buscar_socket (player1, mlista);
							sprintf (buff2, "9/%d,",res);
							
							write ( no_socket_res, buff2,strlen(buff2));
							
							
							break;
						}
				
				//enviar password
				case 10:
			{
					//printf("form 2\n");
					p = strtok (NULL, "/");
					strcpy(Nombre,p);
					
					strcpy (consulta, "SELECT Password FROM jugadores WHERE Nombre ='");
					strcat (consulta, Nombre);
							
					strcat (consulta, "'");
					
					printf("%s\n",consulta);
					err=mysql_query (conn, consulta);
					
					if (err!=0)
					{
						printf ("Error al consultar datos de la base %u %s\n",
								mysql_errno(conn), mysql_error(conn));
						exit (1);
					} 
					resultado = mysql_store_result (conn);
					row = mysql_fetch_row (resultado);
					if (row == NULL)
					{
						printf ("No se han obtenido datos en la consulta\n");
						sprintf(buff2, "10/no hay usario con nombre %s",Nombre);
						write(sock_conn,buff2,strlen(buff2));
						//close(sock_conn);
					}
					else
					{	
					printf("password.%s\n",row[0]);
						sprintf(buff2,"10/password es:%s",row[0]);
						write(sock_conn,buff2,strlen(buff2));
					}
					
					break;
					
				}
				
			//nombre empizan por A
				case 11:
				{
				/*p = strtok (NULL, "/");
				strcpy(Nombre,p);*/
				
				strcpy (consulta, "SELECT Nombre FROM jugadores");
				printf("%s\n",consulta);
				err=mysql_query (conn, consulta);
				
				if (err!=0)
				{
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				} 
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
				{
					printf ("No se han obtenido datos en la consulta\n");
				
				}
				else
				{	
					while (row !=NULL)
					{
						printf ("nombre: %s\n",row[0]);
						strcpy(Nombre,row[0]);
						if(Nombre[0] == 'a')
							encontrado ++;
						row = mysql_fetch_row (resultado);
							
					}
					printf("number:%d\n",encontrado);
					sprintf(buff2,"11/%d",encontrado);
					write(sock_conn,buff2,strlen(buff2));
				}
				
				break;
				
				
				case 12:
					
					for (int i =0; i < nlista.num; i++)
					
					{
						sprintf (buff2,"12/list%d : %s %s ",i,nlista.plistas[i].player_1,nlista.plistas[i].player_2);
						write(sock_conn,buff2,strlen(buff2));
					}
					break;
					
				case 13:
					
					p = strtok (NULL, "/");
					strcpy(Nombre,p);// Nombre = text chat message 
					printf("%s\n",Nombre);
					sprintf(buff2,"13/%s,",Nombre);
					printf("%d\n",nlista.num);
					for (i=0; i<nlista.num+1; i++)
					{ 
						
						pthread_mutex_lock (&lock);
						printf("envio al %d\n ",i );
						write ( param[i], buff2,strlen(buff2));//enviar las listas de connectados
						pthread_mutex_unlock (&lock);
						
					}	
					
					
					write(sock_conn,buff2,strlen(buff2));
					break;
					
					//delete users from database
				case 14:
					
					encontrado=0;
					i=0;
					while ((i<mlista.num) &&(!encontrado))
					{
						
						if (mlista.usuarios[i].no_socket == sock_conn)
						{
							encontrado = 1;	
						}
						if (encontrado==0)
							i=i+1;					
					}
					
					if (encontrado == 1)
					{	
						//player__1[100] = mlista.usuarios[i].nombre;
						//printf("lama 2    player1 %s,\n,",mlista.usuarios[i].nombre);
						
						printf("worked1\n");
						strcpy (consulta, "DELETE FROM jugadores WHERE Nombre = '");
					//strcpy (consulta, "DELETE NaINSERT INTO partidas VALUES ('NULL','NULL','NULL','");
					strcat (consulta, mlista.usuarios[i].nombre);
					strcat (consulta, "'");		
					printf("%s\n",consulta);
					mysql_query (conn, consulta);
					printf("worked2\n");
					}
			
					
					break;
				
			}
			
		}	
}
}

}
}



int main ()
{
	
	//parte de threads y sockets
	//int param[7];
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9055);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	for(int i= 0; i<5;i++)// to connect different users
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		pthread_mutex_lock (&lock);
		param[i]=sock_conn;
		pthread_mutex_unlock (&lock);
		//sock_conn es el socket que usaremos para este cliente
		pthread_create(&thread[i],NULL,AtenderCliente,&param[i]);
	}	
	
	printf ("Espero a que acaben los threads \n");
	for (int i=0; i<7; i++)
		pthread_join(thread[i],NULL);
	printf ("Listo\n");
}




