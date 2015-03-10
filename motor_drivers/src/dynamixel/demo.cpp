/**
 * Program to test the driver Dynamixel
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#include "DynamixelDriver.h"
#include <string>
#include <iostream>


using namespace std;


int main (){

    DynamixelDriver driver2;
    DynamixelDriver driver5;
    DynamixelDriver driver4;

    string command;
    char op;
    //int i;
    bool connected = false;

    //******************************

    cout << "Loading configuration from the file." << endl;
    if (driver4.loadConfigFromFile("dynamixel/cuello_dynamixel4.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    driver4.connect();
    driver4.writeConfigToDriver();

    cout << "Loading configuration from the file." << endl;
    if (driver2.loadConfigFromFile("dynamixel/cuello_dynamixel2.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    driver2.connect();
    driver2.writeConfigToDriver();


    cout << "Loading configuration from the file." << endl;
    if (driver5.loadConfigFromFile("dynamixel/cuello_dynamixel5.xml") != ERR_NOERR){
        perror ("There has been an error loading the configuration.");
    }else {
        cout << "Configuration loaded. Now you can upload it to the driver" << endl;
    }
    driver5.connect();
    driver5.writeConfigToDriver();





    //CONECTAR LOS 3 SERVOS
    /*if (connected == false){
  driver2.connect();
  cout << "Driver Connected" << endl;
 }else{
  cout << "You are already connected" << endl;
 }
 if (connected == false){
  driver5.connect();
  cout << "Driver Connected" << endl;
 }else{
  cout << "You are already connected" << endl;
 }
 if (connected == false){
  driver1.connect();
  cout << "Driver Connected" << endl;
 }else{
  cout << "You are already connected" << endl;
 }
 */


    //HABILITAR LOS 3 SERVOS
    driver2.enable();
    driver5.enable();
    driver4.enable();

    //MENU DE GESTOS
    do{

        printf("1. Sleepy \n");
        printf("2. Crazy \n");
        printf("3. Happy \n");
        printf("4. COMPLETE DEMO \n");
        printf("5. Disable drivers \n");

        cout << "\n \n Choose an option: ";
        cin >> op;

        switch (op){
        case '1':
            //sleepy

            driver2.setMaxVelocity(20);
            driver4.setMaxVelocity(30);
            driver5.setMaxVelocity(40);

            driver2.moveDriverToAbsolutePosition(800);
            driver5.moveDriverToAbsolutePosition(350);
            driver4.moveDriverToAbsolutePosition(690);
            sleep(9);

            driver2.moveDriverToAbsolutePosition(400);
            driver5.moveDriverToAbsolutePosition(580);
            driver4.moveDriverToAbsolutePosition(600);
            sleep(5);

            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(550);
            sleep(2);



            break;

        case '2':
            //crazy
            driver2.setMaxVelocity(512);
            driver5.setMaxVelocity(512);
            driver4.setMaxVelocity(512);

            driver2.moveDriverToAbsolutePosition(210);
            driver5.moveDriverToAbsolutePosition(200);
            driver4.moveDriverToAbsolutePosition(400);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(830);
            driver5.moveDriverToAbsolutePosition(840);
            driver4.moveDriverToAbsolutePosition(600);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);
            sleep(1);

            driver5.moveDriverToAbsolutePosition(300);
            driver4.moveDriverToAbsolutePosition(690);
            sleep(1);
            driver5.moveDriverToAbsolutePosition(750);
            driver4.moveDriverToAbsolutePosition(210);
            sleep(1);

            driver5.moveDriverToAbsolutePosition(512);
            driver2.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);




            break;
        case '3':
            //happy
            driver2.setMaxVelocity(900);
            driver5.setMaxVelocity(900);
            driver4.setMaxVelocity(900);

            driver2.moveDriverToAbsolutePosition(400);
            driver5.moveDriverToAbsolutePosition(550);
            driver4.moveDriverToAbsolutePosition(600);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(640);
            driver5.moveDriverToAbsolutePosition(350);
            driver4.moveDriverToAbsolutePosition(450);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);

            break;


        case '4':
            //complete demo
            //sleepy
            driver2.setMaxVelocity(900);
            driver4.setMaxVelocity(30);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(210);
            sleep(1);

            driver2.setMaxVelocity(1023);
            driver2.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);
            sleep(1);

            driver2.setMaxVelocity(50);
            driver4.setMaxVelocity(50);
            driver5.setMaxVelocity(50);
            driver5.moveDriverToAbsolutePosition(520);
            driver2.moveDriverToAbsolutePosition(800);
            driver4.moveDriverToAbsolutePosition(550);
            sleep(1);

            driver5.moveDriverToAbsolutePosition(520);
            driver2.moveDriverToAbsolutePosition(300);
            driver4.moveDriverToAbsolutePosition(690);
            sleep(1);
            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(550);

            //crazy
            sleep(3);
            driver2.setMaxVelocity(512);
            driver5.setMaxVelocity(512);
            driver4.setMaxVelocity(512);

            driver2.moveDriverToAbsolutePosition(210);
            driver5.moveDriverToAbsolutePosition(200);
            driver4.moveDriverToAbsolutePosition(400);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(830);
            driver5.moveDriverToAbsolutePosition(840);
            driver4.moveDriverToAbsolutePosition(600);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);
            sleep(1);

            driver5.moveDriverToAbsolutePosition(300);
            driver4.moveDriverToAbsolutePosition(690);
            sleep(1);
            driver5.moveDriverToAbsolutePosition(750);
            driver4.moveDriverToAbsolutePosition(210);
            sleep(1);

            driver5.moveDriverToAbsolutePosition(512);
            driver2.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);

            //happy
            sleep(3);
            driver2.moveDriverToAbsolutePosition(400);
            driver5.moveDriverToAbsolutePosition(550);
            driver4.moveDriverToAbsolutePosition(600);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(640);
            driver5.moveDriverToAbsolutePosition(350);
            driver4.moveDriverToAbsolutePosition(450);
            sleep(1);

            driver2.moveDriverToAbsolutePosition(512);
            driver5.moveDriverToAbsolutePosition(512);
            driver4.moveDriverToAbsolutePosition(512);


            break;


        case '5':
            //disable drivers
            driver2.disable();
            driver5.disable();
            driver4.disable();
            printf("\n ----- Drivers disables -----\n");
            break;


        }
    }
    while (op != 0);

    return 0;
}




