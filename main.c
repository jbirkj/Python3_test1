/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ncurses.h>
#include "PCAtest.h"
#include "DS2482.h"
#include <linux/i2c-dev.h>
#include "main.h"
#include "gpio.h"
#include "time.h"
#include "conversion.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>

//#define DEBUG 0
//#define 


char cROMCODE1[8],cROMCODE2[8], cROMCODE3[8], cROMCODE4[8], cROMCODE5[8];
char cWebLogGuid[100];

int main (int argc, char *argv[] )
{
//	printf("=======================================\n");
  
/*	struct tm *timeinfo;
	time_t rawtime;
	char strResponse[128];
*/
	char cReadings[8], cDeviceROMaddr[8];
	int iTemp=0, i=0;

	FILE *ptr_file;

	//1=build into metal, 2=2nd w short wiremount, 3=3rd w long wiremount, 4=loose 18B20 
	//5=metal tip w silicone tube
	cROMCODE1[0]=0x28;	cROMCODE2[0]=0x28;	cROMCODE3[0]=0x28;	cROMCODE4[0]=0x28;	cROMCODE5[0]=0x28;
	cROMCODE1[1]=0xc3;	cROMCODE2[1]=0x0c;	cROMCODE3[1]=0xa6;	cROMCODE4[1]=0x2f;	cROMCODE5[1]=0xe8;
	cROMCODE1[2]=0xc2;	cROMCODE2[2]=0x1b;	cROMCODE3[2]=0xd8;	cROMCODE4[2]=0x90;	cROMCODE5[2]=0xd4;
	cROMCODE1[3]=0x9d;	cROMCODE2[3]=0xe0;	cROMCODE3[3]=0x9c;	cROMCODE4[3]=0x2d;	cROMCODE5[3]=0x45;
	cROMCODE1[4]=0x04;	cROMCODE2[4]=0x04;	cROMCODE3[4]=0x04;	cROMCODE4[4]=0x04;	cROMCODE5[4]=0x05;
	cROMCODE1[5]=0x00;	cROMCODE2[5]=0x00;	cROMCODE3[5]=0x00;	cROMCODE4[5]=0x00;	cROMCODE5[5]=0x0;
	cROMCODE1[6]=0x00;	cROMCODE2[6]=0x00;	cROMCODE3[6]=0x00;	cROMCODE4[6]=0x00;	cROMCODE5[6]=0x0;
	cROMCODE1[7]=0x9b;	cROMCODE2[7]=0xb8;	cROMCODE3[7]=0xfd;	cROMCODE4[7]=0xd9;	cROMCODE5[7]=0x83;



	if ( OWReset(27) ) {
		OWWriteByte( 0xCC ); // CC is Skip ROM command
		OWWriteByte( 0x44 ); // temp conversion started for all OW devices
		//printf("Temperature reading started....\n");
		if ( OWReset(27) ) {
			OWWriteByte( 0x55 );// 55 address specific OW device
			for (i=0; i<8; i++) {
				OWWriteByte( atoi(argv[i+1]) );
				//OWWriteByte( cROMCODE1[i] );
				//printf( "ROmCode %d: %d while ROMCODE1 = %d \n", i, atoi((argv[i+1])), cROMCODE1[i] );
			}
			OWWriteByte( 0xBE ); // reading scratch pad
			for (i=0; i<8; i++) {
				cReadings[i] = OWReadByte();
			}
			iTemp = (int)((cReadings[1]<<8) | cReadings[0]);
			iTemp /= 16;
			//printf("read to %d degrees celcius\n", iTemp);
		}
	}
	fprintf( stdout, "%i", iTemp );
	

//	setup_io();
	if (wiringPiSetup() == -1) {
		return 0;
	}

/*	printf("q=quit\n");
	printf("t=DS2482 device detect(Reset and Init, y=OWReset()\n");
	printf("m=read temp from one 18b20, n=Read ROM code \n");
	printf("b=read temp from more 18b20s, v=set target \n");
	printf("o=enable GPIO, p=diable GPIO \n");
	printf("w=read multiple sensors, e=sent to web and read return (use popen) \n");
*/	
	
/*	while(1) {

		
		if(iTimeCount++ == 1000 ){
			printf("tæller %d", iTC++);
			iTimeCount=0;
		}
		
		cKeyStroke = getchar();
		//printf("keys pressed %x\n", cKeyStroke );
		skipKey = getchar();
		switch (cKeyStroke) {
				case 'b' : // case to read temperature from multiple sensors
					if ( OWReset(27) ) {
						OWWriteByte( 0xCC ); // CC is Skip ROM command
						OWWriteByte( 0x44 ); // temp conversion started for all OW devices
						printf("Temperature reading started....\n");
						if ( OWReset(27) ) {
							OWWriteByte( 0x55 );// 55 address specific OW device
							for (i=0; i<8; i++) {
								OWWriteByte( cROMCODE1[i] );
							}
							OWWriteByte( 0xBE ); // reading scratch pad
							for (i=0; i<8; i++) {
								cReadings[i] = OWReadByte();
							}
							iTemp = (cReadings[1]<<8) | cReadings[0];
							iTemp /= 16;
							printf("read to %d degrees celcius\n", iTemp);
						}
						if ( OWReset(27) ) {
							OWWriteByte( 0x55 );// 55 address specific OW device
							for (i=0; i<8; i++) {
								OWWriteByte( cROMCODE2[i] );
							}
							OWWriteByte( 0xBE ); // reading scratch pad
							for (i=0; i<8; i++) {
								cReadings[i] = OWReadByte();
							}
							iTemp = (cReadings[1]<<8) | cReadings[0];
							iTemp /= 16;
							printf("read to %d degrees celcius\n", iTemp);
						}	
						if ( OWReset(27) ) {
							OWWriteByte( 0x55 );// 55 address specific OW device
							for (i=0; i<8; i++) {
								OWWriteByte( cROMCODE5[i] );
							}
							OWWriteByte( 0xBE ); // reading scratch pad
							for (i=0; i<8; i++) {
								cReadings[i] = OWReadByte();
							}
							iTemp = (cReadings[1]<<8) | cReadings[0];
							iTemp /= 16;
							printf("read to %d degrees celcius\n", iTemp);
						}						
					} else {
						printf("OWReset failed\n");
					}
					break;
				case 'e': 
					printf("Call G before this (e) to get GUID from file\n");
					WebLogWrite2( cWebLogGuid, 11,22,33 );
					break;
				case 'f': 
					printf("Ferment Log\n");

					ptr_file =fopen("GUID.txt","rt");
					if (!ptr_file)
						return 1;
					fgets( &cWebLogGuid[0], 100, ptr_file);
					printf("GUID read is: %s \n\r",cWebLogGuid);
					fclose(ptr_file);

					FermentLog();
					//WebLogWrite2( cWebLogGuid, 11,22,33 );
					break;

				case 'g': 		// get GUID for WebLog
					printf("Reading GUID for Brew@Birxlabs in guid.txt\n");
					
//					FILE *ptr_file;

					ptr_file =fopen("GUID.txt","rt");
					if (!ptr_file)
						return 1;

					fgets( &cWebLogGuid[0], 100, ptr_file);
					printf("GUID read is: %s \n\r",cWebLogGuid);

					fclose(ptr_file);
					
					break;
				case 'm': // read temp from one sensors mounted
					if ( OWReset(27) ) {
						OWWriteByte( 0xCC ); // CC is Skip ROM command
						OWWriteByte( 0x44 ); //temo conversion started 
						printf("Temperature reading started....\n");
						if ( OWReset(27) ) {
							OWWriteByte( 0xCC );
							OWWriteByte( 0xBE ); // reading scratch pad
							for (i=0; i<8; i++) {
								cReadings[i] = OWReadByte();
							}
							iTemp = (cReadings[1]<<8) | cReadings[0];
							iTemp /= 16;
							printf("read to %d degrees celcius\n", iTemp);
						}
					} else {
						printf("OWReset failed\n");
					}
					break;
				case 'n' : // get serial when only one 1w device is present
					if ( OWReset(27) ) {
						OWWriteByte( 0x33 );
						for (i=0; i<8; i++) {
							cDeviceROMaddr[i] = OWReadByte();
							printf("Byte %d: %x \n", i, cDeviceROMaddr[i]);
						}
					}
					break;
				case 'o' : // enable GPIO
					if (SetGPIO(TRUE, 4))
						break;
					else
						printf("enable GPIO failed\n");
					break;
				case 'p' : // disable GPIO
					if (SetGPIO(FALSE, 4))
						break;
					else
						printf("disable GPIO failed\n");
					break;
				case 'q':
					return 0;
					break; 
				case 's':
					return 0;
					break;
				case 't':
					DS2482_detect(27);
					//DS2482_reset();
					break;
				case 'v' : // enter temp target 
					printf("Reading GUID for Brew@Birxlabs in guid.txt\n");
					
//					FILE *ptr_file;

					ptr_file =fopen("GUID.txt","rt");
					if (!ptr_file)
						return 1;

					fgets( &cWebLogGuid[0], 100, ptr_file);
					printf("GUID read is: %s \n\r",cWebLogGuid);

					fclose(ptr_file);

					conversion();

					rawtime = time (NULL);
					timeinfo = localtime(&rawtime);
					strftime(strResponse,128,"%H:%M:%S %d-%b-%Y",timeinfo);
					
					printf("%s\n", strResponse);
					
					break;
				case 'w': 
					//sprintf(syscmd,"curl http://www.birxlabs.com/Brew/Api/Data/AddObservation -G -d guid=42b312c8-c2d0-4640-a617-513382083c4f -d t1=11 -d t2=22 -d t3=33 -d tt=44 -d msg");
					sprintf(syscmd,"curl http://www.birxlabs.com/brew/Api/Data/AddObservation -G  -d guid=1b04d06d-a1b4-4aff-8ff0-4aee63e06bed -d t1=1.1 -d t2=2.1 -d t3=3.1 -d tt=4.0 -d msg");
					// Execute--this will wait for command to complete before continuing. 
					system(syscmd);
					
					break;
				case 'y':
					tmpStat = OWReset(27);
					printf("PPD : %x\n", tmpStat );
					break;
				default:
					printf("No funciton assigned to key\n");
					break;
				} //end switch-case loop
	} //end while(1) loop 
*/
	return 0;
}
