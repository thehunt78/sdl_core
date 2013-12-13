/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include "application_manager/commands/mobile/alert_maneuver_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

AlertManeuverRequest::AlertManeuverRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message),
   result_(mobile_apis::Result::INVALID_ENUM) {
}

AlertManeuverRequest::~AlertManeuverRequest() {
}

void AlertManeuverRequest::Run() {
  LOG4CXX_INFO(logger_, "AlertManeuverRequest::Run");

  if ((!(*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks))) {
    LOG4CXX_ERROR(logger_, "AlertManeuverRequest::Request without parameters!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  Application* app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    if (mobile_apis::Result::INVALID_DATA == processing_result) {
      LOG4CXX_ERROR(logger_, "Wrong soft buttons parameters!");
      SendResponse(false, processing_result);
      return;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == processing_result) {
      LOG4CXX_ERROR(logger_, "UNSUPPORTED_RESOURCE!");
      result_ = processing_result;
    }
  }

  // check TTSChunk parameter
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      // crate HMI basic communication playtone request
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);

      msg_params[hmi_request::tts_chunks] = smart_objects::SmartObject(
          smart_objects::SmartType_Array);
      msg_params[hmi_request::tts_chunks] =
          (*message_)[strings::msg_params][strings::tts_chunks];

      msg_params[strings::app_id] = app->app_id();
      SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params, true);
      pending_requests_.push_back(hmi_apis::FunctionID::TTS_Speak);
    }
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
  }

  SendHMIRequest(hmi_apis::FunctionID::Navigation_AlertManeuver, &msg_params,
                 true);
  pending_requests_.push_back(hmi_apis::FunctionID::Navigation_AlertManeuver);
}

void AlertManeuverRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "AlertManeuverRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  mobile_apis::Result::eType result_code;
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      LOG4CXX_INFO(logger_, "Received Navigation_AlertManeuver event");

      pending_requests_.erase(
          std::find(pending_requests_.begin(),
                    pending_requests_.end(),
                    hmi_apis::FunctionID::Navigation_AlertManeuver));

      result_code =
          static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");

      pending_requests_.erase(
          std::find(pending_requests_.begin(),
                    pending_requests_.end(),
                    hmi_apis::FunctionID::TTS_Speak));

      result_code =
          static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      SendResponse(false, result_code, "Received unknown event");
      return;
    }
  }

  if (mobile_apis::Result::INVALID_ENUM == result_) {
    result_= result_code;
  } else if (mobile_apis::Result::SUCCESS == result_) {
    result_ =
        result_ == result_code
        ? mobile_apis::Result::SUCCESS
        : result_code
        ;
  }

  if (pending_requests_.empty()) {
    bool success = mobile_apis::Result::SUCCESS == result_code;

    if (mobile_apis::Result::INVALID_ENUM != result_) {
      result_code = result_;
    }

    SendResponse(success, result_code, NULL, &(message[strings::msg_params]));
  }
}

}  // namespace commands

}  // namespace application_manager
