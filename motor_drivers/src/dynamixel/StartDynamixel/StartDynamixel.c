/**
 * TestDynamixel.c
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 *      Created on: 01/09/2010
 */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <termio.h>
#include "DatosDynamixel.h"
#include "dxl_hal.h"
#include "dynamixel.h"

// Control table address
#define P_PRESENT_LOAD_L		40
#define P_PRESENT_VOLTAGE 		42
#define P_PRESENT_TEMPERATURE	43
#define UP_CALIBRATION_L		22
#define DOWN_CALIBRATION_L		14
#define TORQUE					24
#define P_PRESENT_POSITION_L 	36
#define P_PRESENT_POSITION_H 	37
#define P_GOAL_POSITION_H		31
#define P_GOAL_POSITION_L		30
#define P_GOAL_SPEED_L			32
#define CW_ANGLE_LIMIT_L		6
#define CCW_ANGLE_LIMIT_L		8

int main()
{
	int baudnum1 = 1;
//	int deviceIndex1 = 0;
	char op;
	int id;
	int AmpPos = 512;
	int speed;
	int blocking;
	int CWAngle;
	int CCWAngle;
	int position;
	char string [256];
	int actual;
	int posRel;
	int position1;
	int maxLimit;
	int minLimit;
	int maxSpeed;
	int newID;

	char portName[200] = "/dev/cuello";
	
		
	// Open USB2Dynamixel 
	//if(dxl_simple_initialize(deviceIndex1, baudnum1) == 0 )
	int aux;
	aux = dxl_initialize(portName, baudnum1);

	if (aux == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
	
	do{
	  printf("Choose an action:\n");
	  printf("1. Synchronize \n");
	  printf("2. Move to position\n");
	  printf("3. Move to position at specific speed\n");
	  printf("4. Get Position\n");
	  printf("5. Set Limits\n");
	  printf("6. Set Speed\n");
	  printf("7. Show Data\n");
	  printf("8. Get Postion in movement\n");
	  printf("9. Show limits, max speed\n");
	  printf("a. Set Device Id\n");
	  printf("b. Get Device Id\n");
	  printf("c. Enless turn mode\n");

	  scanf("%s", string);
	  printf("Leido: %s :fin\n", string);
	  op = *string;
	  switch(op){
	    case '1':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      synchronize(id,AmpPos);
	      break;
	      
	    case '2':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("Ingrese la posición a la que quiere llegar de 0 a 1023 posiciones posibles: \n");
	      scanf("%d",&position);
	      printf("activar blocking = 1         desactivar blocking = 0      blocking 1/0 ? : \n");
	      scanf("%d", &blocking);
	      goToPosition(id, position, blocking);
	      break;
	      
	    case '3':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("Ingrese la posición a la que quiere llegar de 0 a 1023 posiciones posibles: \n");
	      scanf("%d",&position);
	      printf("Ingrese la velocidad: \n");
	      scanf("%d",&speed);
	      printf("activar blocking = 1         desactivar blocking = 0      blocking 1/0 ? : \n");
	      scanf("%d", &blocking);
	      goToPositionAtSpeed(id, position, speed, blocking);
	      break;
	      
	    case '4':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      getPosition(id);
	      break;
	    
	    case '5':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("Ingrese el CWAngle: \n");
	      scanf("%d",&CWAngle);
	      printf("Ingrese el CCWAngle: \n");
	      scanf("%d",&CCWAngle);
	      setLimits(id, CWAngle, CCWAngle);
	      break;
	    
	    case '6':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("Ingrese velocidad: \n");
	      scanf("%d",&speed);
	      setSpeed(id, speed);
	      break;
	      
	    case '7':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      getData(id);
	      break;
	     
	    case '8':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("pos relativa:\n");
	      scanf("%d",&posRel);
	      actual=dxl_read_word(id, P_PRESENT_POSITION_L);
	      position1= posRel + actual;
	      dxl_write_word( id, P_GOAL_POSITION_L, position1 );

	    case '9':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      maxLimit= dxl_read_word(id,CCW_ANGLE_LIMIT_L);
	      minLimit=dxl_read_word(id,CW_ANGLE_LIMIT_L);
	      maxSpeed=dxl_read_word( id, P_GOAL_SPEED_L);
	      printf("max limit= %d\n  min limit= %d\n  max speed= %d\n",maxLimit, minLimit, maxSpeed);
	    case 'a':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
          printf("Ingrese el nuevo identificador para este servo: \n");
	      scanf("%d",&newID);
	      setId(id,newID);
	      break;

	    case 'b':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      getIdDevice(id);
	      break;
	    
	    case 'c':
	      printf("Ingrese el identificador del servo conectado: \n");
	      scanf("%d",&id);
	      printf("Ingrese velocidad: \n");
	      scanf("%d",&speed);
	      setSpeed(id, speed);
	      //GetPositionInMovement(id, speed);
	      break;

	    default:
	      printf("Choose a valid option\n");
	      
	  }
	
	}while(op > 0);

	dxl_terminate();
	return 0;
}
