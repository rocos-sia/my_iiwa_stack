#include <iiwa_ros/conversions.hpp>
#include <iiwa_ros/service/control_mode.hpp>
#include <iiwa_ros/state/cartesian_pose.hpp>
#include <iiwa_ros/command/cartesian_pose.hpp>
#include <iiwa_ros/command/joint_position.hpp>


// meaningful is obtained or until a maximum amount of time passed.
void sleepForMotion( iiwa_ros::service::TimeToDestinationService& iiwa, const double maxSleepTime )
{
    double ttd           = iiwa.getTimeToDestination( );
    ros::Time start_wait = ros::Time::now( );
    while ( ttd < 0.0 && ( ros::Time::now( ) - start_wait ) < ros::Duration( maxSleepTime ) )
    {
        ros::Duration( 0.5 ).sleep( );
        ttd = iiwa.getTimeToDestination( );
    }
    if ( ttd > 0.0 )
    {
        ROS_INFO_STREAM( "Sleeping for " << ttd << " seconds." );
        ros::Duration( ttd ).sleep( );
    }
    else
    {
        ROS_ERROR_STREAM( "cannot get the TimeToDestination" );
    }
}


int main( int argc, char** argv )
{
    // Initialize ROS
    ros::init( argc, argv, "ControlMode" );
    ros::NodeHandle nh( "~" );

    // ROS spinner.
    ros::AsyncSpinner spinner( 1 );
    spinner.start( );

    iiwa_ros::state::CartesianPose iiwa_pose_state;
    iiwa_ros::service::ControlModeService iiwa_control_mode;
    iiwa_ros::command::JointPosition iiwa_joint_command;
    iiwa_ros::service::TimeToDestinationService iiwa_time_destination;


    iiwa_msgs::JointPosition command_joint_position;

    iiwa_pose_state.init( "iiwa" );
    iiwa_control_mode.init( "iiwa" );
    iiwa_joint_command.init( "iiwa" );
    iiwa_time_destination.init( "iiwa" );
    
    // Dynamic parameter to choose the rate at wich this node should run
    double ros_rate = 0.1;
    nh.param( "ros_rate", ros_rate, 0.1 );  // 0.1 Hz = 10 seconds
    ros::Rate* loop_rate_ = new ros::Rate( ros_rate );

    bool job_done = true;

    while ( ros::ok( ) )
    {
        if ( iiwa_pose_state.isConnected( ) )
        {
            command_joint_position.position.a1 = -59 * M_PI / 180;
            command_joint_position.position.a2 = 45 * M_PI / 180;
            command_joint_position.position.a3 = 0 * M_PI / 180;
            command_joint_position.position.a4 = -90 * M_PI / 180;
            command_joint_position.position.a5 = 0 * M_PI / 180;
            command_joint_position.position.a6 = -45 * M_PI / 180;
            command_joint_position.position.a7 = 0 * M_PI / 180;
            iiwa_joint_command.setPosition( command_joint_position );
            sleepForMotion( iiwa_time_destination, 2.0 );

            break;
        }
        else
        {
            ROS_WARN_STREAM( "Robot is not connected..." );
            ros::Duration( 5.0 ).sleep( );  // 5 seconds
        }
    }

    std::string str;

    while ( job_done && ros::ok( ) )
    {
        if ( iiwa_pose_state.isConnected( ) )
        {
            ROS_INFO_STREAM( "The robot will be now set in Cartesian Impedance Mode" );
            // Low stiffness only along Z.
            iiwa_control_mode.setCartesianImpedanceMode( iiwa_ros::conversions::CartesianQuantityFromFloat( 500, 1500, 1500, 300, 300, 300 ), iiwa_ros::conversions::CartesianQuantityFromFloat( 0.7 ) );
            ROS_INFO_STREAM( "In 20 seconds the robot will go back to Position Control Mode" );
            std::cin >> str;

            iiwa_control_mode.setPositionControlMode( );
            ROS_INFO_STREAM( "In 20 seconds the robot will be set in Joint Impedance Mode" );
            std::cin >> str;


            // Low stiffness only for the 6th joint.
            iiwa_control_mode.setJointImpedanceMode( iiwa_ros::conversions::jointQuantityFromFloat( 1500, 1500, 1500, 200, 1500, 100, 1500 ), iiwa_ros::conversions::jointQuantityFromFloat( 0.5 ) );
            ROS_INFO_STREAM( "In 20 seconds the robot will go back to Position Control Mode" );
            std::cin >> str;

            iiwa_control_mode.setPositionControlMode( );
            std::cin >> str;

            ROS_INFO_STREAM( "Job done! Shutting down..." );
            job_done = false;
        }
        else
        {
            ROS_ERROR_STREAM( "Robot is not connected..." );
            ros::Duration( 5.0 ).sleep( );  // 5 seconds
        }
    }
};
