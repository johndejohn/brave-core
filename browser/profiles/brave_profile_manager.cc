/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/profiles/brave_profile_manager.h"

#include "base/metrics/histogram_macros.h"
#include "brave/browser/profiles/brave_profile.h"

BraveProfileManager::BraveProfileManager(const base::FilePath& user_data_dir)
    : ProfileManager(user_data_dir) {}

Profile* BraveProfileManager::CreateProfileHelper(const base::FilePath& path) {
  TRACE_EVENT0("browser", "ProfileManager::CreateProfileHelper");
  SCOPED_UMA_HISTOGRAM_TIMER("Profile.CreateProfileHelperTime");

  return BraveProfile::CreateProfile(path, NULL, Profile::CREATE_MODE_SYNCHRONOUS);
}

Profile* BraveProfileManager::CreateProfileAsyncHelper(
                                                    const base::FilePath& path,
                                                    Delegate* delegate) {
  return BraveProfile::CreateProfile(path,
                                     delegate,
                                     Profile::CREATE_MODE_ASYNCHRONOUS);
}