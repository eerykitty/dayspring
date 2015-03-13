#include "ll_network.hh"

// low(er) level networking code, wraps the C code of libcurve (which itself is a sort of wrapper for czmq, which is itself a wrapper over zmq etc etc). network.cc is the c++ wrapper for this wrapper, it's designed to abstract the networking logic away from the underlying implementation, this file is the networking logic :-)

ll_net::ll_net (std::string address)
{
        context = zctx_new ();
        get_own_certificate ();
        auth = zauth_new (context);
        if (!file_exists (CLIENT_CERT_DIR))
                boost::filesystem::create_directory (CLIENT_CERT_DIR);
        zauth_configure_curve (auth, (char*)"*", CLIENT_CERT_DIR);
        server = curve_server_new (context, &certificate);
        char* endpoint = new char[address.length () + 1];
        strcpy (endpoint, address.c_str ());
        curve_server_bind (server, endpoint);
        delete [] endpoint;
}

ll_net::~ll_net ()
{
        curve_server_destroy (&server);
        zauth_destroy (&auth);
        zctx_destroy (&context);
}

void ll_net::get_own_certificate ()
{
        if (file_exists (CERT_PATH "_secret"))
                certificate = zcert_load (CERT_PATH "_secret");
        else
        {
                certificate = zcert_new ();
                zcert_save (certificate, CERT_PATH);
        }
}
