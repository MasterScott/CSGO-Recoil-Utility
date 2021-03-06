//A simple utility to create log files readable by my SmartCross project
//format of the log files should be:

//END
//AK-47
//timestamp,x1,y1,1
//timestamp,x2,y2,2
//...
//timestamp,x30,y30,30
//OUT
//timestampSettled
//END
//M4A1-S
//timestamp,x1,y1,1
//timestamp,x2,y2,2
//...
//timestamp,x20,y20,20
//OUT
//timestampSettled
//END

#include <chrono>
#include <thread>
#include <iostream>
#include "MemoryManager.h"

using namespace std;

typedef struct{
	float vec[3];
} Vector;



Vector punch;

/*
These variables are kept volatile to improve performance:
*/
volatile int shots;
unsigned long long timeMS = 69;
volatile float x;
volatile float y;
volatile bool running = false;
volatile bool mutex = false;
MemoryManager * manager;

/*
These DWORDs must be updated pretty much every patch
dwShots: offset from local player address showing amount of shots fired
dwPunch: offset from local player address showing current aimpunch
dwLocal: offset of the local player address from the client
*/
DWORD dwLocal = 0x00A844DC;
DWORD dwActive = 0x00002EE8;
DWORD dwList = 0x04A9F8D4;
DWORD dwClip = 0x000031F4;
DWORD dwPunch = 0x00003018;

/*
These DWORDs are calculated internally, don't worry about changing
them with patches or anything.
*/
DWORD client;
DWORD myBase;

void updateTime(){
	timeMS = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void readShots(){
	DWORD dwLocalPlayer = manager->Read<DWORD>(client + dwLocal);
	DWORD dwActiveWeapon = manager->Read<DWORD>(dwLocalPlayer + dwActive);
	DWORD dwWeaponEntity = manager->Read<DWORD>(client + dwList + ((dwActiveWeapon & 0xFFF) - 1) * 0x10);
	shots = manager->Read<int>(dwWeaponEntity + dwClip);
}

void readPunch(){
	myBase = manager->Read<DWORD>(client + dwLocal);
	punch = manager->Read<Vector>(myBase + dwPunch);
}

void updateShots(){
	while (running){
		if (mutex) continue;
		readShots();
	}
}

void log(){
	updateTime();
	cout << timeMS << "," << x << "," << y << "," << shots << "\n";
}

/*
This is hard-coded for 1920 x 1080
todo: fix that.
*/
void calculate(){
	x = punch.vec[1];
	y = punch.vec[0];
	//x = 960 - (21.3333333 * (punch.vec[1]));//positive punchvec1 pulls aim to the left
	//y = 540 + (12         * (punch.vec[0]));//positive punchvec0 pulls aim down...?
}

int main()
{
	manager = new MemoryManager();
	//attach manager to process and find dll
	while (!manager->Attach()) {
		Sleep(100);
	}
	do {
		Sleep(100);
		client = manager->GetModule();
	} while (client == (DWORD)false);



	running = true;//enable threads
	
	
	thread aThread(updateShots);
	//cout << "END" << "\n";
	int lastShots = -1;
	bool logging = true;
	while (logging){
		if (shots != lastShots){
			mutex = true;
			readPunch();
			calculate();
			log();
			lastShots = shots;
			mutex = false;
		}
		else if (shots == 0){
			do{
				readPunch();
				calculate();
				Sleep(1);
			} while (x != 0.0 || y != 0.0);
			log();
			logging = false;
		}
	}
	while (1){
		Sleep(100);
	}
	
	running = false;//disable threads
	return 0;
}

