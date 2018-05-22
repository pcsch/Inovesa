//
// Created by patrick on 02.05.18.
//

#ifndef ASIO_IPC_H
#define ASIO_IPC_H
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include "Communicators/basecommunicator.h"
#include "defines.hpp"

namespace IPCC {

    enum Variables {
        CSRINTENSITY=0, BUNCHPROFILE=1, ENERGYPROFILE=2
    };

    class IPC {
    public:
        IPC(BaseCommunicator& comm): comm(&comm) {
        };
        bool connect();
        bool initTransferVariables();
        bool sendVariables();
        bool receiveParameters();
        void setVariables(std::vector<vfps::csrpower_t> &csr_int, std::vector<vfps::projection_t> &bunch_profiles,
                          std::vector<vfps::projection_t> &energy_profiles) {
            _csr_int = &csr_int;
            _bunch_profiles = &bunch_profiles;
            _energy_profiles = &energy_profiles;
        }
        BaseCommunicator* comm;

        bool* getRequestedVariables() {
            return requested_variables;
        }

//        std::vector<std::vector<std::array<float,2>>> rec_pars;
    std::map<int, std::vector<std::array<float, 2>>> rec_pars;
    int instep = 0; // TODO: Write read function for ints and make this an int
    protected:
        std::vector<vfps::csrpower_t>* _csr_int;
        std::vector<vfps::projection_t>* _bunch_profiles;
        std::vector<vfps::projection_t>* _energy_profiles;
        bool requested_variables[15];
        bool connection_successful;
    };
}


#endif //ASIO_IPC_H
