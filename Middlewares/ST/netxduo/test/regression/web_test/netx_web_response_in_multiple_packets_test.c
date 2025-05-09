/* This case tests response in multiple packets.  */
#include    "tx_api.h"
#include    "nx_api.h"
#include    "fx_api.h"
#include    "nx_web_http_client.h"
#include    "nx_web_http_server.h"

extern void test_control_return(UINT);

#if !defined(NX_DISABLE_IPV4)

#include "test_device_cert.c"
#include "test_ca_cert.c"
#define ca_cert_der test_ca_cert_der
#define ca_cert_der_len test_ca_cert_der_len

#define     DEMO_STACK_SIZE         4096

/* Set up FileX and file memory resources. */
static CHAR             ram_disk_memory[4096];
static FX_MEDIA         ram_disk;
static UCHAR            media_memory[4096];

static UCHAR            server_stack[16000];

/* Define device drivers.  */
extern void _fx_ram_driver(FX_MEDIA *media_ptr);
extern void _nx_ram_network_driver_1024(NX_IP_DRIVER *driver_req_ptr);

/* Set up the HTTP client global variables. */

#define         CLIENT_PACKET_SIZE  (NX_WEB_HTTP_CLIENT_MIN_PACKET_SIZE * 2)

static TX_THREAD           client_thread;
static NX_PACKET_POOL      client_pool;
static NX_WEB_HTTP_CLIENT  my_client;
static NX_IP               client_ip;
static UINT                error_counter;

/* Set up the HTTP server global variables */

#define         SERVER_PACKET_SIZE  (NX_WEB_HTTP_SERVER_MIN_PACKET_SIZE * 2)

static NX_WEB_HTTP_SERVER  my_server;
static NX_PACKET_POOL      server_pool;
static TX_THREAD           server_thread;
static NX_IP               server_ip;
static NXD_ADDRESS         server_ip_address;
static UINT                http_server_start = 0;
static UINT                http_client_stop = 0;

static void thread_client_entry(ULONG thread_input);
static void thread_server_entry(ULONG thread_input);

#define HTTP_SERVER_ADDRESS  IP_ADDRESS(192,168,0,105)
#define HTTP_CLIENT_ADDRESS  IP_ADDRESS(192,168,0,123)

#ifdef NX_WEB_HTTPS_ENABLE
static UINT                https_server_start = 0;
static UINT                https_client_stop = 0;
static UINT loop = 2;
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers;
static CHAR crypto_metadata_server[20000 * NX_WEB_HTTP_SERVER_SESSION_MAX];
static CHAR crypto_metadata_client[20000 * NX_WEB_HTTP_SERVER_SESSION_MAX];
static UCHAR tls_packet_buffer[18500];
static NX_SECURE_X509_CERT certificate;
static NX_SECURE_X509_CERT trusted_certificate;
static NX_SECURE_X509_CERT remote_certificate, remote_issuer;
static UCHAR remote_cert_buffer[2000];
static UCHAR remote_issuer_buffer[2000];
#else
static UINT loop = 1;
#endif /* NX_WEB_HTTPS_ENABLE  */

static UINT server_request_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

static char pkt[] = {
0x48, 0x54,                                     /* ......HT */
0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x32, /* TP/1.0 2 */
0x30, 0x30, 0x20, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, /* 00 ..Con */
};

static char pkt1[] = {
0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, /* tent-Len */
0x67, 0x74, 0x68, 0x3a, 0x20, 0x20, 0x31, 0x32, /* gth:  12 */
0x31, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x74, 0x65, /* 1..Conte */
0x6e, 0x74, 0x2d, 0x54, 0x79, 0x70, 0x65, 0x3a, /* nt-Type: */
0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x70, 0x6c, /*  text/pl */
0x61, 0x69, 0x6e, 0x0d, 0x0a, 0x0d, 0x0a, 0x3c, /* ain....< */
0x68, 0x74, 0x6d, 0x6c, 0x3e, 0x0d, 0x0a, 0x0d, /* html>... */
0x0a, 0x3c, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0d, /* .<head>. */
0x0a, 0x0d, 0x0a, 0x3c, 0x74, 0x69, 0x74, 0x6c, /* ...<titl */
0x65, 0x3e, 0x4d, 0x61, 0x69, 0x6e, 0x20, 0x57, /* e>Main W */
0x69, 0x6e, 0x64, 0x6f, 0x77, 0x3c, 0x2f, 0x74, /* indow</t */
0x69, 0x74, 0x6c, 0x65, 0x3e, 0x0d, 0x0a, 0x3c, /* itle>..< */
0x2f, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0d, 0x0a, /* /head>.. */
0x0d, 0x0a, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x3e, /* ..<body> */
};

