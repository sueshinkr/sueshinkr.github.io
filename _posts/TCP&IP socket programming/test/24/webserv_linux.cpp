// webserv_linux.cpp

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE	2048
#define BUF_SMALL	100

void*	request_handler(void *arg);
char*	content_type(char *file);
void	send_data(FILE *fp, char *ct, char *filename);
void	send_error(FILE *fp);
void	serror_handling(char *message);

int	main(int argc, char *argv[])
{
	int					serv_sock, clnt_sock;
	struct sockaddr_in	serv_adr, clnt_adr;
	socklen_t			clnt_adr_size;
	char				buf[BUF_SIZE];
	pthread_t			t_id;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		serror_handling("bind() error");

	if (::listen(serv_sock, 20) == -1)
		serror_handling("listen() error");

	//	요청 및 응답
	while (1)
	{
		clnt_adr_size = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &clnt_adr_size);
		cout << "Connection Request : " << inet_ntoa(clnt_adr.sin_addr) << " : " << ntohs(clnt_adr.sin_port) << endl;
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		pthread_detach(t_id);
	}
	close(serv_sock);
	return 0;
}

void*	request_handler(void *arg)
{
	int		clnt_sock = *((int*)arg);
	char	req_line[BUF_SMALL];
	FILE	*clnt_read;
	FILE	*clnt_write;

	char	method[BUF_SMALL];
	char	ct[BUF_SMALL];
	char	file_name[BUF_SMALL];

	clnt_read = fdopen(clnt_sock, "r");
	clnt_write = fdopen(dup(clnt_sock), "w");
	fgets(req_line, BUF_SMALL, clnt_read);
	if (strstr(req_line, "HTTP/") == NULL)
	{
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return NULL;
	}

	strcpy(method, strtok(req_line, " /"));
	strcpy(file_name, strtok(NULL, " /"));	// 요청 파일이름 확인
	strcpy(ct, content_type(file_name));	// Content-type 확인
	if (strcmp(method, "GET"))				// Get 방식 요청인지 확인
	{
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return NULL;
	}
	
	fclose(clnt_read);
	send_data(clnt_write, ct, file_name);	// 응답
}

void	send_data(FILE *fp, char *ct, char *file_name)
{
	char	protocol[] = "HTTP/1.0 200 OK\r\n";
	char	server[] = "Server : simple web server\r\n";
	char	cnt_len[] = "Content-length : 2048\r\n";
	char	cnt_type[BUF_SMALL];
	char	buf[BUF_SIZE];
	FILE	*send_file;

	sprintf(cnt_type, "Content-type : %s\r\n\r\n", ct);
	if ((send_file = fopen(file_name, "r")) == NULL)
	{
		send_error(fp);
		return ;
	}

	// 헤더 정보 전송
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	// 요청 데이터 전송
	while (fgets(buf, BUF_SIZE, send_file) != NULL)
	{
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);
}

void	send_error(FILE *fp)	// 오류 발생시 메시지 전달
{
	char	protocol[] = "HTTP/1.0 400 Bad Request\r\n";
	char	server[] = "Server : siple web server\r\n";
	char	cnt_len[] = "Content-length : 2048\r\n";
	char	cnt_type[] = "Content-type : text/html\r\n\r\n";
	char	content[] = "<html><head><title>NETWORK</title></head>"
		"<body><font size = +5><br>오류 발생! 요청 파일명 및 요청 방식 확인!"
		"</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fflush(fp);
}

char*	content_type(char *file)	// Content-Type 구분
{
	char	extension[BUF_SMALL];
	char	file_name[BUF_SMALL];
	
	strcpy(file_name, file);
	strtok(file_name, ".");
	strcpy(extension, strtok(NULL, "."));
	if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
		return "text/html";
	else
		return "text/plain";
}

void	serror_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}