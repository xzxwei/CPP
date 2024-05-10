#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>   //线程库
#include <mutex>
#define MAX_LEN 200  //消息最大长度
#define NUM_COLORS 6

using namespace std;

struct terminal
{
	int id;
	string name;
	int socket;
	thread th;    //线程对象
};

vector<terminal> clients;    //存储所有客户端
string def_col="\033[0m";    
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;     //用于生成客户端id
mutex cout_mtx,clients_mtx;   

string color(int code);
void set_name(int id, char name[]);
void shared_print(string str, bool endLine);
int broadcast_message(string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);

int main()
{
	int server_socket;   //创建一个ipv4 tcp套接字
	if((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1)   
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in server;  //sockaddr_in为用于互联网地址的结构体，指定服务器的地址和端口信息
	server.sin_family=AF_INET;   //地址族，对于ipv4设为AF_INET
	server.sin_port=htons(10000);//主机字节顺序的端口号10000转换为网络字节顺序，因为网络协议标准使用大端字节序，大多数主机用小端字节序
	server.sin_addr.s_addr=INADDR_ANY;   //接受来自本机任何网络接口的连接
	bzero(&server.sin_zero,0);  //清0 sin_zero

	//将套接字与具体的网络地址绑定；将sockaddr_in类型转换位sockaddr类型，因为bind接受一个指向sockaddr的指针
	if((bind(server_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1)
	{
		perror("bind error: ");
		exit(-1);
	}

	//让这个套接字进入监听模式，最大只能接受8个连接
	if((listen(server_socket,8))==-1)
	{
		perror("listen error: ");
		exit(-1);
	}

	//客户端信息
	struct sockaddr_in client;
	int client_socket;   
	unsigned int len=sizeof(sockaddr_in);

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<endl<<def_col;

	while(1)
	{
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))==-1)//接受连接请求，client用于储存接受的连接的客户端地址信息
		{
			perror("accept error: ");
			exit(-1);
		}
		seed++;   //记录是连接的第几个客户端
		thread t(handle_client,client_socket,seed);
		lock_guard<mutex> guard(clients_mtx);  //自动锁定和解锁clients_mtx，确保clients的列表修改
		clients.push_back({seed, string("Anonymous"),client_socket,(move(t))});
	}

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Set name of client
void set_name(int id, char name[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].name=string(name);
			}
	}	
}

// For synchronisation of cout statements
void shared_print(string str, bool endLine=true)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout<<str;
	if(endLine)
			cout<<endl;
}

// Broadcast message to all clients except the sender
int broadcast_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}		
}

// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id)
{
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}		
}

void end_connection(int id)
{
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id==id)	
		{
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handle_client(int client_socket, int id)
{
	char name[MAX_LEN],str[MAX_LEN];
	recv(client_socket,name,sizeof(name),0);  //从client_socket接受数据到name数组
	set_name(id,name);	

	// Display welcome message
	string welcome_message=string(name)+string(" has joined");
	broadcast_message("#NULL",id);	
	broadcast_message(id,id);								
	broadcast_message(welcome_message,id);	
	shared_print(color(id)+welcome_message+def_col);  //服务器输出欢迎信息
	
	while(1)
	{
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		if(bytes_received<=0)
			return;
		if(strcmp(str,"#exit")==0)
		{
			// Display leaving message
			string message=string(name)+string(" has left");		
			broadcast_message("#NULL",id);			
			broadcast_message(id,id);						
			broadcast_message(message,id);
			shared_print(color(id)+message+def_col);
			end_connection(id);							
			return;
		}
		broadcast_message(string(name),id);					
		broadcast_message(id,id);		
		broadcast_message(string(str),id);
		shared_print(color(id)+name+" : "+def_col+str);		
	}	
}
