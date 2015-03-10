/**
 * testDynamixelPV.cpp
 *
 *  Created on: 16/12/2010
 *  Author: Paulina Vélez Núñez
 *
 *  Program to move 2 or more Dynamixel drivers at the same time
 */

#ifndef TESTDYNAMIXELPV_H_
#define TESTDYNAMIXELPV_H_

#include <string>
#include <iostream>
#include "actuator/Actuator.h"

using namespace std;
using namespace actuator;

Actuator actuator1;
Actuator actuator2;
Actuator actuator3;
Actuator actuator4;
Actuator actuator5;

void goto_start_position(){
    actuator1.moveToPosition(2.621);
    actuator2.moveToPosition(2.621);
    actuator3.moveToPosition(2.621);
    actuator4.moveToPosition(2.447);
    actuator5.moveToPosition(2.621);
    usleep(150000);
}

void set_max_speeds(float speed1, float speed2, float speed3, float speed4, float speed5){
    actuator1.setMaxVelocity(speed1);
    actuator1.downloadConfigToDriver();
    actuator2.setMaxVelocity(speed2);
    actuator2.downloadConfigToDriver();
    actuator3.setMaxVelocity(speed3);
    actuator3.downloadConfigToDriver();
    actuator4.setMaxVelocity(speed4);
    actuator4.downloadConfigToDriver();
    actuator5.setMaxVelocity(speed5);
    actuator5.downloadConfigToDriver();
}

