#if !defined( NET_CONNECT_H )
#define NET_CONNECT_H

#include "threadSafeList.h"
#include <mutex>

class net_connect
{
public:

   net_connect( unsigned int buffsize, unsigned int portno );

   ~net_connect();

   void receive();

   int waitForConnection();

   bool isConnected();

   char * getBuffer();
   unsigned int getBufferSize();

private:

   unsigned int mBuffSize;
   net_connect( const net_connect & copy );
   net_connect &operator =( const net_connect &rhs );

   threadSafeList<char *> mTSList;

   bool mConnected;
   int  mSockfd;
   unsigned int mPort;

   std::mutex mConStateLck;

};

void start_receive( net_connect * n );

#endif
