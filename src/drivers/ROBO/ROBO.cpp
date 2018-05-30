/***************************************************************************

    file                 : ROBO.cpp
    created              : Tue Feb 14 18:37:49 IST 2017
    copyright            : (C) 2002 VINAY

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

// 
#include <sstream>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <math.h>

#include <tgf.h> 
#include <track.h> 
#include <car.h> 
#include <raceman.h> 
#include <robottools.h>
#include <robot.h>


#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>


std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}
//111111111111111111111111111111111111111111111111111111111111111111111111111111111//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
// using namespace std;



// using namespace std;
bool TRAINACTIVE=false; // for updating Q 
bool STARTACTIVE=false; // for fillQ(); else loadQ()
#define NUMSTATES 200
#define NUMACTIONS 7

float RLAST = -1;
long int iters = 0;

char getch(){
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }


bool IMRACING = false;
// std::vector<int> topology;
// std::vector<std::vector<float> > network;
// std::vector<int> GETTOPO(std::string line)
// {
//     std::istringstream topline(line);
//     float topo;
    
//     while(topline >> topo) {
//         topology.push_back(topo);
//     }

//     for(int i=0;i<topology.size(); i++)
//         std::cout<<topology[i]<<" ";

//     std::cout<<std::endl;

//     return topology;
// }

// int fileread(std::string file)
// {
//     std::ifstream infile(file.c_str());
//     std::string line;
//     std::getline(infile, line);
    
//     std::vector<int> topology = GETTOPO(line);

//     int numLines = 0; 
//     for(int i = 1; i<topology.size(); i++)
//     {
//         numLines += topology[i];
//     }
//     std::cout<<numLines<<std::endl;

    
//     int netLayer = 0;
//     int netNeuro = 0;

//     for(int i = 0; i<numLines; i++)
//     {
//         std::getline(infile, line);
//         std::istringstream iss(line);

//         float val;
//         std::vector<float> tempNet;
//         while(iss >> val) {

//             tempNet.push_back(val);     
//         }
//         network.push_back(tempNet);
//     }

//     // for(int i=0; i<network.size(); i++)
//     // {
//     //  for(int j=0; j<network[i].size(); j++)
//     //  {
//     //      std::cout<<network[i][j]<<" ";
//     //  }
//     //  std::cout<<std::endl;
//     // }
// }

// std::vector<float> GETRESULT(std::vector<float> input)
// {   
//     std::vector<std::vector<float> > output;
//     input.push_back(1.0);
//     output.push_back(input);
    
//     // int layerNumber = 1;
//     int lineNumber = 0;
    
//     for(int layerNumber=1; layerNumber<topology.size(); layerNumber++)
//     {
//         std::vector<float> tempOut;
//         for(int i=0; i<topology[layerNumber]; i++)
//         {
//             float sum = 0;
//             for(int j=0; j<topology[layerNumber-1]+1; j++)
//             {
//                 // std::cout<<network[layerNumber-1+i][j]<<"->"<<output[layerNumber-1][j]<<std::endl;
//                 sum += output[layerNumber-1][j]*network[(layerNumber-1)*topology[layerNumber]+i][j];
//             }
//             // std::cout<<tanh(sum)<<"\n\n";
//             tempOut.push_back(tanh(sum));
//         }
//         tempOut.push_back(1.0);
//         output.push_back(tempOut);
//         // std::cout<<std::endl<<std::endl<<std::endl;
//     }
    

//     // for(int i = 0; i<output.size(); i++)
//     // {
//     //  for(int j = 0; j<output[i].size(); j++)
//     //  {
//     //      std::cout<<output[i][j]<<" ";
//     //  }
//     //  std::cout<<std::endl;
//     // }
//     return output[output.size()-1];
// }


//2222222222222222222222222222222222222222222222222222222222222222222222222222222222222//


float Q[NUMSTATES][NUMACTIONS];

int fillQ()
{
    for(int i=0; i<NUMSTATES; i++)
    {
        for(int j=0; j<NUMACTIONS; j++)
        {
            Q[i][j] = 0;
        }
    }

    return 0;
}

int showQ()
{
    for(int i=0; i<NUMSTATES; i++)
    {
        for(int j=0; j<NUMACTIONS; j++)
        {
            std::cout<<Q[i][j]<<" ";
        }
        std::cout<<std::endl;
    }

    return 0;
}

int loadQ()
{
    double m;
    
    std::ifstream fileStream("Qmatrix.txt");
    int cnt = 0;//index starts from 0
    while (fileStream >> m)
    {
        int temprow = cnt / NUMACTIONS;
        int tempcol = cnt % NUMACTIONS;
        Q[temprow][tempcol] = m;
        cnt++;
    }
    return 0;
}

float findMaxQ(int pos, int nextAction)
{
    int newpos = pos;
    float qm;
    if(!((nextAction-NUMACTIONS/2.0)<0))
    {
        if(newpos>0)
            newpos--;
    }
    else
    {
        if(newpos<NUMSTATES-1)
            newpos++;
    }

    qm = -100000;
    for(int i = 0; i<NUMACTIONS; i++)
    {
        
        if(Q[newpos][i]>qm)
        {
            qm = Q[newpos][i];
        }
    }

    return qm;
}

int getNextMove(int& pos, int& nextAction)
{
    float meanAction = 0;
    for(int i=0; i<NUMACTIONS; i++)
    {
        meanAction += Q[pos][i];
    }
    meanAction/=NUMACTIONS;

    int index1 = NUMACTIONS/2;
    int index2 = NUMACTIONS/2-1;
    int index = -1;
    float tempVal1 = -10000;
    float tempVal2 = -10000;

    for(int i=NUMACTIONS/2; i<NUMACTIONS; i++)
    {
        if(Q[pos][i]>tempVal1 && Q[pos][i]>meanAction)
        {
            index1 = i;
            tempVal1 = Q[pos][i];
        }
    }

    for(int i=NUMACTIONS/2-1; i>=0; i--)
    {
        if(Q[pos][i]>tempVal2 && Q[pos][i]>meanAction)
        {
            index2 = i;
            tempVal2 = Q[pos][i];
        }
    }

    // std::cout<<"\n\n"<<index1<<" "<<index2<<std::endl;
    // if(posAction.size()-1>NUMACTIONS/3)
    // {
    //     index = NUMACTIONS/2;
    // }
    if(Q[pos][index1]>Q[pos][index2])
    {
        index = index1;
    }
    else
    {
        index = index2;
    }
    if(index==-1)
        return rand()%NUMACTIONS;
    else
        return index;

}

int PRINT()
{
    // open the file (for writing because it is an ofstream, meaning "output file stream")
    std::ofstream output("Qmatrix.txt"); 
    for (int i=0;i<NUMSTATES;i++)
    {
        for (int j=0;j<NUMACTIONS;j++)
        {
            output << Q[i][j] << " "; // behaves like cout - cout is also a stream
        }
        output << "\n";
    } 

    std::cout << GetCurrentWorkingDir() << std::endl;
    return 0;
}


//3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333//


static tTrack	*curTrack;

static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s); 
static void newrace(int index, tCarElt* car, tSituation *s); 
static void drive(int index, tCarElt* car, tSituation *s); 
static void endrace(int index, tCarElt *car, tSituation *s);
static void shutdown(int index);
static int  InitFuncPt(int index, void *pt); 

static const float MAX_UNSTUCK_SPEED = 5.0;
static const float MIN_UNSTUCK_DIST = 3.0;
static const float Gravity = 9.81;                  /* [m/(s*s)] */
static const float FULL_ACCEL_MARGIN = 1.0;   /* [m/s] */ 
static const float SHIFT = 0.9;
static const float SHIFT_MARGIN = 4.0; 

