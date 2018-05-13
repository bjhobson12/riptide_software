#include "riptide_controllers/command_combinator.h"

int main(int argc, char **argv) {
  ros::init(argc, argv, "command_combinator");
  CommandCombinator cc;
  cc.Loop();
}

CommandCombinator::CommandCombinator() {
    linear_x_sub = nh.subscribe<std_msgs::Float64>("command/accel/linear/x", 1, &CommandCombinator::LinearXCB, this);
    linear_y_sub = nh.subscribe<std_msgs::Float64>("command/accel/linear/y", 1, &CommandCombinator::LinearYCB, this);
    linear_z_sub = nh.subscribe<std_msgs::Float64>("command/accel/linear/z", 1, &CommandCombinator::LinearZCB, this);
    depth_sub = nh.subscribe<geometry_msgs::Vector3>("command/accel/depth", 1, &CommandCombinator::DepthCB, this);

    angular_sub = nh.subscribe<geometry_msgs::Vector3>("command/accel/angular", 1, &CommandCombinator::AngularCB, this);
    cmd_pub = nh.advertise<geometry_msgs::Accel>("command/accel", 1);

    current_accel.linear.x = 0;
    current_accel.linear.y = 0;
    current_accel.linear.z = 0;
    current_accel.angular.x = 0;
    current_accel.angular.y = 0;
    current_accel.angular.z = 0;
    linear_accel.x = 0;
    linear_accel.y = 0;
    linear_accel.z = 0;
    depth_accel.x = 0;
    depth_accel.y = 0;
    depth_accel.z = 0;
}

void CommandCombinator::LinearXCB(const std_msgs::Float64::ConstPtr &accel) {
  linear_accel.x = accel->data;
  //CommandCombinator::CombineLinear();
  //cmd_pub.publish(current_accel);
}

void CommandCombinator::LinearYCB(const std_msgs::Float64::ConstPtr &accel) {
  linear_accel.y = accel->data;
  //CommandCombinator::CombineLinear();
  //cmd_pub.publish(current_accel);
}

void CommandCombinator::LinearZCB(const std_msgs::Float64::ConstPtr &accel) {
  linear_accel.z = accel->data;
  //CommandCombinator::CombineLinear();
  //cmd_pub.publish(current_accel);
}

void CommandCombinator::AngularCB(const geometry_msgs::Vector3::ConstPtr &ang_accel) {
  current_accel.angular.x = ang_accel->x;
  current_accel.angular.y = ang_accel->y;
  current_accel.angular.z = ang_accel->z;
  //CommandCombinator::CombineLinear();
  //cmd_pub.publish(current_accel);
}

void CommandCombinator::DepthCB(const geometry_msgs::Vector3::ConstPtr & d_accel) {
  depth_accel.x = d_accel->x;
  depth_accel.y = d_accel->y;
  depth_accel.z = d_accel->z;
  //CommandCombinator::CombineLinear();
}

void CommandCombinator::ResetController() {
    current_accel.linear.x = 0;
    current_accel.linear.y = 0;
    current_accel.linear.z = 0;
    current_accel.angular.x = 0;
    current_accel.angular.y = 0;
    current_accel.angular.z = 0;
    cmd_pub.publish(current_accel);
}

void CommandCombinator::CombineLinear() {
  current_accel.linear.x = linear_accel.x + depth_accel.x;
  current_accel.linear.y = linear_accel.y + depth_accel.y;
  current_accel.linear.z = linear_accel.z + depth_accel.z;
}

void CommandCombinator::Loop() {
  while(!ros::isShuttingDown()) {
    CommandCombinator::CombineLinear();
    cmd_pub.publish(current_accel); // ALWAYS publish a message, regardless of circumstance
    ros::spinOnce();
  }
}
