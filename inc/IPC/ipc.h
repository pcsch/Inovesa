//
// Created by patrick on 02.05.18.
//

#ifndef ASIO_IPC_H
#define ASIO_IPC_H
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include "defines.h"
#include "Communicators/basecommunicator.h"


namespace IPCC {


    class IPC {
    public:
        IPC(BaseCommunicator& comm): comm(&comm) {
        };
        bool connect();
        bool init_transfer_variables();
        bool send_variables();
        bool receive_parameters();
        void set_variables(std::vector<csr_t>& csr_int) {
            _csr_int = &csr_int;
        }
        BaseCommunicator* comm;

//        std::vector<std::vector<std::array<float,2>>> rec_pars;
    std::map<int, std::vector<std::array<float, 2>>> rec_pars;
    protected:
        std::vector<csr_t>* _csr_int;
        short requested_variables[15];
        bool connection_successful;
    };


//    class SocketIPC: public IPC {
//    public:
//        bool connect() override;
//        bool init_transfer_variables() override;
//        bool send_variables() override;
//        bool receive_parameters() override;
//
//        void send(boost::array<char, 10>);
//    private:
//
//    };
}



//template <typename T> bool IPCC::SocketCommunicator::read(T& x) {
//    std::cout << "hi" << std::endl;
//    boost::system::error_code error_code;
//    socket->read_some(boost::asio::buffer(x), error_code);
//    if(error_code){
//        error = error_code.message();
//        return false;
//    }
//    return true;
//}
//template <typename T> bool IPCC::SocketCommunicator::read(const T & x, std::size_t size) {
//    boost::system::error_code error_code;
//    socket->read_some(boost::asio::buffer(x, size), error_code);
//    if(error_code){
//        error = error_code.message();
//        return false;
//    }
//    return true;
//}
//template <typename T> bool IPCC::SocketCommunicator::write(T& x) {
//    boost::system::error_code error_code;
//    socket->write_some(boost::asio::buffer(x), error_code);
//    if(error_code){
//        error = error_code.message();
//        return false;
//    }
//    return true;
//}
//template <typename T> bool IPCC::SocketCommunicator::write(T* x, std::size_t size) {
//    boost::system::error_code error_code;
//    socket->write_some(boost::asio::buffer(x, size), error_code);
//    if(error_code){
//        error = error_code.message();
//        return false;
//    }
//    return true;
//}



#endif //ASIO_IPC_H
