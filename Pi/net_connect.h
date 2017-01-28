#if !defined( NET_CONNECT_H )
#define NET_CONNECT_H

class net_connect
{
public:

   net_connect();

   ~net_connect();

   void test();

private:

   net_connect( const net_connect & copy );
   net_connect &operator =( const net_connect &rhs );

};

#endif
