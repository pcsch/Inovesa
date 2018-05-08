//
// Created by patrick on 07.05.18.
//
#include "Communicators/socketcommunicator.h"

using boost::asio::ip::tcp;

bool IPCC::SocketCommunicator::read(float& x, std::size_t size) {
    boost::system::error_code error_code;
    socket->read_some(boost::asio::buffer(&x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::read(char& x, std::size_t size) {
    boost::system::error_code error_code;
    socket->read_some(boost::asio::buffer(&x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::read(float* x, std::size_t size) {
    boost::system::error_code error_code;
    socket->read_some(boost::asio::buffer(x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::read(char* x, std::size_t size) {
    boost::system::error_code error_code;
    socket->read_some(boost::asio::buffer(x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(float& x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(&x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(char& x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(&x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(std::size_t& x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(&x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(float* x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(char* x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}
bool IPCC::SocketCommunicator::write(std::size_t* x, std::size_t size) {
    boost::system::error_code error_code;
    socket->write_some(boost::asio::buffer(x, size), error_code);
    if(error_code){
        error = error_code.message();
        return false;
    }
    return true;
}

