//-*- C++ -*-
//-*- coding: utf-8 -*-
//
// Author: Bryan Riel
// Copyright 2017-2018

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <gtest/gtest.h>

// isce3::core
#include <isce3/core/DateTime.h>
#include <isce3/core/TimeDelta.h>

TEST(DateTimeTest, StandardConstruction) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592);
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 1);
    ASSERT_EQ(dtime.minutes, 12);
    ASSERT_EQ(dtime.seconds, 30);
    ASSERT_NEAR(dtime.frac, 0.141592, 1.0e-6);
}

TEST(DateTimeTest, NonStandardConstructionV1) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 62, 30.141592);
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 2);
    ASSERT_EQ(dtime.minutes, 2);
    ASSERT_EQ(dtime.seconds, 30);
    ASSERT_NEAR(dtime.frac, 0.141592, 1.0e-6);
}

TEST(DateTimeTest, NonStandardConstructionV2) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 62, 130.141592);
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 2);
    ASSERT_EQ(dtime.minutes, 4);
    ASSERT_EQ(dtime.seconds, 10);
    ASSERT_NEAR(dtime.frac, 0.141592, 1.0e-6);
}

TEST(DateTimeTest, FromString) {
    isce3::core::DateTime dtime("2017-05-12T01:12:30.141592");
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 1);
    ASSERT_EQ(dtime.minutes, 12);
    ASSERT_EQ(dtime.seconds, 30);
    ASSERT_NEAR(dtime.frac, 0.141592, 1.0e-6);
    // Test assignment
    isce3::core::DateTime dtime2;
    dtime2 = "2017-05-12T01:12:30.141592";
    ASSERT_EQ(dtime2.year, 2017);
    ASSERT_EQ(dtime2.months, 5);
    ASSERT_EQ(dtime2.days, 12);
    ASSERT_EQ(dtime2.hours, 1);
    ASSERT_EQ(dtime2.minutes, 12);
    ASSERT_EQ(dtime2.seconds, 30);
    ASSERT_NEAR(dtime2.frac, 0.141592, 1.0e-6);
}

TEST(DateTimeTest, ToString) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592);
    ASSERT_EQ(dtime.isoformat(), "2017-05-12T01:12:30.141592000");
}

TEST(DateTimeTest, BasicTimeDelta) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592);
    dtime += 25.0;
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 1);
    ASSERT_EQ(dtime.minutes, 12);
    ASSERT_EQ(dtime.seconds, 55);
    ASSERT_NEAR(dtime.frac, 0.141592, 1.0e-6);
}

TEST(DateTimeTest, TimeDeltaSub) {
    isce3::core::DateTime dtime1(2017, 5, 12, 1, 12, 30.141592);
    isce3::core::DateTime dtime2(2017, 5, 13, 2, 12, 33.241592);
    isce3::core::TimeDelta dt = dtime2 - dtime1;
    ASSERT_EQ(dt.days, 1);
    ASSERT_EQ(dt.hours, 1);
    ASSERT_EQ(dt.minutes, 0);
    ASSERT_EQ(dt.seconds, 3);
    ASSERT_NEAR(dt.frac, 0.1, 1.0e-6);
    ASSERT_NEAR(dt.getTotalSeconds(), 90003.1, 1.0e-8);
}

TEST(DateTimeTest, TimeDeltaAdd) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592);
    isce3::core::TimeDelta dt(1, 12, 5.5);
    dtime += dt;
    ASSERT_EQ(dtime.year, 2017);
    ASSERT_EQ(dtime.months, 5);
    ASSERT_EQ(dtime.days, 12);
    ASSERT_EQ(dtime.hours, 2);
    ASSERT_EQ(dtime.minutes, 24);
    ASSERT_EQ(dtime.seconds, 35);
    ASSERT_NEAR(dtime.frac, 0.641592, 1.0e-6);
}

TEST(DateTimeTest, Epoch) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592);
    ASSERT_NEAR(dtime.secondsSinceEpoch(), 1494551550.141592, 1.0e-6);
    dtime.secondsSinceEpoch(1493626353.141592026);
    ASSERT_EQ(dtime.isoformat(), "2017-05-01T08:12:33.141592026");
}

TEST(DateTimeTest, TimeDeltaAssign) {
    isce3::core::DateTime dtime(2017, 5, 12, 1, 12, 30.141592026);
    isce3::core::TimeDelta dt;
    dt = 3.5;
    dtime += dt;
    ASSERT_EQ(dtime.isoformat(), "2017-05-12T01:12:33.641592026");
}

TEST(DateTimeTest, Comparison) {
    isce3::core::DateTime dtime1(2017, 5, 12, 1, 12, 30.141592);
    isce3::core::DateTime dtime2(2017, 5, 12, 1, 12, 30.141592);
    isce3::core::DateTime dtime3(2017, 5, 13, 2, 12, 33.241592);
    ASSERT_TRUE(dtime1 == dtime2);
    ASSERT_TRUE(dtime1 != dtime3);
    ASSERT_TRUE(dtime3 > dtime2);
    ASSERT_TRUE(dtime1 <= dtime2);
    ASSERT_TRUE(dtime1 <= dtime3);
}

TEST(DateTimeTest, IsClose) {
    isce3::core::DateTime dtime1(2017, 5, 12, 1, 12, 30.141592);
    isce3::core::DateTime dtime2 = dtime1 + isce3::core::TimeDelta(1e-11);
    ASSERT_TRUE(dtime1.isClose(dtime2));
    isce3::core::TimeDelta errtol(1e-12);
    ASSERT_FALSE(dtime1.isClose(dtime2, errtol));
}

TEST(DateTimeTest, TimeDeltaComparison) {
    isce3::core::TimeDelta dt1(0.5);
    isce3::core::TimeDelta dt2(0.5);
    isce3::core::TimeDelta dt3(-0.5);
    ASSERT_TRUE(dt1 == dt2);
    ASSERT_TRUE(dt1 != dt3);
    ASSERT_TRUE(dt3 < dt2);
    ASSERT_TRUE(dt1 >= dt2);
    ASSERT_TRUE(dt1 >= dt3);
}

TEST(DateTimeTest, Normalize)
{
    isce3::core::DateTime t0(1999, 12, 31, 23, 59, 59);
    isce3::core::TimeDelta dt = 1.0;
    isce3::core::DateTime t1 = t0 + dt;

    isce3::core::DateTime expected(2000, 1, 1);

    EXPECT_TRUE( t1.isClose(expected) );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// end of file
