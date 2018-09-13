#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CANSENDBOOKMARKS_H
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CANSENDBOOKMARKS_H

namespace base {
  class SequencedTaskRunner;
}

namespace bookmarks {
  class BookmarkNode;
}

namespace brave_sync {

class ControllerForBookmarksExports {
public:
  virtual ~ControllerForBookmarksExports() = default;

  // Not sure about to place it here or to have a separate interface
  virtual void CreateUpdateDeleteBookmarks(
    const int &action,
    const std::vector<const bookmarks::BookmarkNode*> &list,
    const bool &addIdsToNotSynced,
    const bool &isInitialSync) = 0;

  virtual base::SequencedTaskRunner *GetTaskRunner() = 0;
};

} // namespace brave_sync

#endif //BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_CANSENDBOOKMARKS_H
