#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/TwistStamped.h"
#include "tf2_msgs/TFMessage.h"
#include "tf/transform_broadcaster.h"
#include <fstream>
#include <iomanip>

// 1.需要手动创建txt文件,代码无法自动创建
// 2.txt的路径为绝对路径,相对路径似乎有问题,不过不影响

void tf_callback(const tf2_msgs::TFMessage& msg){
    static ros::Time tf_time = msg.transforms[0].header.stamp;
    tf::Quaternion q(msg.transforms[0].transform.rotation.x,msg.transforms[0].transform.rotation.y,msg.transforms[0].transform.rotation.z,msg.transforms[0].transform.rotation.w);
    double yaw,pitch,roll;
    tf::Matrix3x3(q).getRPY(roll,pitch,yaw);
    static int count_tf=0;
    count_tf++;
    ROS_INFO("loading tf data...");
    std::ofstream f("/home/wanghuohuo/catkin_clion/src/extract_data/data/tf_data.txt", std::ios::app); // 保存新内容
    f<<std::setprecision(10)<<msg.transforms[0].header.stamp-tf_time<<" "\
                                <<yaw<<" "<<pitch<<" "<<roll<<" "\
                                <<msg.transforms[0].transform.translation.x<<" "<<msg.transforms[0].transform.translation.y<<" "<<msg.transforms[0].transform.translation.z<<" "\
                                <<msg.transforms[0].transform.rotation.x<<" "<<msg.transforms[0].transform.rotation.y<<" "<<msg.transforms[0].transform.rotation.z<<" "<<msg.transforms[0].transform.rotation.w<<" "\
                                <<count_tf<<std::endl;
    f.close();
}

void gps_callback(const sensor_msgs::NavSatFix& msg){
    static ros::Time gps_time = msg.header.stamp;
    static int count_gps=0;
    count_gps++;
    ROS_INFO("loading gps data...");
    std::ofstream f("/home/wanghuohuo/catkin_clion/src/extract_data/data/gps_data.txt", std::ios::app); // 保存新内容

    f<<std::setprecision(10)<<msg.header.stamp-gps_time<<" "\
                                <<msg.latitude<<" "<<msg.longitude<<" "<<msg.altitude<<" "\
                                <<count_gps<<std::endl;
    f.close();
}

void imu_callback(const sensor_msgs::Imu& msg){
    static ros::Time imu_time = msg.header.stamp;
    static int count_imu=0;
    count_imu++;
    ROS_INFO("loading imu data...");
    std::ofstream f("/home/wanghuohuo/catkin_clion/src/extract_data/data/imu_data.txt", std::ios::app); // 保存新内容
    f<<std::setprecision(10)<<msg.header.stamp-imu_time<<" "\
                                <<msg.orientation.x<<" "<<msg.orientation.y<<" "<<msg.orientation.z<<" "<<msg.orientation.w<<" "\
                                <<msg.angular_velocity.x<<" "<<msg.angular_velocity.y<<" "<<msg.angular_velocity.z<<" "\
                                <<msg.linear_acceleration.x<<" "<<msg.linear_acceleration.y<<" "<<msg.linear_acceleration.z<<" "\
                                <<count_imu<<std::endl;
    f.close();
}

void vel_callback(const geometry_msgs::TwistStamped& msg){
    static ros::Time vel_time = msg.header.stamp;
    static int count_vel=0;
    count_vel++;
    ROS_INFO("loading vel data...");
    std::ofstream f("/home/wanghuohuo/catkin_clion/src/extract_data/data/vel_data.txt", std::ios::app); // 保存新内容

    f<<std::setprecision(10)<<msg.header.stamp-vel_time<<" "\
                            <<msg.twist.linear.x<<" "<<msg.twist.linear.y<<" "<<msg.twist.linear.z<<" "\
                            <<msg.twist.angular.x<<" "<<msg.twist.angular.y<<" "<<msg.twist.angular.z<<" "\
                            <<count_vel<<std::endl;
    f.close();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");

    ros::NodeHandle n;
    ros::Subscriber sub_tf  = n.subscribe("/tf",10,tf_callback); // 试下10和1的区别
    ros::Subscriber sub_gps  = n.subscribe("/kitti/oxts/gps/fix",10,gps_callback); // 试下10和1的区别
    ros::Subscriber sub_imu  = n.subscribe("/kitti/oxts/imu",10,imu_callback);
    ros::Subscriber sub_vel  = n.subscribe("/kitti/oxts/gps/vel",10,vel_callback);

    ros::Rate loop_rate(10);

    // tf data中的orientation值与imu中的orientation值相同!
    // tf data: x, y, z,是odom坐标系下坐标
    std::ofstream f_tf("/home/wanghuohuo/catkin_clion/src/extract_data/data/tf_data.txt");
    f_tf<<"time_stamp"<<" "\
        <<"yaw"<<" "<<"pitch"<<" "<<"roll"<<" "\
        <<"translation_x"<<" "<<"translation_y"<<" "<<"translation_z"<<" "\
        <<"rotation_x"<<" "<<"rotation_y"<<" "<<"rotation_z"<<" "<<"rotation_w"<<" "<<"tf_count"<<std::endl;
    f_tf.close();

    // gps data: latitude, longitude, altitude
    std::ofstream f_gps("/home/wanghuohuo/catkin_clion/src/extract_data/data/gps_data.txt");
    f_gps<<"time_stamp"<<" "\
         <<"longitude"<<" "<<"longitude"<<" "<<"altitude"<<" "<<"gps_count"<<std::endl;
    f_gps.close();

    // 注意imu data和velocity data的角速度的值是相同的!
    // imu data: orientation, angulal_vel, linear_vel
    std::ofstream f_imu("/home/wanghuohuo/catkin_clion/src/extract_data/data/imu_data.txt");
    f_imu<<"time_stamp"<<" "\
         <<"orientation_x"<<" "<<"orientation_y"<<" "<<"orientation_z"<<" "<<"orientation_w"<<" "\
         <<"angular_x"<<" "<<"angular_y"<<" "<<"angular_z"<<" "\
         <<"acc_x"<<" "<<"acc_y"<<" "<<"acc_z"<<" "<<"imu_count"<<std::endl;
    f_imu.close();

    // velocity data
    std::ofstream f_vel("/home/wanghuohuo/catkin_clion/src/extract_data/data/vel_data.txt");
    f_vel<<"time_stamp"<<" "\
         <<"linear_x"<<" "<<"linear_y"<<" "<<"linear_z"<<" "\
         <<"angular_x"<<" "<<"angular_y"<<" "<<"angular_z"<<" "<<"vel_count"<<std::endl;
    f_vel.close();

    while (ros::ok())
    {
        ROS_INFO("!!");
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

