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
#include "chrome/browser/importer/external_process_importer_host.h"
#include "chrome/browser/importer/importer_list.h"
#include "chrome/browser/importer/importer_progress_observer.h"
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

  // Import every possible type of data from the Muon profile
  uint16_t items_to_import = 0;
  items_to_import |= source_profile.services_supported;

  // Gross, but good enough for government work? Seems to be a common
  // part throughout the rest of the Brave codebase.
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  Profile* target_profile = profile_manager->GetLastUsedProfile();

  ImportFromSourceProfile(source_profile, target_profile, items_to_import);
}

void ImportFromSourceProfile(const importer::SourceProfile& source_profile,
                             Profile* target_profile,
                             uint16_t items_to_import) {
  // Deletes itself
  ExternalProcessImporterHost* importer_host =
      new ExternalProcessImporterHost;
  // Don't show the warning dialog if import fails
  importer_host->set_headless();

  ImportEndedObserver observer;
  importer_host->set_observer(&observer);
  importer_host->StartImportSettings(source_profile,
                                     target_profile,
                                     items_to_import,
                                     new ProfileWriter(target_profile));
  // If the import process has not errored out, block on it.
  if (!observer.ended()) {
    base::RunLoop loop;
    observer.set_callback_for_import_end(loop.QuitClosure());
    loop.Run();
    observer.set_callback_for_import_end(base::Closure());
  }
}

}
