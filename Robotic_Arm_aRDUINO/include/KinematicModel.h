#pragma once
#include <math.h>
#include <stdio.h>

#include "Mathematics.hpp" 
#include "Arduino.h"

#define LNK4_RAD 2.835

class InverseKinematics
{
private:
    InverseKinematics();
private:
    Matrix3x3 end_effector_orientation; //homogenous transformation matrix 
    End_Effector* end_effector;         //position

    double radius;
    float Arc_Length, max_arc_length;
    double* Joint_Angles;
    float A1, A2, A3;
    
    double Get_Maximum_Arc_Length();

    double Base_Angle();
    double Raw_Angle_Joint1();
    double Raw_Angle_Joint2();
    double Rotating_Joint(); //rotating  head

public:
    static InverseKinematics& GetInstance()
    {
        static InverseKinematics instance;
        return instance;
    }

    inline void Set_EndEffector( End_Effector& end )    { end_effector = &end; }
    inline void Set_Radius(double radius )              { this->radius = radius; }
    inline void Set_Arc_Length( float arc_length );

    //call this after Set_EndEffector()
    double* Calculate_Joint_Angles();

    //call this after clicking shutdown
    void Deallocate_Pointer();   
};