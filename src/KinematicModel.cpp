#include "KinematicModel.h"
#include <math.h>

extern const int NJOINTS;
InverseKinematics::InverseKinematics() : radius(0.0f), Arc_Length(0.0f), max_arc_length(0.0f), Joint_Angles(nullptr)
{   
    if(Arc_Length == NULL)
        Arc_Length = Get_Maximum_Arc_Length();
    
    Joint_Angles = new double[NJOINTS];
}

double InverseKinematics::Get_Maximum_Arc_Length()
{
    double arc_length = (2 * M_PI * LNK4_RAD) * (270 / 360);
    return arc_length; 
}

double InverseKinematics::Base_Angle() 
{
    double theta = atan(end_effector->y / end_effector->x);
    if(theta > 270 && theta < 0)
        return;

    return theta;
}

double InverseKinematics::Raw_Angle_Joint1()
{
    double theta1 = 0.0f;
    
    double fee1 = 0.0f, fee2 = 0.0f;

    fee2 = atan(end_effector->z / end_effector->x);

    double Hypothenous = sqrt( Powof2(end_effector->x) +  Powof2(end_effector->z)); //R1
    fee1 = acos((Powof2(A3) - Powof2(A2) - Powof2(Hypothenous)) / (-2 * A2 * Hypothenous));

    theta1 = fee2 - fee1;
    return theta1;
}

double InverseKinematics::Raw_Angle_Joint2()
{
    double fee3 = 0.0f;
    double theta2 = 0.0f;

    double R1 = sqrt( atan(end_effector->y / end_effector->x) );
    fee3 = acos( (Powof2(R1) - Powof2(A2) - Powof2(A3)) / (-2 * A2 * A3) );
    
    theta2 = fee3 - 180;
    return theta2;
}

void InverseKinematics::Set_Arc_Length( float arc_length )
{
    if(arc_length > max_arc_length)
        this->Arc_Length = max_arc_length;
    else 
        this->Arc_Length = arc_length;
}

double InverseKinematics::Rotating_Joint()
{
    double rotational_angle = 360 * ( Arc_Length / (2 * M_PI * radius) );
    
    if(rotational_angle > 270)
        return 270;
    return rotational_angle;
}

double*  InverseKinematics::Calculate_Joint_Angles()
{
    double Base_Joint   = Base_Angle();
    double Joint1       = Raw_Angle_Joint1();
    double Joint2       = Raw_Angle_Joint2();
    double Joint3       = Rotating_Joint();

    Joint_Angles[0] = Base_Joint;
    Joint_Angles[1] = Joint1;
    Joint_Angles[2] = Joint2;
    Joint_Angles[3] = Joint3;

    return Joint_Angles;
}

void InverseKinematics::Deallocate_Pointer() 
{
    if(Joint_Angles != nullptr) 
    {
        delete[] Joint_Angles;
        Joint_Angles = nullptr;
    }
}