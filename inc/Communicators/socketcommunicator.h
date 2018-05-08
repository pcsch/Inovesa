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
        explicit SocketCommunicator(boost::asio::io_context &io_context) : io_context(&io_context) {
            std::cout << "Waiting for Connection " << std::flush;
            acceptor = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), 6513));
            socket = new tcp::socket(io_context);
        }

        bool init() override {
            acceptor->accept(*socket);
            return true;
        }

        bool write(char &x, std::size_t size) override;

        bool write(float &x, std::size_t size) override;

        bool write(std::size_t &x, std::size_t size) override;

        bool read(float &x, std::size_t size) override;

        bool read(char &x, std::size_t size) override;

        bool write(char *x, std::size_t size) override;

        bool write(float *x, std::size_t size) override;

        bool write(std::size_t *x, std::size_t size) override;

        bool read(float *x, std::size_t size) override;

        bool read(char *x, std::size_t size) override;

        ~SocketCommunicator() {
            delete acceptor;
            delete socket;
        }

    private:
        boost::asio::io_context *io_context;
        tcp::acceptor *acceptor;
        tcp::socket *socket;
    };
}

#endif //ASIO_SOCKETCOMMUNICATOR_H
