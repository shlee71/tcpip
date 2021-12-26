/* 
 * endian_conv.c
 * h : host byte order, n : network byte order
 * s : short ( 16 bit), l : long ( 32bit )
 */

 #include <stdio.h>
 #include <arpa/inet.h>

 int main( int argc, char **argv)
 {
     short host_port_order = 0x1234;
     short net_port_order;

     long host_addr_order = 0x12345678;
     long net_addr_order;

     net_port_order = htons(host_port_order);
     net_addr_order = htonl(host_addr_order);
     
     // Little endian of HOST
     printf(" Host ordered port : %x \n", host_port_order);
     // Big endian of Network
     printf(" Network ordered prot : %x \n", net_port_order);

     printf(" Host ordered Address : %x \n", host_addr_order);
     printf(" Network ordered Address : %x \n", net_addr_order);

     return 0;

 }