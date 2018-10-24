/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/auto_import_muon.h"
#include "brave/common/brave_switches.h"

#include "base/command_line.h"
#include "base/logging.h"

namespace brave {

void AutoImportMuon() {
  LOG(INFO) << "In brave::AutoImportMuon";
  base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kUpgradeFromMuon))
    return;

  LOG(INFO) << "Starting auto-import from Muon";
}

}