static const float ABS_SLIP = 0.9;        /* [-] range [0.95..0.3] */
static const float ABS_MINSPEED = 3.0;    /* [m/s] */ 
static const float TCL_SLIP = 0.9;        /* [-] range [0.95..0.3] */
static const float TCL_MINSPEED = 3.0;    /* [m/s] */         

/* 
 * Module entry point  
 */ 
extern "C" int 
ROBO(tModInfo *modInfo) 
{
    memset(modInfo, 0, 10*sizeof(tModInfo));

    modInfo->name    = strdup("ROBO");		/* name of the module (short) */
    modInfo->desc    = strdup("");	/* description of the module (can be long) */
    modInfo->fctInit = InitFuncPt;		/* init function */
    modInfo->gfId    = ROB_IDENT;		/* supported framework version */
    modInfo->index   = 1;

    return 0; 
} 

/* Module interface initialization. */
static int 
InitFuncPt(int index, void *pt) 
{ 
    tRobotItf *itf  = (tRobotItf *)pt; 

    itf->rbNewTrack = initTrack; /* Give the robot the track view called */ 
				 /* for every track change or new race */ 
    itf->rbNewRace  = newrace; 	 /* Start a new race */
    itf->rbDrive    = drive;	 /* Drive during race */
    itf->rbPitCmd   = NULL;
    itf->rbEndRace  = endrace;	 /* End of the current race */
    itf->rbShutdown = shutdown;	 /* Called before the module is unloaded */
    itf->index      = index; 	 /* Index used if multiple interfaces */
    return 0; 
}

