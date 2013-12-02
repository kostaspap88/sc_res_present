// C code used to debug the AVR PRESENT implementation [Ruhr Universitat Bohum]
// bochum_PRESENT_8bit.cpp : Defines the entry point for the console application.
// retrieved from: http://www.lightweightcrypto.org/
//

#include "stdafx.h"	
#include <stdlib.h>
#include <stdint.h>


int _tmain(int argc, _TCHAR* argv[])
{

	const uint8_t sBox4[] =	{
		0x6,0x5,0x9,0xd,0xc,0x0,0x7,0xf,0xa,0xb,0x2,0x4,0xe,0x3,0x1,0x8
	};
	//	Input values
	uint8_t key[] =	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
	/* IMPORTANT! for AVR assembly debugging this is the input order of key bits:
	uint8_t key[] =	{k9,k8,k7,k6,k5,k4,k3,k2,k1};

	So for input key[] =	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}; in C debug code
	use this in AVR code:

	ldi r16,0x03
	mov k0,r16
	ldi r16,0x00
	mov k1,r16
	ldi r16,0x00
	mov k2,r16
	ldi r16,0x00
	mov k3,r16
	ldi r16,0x00
	mov k4,r16
	ldi r16,0x00
	mov k5,r16
	ldi r16,0x00
	mov k6,r16
	ldi r16,0x00
	mov k7,r16
	ldi r16,0x00
	mov k8,r16
	ldi r16,0x00
	mov k9,r16

	*/

	volatile uint8_t state[] = {0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	/* Important! In AVR code we use the same ordering so for the same plaintext input in AVR do this: 

	ldi r16,0x07
	mov s0,r16
	ldi r16,0x00
	mov s1,r16
	ldi r16,0x00
	mov s2,r16
	ldi r16,0x00
	mov s3,r16
	ldi r16,0x00
	mov s4,r16
	ldi r16,0x00
	mov s5,r16
	ldi r16,0x00
	mov s6,r16
	ldi r16,0x00
	mov s7,r16
	*/

	//	Counter
	uint8_t i = 0;
	//	pLayer variables
	uint8_t position = 0;
	uint8_t element_source = 0;
	uint8_t bit_source = 0;
	uint8_t element_destination	= 0;
	uint8_t bit_destination	= 0;
	uint8_t temp_pLayer[8];
	//	Key scheduling variables
	uint8_t round;
	uint8_t save1;
	uint8_t save2;
	//	****************** Encryption **************************
	round=0;
	do
	{
		//	****************** addRoundkey *************************
		i=0;
		do
		{
			state[i] = state[i] ^ key[i+2];
			i++;
		}
		while(i<=7);

		printf("AfterXOR_State: %x %x %x %x %x %x %x %x \n\n",state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7]);

		//system("PAUSE");

		//	****************** sBox ********************************
		do
		{
			i--;
			state[i] = sBox4[state[i]>>4]<<4 | sBox4[state[i] & 0xF];
		}


		while(i>0);

		printf("afterSbox_State: %x %x %x %x %x %x %x %x \n\n",state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7]);

		//system("PAUSE");

		//	****************** pLayer ******************************



		for(i=0;i<8;i++)
		{
			temp_pLayer[i] = 0;
		}
		for(i=0;i<64;i++)
		{
			position = (16*i) % 63;						//Artithmetic calculation of the pLayer
			if(i == 63)									//exception for bit 63
				position = 63;
			element_source		= i / 8;
			bit_source 			= i % 8;
			element_destination	= position / 8;
			bit_destination 	= position % 8;
			temp_pLayer[element_destination] |= ((state[element_source]>>bit_source) & 0x1) << bit_destination;
		}
		for(i=0;i<=7;i++)
		{
			state[i] = temp_pLayer[i];
		}

		printf("AfterPlayer_State: %x %x %x %x %x %x %x %x \n\n",state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7]);

		//system("PAUSE");
		//	****************** End pLayer **************************
		//	****************** Key Scheduling **********************
		save1  = key[0];
		save2  = key[1];	
		i = 0;
		do
		{
			key[i] = key[i+2];
			i++;
		}
		while(i<8);
		key[8] = save1;
		key[9] = save2;
		i = 0;
		save1 = key[0] & 7;								//61-bit left shift
		do
		{
			key[i] = key[i] >> 3 | key[i+1] << 5;			
			i++;
		}
		while(i<9);
		key[9] = key[9] >> 3 | save1 << 5;

		key[9] = sBox4[key[9]>>4]<<4 | (key[9] & 0xF);	//S-Box application

		if((round+1) % 2 == 1)							//round counter addition
			key[1] ^= 128;
		key[2] = ((((round+1)>>1) ^ (key[2] & 15)) | (key[2] & 240));
		//	****************** End Key Scheduling ******************
		round++;


		printf("UpdatedKey: %x %x %x %x %x %x %x %x %x %x\n\n",key[0],key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9]);

	}
	while(round<31);
	//	****************** addRoundkey *************************
	i = 0;
	do										//final key XOR
	{
		state[i] = state[i] ^ key[i+2];
		i++;
	}
	while(i<=7);

	//	****************** End addRoundkey *********************
	//	****************** End Encryption  **********************



	printf("Final_State: %x %x %x %x %x %x %x %x \n",state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7]);

	system("PAUSE");
	return 0;
}

