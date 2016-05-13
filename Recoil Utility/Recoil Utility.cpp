//A simple utility to create log files readable by my SmartCross project
//format of the log files should be:

//END
//AK-47
//timestamp,x1,y1
//timestamp,x2,y2
//...
//timestamp,x30,y30
//OUT
//timestampSettled
//END
//M4A1-S
//timestamp,x1,y1
//timestamp,x2,y2
//...
//timestamp,x20,y20
//OUT
//timestampSettled
//END

#include "stdafx.h"
#include <chrono>
#include "MemoryManager.h"

using namespace std;

long getTime(){
	long ms = chrono::duration_cast<chrono::milliseconds>(
		chrono::system_clock::now().time_since_epoch()).count();
	return ms;
}

int main()
{
	return 0;
}

