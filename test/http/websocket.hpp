#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <cstdlib>
#include <map>
#include <string>
#include <sstream>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;



    /// Verify that one of the subject alternative names matches the given hostname
    bool verify_subject_alternative_name(const char * hostname, X509 * cert) {
        STACK_OF(GENERAL_NAME) * san_names = NULL;
        
        san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
        if (san_names == NULL) {
            return false;
        }
        
        int san_names_count = sk_GENERAL_NAME_num(san_names);
        
        bool result = false;
        
        for (int i = 0; i < san_names_count; i++) {
            const GENERAL_NAME * current_name = sk_GENERAL_NAME_value(san_names, i);
            
            if (current_name->type != GEN_DNS) {
                continue;
            }
            
            char * dns_name = (char *) ASN1_STRING_data(current_name->d.dNSName);
            
            // Make sure there isn't an embedded NUL character in the DNS name
            if (ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
                break;
            }
            // Compare expected hostname with the CN
            result = (strcasecmp(hostname, dns_name) == 0);
        }
        sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);
        
        return result;
    }

    /// Verify that the certificate common name matches the given hostname
    bool verify_common_name(const char * hostname, X509 * cert) {
        // Find the position of the CN field in the Subject field of the certificate
        int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
        if (common_name_loc < 0) {
            return false;
        }
        
        // Extract the CN field
        X509_NAME_ENTRY * common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
        if (common_name_entry == NULL) {
            return false;
        }
        
        // Convert the CN field to a C string
        ASN1_STRING * common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
        if (common_name_asn1 == NULL) {
            return false;
        }
        
        char * common_name_str = (char *) ASN1_STRING_data(common_name_asn1);
        
        // Make sure there isn't an embedded NUL character in the CN
        if (ASN1_STRING_length(common_name_asn1) != strlen(common_name_str)) {
            return false;
        }
        
        // Compare expected hostname with the CN
        return (strcasecmp(hostname, common_name_str) == 0);
    }

    /**
     * This code is derived from examples and documentation found ato00po
     * http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/ssl/client.cpp
     * and
     * https://github.com/iSECPartners/ssl-conservatory
     */
    bool verify_certificate(const char * hostname, bool preverified, boost::asio::ssl::verify_context& ctx) {
        // The verify callback can be used to check whether the certificate that is
        // being presented is valid for the peer. For example, RFC 2818 describes
        // the steps involved in doing this for HTTPS. Consult the OpenSSL
        // documentation for more details. Note that the callback is called once
        // for each certificate in the certificate chain, starting from the root
        // certificate authority.

        // Retrieve the depth of the current cert in the chain. 0 indicates the
        // actual server cert, upon which we will perform extra validation
        // (specifically, ensuring that the hostname matches. For other certs we
        // will use the 'preverified' flag from Asio, which incorporates a number of
        // non-implementation specific OpenSSL checking, such as the formatting of
        // certs and the trusted status based on the CA certs we imported earlier.
        int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());

        // if we are on the final cert and everything else checks out, ensure that
        // the hostname is present on the list of SANs or the common name (CN).
        if (depth == 0 && preverified) {
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            
            if (verify_subject_alternative_name(hostname, cert)) {
                return true;
            } else if (verify_common_name(hostname, cert)) {
                return true;
            } else {
                return false;
            }
        }

        return preverified;
    }

    /// TLS Initialization handler
    /**
     * WebSocket++ core and the Asio Transport do not handle TLS context creation
     * and setup. This callback is provided so that the end user can set up their
     * TLS context using whatever settings make sense for their application.
     *
     * As Asio and OpenSSL do not provide great documentation for the very common
     * case of connect and actually perform basic verification of server certs this
     * example includes a basic implementation (using Asio and OpenSSL) of the
     * following reasonable default settings and verification steps:
     *
     * - Disable SSLv2 and SSLv3
     * - Load trusted CA certificates and verify the server cert is trusted.
     * - Verify that the hostname matches either the common name or one of the
     *   subject alternative names on the certificate.
     *
     * This is not meant to be an exhaustive reference implimentation of a perfect
     * TLS client, but rather a reasonable starting point for building a secure
     * TLS encrypted WebSocket client.
     *
     * If any TLS, Asio, or OpenSSL experts feel that these settings are poor
     * defaults or there are critically missing steps please open a GitHub issue
     * or drop a line on the project mailing list.
     *
     * Note the bundled CA cert ca-chain.cert.pem is the CA cert that signed the
     * cert bundled with echo_server_tls. You can use print_client_tls with this
     * CA cert to connect to echo_server_tls as long as you use /etc/hosts or
     * something equivilent to spoof one of the names on that cert 
     * (websocketpp.org, for example).
     */
    context_ptr on_tls_init(const char * hostname, websocketpp::connection_hdl) {
        context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

        try {
            ctx->set_options(boost::asio::ssl::context::default_workarounds |
                            boost::asio::ssl::context::no_sslv2 |
                            boost::asio::ssl::context::no_sslv3 |
                            boost::asio::ssl::context::single_dh_use);


            ctx->set_verify_mode(boost::asio::ssl::verify_peer);
            ctx->set_verify_callback(bind(&verify_certificate, hostname, ::_1, ::_2));

            // Here we load the CA certificates of all CA's that this client trusts.
            ctx->load_verify_file("ca-chain.cert.pem");
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        return ctx;
    }


class connection_metadata {
public:
    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri)
      : m_id(id)
      , m_hdl(hdl)
      , m_status("Connecting")
      , m_uri(uri)
      , m_server("N/A")
    {}

    void on_open(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
    }

    void on_fail(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
        m_error_reason = con->get_ec().message();
    }
    
    void on_close(client * c, websocketpp::connection_hdl hdl) {
        m_status = "Closed";
        client::connection_ptr con = c->get_con_from_hdl(hdl);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " (" 
          << websocketpp::close::status::get_string(con->get_remote_close_code()) 
          << "), close reason: " << con->get_remote_close_reason();
        m_error_reason = s.str();
    }

    void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
        if (msg->get_opcode() == websocketpp::frame::opcode::text) {
            m_messages.push_back("<< " + msg->get_payload());
        } else {
            m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
        }
    }

    websocketpp::connection_hdl get_hdl() const {
        return m_hdl;
    }
    
    int get_id() const {
        return m_id;
    }
    
    std::string get_status() const {
        return m_status;
    }

    void record_sent_message(std::string message) {
        m_messages.push_back(">> " + message);
    }

    friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);
