
#include   "tx_api.h"
#include   "nx_api.h"
#include   "nxd_dhcp_client.h"
#include   "nxd_dhcp_server.h"
#include   "netx_dhcp_clone_function.h"

extern void    test_control_return(UINT status);

#if !defined(NX_DISABLE_IPV4)

#define     DEMO_STACK_SIZE             4096
#define     NX_PACKET_SIZE              1536
#define     NX_PACKET_POOL_SIZE         NX_PACKET_SIZE * 8

#define     NX_DHCP_SERVER_IP_ADDRESS_0 IP_ADDRESS(10,0,0,1)   
#define     START_IP_ADDRESS_LIST_0     IP_ADDRESS(10,0,0,10)
#define     END_IP_ADDRESS_LIST_0       IP_ADDRESS(10,0,0,19)
#define     CLIENT_IP_ADDRESS           IP_ADDRESS(10,0,0,15)

#define     NX_DHCP_SUBNET_MASK_0       IP_ADDRESS(255,255,255,0)
#define     NX_DHCP_DEFAULT_GATEWAY_0   IP_ADDRESS(10,0,0,1)
#define     NX_DHCP_DNS_SERVER_0        IP_ADDRESS(10,0,0,1)

/* If defined, the host requests a (previous) client IP address. */

#define REQUEST_CLIENT_IP


/* Define the ThreadX and NetX object control blocks...  */
static TX_THREAD               client_thread;
static NX_PACKET_POOL          client_pool;
static NX_IP                   client_ip;
static NX_DHCP                 dhcp_client;

static TX_THREAD               server_thread;
static NX_PACKET_POOL          server_pool;
static NX_IP                   server_ip;
static NX_DHCP_SERVER          dhcp_server;

/* Define the counters used in the demo application...  */

static ULONG                   state_changes;
static ULONG                   error_counter;
static CHAR                    *pointer;

static UCHAR message[50] = "My Ping Request!" ;


/* Define thread prototypes.  */

static void    server_thread_entry(ULONG thread_input);
static void    client_thread_entry(ULONG thread_input);
static void    dhcp_state_change(NX_DHCP *dhcp_ptr, UCHAR new_state);
static VOID    my_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);

/******** Optionally substitute your Ethernet driver here. ***********/
extern void    _nx_ram_network_driver_1024(struct NX_IP_DRIVER_STRUCT *driver_req);

