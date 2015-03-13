/*************************************************************
 * @author  Deniz Erbilgin
 *
 * @brief   Takes a string of integer values between 0 and 255
 *          formats and sends them using ofSerial with startChar prepended
 *
 ************************************************************/

#include "lightShowSerial.h"

lightShowSerial::lightShowSerial(){}
lightShowSerial::~lightShowSerial(){}

void lightShowSerial::setup(string comPort, int baudRate, int _startChar){
    startChar = _startChar;

    serial.setup(comPort, baudRate);

    if(serial.isInitialized()) cout << "Serial " << comPort << " started successfully" << endl;
}

void lightShowSerial::send(vector<int> _message){
    if(serial.isInitialized()){
        int messageLength = _message.size();
        unsigned char message[messageLength+1];
        message[0] = startChar;
        for (int i = 0; i < messageLength; i++) message[i+1] = _message[i];
        serial.writeBytes(message, messageLength+1);
    }
}