// 

/* Called for every track change or new race. */
static void  
initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s) 
{ 
    curTrack = track;
    *carParmHandle = NULL; 
    if(STARTACTIVE)
    {
        fillQ();
        PRINT();
    }
    if(!STARTACTIVE)
    {
        loadQ();
    }
} 

/* Start a new race. */
static void  
newrace(int index, tCarElt* car, tSituation *s) 
{ 
    // IMRACING = true;
    loadQ();
    // fileread("/home/arush/Codes/simulator/torcs-1.3.7/nn/weights.txt");
    
} 


//-------------------------------------------------------------------//
/* Compute aerodynamic drag coefficient CW */
/* Compute aerodynamic downforce coefficient CA */

//counter 
static int stuck = 0;
static float MAX_UNSTUCK_ANGLE = 30.0/180.0*PI;
static int MAX_UNSTUCK_COUNT = 100;
/* Check if the car is stuck */
bool isStuck(tCarElt* car)
{
	float angle = RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw;
	NORM_PI_PI(angle);

	//angle smaller than 30 degrees?
	if (fabs(angle) > MAX_UNSTUCK_ANGLE &&
        car->_speed_x < MAX_UNSTUCK_SPEED &&
        fabs(car->_trkPos.toMiddle) > MIN_UNSTUCK_DIST) {
        if (stuck > MAX_UNSTUCK_COUNT && car->_trkPos.toMiddle*angle < 0.0) {
            return true;
        } else {
            stuck++;
            return false;
        }
    } else {
        stuck = 0;
        return false;
    }


}


/* Antilocking filter for brakes */
static float filterABS(tCarElt *car, float brake)
{
	/* Don't apply ABS when speed is slow */
    if (car->_speed_x < ABS_MINSPEED) return brake;
	int i;
    float slip = 0.0;
    for (i = 0; i < 4; i++) {
        slip += car->_wheelSpinVel(i) * car->_wheelRadius(i) / car->_speed_x;
    }
	/* Compute average slip of the four wheels */
    slip = slip/4.0;
	/* If the slip variable is below a certain threshold, 
	   reduce the brake value by multiplying it with the slip */
	if (slip < ABS_SLIP) brake = brake*slip;
    return brake;
}

