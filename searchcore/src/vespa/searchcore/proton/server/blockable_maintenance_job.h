// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#pragma once

#include "i_blockable_maintenance_job.h"
#include "move_operation_limiter.h"
#include <mutex>
#include <unordered_set>

namespace proton {

class BlockableMaintenanceJobConfig;
class DiskMemUsageState;
class IMaintenanceJobRunner;

/**
 * Implementation of a maintenance job that can be blocked and unblocked due to various external reasons.
 * A blocked job is not executed by the IMaintenanceJobRunner wrapping the job.
 * When unblocked for a given reason, the job is scheduled for execution again if it is totally unblocked.
 */
class BlockableMaintenanceJob : public IBlockableMaintenanceJob {
private:
    using LockGuard = std::lock_guard<std::mutex>;
    using ReasonSet = std::unordered_set<BlockedReason>;

    mutable std::mutex _mutex;
    ReasonSet _blockReasons;
    bool _blocked;
    IMaintenanceJobRunner *_runner;
    double _resourceLimitFactor;

    void updateBlocked(const LockGuard &guard);

protected:
    MoveOperationLimiter::SP _moveOpsLimiter;

    void internalNotifyDiskMemUsage(const DiskMemUsageState &state);

public:
    BlockableMaintenanceJob(const vespalib::string &name,
                            double delay,
                            double interval);

    BlockableMaintenanceJob(const vespalib::string &name,
                            double delay,
                            double interval,
                            const BlockableMaintenanceJobConfig &config);

    virtual ~BlockableMaintenanceJob();

    bool isBlocked(BlockedReason reason);
    void considerRun();

    virtual void setBlocked(BlockedReason reason) override;
    virtual void unBlock(BlockedReason reason) override;
    virtual bool isBlocked() const override;
    virtual void registerRunner(IMaintenanceJobRunner *runner) override { _runner = runner; }

};

}
