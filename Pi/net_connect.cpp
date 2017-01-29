#include "net_connect.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>

#include <string.h>

void start_receive( net_connect * n, int port )
{
   n->receive( port );
}

net_connect::net_connect(unsigned int buffsize)
{
    printf("Constructing net_connect Class! buffsize %u\n", buffsize);
    mBuffSize = buffsize;
    mNumBufs = 0;
}

net_connect::~net_connect()
{
}

// You must free the buffer you receive!!!!!!!!!!!
char * net_connect::getBuffer()
{
   char * retBuffer = NULL;
   //printf("getting buf\n");
   std::unique_lock<std::mutex> lk(bufListLck);
   bufListCV.wait(lk, [&]{return (mNumBufs > 0);});

   retBuffer = buffers.front();
   buffers.pop_front();
   mNumBufs--;
   //printf("- %u bufs\n", mNumBufs);
   lk.unlock();
   return retBuffer;
}

unsigned int net_connect::getBufferSize()
{
   return mBuffSize;
}

void net_connect::receive( int port )
{
   int sockfd, newsockfd, portno;
   socklen_t clilen;
   char * newbuffer = NULL;
   struct sockaddr_in serv_addr, cli_addr;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0)
   {
      perror("ERROR opening socket");
      exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = port;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      perror("ERROR on binding");
      exit(1);
   }

   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

   if (newsockfd < 0)
   {
      perror("ERROR on accept");
      exit(1);
   }

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
      n = read( newsockfd,newbuffer,mBuffSize );
      if( n == 0 )
      {
         break;
      }

      if (n < 0)
      {
         perror("ERROR reading from socket");
         exit(1);
      }

      //printf("Here is the message: %d %d %d %d\n",newbuffer[0], newbuffer[1], newbuffer[2], newbuffer[3]);

      {
         std::lock_guard<std::mutex> lk(bufListLck);
         buffers.push_back( newbuffer );
         mNumBufs++;
         //printf("+ %u bufs\n", mNumBufs);
      }
      bufListCV.notify_one();

      newbuffer = NULL;

      // arbitrary limit
      if( buffers.size() > 15 )
      {
         printf("Too many buffers allocated, exiting...\n");
         break;
      }
   }

   /* Write a response to the client */
   // n = write(newsockfd,"I got your message",18);
   //
   // if (n < 0)
   // {
   //    perror("ERROR writing to socket");
   //    exit(1);
   // }

   return;
}
