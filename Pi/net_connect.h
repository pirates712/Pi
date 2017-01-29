#if !defined( NET_CONNECT_H )
#define NET_CONNECT_H

#include <mutex>
#include <condition_variable>
#include <list>

class net_connect
{
public:

   net_connect( unsigned int buffsize );

   ~net_connect();

   void receive( int port );

   char * getBuffer();
   unsigned int getBufferSize();

private:
   std::mutex bufListLck;
   std::condition_variable bufListCV;
   unsigned int mNumBufs;
   unsigned int mBuffSize;
   net_connect( const net_connect & copy );
   net_connect &operator =( const net_connect &rhs );
   std::list<char *> buffers;

};

void start_receive( net_connect * n, int port );

#endif
