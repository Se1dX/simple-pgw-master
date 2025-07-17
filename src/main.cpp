#include <iostream>
#include <boost/asio.hpp>
#include "control_plane.h"

int main() {
    using namespace boost::asio::ip;
    
    control_plane cp;
    
    cp.add_apn("internet", make_address_v4("192.168.1.1"));
    
    auto pdn = cp.create_pdn_connection(
        "internet", 
        make_address_v4("10.0.0.1"),  // SGW address
        12345                         // SGW TEID
    );
    
    auto bearer = cp.create_bearer(pdn, 54321);
    
    std::cout << "PGW Simulation Started!\n";
    std::cout << "UE IP: " << pdn->get_ue_ip_addr().to_string() << "\n";
    std::cout << "Bearer TEID: " << bearer->get_dp_teid() << "\n";
    
    return 0;
}