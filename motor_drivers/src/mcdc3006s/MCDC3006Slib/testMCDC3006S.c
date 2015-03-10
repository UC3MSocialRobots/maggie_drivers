/***************************************************************************//**
 * @file prbDriverMCDC3006S.c
 * 
 * @brief The test file for the driver MCDC3006S
 * @author Victor Gonzalez ( vgonzale@ing.uc3m.es )
 * @date 30/06/2009
*******************************************************************************/

#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"

#define AD_HOME getenv( "AD_HOME" )	/* char *getenv(const char *nombre); */

#define TRUE 1
#define FALSE 0

#define DEFAULT_SERIAL_DEVICE "/dev/brazo_derecho"
#define DEFAULT_SEMFILE "/robot/etc/semDrive"

typedef int boolean;


void mainMenu (driverSensor_t * odo, driverConf_t * dc);
int moveToPosition (long int position, boolean relative, driverSensor_t * odo, driverConf_t * dc);
int moveVelocity (long int velocity, driverConf_t * dc);


int main( int argc, char * argv[] ) {
  driverSensor_t odo;
  driverConf_t dc;
  
  // Initial driverConf_t dc parameters
  
  dc.maxPos = 100000; // In pulses of the driver
  dc.minPos = -100000; // In pulses of the driver
  dc.maxVel = 1000; // In rpm
  dc.maxAcc = 200; 
  dc.maxDec = 200;
  
  
  int baudrate = 19200;
//  int baudrate = 9600;
  char serialDevice[128];
  int initValue; 		/// Used to monitor the status of the initCommunication() method
  key_t keySem;
  
//  int RSd = -1; // Driver File Descriptor
//  int semID;	//
  
#ifdef DEBUG   
  printf ("$AD_HOME is %s\n\r", AD_HOME); //\TODO: Error check here. Exit the program if $AD_HOME is null
#endif
  
  if( argc > 1 ) strcpy( serialDevice, argv[1] );
  else {
	  strcpy( serialDevice, DEFAULT_SERIAL_DEVICE); //"/dev/brazo_derecho" );
#ifdef DEBUG
	  printf ("SerialDevice is %s\n\r", serialDevice);
#endif
  }
  
  if( argc > 2 ) strcpy( dc.sem.semFile, argv[2] );
  else {
    strcpy( dc.sem.semFile, AD_HOME );
    strcat( dc.sem.semFile, DEFAULT_SEMFILE ); //"/robot/etc/semDrive" );

  }
 
  initValue = initCommunication(baudrate, serialDevice, &dc.RSd, dc.sem.semFile, &dc.sem.semID );
    
  switch (initValue) {
	case -1:
		fprintf(stderr, "Could not establish connections with serial port at %s\n\r", serialDevice);
		return -1;
		break;
	case -2:
		fprintf(stderr, "Could not create semaphore file at %s\n\r", dc.sem.semFile);
		return -2;
		break;
	case -3:
		fprintf(stderr, "Could not set zero odometry\n\r");
		return -3;
		break;
	case -4:
		fprintf (stderr, "Semaphore related error\n\r");
		return -4;
		break;
	default:
		break;
  }
  
   
  setDriverConf( dc ); 

  // Executing the main menu
  mainMenu(&odo,&dc);

  // Removing the Semaphore
  rmSem( dc.sem.semID );

  return 0;
}

/*!
 * \brief  Main Menu. Shows the menu and performs usered entered commands until the user exits de program
 * 
 */
