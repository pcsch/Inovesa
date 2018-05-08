//
// Created by patrick on 07.05.18.
//

#ifndef ASIO_BASECOMMUNICATOR_H
#define ASIO_BASECOMMUNICATOR_H

#include <cstdlib> // std::size_t
#include <string>

namespace IPCC {
    class BaseCommunicator {
    public:
        virtual bool init() = 0;

        virtual bool write(char &x, std::size_t size) = 0;

        virtual bool write(float &x, std::size_t size) = 0;

        virtual bool write(std::size_t &x, std::size_t size) = 0;

        virtual bool read(float &x, std::size_t size) =0;

        virtual bool read(char &x, std::size_t size) = 0;

        virtual bool write(char *x, std::size_t size) = 0;

        virtual bool write(float *x, std::size_t size) = 0;

        virtual bool write(std::size_t *x, std::size_t size) = 0;

        virtual bool read(float *x, std::size_t size) =0;

        virtual bool read(char *x, std::size_t size) = 0;

//        virtual bool read() {};
        std::string error;
    };
}


#endif //ASIO_BASECOMMUNICATOR_H
