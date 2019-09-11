#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

double goto_X1 = 2.0, goto_Y1 = 0.0;
double goto_X2 = 2.0, goto_Y2 = 2.0;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Request robot to move to Pickup location
  goal.target_pose.pose.position.x = goto_X1;
  goal.target_pose.pose.position.y = goto_Y1;
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("robot heading towards pick up");
  ac.sendGoal(goal);
  ac.waitForResult();

  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {

    ROS_INFO("Robot pick up object executed");
    // Wait for 5 seconds
    ros::Duration(5).sleep();

    // Request robot to move to Dropoff location
    goal.target_pose.pose.position.x = goto_X2;
    goal.target_pose.pose.position.y = goto_Y2;
    goal.target_pose.pose.orientation.w = 1.0;

    ROS_INFO("Robot heading towards drop off");
    ac.sendGoal(goal);
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      // Robot reached dropoff zone
      ROS_INFO("Robot dropped object");
    }
    else
    {
      ROS_INFO("Unable to dropoff");
    }
  }
  else
  {
    ROS_INFO("Unable to pickup");
  }

  return 0;
}
