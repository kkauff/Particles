Particle Simulator 

To use with open gazer (eye gaze guided particle motion):

Install and build Opengazer from http://www.inference.phy.cam.ac.uk/opengazer/#opengazerpreviousdownload
(be sure to install all required libraries).  

Open OutputMethods.cpp from the Opengazer project.  In the function 

void StreamStore::store(const TrackerOutput& output)

copy and paste the following directly under the line    
 
id = (int) output.targetid;

//Copy and paste the following

    //output coordinates to xyz.txt
    ofstream fout("../../xyz.txt");
    fout << x << " " << y;
    fout << flush;
    fout.close();
    
    //output target to tar.txt
    ofstream fidout ("../../tar.txt");
    double tx = output.target.x;
    double ty = output.target.y;
    fidout << tx << " " << ty;
    fidout << flush;
    fidout.close();

This will read the Opengazer coordinates to two files.  The xyz.txt file holds the Opengazer 
estimated coordinates and the tar.txt file holds the Opengazer closest target coordinates.  

This program currently uses the xyz.txt estimated coordinates. 

After following the calibration instructions in the Opengazer README, build and run this project.  

Red spheres are control points, used to move the particles in a predefined way.  The control 
particles can be modified in the constructor for SimpleSystem.  The particles are affected by the
induced velocity and attractive force of the control particles and by the Opengazer eye gaze tracking
coordinates.  

Remember to hold your head very still when using the programs, and a lamp positioned directly behind 
the webcam.



*******************************************************************************
To compile and run the code, decompress the zip file into a directory, and type 

$ make 

Then, to run the code, you can pass the following parameters:

// To run the code with the default settings being Simple system, Gazer Method
//   and stepsize of .02, simply enter:
$ a3 

// Optional parameters:
$ a3 integration_method step_size system_type

// integration method: choose either "e" for euler, or "t" for trapezoid
// step_size: .02 seems to work best for cloth or pendulum. Slightly smaller 
is better for chain.

// system_type: There are 4 system types to choose from: "simple" "pendulum" 
"chain" and "cloth". Try them all! :D

// * Note * you have to specify all previous parameters to specify the system 

Then, to play with the code: 
press "s" and the cloth will shake around
press "w" and the particles will disappear just showing the wireframe


******************************************************************************



