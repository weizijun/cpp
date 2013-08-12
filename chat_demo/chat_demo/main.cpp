
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

#include <iostream>

using namespace std;

void readcb(bufferevent *bev,void *ptr)
{
	char buf[1024];
	int n;
	evbuffer *input = bufferevent_get_input(bev);
	while((n = evbuffer_remove(input,buf,sizeof(buf))) > 0)
	{
		fwrite(buf,1,n,stdout);
		bufferevent_write(bev, buf, n);
	}

	char MESSAGE[] = "Hello, KyLin!\n";
	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

void writecb(struct bufferevent *bev, void *user_data)
{
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		//bufferevent_free(bev);
	} else {
		printf("answer\n");
	}

	char MESSAGE[] = "Hello, Weizijun!\n";
	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
	Sleep(1000);
}

void eventcb(bufferevent *bev,short events,void *ptr)
{
	if (events & BEV_EVENT_CONNECTED)
	{
		cout << "Connect okay" << endl;
	} 
	else if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF))
	{
		event_base *base = (event_base *)ptr;
		if (events & BEV_EVENT_ERROR)
		{
			fprintf(stderr, "BEV_EVENT_ERROR|BEV_EVENT_EOF!");
		}
		bufferevent_free(bev);
		event_base_loopexit(base,NULL);
	}	
}

int main()
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event_base *base;
	struct bufferevent *bev;

	base = event_base_new();

	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);

	if(NULL == bev)
	{
		fprintf(stderr, "Could not initialize libevent!");
		return -1;
	}

	bufferevent_setcb(bev,readcb,writecb,eventcb,base);
	if (0 != bufferevent_enable(bev,EV_READ|EV_WRITE))
	{
		fprintf(stderr, "bufferevent_enable error");
		return -1;
	}

	struct sockaddr_in sin;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(9995);

	bufferevent_socket_connect(bev,(sockaddr *)&sin,sizeof(sin));

	event_base_dispatch(base);
	return 0;
};