/* Compute gear */
static int getGear(tCarElt *car)
{
	/* shift to the first gear, if in neutral or reverse */
	if (car->_gear <= 0) return 1;	
	float gr_up = car->_gearRatio[car->_gear + car->_gearOffset];
    float omega = car->_enginerpmRedLine/gr_up;
    float wr = car->_wheelRadius(2);
	/* shift up if allowed speed for the current gear (omega*wr*SHIFT) is exceeded */
    if (omega*wr*SHIFT < car->_speed_x) 
	{
        return car->_gear + 1;
    } 
	else 
	{
        float gr_down = car->_gearRatio[car->_gear + car->_gearOffset - 1];
        omega = car->_enginerpmRedLine/gr_down;
        /* If current gear is greater than one, check if current speed is lower than 
        allowed speed with the next lower gear. If so, shift down. */
        if (car->_gear > 1 && omega*wr*SHIFT > car->_speed_x + SHIFT_MARGIN) 
		{
            return car->_gear - 1;
        }
    }
    /* If all of the above didn't apply, return the current gear. */
    return car->_gear;
}  

/* Compute the allowed speed on a segment */
static float getAllowedSpeed(tTrackSeg *segment)
{
    if (segment->type == TR_STR) {
        return FLT_MAX;
    } else {
        float mu = segment->surface->kFriction;
        return sqrt(mu*Gravity*segment->radius);
    }
}   

/* Compute the length to the end of the segment */
static float getDistToSegEnd(tCarElt* car)
{
    if (car->_trkPos.seg->type == TR_STR) {
        return car->_trkPos.seg->length - car->_trkPos.toStart;
    } else {
        return (car->_trkPos.seg->arc - car->_trkPos.toStart)*car->_trkPos.seg->radius;
    }
}  





/* Compute fitting acceleration */
static float getAccel(tCarElt* car)
{
    float allowedspeed = getAllowedSpeed(car->_trkPos.seg);
    float gr = car->_gearRatio[car->_gear + car->_gearOffset];
    float rm = car->_enginerpmRedLine;
    if (allowedspeed > car->_speed_x + FULL_ACCEL_MARGIN) {
        return 1.0;
    } else {
        return allowedspeed/car->_wheelRadius(REAR_RGT)*gr /rm;
    }
}          

/* Compute if need to brake */
static float getBrake(tCarElt* car)
{
    tTrackSeg *segptr = car->_trkPos.seg;
    float currentspeedsqr = car->_speed_x*car->_speed_x;
    float mu = segptr->surface->kFriction;
    /* maxlookaheddist is the distance we have to check (formula with special case v2 = 0) */
    float maxlookaheaddist = currentspeedsqr/(2.0*mu*Gravity);
    /* lookaheaddist holds the distance we have already checked */
	float lookaheaddist = getDistToSegEnd(car);
    /* Compute the allowed speed on the current segment.  */
	float allowedspeed = getAllowedSpeed(segptr);
    if (allowedspeed < car->_speed_x) return 1.0;
	segptr = segptr->next;
    while (lookaheaddist < maxlookaheaddist) {
	allowedspeed = getAllowedSpeed(segptr);
        if (allowedspeed < car->_speed_x) {
        	/* Compute the braking distance according to the formula */
			float allowedspeedsqr = allowedspeed*allowedspeed;
            float brakedist = (currentspeedsqr - allowedspeedsqr) / (2.0*mu*Gravity);
			// float brakedist = mass*(currentspeedsqr - allowedspeedsqr) /
   //                    (2.0*(mu*G*mass + allowedspeedsqr*(CA*mu + CW)));

			if (brakedist > lookaheaddist) {
			return 1.0;
            }
        }
        lookaheaddist += segptr->length;
        segptr = segptr->next;
    }
    return 0.0;
} 



