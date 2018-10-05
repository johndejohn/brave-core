/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/client/brave_sync_client_impl.h"

#include "base/logging.h"
#include "brave/components/brave_sync/api/brave_sync_event_router.h"
#include "brave/components/brave_sync/client/client_ext_impl_data.h"
#include "brave/common/extensions/api/brave_sync.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/browser/extension_registry.h"

namespace brave_sync {

BraveSyncClientImpl::BraveSyncClientImpl(Profile* profile) :
    handler_(nullptr),
    brave_sync_event_router_(new extensions::BraveSyncEventRouter(profile)),
    profile_(profile),
    startup_complete_(false),
    set_load_pending_(false) {
  DCHECK(profile_);
}

BraveSyncClientImpl::~BraveSyncClientImpl() {
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::~BraveSyncClientImpl";
}

void BraveSyncClientImpl::Shutdown() {
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::Shutdown";
}

void BraveSyncClientImpl::ExtensionStartupComplete() {
  LOG(WARNING) << "TAGAB BraveSyncClientImpl::ExtensionStartupComplete";
  DCHECK(!startup_complete_);
  startup_complete_ = true;
  if (set_load_pending_) {
    LOG(WARNING) << "TAGAB BraveSyncClientImpl::ExtensionStartupComplete loading pending";
    brave_sync_event_router_->LoadClient();
  }
}

void BraveSyncClientImpl::SetSyncToBrowserHandler(SyncLibToBrowserHandler *handler) {
  DCHECK(handler);
  DCHECK(!handler_);
  handler_ = handler;
}

SyncLibToBrowserHandler *BraveSyncClientImpl::GetSyncToBrowserHandler() {
  DCHECK(handler_);
  return handler_;
}

void BraveSyncClientImpl::LoadClient() {
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::LoadClient";
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::LoadClient WILL DO LOAD";
  if (startup_complete_) {
    LOG(ERROR) << "TAGAB BraveSyncClientImpl::LoadClient WILL DO LOAD RIGHT HERE";
    brave_sync_event_router_->LoadClient();
  } else {
    LOG(ERROR) << "TAGAB BraveSyncClientImpl::LoadClient WILL DO LOAD PENDING";
    DCHECK(!set_load_pending_);
    set_load_pending_ = true;
    // Not happy with this, but extensions::ExtensionRegistryObserver approach does not work
  }
}

void BraveSyncClientImpl::SendGotInitData(const Uint8Array &seed, const Uint8Array &device_id,
  const client_data::Config &config) {
  extensions::api::brave_sync::Config config_extension;
  ConvertConfig(config, config_extension);
  brave_sync_event_router_->GotInitData(seed, device_id, config_extension);
}

void BraveSyncClientImpl::SendFetchSyncRecords(
  const std::vector<std::string> &category_names, const base::Time &startAt,
  const int &max_records) {
  brave_sync_event_router_->FetchSyncRecords(category_names, startAt, max_records);
}

void BraveSyncClientImpl::SendFetchSyncDevices() {
  NOTIMPLEMENTED();
}

void BraveSyncClientImpl::SendResolveSyncRecords(const std::string &category_name,
  const SyncRecordAndExistingList &records_and_existing_objects) {

  std::vector<extensions::api::brave_sync::RecordAndExistingObject> records_and_existing_objects_ext;

  ConvertResolvedPairs(records_and_existing_objects, records_and_existing_objects_ext);

  brave_sync_event_router_->ResolveSyncRecords(category_name,
    records_and_existing_objects_ext);
}

void BraveSyncClientImpl::SendSyncRecords(const std::string &category_name,
  const RecordsList &records) {

  std::vector<extensions::api::brave_sync::SyncRecord> records_ext;
  ConvertSyncRecordsFromLibToExt(records, records_ext);

  brave_sync_event_router_->SendSyncRecords(category_name, records_ext);
}

void BraveSyncClientImpl::SendDeleteSyncUser()  {
  NOTIMPLEMENTED();
}

void BraveSyncClientImpl::SendDeleteSyncCategory(const std::string &category_name) {
  NOTIMPLEMENTED();
}

void BraveSyncClientImpl::SendGetBookmarksBaseOrder(const std::string &device_id, const std::string &platform) {
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::SendGetBookmarksBaseOrder: device_id="<<device_id<<" platform="<<platform;
  brave_sync_event_router_->SendGetBookmarksBaseOrder(device_id, platform);
}

void BraveSyncClientImpl::SendGetBookmarkOrder(const std::string &prevOrder, const std::string &nextOrder) {
  LOG(ERROR) << "TAGAB BraveSyncClientImpl::SendGetBookmarkOrder: prevOrder="<<prevOrder<<" nextOrder="<<nextOrder;
  brave_sync_event_router_->SendGetBookmarkOrder(prevOrder, nextOrder);
}

void BraveSyncClientImpl::NeedSyncWords(const std::string &seed) {
  brave_sync_event_router_->NeedSyncWords(seed);
}

void BraveSyncClientImpl::NeedBytesFromSyncWords(const std::string &words) {
  brave_sync_event_router_->NeedBytesFromSyncWords(words);
}

} // namespace brave_sync