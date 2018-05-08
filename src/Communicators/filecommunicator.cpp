//
// Created by patrick on 07.05.18.
//

#include "Communicators/filecommunicator.h"

#include <iostream>
#include <unistd.h>

#define SLEEP_TIME 0.1

bool IPCC::FileCommunicator::read(float& x, std::size_t size) {
    //TODO: Error check
//    _ifile.read((char*)&x, size);
    std::streamsize s = 0;
    while(s == 0) {
        s = _ifile.readsome((char*)&x, size);
        sleep(SLEEP_TIME);
    }
    if(_ifile.eof()) {
        _ifile.close();
        _ifile.open(_filename+".in", std::ios::in | std::ios::out | std::ios::trunc);
    }
    return true;
}
bool IPCC::FileCommunicator::read(char& x, std::size_t size) {
    //TODO: Error check
    std::streamsize s = 0;
    while(s == 0) {
        s = _ifile.readsome(&x, size);
        sleep(SLEEP_TIME);
    }
    if(_ifile.eof()) {
        _ifile.close();
        _ifile.open(_filename+".in", std::ios::in | std::ios::out | std::ios::trunc);
    }
    return true;
}
bool IPCC::FileCommunicator::read(float* x, std::size_t size) {
    //TODO: Error check
    std::streamsize s = 0;
    while(s == 0) {
        s = _ifile.readsome((char*)x, size);
        sleep(SLEEP_TIME);
    }
    if(_ifile.eof()) {
        _ifile.close();
        _ifile.open(_filename+".in", std::ios::in | std::ios::out | std::ios::trunc);
    }
    return true;
}
bool IPCC::FileCommunicator::read(char* x, std::size_t size) {
    //TODO: Error check
    std::streamsize s = 0;
    while(s == 0) {
        s = _ifile.readsome(x, size);
        sleep(SLEEP_TIME);
    }
    if(_ifile.eof()) {
        _ifile.close();
        _ifile.open(_filename+".in", std::ios::in | std::ios::out | std::ios::trunc);
    }
    return true;
}
bool IPCC::FileCommunicator::write(float& x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write((char*)&x, size);
    _ofile.flush();
    return true;
}
bool IPCC::FileCommunicator::write(char& x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write(&x, size);
    _ofile.flush();
    return true;
}
bool IPCC::FileCommunicator::write(std::size_t& x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write((char*)x, size);
    _ofile.flush();
    return true;
}
bool IPCC::FileCommunicator::write(float* x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write((char*)x, size);
    _ofile.flush();
    return true;
}
bool IPCC::FileCommunicator::write(char* x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write(x, size);
    _ofile.flush();
    return true;
}
bool IPCC::FileCommunicator::write(std::size_t* x, std::size_t size) {
    //TODO: Error check
    _ofile.seekg (0, std::ios::end);
    _ofile.write((char*)x, size);
    _ofile.flush();
    return true;
}