void mainMenu(driverSensor_t * odo, driverConf_t * dc) {
	
	driverStatus_t drvStat;
	
	int option = -1;
	int subOption;
	int exit = FALSE;
	int subOptionExit = FALSE;
	
	long int param; 	// Aux parameter to read/write commands into de driver
	long int oldPos; 	// Parameter used to store the last position of the driver
	
	do {
		printf("\n\r\n\r");
		printf("--- MAIN MENU ---\n\r");
		printf("Available options\n\r");
		printf("1.Set/Print Max/Min positions\n\r");
		printf("2.Set/Print Max velocity\n\r");
		printf("3.Set/Print Max acceleration\n\r");
		printf("4.Set/Print Max decceleration\n\r");
		printf("5.Print Odometry Data\n\r");
		printf("6.Move the driver\n\r");
		printf("7.Enable Driver\n\r");
		printf("8.Disable Driver\n\r");
		printf("9.Show Driver Status\n\r");
		printf("10.Calibrate the Driver\n\r");
		printf("0.Exit\n\r");
		printf("\n\r\n\r");
		printf("Choose an option:");
		scanf("%d", &option);
		printf ("\n\r");
		if (option == 1){ 	//Set/Print Max/Min positinons
			do {
				printf("\n\r\n\r");
				printf("--- Change or Print Max and Min positions ---\n\r");
				printf("Available options:\n\r");
				printf("1.Set Max Pos\n\r");
				printf("2.Set Min Pos\n\r");
				printf("3.Print Max and Min Pos\n\r");
				printf("0.Back to Main menu\n\r");
				printf("Option:");
				scanf("%d",&subOption);
				printf("\n\r");
				
				switch (subOption){
				case 1:
					printf ("Please write the max position: ");
					scanf("%ld",&dc->maxPos); 		// Store the Max Pos keyboard input in the driver Configuration structure  
					printf ("\n\r");
					if (setDriverMaxPos(dc->RSd, dc->sem.semID, dc->maxPos) == ERR_NOERR ){
						if ( getDriverMaxPos(dc->RSd, dc->sem.semID) >= ERR_NOERR)
							printf ("OK. Curent Max Position is: %ld\n\r",getDriverMaxPos(dc->RSd, dc->sem.semID));
						else 
							fprintf (stderr, "There has been an error.\n\r");
					}
					else 
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 2:
					printf ("Please write the min position: ");
					scanf("%ld",&param);
					printf ("\n\r");
					if (setDriverMinPos(dc->RSd, dc->sem.semID, param) == ERR_NOERR )
						printf ("OK. Curent Min Position is: %ld \n\r", getDriverMinPos(dc->RSd, dc->sem.semID));
					else
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 3:
					
					if (getDriverMaxPos(dc->RSd, dc->sem.semID) >= ERR_NOERR){
						printf ("MAX Pos: %ld \n\r", dc->maxPos);
					}else printf ("There has been an error getting the max position\n\r");
					
					printf ("MIN Pos: %ld \n\r",getDriverMinPos(dc->RSd, dc->sem.semID));
					subOptionExit = FALSE;
					break;
				case 0:
					subOptionExit = TRUE;
					printf ("subOption: %d. Option: %d\n\r", subOption, option);
					continue;
				default:
					printf ("Incorrect Option.\n\r");
					subOptionExit = FALSE;
					break;
				}	
			} while (subOptionExit != TRUE); 	
		}										// End Option 1
		else if (option == 2){					// Set/Print Max velocity
			do {
				printf("\n\r\n\r");
				printf("--- Change or Print Max velocity ---\n\r");
				printf("Available options:\n\r");
				printf("1.Set Max Velocity\n\r");
				printf("2.Print Max Velocities\n\r");
						printf("0.Back to Main menu\n\r");
						printf("Option:");
				scanf("%d",&subOption);
				printf("\n\r");

				switch (subOption) {
				case 1:
					printf ("Please write the max velocity: ");
					scanf("%ld",&param);
					printf ("\n\r");
					if (setDriverMaxVel(dc->RSd, dc->sem.semID, param) == ERR_NOERR)
						printf ("OK. Curent Max velocity is: %ld\n\r", getDriverMaxVel(dc->RSd, dc->sem.semID));
					else
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 2:
					printf ("MAX Velocity: %ld \n\r",getDriverMaxVel(dc->RSd, dc->sem.semID));
					subOptionExit = FALSE;
					break;
				case 0:
					subOptionExit = TRUE;
					break;
				default:
					printf ("Incorrect Option.\n\r");
					subOptionExit = FALSE;
					break;
				}
			} while (subOptionExit != TRUE);

		}										// End Option 2
		else if (option == 3)	{				// Set/Print Max acceleration/deceleration\n\r
			do {
				printf("\n\r\n\r");
				printf("--- Change or Print Max acceleration ---\n\r");
				printf("Available options:\n\r");
				printf("1.Set Max Acceleration\n\r");
				printf("2.Print Max Acceleration\n\r");
				printf("0.Back to Main menu\n\r");
				printf("Option:");
				scanf("%d",&subOption);
				printf("\n\r");

				switch (subOption) {
				case 1:
					printf ("Please write the max acceleration: ");
					scanf("%ld",&param);
					printf ("\n\r");
					if (setDriverMaxAcc(dc->RSd, dc->sem.semID, param) == ERR_NOERR)
						printf ("OK. Curent Max acceleration is: %ld\n\r", getDriverMaxAcc(dc->RSd, dc->sem.semID));
					else
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 2:
					printf ("MAX acceleration: %ld \n\r",getDriverMaxAcc(dc->RSd, dc->sem.semID));
					subOptionExit = FALSE;
					break;
				case 0:
					subOptionExit = TRUE;
					break;
				default:
					printf ("Incorrect Option.\n\r");
					subOptionExit = FALSE;
					break;
				}
			} while (subOptionExit != TRUE);
		}		// End option 3
		else if (option == 4)	{	// Set or Print max decceleration
			do {
				printf("\n\r\n\r");
				printf("--- Change or Print Max decceleration ---\n\r");
				printf("Available options:\n\r");
				printf("1.Set Max Decceleration\n\r");
				printf("2.Print Max Decceleration\n\r");
				printf("0.Back to Main menu\n\r");
				printf("Option:");
				scanf("%d",&subOption);
				printf("\n\r");

				switch (subOption) {
				case 1:
					printf ("Please write the max decceleration: ");
					scanf("%ld",&param);
					printf ("\n\r");
					if (setDriverMaxDec(dc->RSd, dc->sem.semID, param) == ERR_NOERR)
						printf ("OK. Curent Max acceleration is: %ld\n\r", getDriverMaxDec(dc->RSd, dc->sem.semID));
					else
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 2:
					printf ("MAX acceleration: %ld \n\r",getDriverMaxDec(dc->RSd, dc->sem.semID));
					subOptionExit = FALSE;
					break;
				case 0:
					subOptionExit = TRUE;
					break;
				default:
					printf ("Incorrect Option.\n\r");
					subOptionExit = FALSE;
					break;
				}
			} while (subOptionExit != TRUE);
		}										// End of option 4
		else if (option==5) {					// Option 5: Print Odometry Data
			getDriverSensor(dc->RSd, dc->sem.semID, odo);
			printf ("Odometry data:\n\r");
			printf ("Current position: %ld \n\r", odo->p);
			printf ("Current velocity: %ld \n\r", odo->v); // End of option 5
		}
		else if (option==6) {							// Option 6: Do movements

			long int velo; 			// Parameter that will be used store the velocity of the driver (rpm)
			
			do {
				printf("\n\r\n\r");
				printf("--- Do Movements menu ---\n\r");
				printf("Available options:\n\r");
				printf("1.Move to an absolute position\n\r");
				printf("2.Move to a relative position\n\r");
				printf("3.Move by velocity.\n\r");
				printf("0.Back to Main menu\n\r");
				printf("Option:");
				scanf("%d",&subOption);
				printf("\n\r");

				switch (subOption) {

				case 1:				// Move to an absolute position
					printf ("Please write the absolute position in pulses: ");
					scanf("%ld",&param);
					printf ("\n\r");

					printf ("Trying to move to %ld\n\r",param);
					if (moveToPosition(param, FALSE, odo, dc) != ERR_NOERR )
						fprintf (stderr, "There has been an error.\n\r");

					subOptionExit = FALSE;
					break;
				case 2:			//Move to a relative position
					printf ("Please write the relative position in pulses: ");
					scanf("%ld",&param);
					printf ("\n\r");

					printf ("Trying to move to %ld\n\r",param);
					if (moveToPosition(param, TRUE, odo, dc) != ERR_NOERR)
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 3:		// Move by velocity

					printf ("Please write velocity in rpm: ");
					scanf("%ld",&param);
					printf ("\n\r");
					printf ("Trying to move at %ld rpm\n\r",param);
					if (moveVelocity (param, dc) != ERR_NOERR )
						fprintf (stderr, "There has been an error.\n\r");
					subOptionExit = FALSE;
					break;
				case 0:
					subOptionExit = TRUE;
					break;
				default:
					printf ("Incorrect Option.\n\r");
					subOptionExit = FALSE;
					break;
				}
			} while (subOptionExit != TRUE);
		}												// End of Option 6
		else if (option == 7){							// Enable driver
			
			if (enableDriver(dc->RSd, dc->sem.semID) != ERR_NOERR){
				fprintf (stderr, "Error while attempting to enable the driver.\n\r");
				break;
			}
			printf ("OK. Driver enabled\n\r");
			
		}												// End of Option 7
		else if (option == 8){							// Disable Driver
			if (disableDriver(dc->RSd, dc->sem.semID) != ERR_NOERR){
				fprintf (stderr, "Error while attempting to disable the driver.\n\r");
				break;
			}
			printf ("OK. Driver disabled\n\r");
		}												// End of Option 8
		else if (option == 9){							// Disable Driver
			if (getDriverStatus(dc->RSd, dc->sem.semID,&drvStat) != ERR_NOERR){
				fprintf (stderr, "Error reading information from the Driver.\n\r");
				break;
			}
			printf ("Printing the driver status. 1 TRUE, 0 False\n\r");
			printf ("Driver Disabled: %d\n\r",drvStat.disabled);
			printf ("Current Limiting: %d\n\r",drvStat.curLimiting);
			printf ("Over Temperature: %d\n\r",drvStat.overTemperature);
			printf ("Over Voltage: %d\n\r",drvStat.overVoltage);
			printf ("Limit Sensor Reached: %d\n\r\n",drvStat.sensorReached);
		}
		else if (option == 10){							// Calibrate Driver
			if (calibrateDriver(dc->RSd, dc->sem.semID, 26535, 4000, 500, 10000) != ERR_NOERR){
				fprintf (stderr, "Error calibrating the driver.\n\r");
				option = -1;
				break;
			}
			printf ("Calibration succeeded\n\r");
		}
		else if (option != 0){
			printf("Incorrect Option\n\r");
			exit = FALSE;
			break;
		}
		else{ 
			exit = TRUE;
			printf ("Bye bye!\n\r");
			break;
		}
	} while (exit != TRUE);
}

	/*!
	 * \brief   Moves the Driver MCDC3006S to an absolute position.
	 *  
	 * \param position is the position where the driver should move   
	 * \param relative is a boolean that checks if the movement is relative or not
	 * \param odo is the reference to a t_odometry type where the driver odometry is stored
	 * \param dc is the Driver Configuration structure where some data of the driver is stored
	 * 
	 * \return  0 if everything OK
	 * \return -1 at the driver level
	 */
