// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include <vespa/vespalib/testkit/testapp.h>
#include <vespa/vespalib/util/rusage.h>

using namespace vespalib;

class Test : public TestApp
{
public:
    int Main() override;
    void testRUsage();
};

int
Test::Main()
{
    TEST_INIT("rusage_test");
    testRUsage();
    TEST_DONE();
}

void
Test::testRUsage()
{
    RUsage r1;
    EXPECT_EQUAL("", r1.toString());
    RUsage r2;
    EXPECT_EQUAL(r2.toString(), r1.toString());
    RUsage diff = r2-r1;
    EXPECT_EQUAL(diff.toString(), r2.toString());
    {
        RUsage then = RUsage::createSelf(17765895674);
        RUsage now = RUsage::createSelf();
        EXPECT_NOT_EQUAL(now.toString(), then.toString());
    }
    {
        RUsage then = RUsage::createChildren(1337583);
        RUsage now = RUsage::createChildren();
        EXPECT_NOT_EQUAL(now.toString(), then.toString());
    }
    {
        timeval a, b, c, d, r;
        a.tv_usec = 7;
        a.tv_sec = 7;
        b.tv_usec = 7;
        b.tv_sec = 7;
        c.tv_usec = 1;
        c.tv_sec = 8;
        d.tv_usec = 9;
        d.tv_sec = 4;
        r = a - b;
        EXPECT_EQUAL(0, r.tv_sec);
        EXPECT_EQUAL(0, r.tv_usec);
        r = b - a;
        EXPECT_EQUAL(0, r.tv_sec);
        EXPECT_EQUAL(0, r.tv_usec);
        r = a - c;
        EXPECT_EQUAL(-1, r.tv_sec);
        EXPECT_EQUAL( 6, r.tv_usec);
        r = c - a;
        EXPECT_EQUAL(0, r.tv_sec);
        EXPECT_EQUAL(999994, r.tv_usec);
        r = a - d;
        EXPECT_EQUAL(2, r.tv_sec);
        EXPECT_EQUAL(999998, r.tv_usec);
        r = d - a;
        EXPECT_EQUAL(-3, r.tv_sec);
        EXPECT_EQUAL( 2, r.tv_usec);
    }
}

TEST_APPHOOK(Test)