static char pkt2[] = {
0x0a, 0x3c, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0d, /* .<head>. */
0x0a, 0x0d, 0x0a, 0x3c, 0x74, 0x69, 0x74, 0x6c, /* ...<titl */
0x65, 0x3e, 0x4d, 0x61, 0x69, 0x6e, 0x20, 0x57, /* e>Main W */
0x69, 0x6e, 0x64, 0x6f, 0x77, 0x3c, 0x2f, 0x74, /* indow</t */
0x69, 0x74, 0x6c, 0x65, 0x3e, 0x0d, 0x0a, 0x3c, /* itle>..< */
0x2f, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0d, 0x0a, /* /head>.. */
0x0d, 0x0a, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x3e, /* ..<body> */
};

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_web_response_in_multiple_packets_test_application_define(void *first_unused_memory)
#endif
{
CHAR    *pointer;
UINT    status;


    error_counter = 0;

    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Create a helper thread for the server. */
    tx_thread_create(&server_thread, "HTTP Server thread", thread_server_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     NX_WEB_HTTP_SERVER_PRIORITY, NX_WEB_HTTP_SERVER_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create the server packet pool.  */
    status =  nx_packet_pool_create(&server_pool, "HTTP Server Packet Pool", SERVER_PACKET_SIZE, 
                                    pointer, SERVER_PACKET_SIZE*8);
    pointer = pointer + SERVER_PACKET_SIZE * 8;
    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&server_ip, "HTTP Server IP", HTTP_SERVER_ADDRESS, 
                          0xFFFFFF00UL, &server_pool, _nx_ram_network_driver_1024,
                          pointer, 4096, 1);
    pointer =  pointer + 4096;
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for the server IP instance.  */
    status = nx_arp_enable(&server_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;
    if (status)
        error_counter++;

     /* Enable TCP traffic.  */
    status = nx_tcp_enable(&server_ip);
    if (status)
        error_counter++;

    /* Create the HTTP Client thread. */
    status = tx_thread_create(&client_thread, "HTTP Client", thread_client_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              NX_WEB_HTTP_SERVER_PRIORITY + 2, NX_WEB_HTTP_SERVER_PRIORITY + 2, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;
    if (status)
        error_counter++;

    /* Create the Client packet pool.  */
    status =  nx_packet_pool_create(&client_pool, "HTTP Client Packet Pool", CLIENT_PACKET_SIZE, 
                                    pointer, CLIENT_PACKET_SIZE*8);
    pointer = pointer + CLIENT_PACKET_SIZE * 8;
    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&client_ip, "HTTP Client IP", HTTP_CLIENT_ADDRESS, 
                          0xFFFFFF00UL, &client_pool, _nx_ram_network_driver_1024,
                          pointer, 2048, 1);
    pointer =  pointer + 2048;
    if (status)
        error_counter++;

    status  = nx_arp_enable(&client_ip, (void *) pointer, 1024);
    pointer =  pointer + 2048;
    if (status)
        error_counter++;

     /* Enable TCP traffic.  */
    status = nx_tcp_enable(&client_ip);
    if (status)
        error_counter++;
}

#ifdef NX_WEB_HTTPS_ENABLE
/* Define the TLS setup callback function.  */
static UINT tls_setup_callback(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *tls_session)
{
UINT status;


    /* Initialize and create TLS session.  */
    status = nx_secure_tls_session_create(tls_session, &nx_crypto_tls_ciphers, crypto_metadata_client, sizeof(crypto_metadata_client));
    
    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Allocate space for packet reassembly.  */
    status = nx_secure_tls_session_packet_buffer_set(&(client_ptr -> nx_web_http_client_tls_session), tls_packet_buffer, sizeof(tls_packet_buffer));

    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Add a CA Certificate to our trusted store for verifying incoming server certificates.  */
    nx_secure_x509_certificate_initialize(&trusted_certificate, ca_cert_der, ca_cert_der_len, NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE);
    nx_secure_tls_trusted_certificate_add(&(client_ptr -> nx_web_http_client_tls_session), &trusted_certificate);

    /* Need to allocate space for the certificate coming in from the remote host.  */
    nx_secure_tls_remote_certificate_allocate(&(client_ptr -> nx_web_http_client_tls_session), &remote_certificate, remote_cert_buffer, sizeof(remote_cert_buffer));
    nx_secure_tls_remote_certificate_allocate(&(client_ptr -> nx_web_http_client_tls_session), &remote_issuer, remote_issuer_buffer, sizeof(remote_issuer_buffer));

    return(NX_SUCCESS);
}
#endif /* NX_WEB_HTTPS_ENABLE  */

void thread_client_entry(ULONG thread_input)
{
UINT            i;
UINT            status;
NX_PACKET       *recv_packet;


    /* Give IP task and driver a chance to initialize the system. */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* Set server IP address.  */
    server_ip_address.nxd_ip_address.v4 = HTTP_SERVER_ADDRESS;
    server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;

    /* First loop test HTTP, second loop test HTTPS.  */
    for (i = 0; i < loop ; i++)
    {
        if (i == 0)
        {

            /* Wait HTTP server started.  */
            while(!http_server_start)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#ifdef NX_WEB_HTTPS_ENABLE
        else
        {

            /* Wait HTTPS server started.  */
            while(!https_server_start)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#endif /* NX_WEB_HTTPS_ENABLE  */

            /* Create an HTTP client instance.  */
            status = nx_web_http_client_create(&my_client, "HTTP Client", &client_ip, &client_pool, 1536);

            /* Check status.  */
            if (status)
                error_counter++;

            /* Send a GET request.  */
            if (i == 0)
            {
                status = nx_web_http_client_get_start(&my_client, &server_ip_address,
                                                      NX_WEB_HTTP_SERVER_PORT, "/index.htm",
                                                      "www.abc.com", "name", "password", NX_WAIT_FOREVER);
            }
#ifdef NX_WEB_HTTPS_ENABLE
            else
            {
                status = nx_web_http_client_get_secure_start(&my_client, &server_ip_address,
                                                             NX_WEB_HTTPS_SERVER_PORT, "/index.htm",
                                                             "www.abc.com", "name", "password",
                                                             tls_setup_callback, NX_WAIT_FOREVER);
            }
#endif /* NX_WEB_HTTPS_ENABLE  */

            /* Check status.  */
            if (status)
                error_counter++;

            /* Get response from server.  */
            while (1)
            {
                status = nx_web_http_client_response_body_get(&my_client, &recv_packet, 1 * NX_IP_PERIODIC_RATE);

                if (status)
                {
                    break;
                }
                else
                {
                    nx_packet_release(recv_packet);
                }
            }

            /* Check status.  */
            if (status != NX_WEB_HTTP_GET_DONE)
                error_counter++;
            else
                nx_packet_release(recv_packet);

            status = nx_web_http_client_delete(&my_client);
            if (status)
               error_counter++;

        /* Set the flag.  */
        if (i == 0)
        {
            http_client_stop = 1;
        }
#ifdef NX_WEB_HTTPS_ENABLE
        else
        {
            https_client_stop = 1;
        }
#endif /* NX_WEB_HTTPS_ENABLE  */
    }
}


/* Define the helper HTTP server thread.  */
void    thread_server_entry(ULONG thread_input)
{
UINT            i;
UINT            status;
FX_FILE         my_file;
UINT            server_port = NX_WEB_HTTP_SERVER_PORT;


    /* Print out test information banner.  */
    printf("NetX Test:   Web Response in Multiple Packets Test.....................");

    /* Check for earlier error. */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    fx_media_format(&ram_disk,
                    _fx_ram_driver,               // Driver entry
                    ram_disk_memory,              // RAM disk memory pointer
                    media_memory,                 // Media buffer pointer
                    sizeof(media_memory),         // Media buffer size
                    "MY_RAM_DISK",                // Volume Name
                    1,                            // Number of FATs
                    32,                           // Directory Entries
                    0,                            // Hidden sectors
                    256,                          // Total sectors
                    512,                          // Sector size
                    8,                            // Sectors per cluster
                    1,                            // Heads
                    1);                           // Sectors per track   
    
    /* Open the RAM disk.  */
    status = fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, media_memory, sizeof(media_memory)) ;
    status += fx_file_create(&ram_disk, "index.htm");
    status += fx_file_open(&ram_disk, &my_file, "index.htm", FX_OPEN_FOR_WRITE);
    status += fx_file_write(&my_file, "https server", 12);
    status += fx_file_close(&my_file);
    if(status)
        error_counter++;

    /* Give NetX a chance to initialize the system. */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* First loop test HTTP, second loop test HTTPS.  */
    for (i = 0; i < loop; i++)
    {

        if (i == 1)
        {
            server_port = NX_WEB_HTTPS_SERVER_PORT;
        }

        /* Create the HTTP Server. */
        status = nx_web_http_server_create(&my_server, "My HTTP Server", &server_ip, server_port, &ram_disk,
                                           &server_stack, sizeof(server_stack), &server_pool,
                                           NX_NULL, server_request_callback);
        if (status)
            error_counter++;

#ifdef NX_WEB_HTTPS_ENABLE
        /* Set TLS for HTTPS.  */
        if (i == 1)
        {
            /* Initialize device certificate (used for all sessions in HTTPS server). */
            memset(&certificate, 0, sizeof(certificate));
            nx_secure_x509_certificate_initialize(&certificate, test_device_cert_der, test_device_cert_der_len, NX_NULL, 0, test_device_cert_key_der, test_device_cert_key_der_len, NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER);

            /* Setup TLS session data for the TCP server. */
            status = nx_web_http_server_secure_configure(&my_server, &nx_crypto_tls_ciphers,
                                                         crypto_metadata_server, sizeof(crypto_metadata_server), tls_packet_buffer, sizeof(tls_packet_buffer),
                                                         &certificate, NX_NULL, 0, NX_NULL, 0, NX_NULL, 0);
            if (status)
                error_counter++;
        }
#endif /* NX_WEB_HTTPS_ENABLE  */

        /* OK to start the HTTP Server.   */
        status = nx_web_http_server_start(&my_server);
        if (status)
            error_counter++;

        /* Set the flag.  */
        if (i == 0)
        {
            http_server_start = 1;

            /* Wait HTTP test finished.  */
            while(!http_client_stop)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#ifdef NX_WEB_HTTPS_ENABLE
        else
        {
            https_server_start = 1;

            /* Wait HTTPS test finished.  */
            while(!https_client_stop)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#endif /* NX_WEB_HTTPS_ENABLE  */

        status = nx_web_http_server_delete(&my_server);
        if (status)
            error_counter++;
    }

    /* Check packet pool.  */
    if (server_pool.nx_packet_pool_available != server_pool.nx_packet_pool_total)
    {
        error_counter++;
    }

    if (client_pool.nx_packet_pool_available != client_pool.nx_packet_pool_total)
    {
        error_counter++;
    }

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
}

/* Define the server request callback function.  */
static UINT server_request_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
NX_PACKET   *response_pkt;
UINT         status;

    /* Process multipart data.  */
    if(request_type == NX_WEB_HTTP_SERVER_GET_REQUEST)
    {

        /* Allocate a response packet.  */
        status =  nx_web_http_server_response_packet_allocate(server_ptr, &response_pkt, TX_WAIT_FOREVER);

        /* Check status.  */
        if (status)
        {
            error_counter++;
        }

        status = nx_packet_data_append(response_pkt, pkt, sizeof(pkt), &server_pool, NX_WAIT_FOREVER);

        if(status == NX_SUCCESS)
        {
            if(nx_web_http_server_callback_packet_send(server_ptr, response_pkt) !=  NX_SUCCESS)
            {
                nx_packet_release(response_pkt);
            }
        }

        /* Allocate a response packet.  */
        status =  nx_web_http_server_response_packet_allocate(server_ptr, &response_pkt, TX_WAIT_FOREVER);

        /* Check status.  */
        if (status)
        {
            error_counter++;
        }

        status = nx_packet_data_append(response_pkt, pkt1, sizeof(pkt1), &server_pool, NX_WAIT_FOREVER);

        if(status == NX_SUCCESS)
        {
            if(nx_web_http_server_callback_packet_send(server_ptr, response_pkt) !=  NX_SUCCESS)
            {
                nx_packet_release(response_pkt);
            }
        }

        /* Allocate a response packet.  */
        status =  nx_web_http_server_response_packet_allocate(server_ptr, &response_pkt, TX_WAIT_FOREVER);

        /* Check status.  */
        if (status)
        {
            error_counter++;
        }

        status = nx_packet_data_append(response_pkt, pkt2, sizeof(pkt2), &server_pool, NX_WAIT_FOREVER);

        if(status == NX_SUCCESS)
        {
            if(nx_web_http_server_callback_packet_send(server_ptr, response_pkt) !=  NX_SUCCESS)
            {
                nx_packet_release(response_pkt);
            }
        }
    }
    else
    {
        /* Indicate we have not processed the response to client yet.  */
        return(NX_SUCCESS);
    }

    /* Indicate the response to client is transmitted.  */
    return(NX_WEB_HTTP_CALLBACK_COMPLETED);
}

#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_web_response_in_multiple_packets_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   Web Response in Multiple Packets Test.....................N/A\n");

    test_control_return(3);  
}      
#endif