int main (){

    string command;
    char op;


    printf("\n\n ---------You will manage two drivers DYNAMIXEL AX-12----------\n\n");

    cout << "Loading configuration from the file." << endl;
    if (actuator1.loadConfigFromFile("necky/actuator/cuello1.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    actuator1.link();
    actuator1.downloadConfigToDriver();

    cout << "Loading configuration from the file." << endl;
    if (actuator2.loadConfigFromFile("necky/actuator/cuello2.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    actuator2.link();
    actuator2.downloadConfigToDriver();

    cout << "Loading configuration from the file." << endl;
    if (actuator3.loadConfigFromFile("necky/actuator/cuello3.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    actuator3.link();
    actuator3.downloadConfigToDriver();

    cout << "Loading configuration from the file." << endl;
    if (actuator4.loadConfigFromFile("necky/actuator/cuello4.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    actuator4.link();
    actuator4.downloadConfigToDriver();

    cout << "Loading configuration from the file." << endl;
    if (actuator5.loadConfigFromFile("necky/actuator/cuello5.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    actuator5.link();
    actuator5.downloadConfigToDriver();

    do{

        printf("\n\ne. Enable drivers \n");
        printf("d. Disable drivers\n");
        printf("1. Move to absolute position\n");
        printf("2. Print motor sensor data \n");
        printf("3. Print limits \n\n");

        printf("a. Angry \n");
        printf("b. Bored \n");
        printf("i. Suspicious \n");
        printf("l. Laugh \n");
        printf("m. Sad \n");
        printf("o. Normal \n");
        printf("p. Surprised \n");
        printf("s. Sleepy \n");
        printf("t. Waiting \n");
        printf("w. Wake up \n");

        printf("y. Yes \n");
        printf("n. No \n");
        printf("x. Complete Demo \n");

        cout << "\n \n Choose an option: ";
        cin >> op;

        switch (op){

        case 'e':
            //enable drivers
            actuator1.enable();
            actuator2.enable();
            actuator3.enable();
            actuator4.enable();
            actuator5.enable();
            printf("\n ----- Drivers enabled -----\n");
            break;

        case 'd':
            //disable drivers
            actuator1.disable();
            actuator2.disable();
            actuator3.disable();
            actuator4.disable();
            actuator5.disable();
            printf("\n ----- Drivers disabled -----\n");
            break;

        case '1':
            cout << "Position to driver 1 (in rad): ";
            cin >> command;
            actuator1.enable();

            cout << "Position to driver 2 (in rad): ";
            cin >> command;
            actuator2.enable();

            cout << "Position to driver 3 (in rad): ";
            cin >> command;
            actuator3.enable();

            cout << "Position to driver 4 (in rad): ";
            cin >> command;
            actuator4.enable();

            cout << "Position to driver 5 (in rad): ";
            cin >> command;
            actuator5.enable();

            //move to position
            actuator1.moveToPosition(atof (command.data()));
            actuator2.moveToPosition(atof (command.data()));
            actuator3.moveToPosition(atof (command.data()));
            actuator4.moveToPosition(atof (command.data()));
            actuator5.moveToPosition(atof (command.data()));
            break;

        case '2':
            //print motor sensor data
            printf("\n ----- Motor Sensor Data -----\n");
            cout << "Actuator 1: " << endl;
            actuator1.printMotorSensorData();
            cout << "Driver 1: " << endl;
            actuator1.printDriverMotorSensorData();
            cout << "Actuator 2: " << endl;
            actuator2.printMotorSensorData();
            cout << "Driver 2: " << endl;
            actuator2.printDriverMotorSensorData();
            cout << "Actuator 3: " << endl;
            actuator3.printMotorSensorData();
            cout << "Driver 3: " << endl;
            actuator3.printDriverMotorSensorData();
            cout << "Actuator 4: " << endl;
            actuator4.printMotorSensorData();
            cout << "Driver 4: " << endl;
            actuator4.printDriverMotorSensorData();
            cout << "Actuator 5: " << endl;
            actuator5.printMotorSensorData();
            cout << "Driver 5: " << endl;
            actuator5.printDriverMotorSensorData();
            break;

        case '3':
            //print limits from drivers
            printf("\n ----- Limits of Drivers -----\n");

            cout << "=== Actuator 1 === ";
            actuator1.printLimits();
            cout << "=== Actuator 2 === ";
            actuator2.printLimits();
            cout << "=== Actuator 3 === ";
            actuator3.printLimits();
            cout << "=== Actuator 4 === ";
            actuator4.printLimits();
            cout << "=== Actuator 5 === ";
            actuator5.printLimits();
            break;

        case 's':
            //sleepy
            goto_start_position();
            set_max_speeds(3.8, 3, 3, 3, 3);

            actuator1.moveToPosition(2.43);
            actuator2.moveToPosition(1.39);
            actuator3.moveToPosition(1.46);
            actuator4.moveToPosition(3.2);
            actuator5.moveToPosition(3.34);
            usleep(150000);

            break;

        case 'b':
            //Bored
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(5, 5, 5, 5, 5);

            for (int i=0; i<2; i++){
                actuator1.moveToPosition(2.4);
                usleep(2500000);
                actuator1.moveToPosition(2.86);
                usleep(2500000);
                actuator1.moveToPosition(2.63);
                usleep(3000000);
                actuator4.moveToPosition(2.3);
                actuator2.moveToPosition(2.9);
                usleep(750000);
                for (int j=0; j<2; j++){
                    actuator5.moveToPosition(2.4);
                    usleep(2500000);
                    actuator5.moveToPosition(2.86);
                    usleep(2500000);
                }

                actuator3.moveToPosition(3.39);
                usleep(1500000);
                actuator5.moveToPosition(2.4);
                actuator3.moveToPosition(1.87);
                usleep(3000000);
                actuator3.moveToPosition(2.63);
                usleep(1000000);
            }
            goto_start_position();
            usleep(50000);



            break;

        case 'w':
            //wake up
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);
            set_max_speeds(5, 5, 5, 5, 5);

            actuator3.moveToPosition(3.39);
            usleep(1500000);
            actuator3.moveToPosition(1.87);
            usleep(3000000);
            actuator3.moveToPosition(2.63);
            usleep(100000);

            actuator1.moveToPosition(2.73);
            actuator2.moveToPosition(3.11);
            actuator3.moveToPosition(3.33);
            actuator4.moveToPosition(3.12);
            actuator5.moveToPosition(3.06);
            usleep(50000);


            break;


        case 'i':
            //suspicious
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(5, 5, 5, 5, 5);

            actuator2.moveToPosition(1.77);
            usleep(2500000);
            actuator4.moveToPosition(1.73);

            usleep(50000);


            break;

        case 't':
            //waiting
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(5, 5, 5, 5, 5);

            for (int i=0; i<10; i++){
                actuator1.moveToPosition(2.5);
                usleep(2500000);
                actuator1.moveToPosition(2.76);
                usleep(2500000);
            }

            usleep(50000);


            break;

        case 'l':
            //laugh
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(7, 7, 10, 7, 7);

            actuator1.moveToPosition(2.64);
            actuator2.moveToPosition(2);
            actuator3.moveToPosition(3.2);
            actuator4.moveToPosition(3.2);
            actuator5.moveToPosition(2.61);
            usleep(1000000);
            actuator3.moveToPosition(1.8);
            usleep(1000000);
            actuator3.moveToPosition(3.2);

            usleep(1000000);
            goto_start_position();
            actuator2.moveToPosition(3.22);
            actuator3.moveToPosition(1.8);
            actuator4.moveToPosition(2);
            usleep(1000000);
            actuator3.moveToPosition(3.4);
            actuator4.moveToPosition(1.4);
            usleep(1000000);
            actuator3.moveToPosition(1.8);
            actuator4.moveToPosition(2);
            usleep(1500000);
            actuator4.moveToPosition(1.5);
            actuator3.moveToPosition(1.8);
            usleep(1000000);
            goto_start_position();
            break;

        case 'a':
            //angry
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            actuator2.moveToPosition(2.047);

            break;

        case 'm':
            //sad
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(3.8, 3.8, 3.8, 3.8, 3.8);

            actuator4.moveToPosition(3);
            usleep(750000);


            actuator1.moveToPosition(2.3);
            actuator3.moveToPosition(3.06);
            actuator4.moveToPosition(3.26);
            actuator5.moveToPosition(2.51);
            usleep(1500000);
            actuator1.moveToPosition(2.9);
            usleep(1500000);
            actuator1.moveToPosition(2.3);
            usleep(1500000);
            actuator1.moveToPosition(2.9);
            usleep(1500000);
            set_max_speeds(3.8, 3.8, 3.8, 5, 3.8);
            actuator1.moveToPosition(2.3);
            actuator4.moveToPosition(2.0);
            usleep(1500000);

            sleep(1);
            break;

        case 'o':
            //Normal
            set_max_speeds(3.8, 3.8, 3.8, 3.8, 3.8);
            goto_start_position();
            sleep(1);
            break;

        case 'p':
            //Surprised
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(0, 15, 0, 8, 0);

            actuator2.moveToPosition(3.43);
            actuator4.moveToPosition(2.93);
            usleep(1500000);
            break;

        case 'y':
            //Yes
            set_max_speeds(0, 0, 0, 10, 0);
            goto_start_position();
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            goto_start_position();
            break;

        case 'n':
            //No
            set_max_speeds(0, 0, 0, 0, 10);
            goto_start_position();

            actuator5.moveToPosition(2.53);
            usleep(150000);
            actuator5.moveToPosition(2.73);
            usleep(150000);
            actuator5.moveToPosition(2.53);
            usleep(150000);
            actuator5.moveToPosition(2.73);
            usleep(150000);

            goto_start_position();
            break;

        case 'x':

            //sleepy
            goto_start_position();
            set_max_speeds(3.8, 3, 3, 3, 3);

            actuator1.moveToPosition(2.43);
            actuator2.moveToPosition(1.39);
            actuator3.moveToPosition(1.46);
            actuator4.moveToPosition(3.2);
            actuator5.moveToPosition(3.34);

            sleep(10);

            //wake up
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);
            set_max_speeds(5, 5, 5, 5, 5);

            actuator3.moveToPosition(3.39);
            usleep(1500000);
            actuator3.moveToPosition(1.87);
            usleep(3000000);
            actuator3.moveToPosition(2.63);
            usleep(100000);

            actuator1.moveToPosition(2.73);
            actuator2.moveToPosition(3.11);
            actuator3.moveToPosition(3.33);
            actuator4.moveToPosition(3.12);
            actuator5.moveToPosition(3.06);
            usleep(5000000);

            //suspicious
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(5, 5, 5, 5, 5);

            actuator2.moveToPosition(1.77);
            usleep(2500000);
            actuator4.moveToPosition(1.73);

            usleep(9000000);

            //waiting
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(2, 5, 5, 5, 5);

            for (int i=0; i<6; i++){
                actuator1.moveToPosition(2.5);
                usleep(2500000);
                actuator1.moveToPosition(2.76);
                usleep(2500000);
            }

            usleep(2500000);

            //Surprised
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(0, 15, 0, 8, 0);

            actuator2.moveToPosition(3.43);
            actuator4.moveToPosition(2.93);
            usleep(3500000);

            //Yes
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(0, 0, 0, 10, 0);
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            goto_start_position();

            //chiste
            sleep(10);

            //laugh
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(7, 7, 10, 7, 7);

            actuator1.moveToPosition(2.64);
            actuator2.moveToPosition(2);
            actuator3.moveToPosition(3.2);
            actuator4.moveToPosition(3.2);
            actuator5.moveToPosition(2.61);
            usleep(1000000);
            actuator3.moveToPosition(1.8);
            usleep(1000000);
            actuator3.moveToPosition(3.2);

            usleep(1000000);
            goto_start_position();
            actuator2.moveToPosition(3.22);
            actuator3.moveToPosition(1.8);
            actuator4.moveToPosition(2);
            usleep(1000000);
            actuator3.moveToPosition(3.4);
            actuator4.moveToPosition(1.4);
            usleep(1000000);
            actuator3.moveToPosition(1.8);
            actuator4.moveToPosition(2);
            usleep(1500000);
            actuator4.moveToPosition(1.5);
            actuator3.moveToPosition(1.8);
            usleep(1000000);
            goto_start_position();

            usleep(2000000);

            //Bored
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(5, 5, 5, 5, 5);

            actuator1.moveToPosition(2.4);
            usleep(2500000);
            actuator1.moveToPosition(2.86);
            usleep(2500000);
            actuator1.moveToPosition(2.63);
            usleep(3000000);
            actuator4.moveToPosition(2.3);
            actuator2.moveToPosition(2.9);
            usleep(750000);
            
            actuator5.moveToPosition(2.4);
            usleep(2500000);
            actuator5.moveToPosition(2.86);
            usleep(2500000);
            
            actuator3.moveToPosition(3.39);
            usleep(1500000);
            actuator5.moveToPosition(2.4);
            actuator3.moveToPosition(1.87);
            usleep(3000000);
            actuator3.moveToPosition(2.63);
            usleep(1000000);

            goto_start_position();
            usleep(50000);

            //Te aburres?
            sleep(5);

            //Yes
            set_max_speeds(0, 0, 0, 10, 0);
            goto_start_position();
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            actuator4.moveToPosition(3.531);
            usleep(150000);
            actuator4.moveToPosition(1.791);
            usleep(150000);
            goto_start_position();

            //Pues a dormir
            sleep(3);

            //No
            set_max_speeds(0, 0, 0, 0, 10);
            goto_start_position();

            actuator5.moveToPosition(2.53);
            usleep(150000);
            actuator5.moveToPosition(2.73);
            usleep(150000);
            actuator5.moveToPosition(2.53);
            usleep(150000);
            actuator5.moveToPosition(2.73);
            usleep(1500000);

            //Que sí
            sleep(3);

            //sad
            set_max_speeds(4, 4, 4, 4, 4);
            goto_start_position();
            usleep(2500000);

            set_max_speeds(3.8, 3.8, 3.8, 3.8, 3.8);

            actuator4.moveToPosition(3);
            usleep(750000);


            actuator1.moveToPosition(2.3);
            actuator3.moveToPosition(3.06);
            actuator4.moveToPosition(3.26);
            actuator5.moveToPosition(2.51);
            usleep(1500000);
            actuator1.moveToPosition(2.9);
            usleep(1500000);
            actuator1.moveToPosition(2.3);
            usleep(1500000);
            actuator1.moveToPosition(2.9);
            usleep(1500000);
            set_max_speeds(3.8, 3.8, 3.8, 5, 3.8);
            actuator1.moveToPosition(2.3);
            actuator4.moveToPosition(2.0);
            usleep(4000000);

            //sleepy
            goto_start_position();
            usleep(2500000);
            set_max_speeds(3.8, 3, 3, 3, 3);

            actuator1.moveToPosition(2.43);
            actuator2.moveToPosition(1.39);
            actuator3.moveToPosition(1.46);
            actuator4.moveToPosition(3.2);
            actuator5.moveToPosition(3.34);
            usleep(150000);

            break;

        }
    }
    while (op != 0);

    return 0;
}

#endif /* TESTDYNAMIXELPV_H_ */
