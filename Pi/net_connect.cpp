#include "net_connect.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>

#include <string.h>

void start_receive( net_connect * n )
{
   n->receive();
}

net_connect::net_connect(unsigned int buffsize, unsigned int portno)
{
    printf("Constructing net_connect Class! buffsize %u\n", buffsize);
    mBuffSize = buffsize;
    mConnected = false;
    mPort = portno;
}

net_connect::~net_connect()
{
}

bool net_connect::isConnected()
{
   bool connected;
   {
      std::lock_guard<std::mutex> lk(mConStateLck);
      connected = mConnected;
   }
   return connected;
}

// You must free the buffer you receive!!!!!!!!!!!
char * net_connect::getBuffer()
{
   return mTSList.getAndPopFront();
}

unsigned int net_connect::getBufferSize()
{
   return mBuffSize;
}

int net_connect::waitForConnection()
{
   int sockfd;
   socklen_t clilen;
   struct sockaddr_in serv_addr, cli_addr;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0)
   {
      perror("ERROR opening socket");
      return -1;
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(mPort);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      perror("ERROR on binding");
      return -1;
   }

   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   /* Accept actual connection from the client */
   mSockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

   if (mSockfd < 0)
   {
      perror("ERROR on accept");
      return -1;
   }

   //Set connection state on success
   {
      std::lock_guard<std::mutex> lk(mConStateLck);
      mConnected = true;
   }

   return 0;
}

void net_connect::receive()
{
   char * newbuffer = NULL;
   int  bytesRead;
   while(1)
   {
      /* If connection is established then start communicating */
      newbuffer = (char *)malloc( mBuffSize );
      if( newbuffer == NULL )
      {
         printf("Error! Unable to allocate memory for newbuffer!\n");
         break;
      }
      bzero(newbuffer,mBuffSize);
      int value = 0;
      bytesRead = read( mSockfd,newbuffer,mBuffSize );
      if( bytesRead == 0 )
      {
         {
            std::lock_guard<std::mutex> lk(mConStateLck);
            mConnected = false;
         }
         break;
      }

      if (bytesRead < 0)
      {
         perror("ERROR reading from socket");
         {
            std::lock_guard<std::mutex> lk(mConStateLck);
            mConnected = false;
         }
         break;
      }

      //printf("Here is the message: %d %d %d %d\n",newbuffer[0], newbuffer[1], newbuffer[2], newbuffer[3]);

      mTSList.push_back( newbuffer );

      newbuffer = NULL;

      // arbitrary limit
      if( mTSList.size() > 15 )
      {
         printf("Too many buffers allocated, exiting...\n");
         break;
      }
   }

   return;
}
