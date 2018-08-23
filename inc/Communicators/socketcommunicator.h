//
// Created by patrick on 07.05.18.
//

#ifndef ASIO_SOCKETCOMMUNICATOR_H
#define ASIO_SOCKETCOMMUNICATOR_H

#include <boost/asio.hpp>
#include <iostream>
#include "basecommunicator.h"

using boost::asio::ip::tcp;

namespace IPCC {

    class SocketCommunicator : public BaseCommunicator {
    public:
        explicit SocketCommunicator(boost::asio::io_service &io_context, int port) : io_context(&io_context), port(port) {
            acceptor = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), port));
            socket = new tcp::socket(io_context);
        }

        bool init() override {
            std::cout << "Waiting for Connection on port " << port << std::flush;
            acceptor->accept(*socket);
            return true;
        }

        bool write(char &x, std::size_t size) override;

        bool write(float &x, std::size_t size) override;

        bool write(int &x, std::size_t size) override;

        bool write(std::size_t &x, std::size_t size) override;

        bool read(float &x, std::size_t size) override;

        bool read(int &x, std::size_t size) override;

        bool read(char &x, std::size_t size) override;

        bool write(char *x, std::size_t size) override;

        bool write(int *x, std::size_t size) override;

        bool write(float *x, std::size_t size) override;

        bool write(std::size_t *x, std::size_t size) override;

        bool read(float *x, std::size_t size) override;

        bool read(int *x, std::size_t size) override;

        bool read(char *x, std::size_t size) override;

        ~SocketCommunicator() {
            delete acceptor;
            delete socket;
        }

    private:
        boost::asio::io_service *io_context;
        tcp::acceptor *acceptor;
        tcp::socket *socket;
        int port;
    };
}

#endif //ASIO_SOCKETCOMMUNICATOR_H
