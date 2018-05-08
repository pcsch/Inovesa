#include "IPC/ipc.h"

bool IPCC::IPC::connect() {
    comm->init();
    connection_successful = false; // TODO: perform actual check
    char buf[5];;  // 5 to be able to compare with 1111\0 (zero terminator)
    while(!connection_successful) {
        strcpy(buf, "0000");
        if (!comm->read(buf, 4 * sizeof(char))) { // TODO: Proper Error Handling
            std::cerr << comm->error << std::endl;
        }
        if (strcmp(buf, "1111") == 0) {
//        std::cerr << buf << std::endl;
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
    }
    std::cout << "Connection successful" << std::endl;
    return connection_successful;
}
bool IPCC::IPC::init_transfer_variables() {
    char buf[5];
    buf[4] = '\0';
//    int l = 0;
//    while(strcmp(buf, "1000")!=0) {
//        l ++;
        if(!comm->read(buf, 4*sizeof(char))) {
            std::cerr << comm->error << std::endl;
        }
//        if(l > 100) {
//            return false;
//        }
//        sleep(1);
//        std::cerr << buf << std::endl;
//    }
    std::cout << buf << std::endl;
    if(strcmp(buf, "1000")!=0) {
            std::cerr << buf << std::endl;
        std::cerr << "Error in Init_transfer_varaibles()" << std::endl;
        return false;
    }
    buf[0] = '1';
    buf[1] = '0';
    buf[2] = '0';
    buf[3] = '0';
    if(!comm->write(buf, 4*sizeof(char))) { // TODO: Proper Error Handling
        std::cerr << comm->error << std::endl;
    }
    char varbuf[15];
    if(!comm->read(varbuf, 15*sizeof(char))) { // TODO: Proper Error Handling
        std::cerr << comm->error << std::endl;
    }
    for(int i=0; i<15; i++){
        requested_variables[i] = varbuf[i];
    }
    std::cout.write(varbuf, sizeof(varbuf));
    std::cout << std::endl;
    std::cout << "Variable list received" << std::endl;
    return true; // TODO: error check
}
bool IPCC::IPC::send_variables() {
    std::cout << "Sending Variables" << std::endl;
    std::size_t sz = _csr_int->size();
    std::size_t so = sizeof(csr_t);
    char next[] = "next";
    if(!comm->write(&sz, sizeof(sz))) { // TODO: Proper Error Handling
        std::cerr << comm->error << std::endl;
    }
    if(!comm->write(next, sizeof(next))) {
        std::cerr << comm->error << std::endl;
    }
    if(!comm->write(&so, sizeof(so))) {
        std::cerr << comm->error << std::endl;
    }
    if(!comm->write(next, sizeof(next))) {
        std::cerr << comm->error << std::endl;
    }
    if(!comm->write(_csr_int->data(), (size_t)sz*so)) {
        std::cerr << comm->error << std::endl;
    }
    char end[] = "end.......";
    if(!comm->write(end, sizeof(end))) {
        std::cerr << comm->error << std::endl;
    }
    std::cout << "Variables Sent" << std::endl;
    return true;
}

bool IPCC::IPC::receive_parameters() {
    std::cout << "Receiving Parameters" << std::endl;
    float buf[4096];
    if(!comm->read(buf, sizeof(buf))) { // TODO: Proper Error Handling
        std::cerr << "Socket I/O Error" << std::endl;
    }
    int lauf = 0;  // First one is an id of an parameter
    rec_pars.clear();
    int c_var;
    while(buf[lauf] != -1) { // buf[lauf] is always a parameter id. If it is -1 the stream of data ended.
        std::vector<std::array<float, 2>> x;
        std::cout << "Variable " << buf[lauf] << std::endl;
        c_var = buf[lauf];
        lauf += 1; // lauf is now the position of the number of data for this parameter
        for(int i=lauf+1; i<buf[lauf]*2+lauf; i+=2) {
            std::cout << "\t" << buf[i] << " " << buf[i + 1] << std::endl;
            x.emplace_back(std::array<float, 2>{{buf[i], buf[i+1]}});
        }
        lauf = (int)buf[lauf]*2+lauf+1; // lauf is one more than the position of the previous id,
                                        // buf[lauf]*2 is the number of values to skip
                                        // +1 because we want it to point to the next id
//        rec_pars.emplace_back(x);
        rec_pars[c_var] = x;
    }
    std::cout << std::endl;
    return true;
}