private:
    int m_id;
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};

std::ostream & operator<< (std::ostream & out, connection_metadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it) {
        out << *it << "\n";
    }

    return out;
}

class websocket_endpoint {
public:
    websocket_endpoint () : m_next_id(0) {

        // m_endpoint.set_access_channels(websocketpp::log::alevel::all);
        // m_endpoint.clear_access_channels(websocketpp::log::alevel::frame_payload);
        // m_endpoint.set_error_channels(websocketpp::log::elevel::all);

        // // Initialize ASIO
        // m_endpoint.init_asio();
        // //m_endpoint.start_perpetual();
        
        // //m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
        // m_endpoint.run();
    }

    ~websocket_endpoint() {
        //m_endpoint.stop_perpetual();
        
        for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
            if (it->second->get_status() != "Open") {
                // Only close open connections
                continue;
            }
            
            std::cout << "> Closing connection " << it->second->get_id() << std::endl;
            
            websocketpp::lib::error_code ec;
            m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
            if (ec) {
                std::cout << "> Error closing connection " << it->second->get_id() << ": "  
                          << ec.message() << std::endl;
            }
        }
        
        //m_thread->join();
    }

    int connect(std::string const & uri) {
        std::string hostname = "dds.dui.ai";
        m_endpoint.set_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_access_channels(websocketpp::log::alevel::frame_payload);
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        m_endpoint.init_asio();

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        m_endpoint.set_tls_init_handler(bind(&on_tls_init, hostname.c_str(), ::_1));
        websocketpp::lib::error_code ec;
        client::connection_ptr con = m_endpoint.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }
        int new_id = m_next_id++;
        connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri);
        m_connection_list[new_id] = metadata_ptr;

        con->set_open_handler(websocketpp::lib::bind(
            &connection_metadata::on_open,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_fail_handler(websocketpp::lib::bind(
            &connection_metadata::on_fail,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_close_handler(websocketpp::lib::bind(
            &connection_metadata::on_close,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_message_handler(websocketpp::lib::bind(
            &connection_metadata::on_message,
            metadata_ptr,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2
        ));


        m_endpoint.connect(con);
        m_endpoint.run();
        return new_id;
    }

    void close(int id, websocketpp::close::status::value code, std::string reason) {
        websocketpp::lib::error_code ec;
        
        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }
        
        m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
        if (ec) {
            std::cout << "> Error initiating close: " << ec.message() << std::endl;
        }
    }

    void send(int id, std::string message) {
        std::cout << ">id " << id << " message:" <<  message << std::endl;
        std::cout << ">m_connection_list :" <<  m_connection_list.size() << std::endl;
        websocketpp::lib::error_code ec;
        
        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }
        
        m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "> Error sending message: " << ec.message() << std::endl;
            return;
        }
        
        metadata_it->second->record_sent_message(message);
    }

    connection_metadata::ptr get_metadata(int id) const {
        con_list::const_iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            return connection_metadata::ptr();
        } else {
            return metadata_it->second;
        }
    }
private:
    typedef std::map<int,connection_metadata::ptr> con_list;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};


int websocketTest() {
    bool done = false;
    std::string input;
    websocket_endpoint endpoint;

    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            done = true;
        } else if (input == "help") {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "send <connection id> <message>\n"
                << "close <connection id> [<close code:default=1000>] [<close reason>]\n"
                << "show <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else if (input.substr(0,7) == "connect") {
            int id = endpoint.connect(input.substr(8));
            if (id != -1) {
                std::cout << "> Created connection with id " << id << std::endl;
            }
        } else if (input.substr(0,4) == "send") {
            std::stringstream ss(input);
            
            std::string cmd;
            int id;
            std::string message;
            
            ss >> cmd >> id;
            std::getline(ss,message);
            
            endpoint.send(id, message);
        } else if (input.substr(0,5) == "close") {
            std::stringstream ss(input);
            
            std::string cmd;
            int id;
            int close_code = websocketpp::close::status::normal;
            std::string reason;
            
            ss >> cmd >> id >> close_code;
            std::getline(ss,reason);
            
            endpoint.close(id, close_code, reason);
        } else if (input.substr(0,4) == "show") {
            int id = atoi(input.substr(5).c_str());

            connection_metadata::ptr metadata = endpoint.get_metadata(id);
            if (metadata) {
                std::cout << *metadata << std::endl;
            } else {
                std::cout << "> Unknown connection id " << id << std::endl;
            }
        } else {
            std::cout << "> Unrecognized Command" << std::endl;
        }
    }

    return 0;
}