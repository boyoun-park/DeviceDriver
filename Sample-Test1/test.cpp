#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace testing;

class MockDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriverTest, readException) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	EXPECT_CALL(mock, read(0x1))
		.WillOnce(Return(0x1))
		.WillOnce(Return(0x2))
		.WillRepeatedly(Return(0x1));

	EXPECT_THROW(driver.read(0x1), ReadFailException);
}

TEST(DeviceDriverTest, normalRead) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	EXPECT_CALL(mock, read(0x1))
		.Times(5)
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(0xFF, Eq(driver.read(0x1)));
}

TEST(DeviceDriverTest, writeException) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	EXPECT_CALL(mock, read(0x1))
		.WillRepeatedly(Return(0x1));

	EXPECT_THROW(driver.write(0x1, 0x1), WriteFailException);
}

TEST(DeviceDriverTest, normalWrite) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	EXPECT_CALL(mock, read(0x1))
		.WillOnce(Return(0xFF))
		.WillRepeatedly(Return(0x1));

	driver.write(0x1, 0x1);
	EXPECT_THAT(0x1, Eq(driver.read(0x1)));
}