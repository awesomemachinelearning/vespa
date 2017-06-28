// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include "blockable_maintenance_job.h"
#include "disk_mem_usage_state.h"
#include "imaintenancejobrunner.h"

namespace proton {

void
BlockableMaintenanceJob::updateBlocked(const LockGuard &)
{
    _blocked = !_blockReasons.empty();
}

void
BlockableMaintenanceJob::internalNotifyDiskMemUsage(const DiskMemUsageState &state)
{
    bool resourcesOK = !state.aboveDiskLimit(_resourceLimitFactor) && !state.aboveMemoryLimit(_resourceLimitFactor);
    if (resourcesOK) {
        if (isBlocked(BlockedReason::RESOURCE_LIMITS)) {
            unBlock(BlockedReason::RESOURCE_LIMITS);
        }
    } else {
        setBlocked(BlockedReason::RESOURCE_LIMITS);
    }
}

BlockableMaintenanceJob::BlockableMaintenanceJob(const vespalib::string &name,
                                                 double delay,
                                                 double interval)
    : BlockableMaintenanceJob(name, delay, interval, 1.0)
{
}

BlockableMaintenanceJob::BlockableMaintenanceJob(const vespalib::string &name,
                                                 double delay,
                                                 double interval,
                                                 double resourceLimitFactor)
    : IBlockableMaintenanceJob(name, delay, interval),
      _mutex(),
      _blockReasons(),
      _blocked(false),
      _runner(nullptr),
      _resourceLimitFactor(resourceLimitFactor)
{
}

BlockableMaintenanceJob::~BlockableMaintenanceJob()
{
}

bool
BlockableMaintenanceJob::isBlocked(BlockedReason reason)
{
    LockGuard guard(_mutex);
    return (_blockReasons.find(reason) != _blockReasons.end());
}

void
BlockableMaintenanceJob::considerRun()
{
    if (_runner && !isBlocked()) {
        _runner->run();
    }
}

void
BlockableMaintenanceJob::setBlocked(BlockedReason reason)
{
    LockGuard guard(_mutex);
    _blockReasons.insert(reason);
    updateBlocked(guard);
}

void
BlockableMaintenanceJob::unBlock(BlockedReason reason)
{
    bool considerRun = false;
    {
        LockGuard guard(_mutex);
        _blockReasons.erase(reason);
        updateBlocked(guard);
        considerRun = !_blocked;
    }
    if (_runner && considerRun) {
        _runner->run();
    }
}

bool
BlockableMaintenanceJob::isBlocked() const
{
    LockGuard guard(_mutex);
    return _blocked;
}

}
