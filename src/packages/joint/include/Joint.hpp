/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/CoreNode.hpp>
#include <core/mw/Subscriber.hpp>
#include <core/mw/Publisher.hpp>

#include <core/joint/SubscriberConfiguration.hpp>
#include <core/common_msgs/Float32.hpp>
#include <comau_edo/edo_msgs/EdoJointCtrl.hpp>
#include <core/control_msgs/PID_param.hpp>
#include <core/control_msgs/Encoder_State.hpp>
#include <string.h>
#include <stdio.h>
#include <comau_edo/edo_msgs/EdoJointVersion.hpp>
//#include <core/joint/ROSSerialNode.hpp>

namespace core {
namespace joint {

class ROSSerialNode;

class Joint:
	public core::mw::CoreNode,
	public core::mw::CoreConfigurable<SubscriberConfiguration>
{

public:
	Joint(const char* name, 
		core::os::Thread::Priority priority = core::os::Thread::PriorityEnum::NORMAL,
		uint8_t id = 0
	);
	
	virtual	~Joint();

	void set_id(uint8_t id);

	uint8_t	get_id(void);

	bool set_state_topic(const char * topic, size_t size);
	
	bool set_ctrl_topic(const char * topic, size_t size);
	
	bool set_config_topic(const char * topic, size_t size);
	
	bool set_version_topic(const char * topic, size_t size);

	void set_ros_node(core::joint::ROSSerialNode *node_ptr);

	const float & getPos(void);
	const float & getVel(void);
	const float & getCurrent(void);
	const uint8_t & getCommandFlag(void);
	const bool & stateUpdated(void);
	void setStateUpdated(const bool & updated);

// Publishers and subscribers
	core::mw::Subscriber<core::control_msgs::Encoder_State, 5> _subscriber_state;
	core::mw::Publisher<comau_edo::edo_msgs::EdoJointCtrl> _publisher_ctrl;
	core::mw::Publisher<core::control_msgs::PID_param> _publisher_config;
	core::mw::Subscriber<comau_edo::edo_msgs::EdoJointVersion, 1> _subscriber_version;

private:

	uint8_t _id;
	float _pos;
	float _vel;
	float _current;
	uint8_t _commandFlag; // ack
	bool _state_updated;
	
	char _state_topic[10];
	char _ctrl_topic[10];
	char _config_topic[10];
	char _version_topic[16];

	core::joint::ROSSerialNode *_rosNode;
// CoreNode events to override

private:

	bool
	onConfigure();

	bool
	onPrepareMW();

	bool
	onLoop();

	bool
	onStart();

	bool
	onStop();

	static bool
	callback_state_(
	const core::control_msgs::Encoder_State& msg,
	void*                             context
	);

	static bool
	callback_version_(
	const comau_edo::edo_msgs::EdoJointVersion& msg,
	void*                             context
	);
};
}
}