// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief a header file with declaration of ImageWindowOutput class
 * @file image_window_output.cpp
 */

#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include "cv_bridge/cv_bridge.h"
#include "vino_core_lib/pipeline.h"
#include "vino_core_lib/outputs/rviz_output.h"

Outputs::RvizOutput::RvizOutput()
{
  image_topic_ = nullptr;
  pub_image_ = nh_.advertise<sensor_msgs::Image>("/openvino_toolkit/images", 16);
  image_window_output_ = std::make_shared<Outputs::ImageWindowOutput>("WindowForRviz", 950);
}

void Outputs::RvizOutput::feedFrame(const cv::Mat & frame)
{
  image_window_output_->feedFrame(frame);
}

void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::FaceDetectionResult> & results)
{
  image_window_output_->accept(results);
}

void Outputs::RvizOutput::accept(
  const std::vector<vino_core_lib::ObjectDetectionResult> & results)
{
  image_window_output_->accept(results);
}

void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::EmotionsResult> & results)
{
  image_window_output_->accept(results);
}

void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::AgeGenderResult> & results)
{
  image_window_output_->accept(results);
}

void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::HeadPoseResult> & results)
{
  image_window_output_->accept(results);
}

void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::ObjectSegmentationResult>& results)
{
  image_window_output_->accept(results);
}
void Outputs::RvizOutput::accept(const std::vector<vino_core_lib::PersonReidentificationResult> & results)
{
  image_window_output_->accept(results);
}
 

void Outputs::RvizOutput::handleOutput()
{
  image_window_output_->setPipeline(getPipeline());
  image_window_output_->decorateFrame();
  cv::Mat frame = image_window_output_->getFrame();
  std_msgs::Header header = getHeader();
  std::shared_ptr<cv_bridge::CvImage> cv_ptr =
    std::make_shared<cv_bridge::CvImage>(header, "bgr8", frame);
   sensor_msgs::Image image_msg;
  image_topic_ = cv_ptr->toImageMsg();
//    image_topic_ = std::make_shared<sensor_msgs::Image>(image_msg);
  pub_image_.publish(image_topic_);
}
  std::shared_ptr<sensor_msgs::Image> image_topic_;
std_msgs::Header Outputs::RvizOutput::getHeader()
{
  std_msgs::Header header;
  header.frame_id = getPipeline()->getInputDevice()->getFrameID();

  std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
  int64 ns = tp.time_since_epoch().count();
  header.stamp.sec = ns / 1000000000;
  header.stamp.nsec = ns % 1000000000;
  return header;
}
