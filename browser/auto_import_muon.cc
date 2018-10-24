/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/auto_import_muon.h"

#include "brave/browser/brave_browser_process_impl.h"
#include "brave/common/brave_switches.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/importer/importer_list.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/importer/importer_data_types.h"

namespace brave {

void AutoImportMuon() {
  LOG(INFO) << "In brave::AutoImportMuon";
  base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kUpgradeFromMuon))
    return;

  LOG(INFO) << "Starting auto-import from Muon";

  // ImporterList::DetectSourceProfiles
  // -> profiles_loaded_callback
  // based on https://cs.chromium.org/chromium/src/chrome/browser/first_run/first_run.cc?l=560-577&rcl=e2569db721bb3bd6b12991d581c60933bf555f30
  base::RunLoop run_loop;
  auto importer_list = std::make_unique<ImporterList>();
  importer_list->DetectSourceProfiles(
      g_browser_process->GetApplicationLocale(),
      false,  // include_interactive_profiles
      run_loop.QuitClosure());
  run_loop.Run();

  bool brave_profile_found = false;
  size_t brave_profile_index = 0;
  for (size_t i = 0; i < importer_list->count(); i++) {
    const auto& source_profile = importer_list->GetSourceProfileAt(i);
    if (source_profile.importer_type == importer::TYPE_BRAVE) {
      brave_profile_found = true;
      brave_profile_index = i;
      break;
    }
  }
  if (!brave_profile_found) {
    LOG(INFO) << "Could not find Muon profile to auto-import";
    return;
  }

  const importer::SourceProfile& source_profile =
      importer_list->GetSourceProfileAt(brave_profile_index);

  uint16_t items_to_import = 0;
  items_to_import |= source_profile.services_supported;

  // Gross, but good enough for government work? Seems to be a common
  // part throughout the rest of the Brave codebase.
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  Profile* current_profile = profile_manager->GetLastUsedProfile();

  if (current_profile) {
    LOG(INFO) << "I have a profile to import from: " << current_profile->GetDebugName();
  }
}

}
