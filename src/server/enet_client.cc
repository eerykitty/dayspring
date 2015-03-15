#include <enet/enet.h>
#include <iostream>
#include <string>
#include "ige_sync.pb.h"
#include "mid.hh"
#include "message.hh"

using namespace std;



int main()
{
        enet_initialize ();
        ENetHost * client;
        client = enet_host_create (NULL /* create a client host */,
                        1 /* only allow 1 outgoing connection */,
                        2 /* allow up 2 channels to be used, 0 and 1 */,
                        0,
                        0);
        if (client == NULL)
        {
                fprintf (stderr,
                                "An error occurred while trying to create an ENet client host.\n");
                exit (EXIT_FAILURE);
        }

        ENetAddress address;
        ENetEvent event;
        ENetPeer *peer;
        enet_address_set_host (& address, "127.0.0.1");
        address.port = 1234;
        peer = enet_host_connect (client, & address, 2, 0);
        if (enet_host_service (client, & event, 5000) > 0 &&
                        event.type == ENET_EVENT_TYPE_CONNECT)
        {
printf ("WE IN\n");
                uint32_t* x = malloc(sizeof(uint32_t)+sizeof(char));
                *uint32_t = 3;
                ENetPacket * packet = enet_packet_create (x,
                                sizeof(uint32_t)+sizeof(char),
                                ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send (peer, 0, packet);
                enet_host_flush (client);

                while(1)
                {
                        enet_host_service (client, & event, 3000);
                        if (event.type == ENET_EVENT_TYPE_DISCONNECT)
                        {
                                return 0;
                        }
                        else if (event.type == ENET_EVENT_TYPE_RECEIVE)
                        {
                                printf ("message id -> %u, message -> %s\n", *((uint32_t*)event.packet -> data), (char*)((uint32_t*)(event.packet -> data) + 1));
                                enet_packet_destroy (event.packet);
                        }
                }
                //enet_peer_disconnect (peer, 0);
                /* Allow up to 3 seconds for the disconnect to succeed
                 * and drop any packets received packets.
                 */
                /*while (enet_host_service (client, & event, 3000) > 0)
                {
                        switch (event.type)
                        {
                                case ENET_EVENT_TYPE_RECEIVE:
                                        enet_packet_destroy (event.packet);
                                        break;
                                case ENET_EVENT_TYPE_DISCONNECT:
                                        puts ("Disconnection succeeded.");
                                        return 0;
                        }
                }*/
        }


                enet_host_destroy(client);

                enet_deinitialize ();
                return 0;
        }
