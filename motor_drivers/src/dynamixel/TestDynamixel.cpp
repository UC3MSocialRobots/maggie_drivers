/**
 * Program to test the driver Dynamixel
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#include "DynamixelDriver.h"
//#include "long_term_memory/ltm_path.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void menu ();
void printMenu();


int main (){
    menu();
}

void printMenu(){

    // Connect and Disconnect
    cout << endl << "--- CONNECT / DISCONNECT ---" << endl;
    cout <<  "[co], [con], [connect] : Connect the driver with the PC" << endl;
    cout <<  "[dis], [disconnect] : Disconnect the driver from the PC" << endl;

    // Limits
    cout << endl << "--- LIMITS ---" << endl;
    cout <<  "[mp], [maxpos] : Set Maximum Position" << endl;
    cout <<  "[mip], [minpos] : Set Minimum Position" << endl;
    cout <<  "[mv], [maxvel] : Set Maximum Velocity" << endl;

    // Movements
    cout << endl << "--- MOVEMENTS ---" << endl;
    cout <<  "[mabs],[abs] : Move to an Absolute Position" << endl;
    cout <<  "[mrel],[rel] : Move to a Relative Position" << endl;
    cout <<  "[v],[vel] : Move by velocity" << endl;
    // Print Data
    cout << endl << "--- PRINT DATA ---" << endl;
    cout <<  "[pl],[plim], [plimit] : Print all limits" << endl;
    cout <<  "[ms],[sens], [sensor] : Print Motor Sensor Data" << endl;
    cout <<  "[ps],[stat], [pstatus] : Print Status" << endl;
    cout <<  "[pc],[pconf] : Print the Driver Configuration" << endl;
    cout <<  "[pa],[pall], [print] : Print all Configuration Data: Limits, Motor Sensor, Status and Configuration" << endl;
    // Calibration and power
    cout << endl << "--- CALIBRATION & POWER ---" << endl;
    cout <<  "[c],[cal], [calibrate] : Calibrate the Driver" << endl;
    cout <<  "[e],[en], [enable] : Enable the Driver" << endl;
    cout <<  "[d],[di], [disable] : Disable the Driver" << endl;

    // Configuration
    cout << endl << "--- SAVE & LOAD DRIVER CONFIGURATION ---" << endl;
    cout <<  "[l], [load] : Load configuration from file" << endl;
    cout <<  "[s], [save] : Save configuration to a file" << endl;
    cout <<  "[do], [download] : Download configuration to the driver" << endl;

    // Program section
    cout << endl << "--- OTHER ---" << endl;
    cout <<  "[h], [help] : Print this menu" << endl;
    cout <<  "[q],[quit], [exit] : Exit program" << endl;
}

void menu (){
    bool exitMenu = false;
    bool connected = false;
    string keyInput;
    string option;

    string defaultConfigFile = "necky/dynamixel/cuello_dynamixel.xml";

    DynamixelDriver driver;

    printMenu();
    do {
        option.clear();
        cout <<  endl << "-----------------------------------------------------" << endl;
        cout <<  " Option: ";
        cin >> option;

        // Print help (the menu)
        if (option.compare("h") == 0 || option.compare("help") == 0 ){
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printMenu();

            // Connect the driver to the computer
        }else if (option.compare("co")  == 0 || option.compare ("con") == 0 || option.compare ("connect") == 0 ){
            if (connected == false){
                driver.connect();
                cout << "Driver Connected" << endl;
            }else{
                cout << "You are already connected" << endl;
            }

            // Disconnect the driver from the computer
        }else if (option.compare("dis")  == 0 || option.compare ("disconnect") == 0 ){
            driver.disconnect();
            connected = false;
            cout << "Driver Disconnected";

            // Set max position
        }else if (option.compare("mp")  == 0 || option.compare ("maxpos") == 0 ){
            cout << "Maximum Position (in pulses): ";
            cin >> keyInput;
            driver.setMaxPosition(atol(keyInput.data()));

            // Set min position
        }else if (option.compare("mip") == 0 || option.compare ("minpos") == 0 ){
            cout << "Minimum Position (in pulses): ";
            cin >> keyInput;
            driver.setMinPosition(atol(keyInput.data()));

            // Set max velocity
        }else if (option.compare("mv") == 0 || option.compare ("maxvel") == 0 ){
            cout << "Maximum Velocity (in rpm): ";
            cin >> keyInput;
            driver.setMaxVelocity(atol(keyInput.data()));

            // Move to an absolute position
        }else if (option.compare("mabs") == 0 || option.compare ("abs") == 0 ){
            cout << "Set the position to move (in pulses): ";
            cin >> keyInput;
            driver.moveDriverToAbsolutePosition(atol(keyInput.data()));

            // Move to an relative position
        }else if (option.compare("mrel") == 0 || option.compare ("rel") == 0 ){
            cout << "Set the position to move (in pulses): ";
            cin >> keyInput;
            driver.moveDriverToRelativePosition(atol(keyInput.data()));

            // Move by velocity
        }else if (option.compare("v") == 0 || option.compare ("vel") == 0 ) {
            cout << "Set the velocity to move (in rpm): ";
            cin >> keyInput;
            driver.moveDriverByVelocity(atol(keyInput.data()));

            // Print limits
        }else if (option.compare ("pl") == 0 || option.compare("plimit") == 0 || option.compare ("plim") == 0 ){
            driver.printLimits();

            // Print Motor Sensor
        }else if (option.compare("ms") == 0 || option.compare ("sens") == 0 || option.compare ("sensor") == 0 ){
            driver.printMotorSensorInfo();

            // Print the driver status
        }else if (option.compare("ps") == 0 || option.compare ("pstatus") == 0 || option.compare("stat") == 0 ){
            driver.printStatus();

            // Print configuration
        }else if (option.compare("pc") == 0 || option.compare ("pconf") == 0 ){
            driver.printConfig();

            // print all (limits, status Motor Sensor, configuration)
        }else if (option.compare("pa") == 0 || option.compare ("pall") == 0|| option.compare ("print") == 0 ){
            driver.printLimits();
            driver.printMotorSensorInfo();
            driver.printStatus();
            driver.printConfig();

            // Calibrate the driver
        }else if (option.compare("c") == 0 || option.compare ("cal") == 0 || option.compare ("calibrate") == 0 ){
            cout << "Calibrating the driver." << endl;
            bool b = true;
            if (driver.calibrate(b) != ERR_NOERR)
                perror ("Could not calibrate the driver\n");
            else
                printf ("Driver Calibrated\n");

            // Enable the driver
        }else if (option.compare("e") == 0 || option.compare ("en") == 0 || option.compare ("enable") == 0 ){	// Enable the driver
            driver.enable();
            cout << "Driver Enabled";

            // Disable the driver
        }else if (option.compare("d") == 0 || option.compare ("di") == 0 || option.compare ("disable") == 0 ){	// Disable the driver
            cout << "Disabling Driver" << endl;
            driver.disable();
            cout << "OK, Driver Disabled" << endl;

            // Load the configuration from a file
        }else if (option.compare("l") == 0 || option.compare ("load") == 0 ){
            string lFile;
            cout << "Please, write the filename: ";
            cin >> lFile;
            cout << "Loading configuration from the file." << endl;
            if (driver.loadConfigFromFile(lFile) != ERR_NOERR){
                perror ("There has been an error loading the configuration.");
            }else {
                cout << "Configuration loaded. Now you can download it to the driver" << endl;
            }

            // Save the configuration to a file
        }else if (option.compare("s") == 0 || option.compare ("save") == 0 ){
            string sFile;
            cout << "Please, write the filename to save: ";
            cin >> sFile;
            if (driver.writeConfigToFile(sFile) != ERR_NOERR){
                perror ("Error saving the configuration to the file.");
            }else{
                cout << "File saved successfully" << endl;
            }

            // Upload configuration to the driver
        }else if (option.compare("do") == 0 || option.compare ("download") == 0 ){
            cout << "Writing the configuration to the driver." << endl;
            if (driver.writeConfigToDriver() != ERR_NOERR){
                perror ("There has been an error uploading the configuration to the driver");
            } else {
                cout << "Configuration uploaded to the driver" << endl;
            }

            // Quit the program
        }else if (option.compare("q") == 0 || option.compare ("quit") == 0 || option.compare("exit") == 0 ){
            exitMenu = true;
            break;

        }else {
            cout << "Unknown command" << endl;
            cout << "Press \"h\" to print all the supported commands" << endl;
            cout <<  " Option: ";
        }
    }while (exitMenu == false);
}
