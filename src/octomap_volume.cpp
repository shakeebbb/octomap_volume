#include "ros/ros.h"
#include "octomap_volume.h"
#include "std_msgs/Float64.h"

ros::Publisher volumePub;

void octomap_cb(const octomap_msgs::Octomap& msg);

using namespace std;

// Main
int main(int argc, char **argv)
{
	ros::init(argc, argv, "octomap_volume");
	ros::NodeHandle nh(ros::this_node::getName());

  ros::Subscriber octomapSub = nh.subscribe("octomap_in", 10, octomap_cb);
  volumePub = nh.advertise<std_msgs::Float64>("volume_out", 10);
  
  #ifdef WITH_ROUGHNESS
    ROS_WARN("octomap_volume: Running With Roughness");
  #else
    ROS_WARN("octomap_volume: Running Without Roughness");
  #endif
	
	ros::spin();
	return 0;
}

// ***************************************** //
void octomap_cb(const octomap_msgs::Octomap& msg)
{
  octomap::AbstractOcTree* tree = octomap_msgs::msgToMap(msg);
  OcTreeT* octTree = (OcTreeT*)tree;

  std_msgs::Float64 volumeOut;

	if(!octTree)
		return;

  volumeOut.data = 0;

	for( OcTreeT::leaf_iterator it = octTree->begin_leafs(); it!=octTree->end_leafs(); ++it )
		volumeOut.data += (msg.resolution * msg.resolution * msg.resolution);

  volumePub.publish(volumeOut);
  delete octTree;
}
