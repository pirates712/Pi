#if !defined( NET_CONNECT_H )
#define NET_CONNECT_H

#include "threadSafeList.h"

class net_connect
{
public:

   net_connect( unsigned int buffsize );

   ~net_connect();

   void receive( int port );

   char * getBuffer();
   unsigned int getBufferSize();

private:

   unsigned int mBuffSize;
   net_connect( const net_connect & copy );
   net_connect &operator =( const net_connect &rhs );

   threadSafeList<char *> mTSList;

};

void start_receive( net_connect * n, int port );

#endif
