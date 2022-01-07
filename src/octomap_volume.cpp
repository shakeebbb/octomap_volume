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

 	volumeOut.data = octTree->getNumLeafNodes() * msg.resolution * msg.resolution * msg.resolution;
  volumePub.publish(volumeOut);
}
