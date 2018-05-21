/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_PROFILES_BRAVE_PROFILE_MANAGER_H_
#define BRAVE_BROWSER_PROFILES_BRAVE_PROFILE_MANAGER_H_

#include "chrome/browser/profiles/profile_manager.h"

class BraveProfileManager : public ProfileManager {
 public:
  explicit BraveProfileManager(const base::FilePath& user_data_dir);

 protected:
  // ProfileManager implementation.
  Profile* CreateProfileHelper(const base::FilePath& path) override;
  Profile* CreateProfileAsyncHelper(const base::FilePath& path,
                                            Delegate* delegate) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(BraveProfileManager);
};

#endif  // BRAVE_BROWSER_PROFILES_BRAVE_PROFILE_MANAGER_H_