/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_skip_discover_test_application_define(void *first_unused_memory)
#endif
{

UINT    status;


    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Create the client thread.  */
    tx_thread_create(&client_thread, "thread client", client_thread_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Create the server thread.  */
    tx_thread_create(&server_thread, "thread server", server_thread_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create the client packet pool.  */
    status =  nx_packet_pool_create(&client_pool, "NetX Main Packet Pool", 1024, pointer, NX_PACKET_POOL_SIZE);
    pointer = pointer + NX_PACKET_POOL_SIZE;

    /* Check for pool creation error.  */
    if (status)
        error_counter++;
    
    /* Create the server packet pool.  */
    status =  nx_packet_pool_create(&server_pool, "NetX Main Packet Pool", 1024, pointer, NX_PACKET_POOL_SIZE);
    pointer = pointer + NX_PACKET_POOL_SIZE;

    /* Check for pool creation error.  */
    if (status)
        error_counter++;

    /* Create an IP instance for the DHCP Client.  */
    status = nx_ip_create(&client_ip, "DHCP Client", IP_ADDRESS(0, 0, 0, 0), 0xFFFFFF00UL, &client_pool, _nx_ram_network_driver_1024, pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
        error_counter++;
    
    /* Create an IP instance for the DHCP Server.  */
    status = nx_ip_create(&server_ip, "DHCP Server", NX_DHCP_SERVER_IP_ADDRESS_0, 0xFFFFFF00UL, &server_pool, _nx_ram_network_driver_1024, pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for DHCP Client IP.  */
    status =  nx_arp_enable(&client_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;
    
    /* Enable ARP and supply ARP cache memory for DHCP Server IP.  */
    status =  nx_arp_enable(&server_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;

    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&client_ip);

    /* Check for UDP enable errors.  */
    if (status)
        error_counter++;
    
    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&server_ip);

    /* Check for UDP enable errors.  */
    if (status)
        error_counter++;

    /* Enable ICMP.  */
    status =  nx_icmp_enable(&client_ip);

    /* Check for errors.  */
    if (status)
        error_counter++;

    /* Enable ICMP.  */
    status =  nx_icmp_enable(&server_ip);

    /* Check for errors.  */
    if (status)
        error_counter++;

    return;
}

/* Define the test threads.  */

void    server_thread_entry(ULONG thread_input)
{

UINT        status;
UINT        iface_index;
UINT        addresses_added;

    printf("NetX Test:   DHCP Skip Discover Test...................................");

    /* Create the DHCP Server.  */
    status =  nx_dhcp_server_create(&dhcp_server, &server_ip, pointer, DEMO_STACK_SIZE, 
                                   "DHCP Server", &server_pool);
    
    pointer = pointer + DEMO_STACK_SIZE;
    
    /* Check for errors creating the DHCP Server. */
    if (status)
        error_counter++;

    /* Load the assignable DHCP IP addresses for the first interface.  */
    iface_index = 0;

    status = nx_dhcp_create_server_ip_address_list(&dhcp_server, iface_index, START_IP_ADDRESS_LIST_0, 
                                                   END_IP_ADDRESS_LIST_0, &addresses_added);

    /* Check for errors creating the list. */
    if (status)
    {
        error_counter++;
    }

    /* Verify all the addresses were added to the list. */
    if (addresses_added != 10)
    {
        error_counter++;
    }

    status = nx_dhcp_set_interface_network_parameters(&dhcp_server, iface_index, NX_DHCP_SUBNET_MASK_0, 
                                                      NX_DHCP_DEFAULT_GATEWAY_0, NX_DHCP_DNS_SERVER_0);

    /* Check for errors setting network parameters. */
    if (status)
    {
        error_counter++;
    }

    /* Change the pointer function to check user options.  */
    server_ip.nx_ip_udp_packet_receive = my_udp_packet_receive;

    /* Start DHCP Server task.  */
    status = nx_dhcp_server_start(&dhcp_server);

    /* Check for errors starting up the DHCP server.  */
    if (status)
    {
        error_counter++;
    }
    
    tx_thread_sleep(20 * NX_IP_PERIODIC_RATE);

    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {
        printf("SUCCESS!\n");
        test_control_return(0);
    }

    return;
}

UINT length;

/* Define the test threads.  */

void    client_thread_entry(ULONG thread_input)
{

UINT        status;
NX_PACKET   *my_packet;  
UCHAR       buffer[4];
UINT        buffer_size = 4;
ULONG       *long_ptr;
ULONG       requested_ip;
UINT        skip_discover_message = NX_TRUE;


    /* Create the DHCP instance.  */
    status =  nx_dhcp_create(&dhcp_client, &client_ip, "dhcp_client");
    if (status)
        error_counter++;

#ifdef NX_DHCP_CLIENT_USER_CREATE_PACKET_POOL
    status = nx_dhcp_packet_pool_set(&dhcp_client, &client_pool);
    if (status)
        error_counter++;
#endif /* NX_DHCP_CLIENT_USER_CREATE_PACKET_POOL  */

    /* Set the client IP if the host is configured to do so. */
    requested_ip = (ULONG)CLIENT_IP_ADDRESS;

    /* Request a specific IP address using the DHCP client address option. */
    status = nx_dhcp_request_client_ip(&dhcp_client, requested_ip, skip_discover_message);
    if (status)
        error_counter++;

    /* Register state change variable.  */
    status =  nx_dhcp_state_change_notify(&dhcp_client, dhcp_state_change);
    if (status)
        error_counter++;

    /* Start the DHCP Client.  */
    status =  nx_dhcp_start(&dhcp_client);
    if (status)
        error_counter++;
                           
    /* Check for address resolution.  */
    status =  nx_ip_status_check(&client_ip, NX_IP_ADDRESS_RESOLVED, (ULONG *) &status, 20 * NX_IP_PERIODIC_RATE);

    /* Check status.  */
    if (status)              
        error_counter++;

    /* Get the DNS Server address.  */
    status = nx_dhcp_user_option_retrieve(&dhcp_client, NX_DHCP_OPTION_DNS_SVR, buffer, &buffer_size);

    /* Check status.  */
    if (status == NX_SUCCESS)
    {

        /* Set the pointer.  */
        long_ptr = (ULONG *)(buffer);

        /* Check the DNS address.  */
        if (*long_ptr != NX_DHCP_DNS_SERVER_0)
            error_counter++;
    }
    else
    {
        error_counter++;
    }

    /* Get the lease time value.  */
    status = nx_dhcp_user_option_retrieve(&dhcp_client, NX_DHCP_OPTION_DHCP_LEASE, buffer, &buffer_size);

    /* Check status.  */
    if (status == NX_SUCCESS)
    {

        /* Set the pointer.  */
        long_ptr = (ULONG *)(buffer);

        /* Check the lease time value.  */
        if (*long_ptr != NX_DHCP_DEFAULT_LEASE_TIME)
            error_counter++;
    }
    else
    {
        error_counter++;
    }

    length = sizeof(message);

    /* Send pings to another host on the network...  */
    status =  nx_icmp_ping(&client_ip, NX_DHCP_SERVER_IP_ADDRESS_0, (CHAR *)message, length, &my_packet, NX_IP_PERIODIC_RATE);
    if (status)
        error_counter++;
    else
        nx_packet_release(my_packet);

    /* Stopping the DHCP client. */
    nx_dhcp_stop(&dhcp_client);

    /* All done. Return resources to NetX and ThreadX. */    
    nx_dhcp_delete(&dhcp_client);

    return;
}

VOID    my_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr)
{

UCHAR  *server_id = NX_NULL;

    /* Search the Class ID option.  */
    server_id = dhcp_search_buffer((packet_ptr -> nx_packet_prepend_ptr + 8), NX_DHCP_OPTION_DHCP_SERVER, (packet_ptr -> nx_packet_length - 8));

    /* Check if have the Class ID option.  */
    if (server_id != NX_NULL)
        error_counter++;

    /* Call the actual function to receive the UDP packet.  */
    _nx_udp_packet_receive(ip_ptr, packet_ptr);
}

void dhcp_state_change(NX_DHCP *dhcp_ptr, UCHAR new_state)
{

    /* Increment state changes counter.  */
    state_changes++;

    return;
}
#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_skip_discover_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   NetX DHCP Skip Discover Test..............................N/A\n"); 

    test_control_return(3);  
}      
#endif
