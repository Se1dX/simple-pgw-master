#include "control_plane.h"
#include <stdexcept>
#include <random>

using namespace boost::asio::ip;

template<typename MapType>
static uint32_t generate_unique_teid(const MapType& existing) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> dist(1, 0xFFFFFFFE);
    
    uint32_t teid;
    do {
        teid = dist(gen);
    } while (existing.find(teid) != existing.end());
    
    return teid;
}

static address_v4 generate_unique_ip(const address_v4& apn_gw, 
                                   const std::unordered_map<address_v4, 
                                   std::shared_ptr<pdn_connection>>& existing) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint8_t> dist(2, 254);
    
    address_v4 ip;
    do {
        auto bytes = apn_gw.to_bytes();
        bytes[3] = dist(gen);
        ip = address_v4(bytes);
    } while (existing.find(ip) != existing.end());
    
    return ip;
}

std::shared_ptr<pdn_connection> control_plane::find_pdn_by_cp_teid(uint32_t cp_teid) const {
    auto it = _pdns.find(cp_teid);
    return it != _pdns.end() ? it->second : nullptr;
}

std::shared_ptr<pdn_connection> control_plane::find_pdn_by_ip_address(const address_v4& ip) const {
    auto it = _pdns_by_ue_ip_addr.find(ip);
    return it != _pdns_by_ue_ip_addr.end() ? it->second : nullptr;
}

std::shared_ptr<bearer> control_plane::find_bearer_by_dp_teid(uint32_t dp_teid) const {
    auto it = _bearers.find(dp_teid);
    return it != _bearers.end() ? it->second : nullptr;
}

std::shared_ptr<pdn_connection> control_plane::create_pdn_connection(
    const std::string& apn, 
    address_v4 sgw_addr,
    uint32_t sgw_cp_teid) 
{
    auto apn_it = _apns.find(apn);
    if (apn_it == _apns.end()) {
        throw std::runtime_error("APN not found");
    }

    address_v4 apn_gw = apn_it->second;
    uint32_t cp_teid = generate_unique_teid(_pdns);
    address_v4 ue_ip = generate_unique_ip(apn_gw, _pdns_by_ue_ip_addr);

    auto pdn = pdn_connection::create(cp_teid, apn_gw, ue_ip);
    pdn->set_sgw_cp_teid(sgw_cp_teid);
    pdn->set_sgw_addr(sgw_addr);

    _pdns[cp_teid] = pdn;
    _pdns_by_ue_ip_addr[ue_ip] = pdn;

    return pdn;
}

void control_plane::delete_pdn_connection(uint32_t cp_teid) {
    auto pdn_it = _pdns.find(cp_teid);
    if (pdn_it == _pdns.end()) return;

    auto pdn = pdn_it->second;
    _pdns_by_ue_ip_addr.erase(pdn->get_ue_ip_addr());
    
    for (auto it = _bearers.begin(); it != _bearers.end(); ) {
        if (it->second->get_pdn_connection()->get_cp_teid() == cp_teid) {
            it = _bearers.erase(it);
        } else {
            ++it;
        }
    }

    _pdns.erase(pdn_it);
}

std::shared_ptr<bearer> control_plane::create_bearer(
    const std::shared_ptr<pdn_connection>& pdn, 
    uint32_t sgw_teid) 
{
    uint32_t dp_teid = generate_unique_teid(_bearers);
    auto br = std::make_shared<bearer>(dp_teid, *pdn);
    br->set_sgw_dp_teid(sgw_teid);

    _bearers[dp_teid] = br;
    
    pdn->add_bearer(br);
    
    return br;
}

void control_plane::delete_bearer(uint32_t dp_teid) {
    auto bearer_it = _bearers.find(dp_teid);
    if (bearer_it == _bearers.end()) return;

    auto bearer = bearer_it->second;
    if (auto pdn = bearer->get_pdn_connection()) {
        pdn->remove_bearer(dp_teid);
    }
    
    _bearers.erase(bearer_it);
}

void control_plane::add_apn(std::string apn_name, address_v4 apn_gateway) {
    _apns[apn_name] = apn_gateway;
}