int moveToPosition (long int position, boolean relative, driverSensor_t * odo, driverConf_t * dc){

	int aux;
	int counter = 0;
	int currentError, oldError; 	// Used to measure if the movement is aproaching to its destinity or not
	
	if (relative == FALSE){
		if (moveDriverAbsPos(dc->RSd, dc->sem.semID, position) == ERR_NOERR)
			printf ("OK. Moving to absolute %ld\n\r",position);
		else{
			fprintf (stderr, "There has been an error. moveAbsPos() did not return ERR_NOERR\n\r"); 
			return -1;		//Error in the driver
		}
	}else{
		if (moveDriverRelPos(dc->RSd, dc->sem.semID, position) == ERR_NOERR)
			printf ("OK. Moving to relative %ld\n\r",position);
		else{
			fprintf (stderr, "There has been an error. moveRelPos() did not return ERR_NOERR\n\r"); 
			return -1;		//Error in the driver
		}
			
	}
	

	getDriverSensor(dc->RSd, dc->sem.semID, odo);
	currentError = position - odo->p; 

	printf ("OK. The driver has arrived to its destination!\n\r");
	printf ("Current driver position: %ld\n\r", odo->p);
	printf ("You introduced: %ld\n\r",position);
	return 0;					
}

/**
 * \brief   Moves the Driver MCDC3006S at a determined velocity.
 *  
 * \param velocity is the velocity in rpm at the driver will move
 * \param dc is the Driver Configuration structure where some data of the driver is stored

 * \return  0 if everything OK
 * \return -1 if there is an error at the driver level

 */
int moveVelocity (long int velocity, driverConf_t * dc){
	if (moveDriverVel(dc->RSd, dc->sem.semID, velocity) == 0)
		printf ("OK. Moving at %ld rpm\n\r",velocity);
	else{
		fprintf (stderr, "There has been an error. moveVel() did not returned 0\n\r");
		return -1;		//Error in the driver
	}
		
	return 0;
}


int testMovementMonitor (){
	
	long int destination;
	short crashed;
	int timeOut;
	int range;
	int currLimit;
	
}
