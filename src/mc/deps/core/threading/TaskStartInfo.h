#pragma once

#include "mc/_HeaderOutputPredefine.h"

struct TaskStartInfo {
public:
    // member variables
    // NOLINTBEGIN
    ::std::string_view                         name;
    ::std::thread::id                          affinity;
    unsigned int                               priority;
    int                                        priorityBackDown;
    bool                                       mLinkCancellation;
    ::std::chrono::steady_clock::time_point    startAtTime;
    // NOLINTEND

public:
    // static variables
    // NOLINTBEGIN
    MCNAPI static ::std::thread::id const& NoAffinity();
    // NOLINTEND
};