/* Drive during race. */
static void  
drive(int index, tCarElt* car, tSituation *s) 
{ 
    memset((void *)&car->ctrl, 0, sizeof(tCarCtrl)); 
	
	float angle;
	const float SC = 1.0;
	
	if(isStuck(car))
	{	
		angle = -RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw;
		NORM_PI_PI(angle);	
		car->ctrl.steer = -angle/ car->_steerLock;	/*steering angle*/
		car->ctrl.gear = -1;		//reverse gear
		car->ctrl.accelCmd = 0.5;	/*30% acceleraor pedal*/
		//car->ctrl.brakeCmd = 0.0;
		if (car->ctrl.brakeCmd == 0.0) {
       		car->ctrl.accelCmd = getAccel(car);
    	} else {
       		car->ctrl.accelCmd = 0.0;
    	}
    	
	}
	else
	{
		angle = RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw;
		NORM_PI_PI(angle);
	// 
		float steerangle = angle - SC*(car->_trkPos.toMiddle-0*car->_trkPos.toLeft)/ car->_trkPos.seg->width;
		// std::cout<<" Middle : "<< car->_trkPos.toMiddle<< std::endl;
		// std::cout<<" Left : "<< car->_trkPos.toLeft<< std::endl;
		// std::cout<<" Right : "<< car->_trkPos.toRight<< std::endl;
		float steerLimit = steerangle/ car->_steerLock;
        if(steerLimit<-1.0)
            steerLimit = -1.0;
        if(steerLimit>1.0)
            steerLimit = 1.0;
        
		// car->ctrl.steer = steerLimit;	/*steering angle*/
		// car->ctrl.gear = 2;		/*second gear*/
		//car->ctrl.accelCmd = 0.5;	/*30% acceleraor pedal*/
		car->ctrl.brakeCmd = filterABS(car, getBrake(car));
    	if (car->ctrl.brakeCmd == 0.0) {
       		car->ctrl.accelCmd = getAccel(car);
    	} else {
       		car->ctrl.accelCmd = 0.0;
    	}
    	car->ctrl.accelCmd = (getAccel(car));
        car->ctrl.gear = getGear(car);


        if(TRAINACTIVE)
        {
         //    if(car->_speed_x<25)
         //        car->ctrl.accelCmd = 0.9;
         //    else
         //        car->ctrl.accelCmd = 0.0;
        	car->ctrl.gear = 1;//getGear(car);
        }


        // loop starts 
    
        float tempPos = steerangle/ car->_steerLock;
        if(tempPos<-10.0)
            tempPos = -10.0;
        if(tempPos>10.0)
            tempPos = 10.0;

        int pos = int(((tempPos+10.0)/10.0)*NUMSTATES/2);
        float R = ((1-(std::abs(pos-NUMSTATES/2)*1.0/(NUMSTATES/2.0)))/1.0);
        
        R = R*R*R*R*R*R*R;
        float scale = R;
        if(std::abs(pos-NUMSTATES/2)==0)
        {
            R=1.0;
        }
        // else
        // {
        //     R=0;
        // }
        // if(abs(R)<0.2)
        // {
        //     R=-1.0; 
        // }

        if(std::abs(pos-NUMSTATES/2)>(NUMSTATES/2)*0.1)
            R=-1.0;

        
        int nextAction = (rand()%NUMACTIONS);//-NUMACTIONS/2)*1.0/(NUMACTIONS/2);

        if(!TRAINACTIVE)
            nextAction = getNextMove(pos, nextAction);

        if((pos-NUMSTATES/2)<-(NUMSTATES/2)*0.15)
            nextAction = 0;
        if((pos-NUMSTATES/2)>(NUMSTATES/2)*0.15)
            nextAction = NUMACTIONS-1;

        if(TRAINACTIVE)
        {
            if((nextAction-NUMACTIONS/2)<0)
                car->ctrl.steer = (1.0)*(nextAction-NUMACTIONS/2.0)/(NUMACTIONS/2.0);
            else
                car->ctrl.steer = (1.0)*(nextAction+1-NUMACTIONS/2.0)/(NUMACTIONS/2.0);
        }
        else
        {
            if((nextAction-NUMACTIONS/2)<0)
                car->ctrl.steer = (1.0-scale)*(nextAction-NUMACTIONS/2.0)/(NUMACTIONS/2.0);
            else
                car->ctrl.steer = (1.0-scale)*(nextAction+1-NUMACTIONS/2.0)/(NUMACTIONS/2.0);

        }

        float Qmax = findMaxQ(pos, nextAction);

        // if(RLAST == -1000)
        //     RLAST = R;
        // if(TRAINACTIVE)
        {
            // Q.at<float>(QPOS.y, QPOS.x) = Q.at<float>(QPOS.y, QPOS.x) +0.99*(R_VAL + 0.75*(Q_MAX) - Q.at<float>(QPOS.y, QPOS.x));
            // Q[pos][nextAction] = Q[pos][nextAction] + 0.95*(R*RLAST + 0.75*Qmax - Q[pos][nextAction]);
            Q[pos][nextAction] = Q[pos][nextAction] + 0.95*(R*RLAST + 0.75*Qmax - Q[pos][nextAction]);
        }

        // if(TRAINACTIVE)
	        PRINT();
	    
	    RLAST = R;
	      
        // showQ();
        // getch();
        std::cout<<"Pos:"<<pos <<" Action:"<<nextAction<<" Reward:"<<R <<" Qmax:"<<Qmax<<" Qcurrent:"<<Q[pos][nextAction]<<std::endl;
        // loop ends

        // if(++iters%15==0)
        // {
        //     std::ostringstream ss;
        //     ss<<"bash /home/arush/Codes/screenRecord/run.sh "<<iters;
        //     std::string sstr = ss.str(); 
        //     std::system(sstr.c_str());
        // }
	    // std::cout<<"S0 \n"<<std::endl;
	    // screen(img);
		// std::ostringstream ss;
		// ss<<"~/Desktop/folder/"<<iters++<<".jpg";
	 //    imwrite(ss.str(), img);

      // unsigned int microseconds = 33000;
      // usleep(microseconds);


        // NN Approach
        //TRAIN
        // if(IMRACING)
        // {
        //     std::cout<<"in: ";
        //     // std::cout<<car->race.distRaced<<" ";
        //     // std::cout<<car->_speed_x/100.0<<" ";
        //     // std::cout<< car->_trkPos.seg->radius/100.0 <<" ";
        //     std::cout<< (RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw)/10.0 <<" ";
        //     std::cout<<car->_trkPos.toMiddle/20.0<<" ";
        //     std::cout<<car->_trkPos.seg->width/20.0<<" ";
        //     std::cout<<car->_steerLock<<" ";
        //     std::cout<<std::endl;

        //     std::cout<<"out: "; 
        //     std::cout<<car->ctrl.steer<<" ";
        //     // std::cout<<car->ctrl.brakeCmd<<" ";
        //     // std::cout<<car->ctrl.accelCmd<<" ";
        //     // std::cout<<car->ctrl.gear/6.0<<" ";
        //     std::cout<<std::endl;
        // }

        //Test
        // std::vector<float> input;
        // // input.push_back(car->race.distRaced/2100.0);
        // // input.push_back(car->_speed_x/100.0);
        // // input.push_back(car->_trkPos.seg->radius/100.0);
        // input.push_back((RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw)/10.0);
        // input.push_back(car->_trkPos.toMiddle/20.0);
        // input.push_back(car->_trkPos.seg->width/20.0);
        // input.push_back(car->_steerLock);
        
        // std::vector<float> result = GETRESULT(input);
        // car->ctrl.steer = result[0];
        // car->ctrl.brakeCmd = result[1];
        // car->ctrl.accelCmd = result[2];
        // car->ctrl.gear = int(result[3]*6);
	}
	
}        


/* End of the current race */
static void
endrace(int index, tCarElt *car, tSituation *s)
{
}

/* Called before the module is unloaded */
static void
shutdown(int index)
{
    IMRACING = false;
}

