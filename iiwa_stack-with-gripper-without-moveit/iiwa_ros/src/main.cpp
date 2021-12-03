/**
 * Copyright (C) 2016-2019 Salvatore Virga - salvo.virga@tum.de, Marco Esposito - marco.esposito@tum.de
 * Technische Universität München
 * Chair for Computer Aided Medical Procedures and Augmented Reality
 * Fakultät für Informatik / I16, Boltzmannstraße 3, 85748 Garching bei München, Germany
 * http://campar.in.tum.de
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <csignal>
#include <thread>
#include <ros/ros.h>
#include <iiwa_ros/state/joint_position.hpp>
#include <iiwa_ros/command/joint_position.hpp>
#include <cmath>

iiwa_ros::state::JointPosition jp_state;
iiwa_ros::command::JointPosition jp_command;

void print_joints_state()
{
  ros::Rate loop_hz(10);
  while (ros::ok())
  {
    auto joint_position_ = jp_state.getPosition();
    ROS_INFO_STREAM(
        std::to_string(joint_position_.position.a1)
            << " " << std::to_string(joint_position_.position.a2) << " " << std::to_string(joint_position_.position.a3)
            << " " << std::to_string(joint_position_.position.a4) << " " << std::to_string(joint_position_.position.a5)
            << " " << std::to_string(joint_position_.position.a6) << " " << std::to_string(joint_position_.position.a7)
            << std::endl;);

    loop_hz.sleep();
  }
}

void command_joint()
{
  const double freency = 0.1;
  const double amplitude = 3.1415926 / 2;
  double omega = freency * 2 * 3.1415926 * 1e-9;
  ros::Time start_time = ros::Time::now();
  iiwa_msgs::JointPosition position;
  ros::Rate loop_hz(100);
  while (ros::ok())
  {
    double sinArgument = (ros::Time::now() - start_time).toSec() * omega;
    position.position.a1 = position.position.a3 = position.position.a5 = std::sin(sinArgument) * amplitude;
    loop_hz.sleep();
  }
}

int main()
{
  jp_state.init("iiwa");
  jp_command.init("iiwa");

  // Wait a bit, so that you can be sure the subscribers are connected.
  ros::Duration(0.5).sleep();

  std::thread thread1(print_joints_state);
  thread1.detach();

  std::thread thread2(command_joint);
  thread2.detach();

  ros::spin();
  return 0;
}
