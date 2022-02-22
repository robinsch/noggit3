// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <noggit/AsyncObject.h>

#include <array>
#include <atomic>
#include <condition_variable>
#include <list>
#include <memory>
#include <thread>
#include <vector>
#include <string>

class AsyncLoader
{
public:
  static AsyncLoader& instance()
  {
    static AsyncLoader async_loader(2);
    return async_loader;
  }

  //! Ownership is _not_ transferred. Call ensure_deletable to ensure 
  //! that a previously enqueued object can be destroyed.
  void queue_for_load (AsyncObject*);
  
  void ensure_deletable (AsyncObject*);

  AsyncLoader(int numThreads);
  ~AsyncLoader();

  bool important_object_failed_loading() const { return _important_object_failed_loading; }
  std::vector<std::string> const& name_important_object_failed_loading() const { return _names_important_object_failed_loading; }
  void reset_object_fail() { _important_object_failed_loading = false; _names_important_object_failed_loading.clear(); }

private:
  void process();

  std::mutex _guard;
  std::condition_variable _state_changed;
  std::atomic<bool> _stop;
  std::array<std::list<AsyncObject*>, (size_t)async_priority::count> _to_load;
  std::list<AsyncObject*> _currently_loading;
  std::list<std::thread> _threads;
  bool _important_object_failed_loading = false;
  std::vector<std::string> _names_important_object_failed_loading;
};
