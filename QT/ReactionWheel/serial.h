/*
 * File:   SimpleSerial.h
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 *
 * Created on September 10, 2009, 12:12 PM
 *
 *
 * Edited by Douglas Carroll for use with reaction wheel project.
 * Accessed from: https://github.com/fedetft/serial-port/blob/master/1_simple/SimpleSerial.h
 *
 */

#ifndef SERIAL_H
#define SERIAL_H
#include <boost/asio.hpp>

class Serial {

public:
    Serial(std::string port, unsigned int baud_rate)
    : io(), serial(io,port) {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    // Writes a string over serial
    void writeString(std::string s){
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    // Boost api read function.
    // Will read data until an \n or \r\n.
    // String returned without \n or \r\n
    std::string readLine(){
        //Reading data char by char, code is optimized for simplicity, not speed
        using namespace boost;
        char c;
        std::string result;
        for(;;) {
            asio::read(serial,asio::buffer(&c,1));
            switch(c) {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result += c;
            }
        }
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};
#endif // SERIAL_H
