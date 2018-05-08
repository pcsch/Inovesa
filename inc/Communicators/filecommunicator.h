//
// Created by patrick on 07.05.18.
//

#ifndef ASIO_FILECOMMUNICATOR_H
#define ASIO_FILECOMMUNICATOR_H

#include <fstream>
#include <string>
#include "basecommunicator.h"

namespace IPCC {
    class FileCommunicator : public BaseCommunicator {
    public:
        explicit FileCommunicator(const std::string filename) : _filename(filename) {
        }

        bool init() override {
            if(!_ofile.is_open()) {
                _ofile.open(_filename + ".out", std::ios::out | std::ios::in | std::ios::trunc);
            }
            if(!_ifile.is_open()) {
                _ifile.open(_filename + ".in", std::ios::in | std::ios::out | std::ios::trunc);
            }
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


        ~FileCommunicator() {
            _ifile.close();
            _ofile.close();
        }

    private:
        std::string _filename;
        std::fstream _ofile;
        std::fstream _ifile;
    };

}

#endif // ASIO_FILECOMMUNICATOR_H
