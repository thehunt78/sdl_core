﻿/*
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "application_manager/application_manager.h"

namespace policy {
SDL_CREATE_LOG_VARIABLE("PolicyHandler")

#ifdef EXTERNAL_PROPRIETARY_MODE
AppPermissionDelegate::AppPermissionDelegate(
    const uint32_t connection_key,
    const PermissionConsent& permissions,
    const ExternalConsentStatus& external_consent_status,
    policy::PolicyHandlerInterface& policy_handler)
    : connection_key_(connection_key)
    , permissions_(permissions)
    , external_consent_status_(external_consent_status)
    , policy_handler_(policy_handler) {}
#else
AppPermissionDelegate::AppPermissionDelegate(
    const uint32_t connection_key,
    const PermissionConsent& permissions,
    policy::PolicyHandlerInterface& policy_handler)
    : connection_key_(connection_key)
    , permissions_(permissions)
    , policy_handler_(policy_handler) {}
#endif

void AppPermissionDelegate::threadMain() {
  SDL_LOG_AUTO_TRACE();

#ifdef EXTERNAL_PROPRIETARY_MODE
  policy_handler_.OnAppPermissionConsentInternal(
      connection_key_, external_consent_status_, permissions_);
#else
  policy_handler_.OnAppPermissionConsentInternal(connection_key_, permissions_);
#endif
}

void AppPermissionDelegate::exitThreadMain() {
  // Do nothing
}

}  // namespace policy
