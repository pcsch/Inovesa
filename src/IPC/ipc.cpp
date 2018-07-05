#include "IPC/ipc.h"

/**
 * @brief Connect to the other side using the comm object
 */
bool IPCC::IPC::connect() {
    comm->init();
    connection_successful = false; // TODO: perform actual check
    char buf[5];;  // 5 to be able to compare with 1111\0 (zero terminator)
    while(!connection_successful) {
        strcpy(buf, "0000");
        if (!comm->read(buf, 4 * sizeof(char))) { // TODO: Proper Error Handling
            std::cerr << comm->error << std::endl;
            return false;
        }
        if (strcmp(buf, "1111") == 0) {
            connection_successful = true;
        }
        sleep(0.1);
        std::cout << "." << std::flush;
    }
    std::cout << std::endl;
    buf[0] = '1';
    buf[1] = '1';
    buf[2] = '1';
    buf[3] = '1';
    if (!comm->write(buf, 4 * sizeof(char))) {
        std::cerr << comm->error << std::endl;
        return false;
    }
    std::cout << "Connection successful" << std::endl;
    return connection_successful;
}

/**
 * @brief Initialise the variables to transfer to the other side
 *
 * This method reads the number of outsteps to skip between communications and also reads the
 * variables that are requested and that will later be send to the other side.
 */
bool IPCC::IPC::initTransferVariables() {
    char buf[5];
    buf[4] = '\0';
    if(!comm->read(buf, 4*sizeof(char))) { // Wait and read
        std::cerr << comm->error << std::endl;
        return false;
    }
    if(strcmp(buf, "1000")!=0) { // 1000 signals that the other side is ready
        std::cerr << buf << std::endl;
        std::cerr << "Error in Init_transfer_varaibles()" << std::endl;
        return false;
    }
    buf[0] = '1';
    buf[1] = '0';
    buf[2] = '0';
    buf[3] = '0';
    if(!comm->write(buf, 4*sizeof(char))) { // Write 1000 to signal preparedness to go further
        std::cerr << comm->error << std::endl;
        return false;
    }
    if(!comm->read(instep, sizeof(instep))) { // Read the number of outsteps to skip between communications
        std::cerr << comm->error << std::endl;
        return false;
    }
    char varbuf[15];
    if(!comm->read(varbuf, 15*sizeof(char))) { // Read the variables that are requested by the other side
        std::cerr << comm->error << std::endl;
        return false;
    }
    for(int i=0; i<15; i++){
        requested_variables[i] = varbuf[i] == '1';
    }
    return true; // TODO: error check
}

/**
 * @brief Send the requested variables to the other side for processing
 *
 * This method iterates over the requested variables (Simulated values such as CSR Intensity or Bunch Profile etc)
 * (retrieved by IPCC::IPC::initTransferVariables) and sends them over the communication interface to the other side.
 */
bool IPCC::IPC::sendVariables() {
    char next[] = "next"; // TODO: Better Separator
    float * ptr = nullptr;
    std::size_t sz;
    std::size_t so;
    for(int i=0; i<15; i++) {
        if(!requested_variables[i]) {
            continue;
        }
        switch(i) {
            case IPCC::Variables::CSRINTENSITY:
                sz = _csr_int->size();
                so = sizeof(vfps::csrpower_t);
                ptr = _csr_int->data();
                break;
            case IPCC::Variables::BUNCHPROFILE:
                sz = _bunch_profiles->size();
                so = sizeof(vfps::projection_t);
                ptr = _bunch_profiles->data();
                break;
            case IPCC::Variables::ENERGYPROFILE:
                sz = _energy_profiles->size();
                so = sizeof(vfps::projection_t);
                ptr = _energy_profiles->data();
                break;
            default:
                break;
        }
        if(!comm->write(&sz, sizeof(sz))) { // TODO: Proper Error Handling
            std::cerr << comm->error << std::endl;
            return false;
        }
        if(!comm->write(next, sizeof(next))) {
            std::cerr << comm->error << std::endl;
            return false;
        }
        if(!comm->write(&so, sizeof(so))) {
            std::cerr << comm->error << std::endl;
            return false;
        }
        if(!comm->write(next, sizeof(next))) {
            std::cerr << comm->error << std::endl;
            return false;
        }
    //    if(!comm->write(_csr_int->data(), (size_t)sz*so)) {
        if(!comm->write(ptr, (size_t)sz*so)) {
            std::cerr << comm->error << std::endl;
            return false;
        }
    }
    char end[] = "end.......";
    if(!comm->write(end, sizeof(end))) {
        std::cerr << comm->error << std::endl;
    }
    return true;
}

/**
 * @brief Receive parameters to modify
 */
bool IPCC::IPC::receiveParameters() {
    float buf[4096];
    if(!comm->read(buf, 4096*sizeof(float))) { // TODO: Proper Error Handling
        std::cerr << comm->error << std::endl;
        return false;
    }
    int lauf = 0;  // First one is an id of an parameter
    rec_pars.clear();
    int c_var;
    while(buf[lauf] != -1) { // buf[lauf] is always a parameter id. If it is -1 the stream of data ended.
        std::vector<std::array<float, 2>> x;
        c_var = buf[lauf];
        lauf += 1; // lauf is now the position of the number of data for this parameter
        for(int i=lauf+1; i<buf[lauf]*2+lauf; i+=2) {
            x.emplace_back(std::array<float, 2>{{buf[i], buf[i+1]}});
        }
        lauf = (int)buf[lauf]*2+lauf+1; // lauf is one more than the position of the previous id,
                                        // buf[lauf]*2 is the number of values to skip
                                        // +1 because we want it to point to the next id
        rec_pars[c_var] = x;
    }
    return true;
}