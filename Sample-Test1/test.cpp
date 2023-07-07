#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"
#include "../DeviceDriver/App.cpp"

using namespace testing;

class MockDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriverTest, readException) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	const long dummyAddress = 0x1;
	const unsigned char correctDummyData = 0x1;
	const unsigned char wrongDummyData = 0x2;

	EXPECT_CALL(mock, read(dummyAddress))
		.WillOnce(Return(correctDummyData))
		.WillOnce(Return(wrongDummyData))
		.WillRepeatedly(Return(correctDummyData));

	EXPECT_THROW(driver.read(dummyAddress), ReadFailException);
}

TEST(DeviceDriverTest, normalRead) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	const int readTimesinDriver = 5;
	const long dummyAddress = 0x1;
	const unsigned char dummyData = 0x2;

	EXPECT_CALL(mock, read(dummyAddress))
		.Times(readTimesinDriver)
		.WillRepeatedly(Return(dummyData));

	EXPECT_THAT(dummyData, Eq(driver.read(dummyAddress)));
}

TEST(DeviceDriverTest, writeException) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	const long dummyAddress = 0x1;
	const unsigned char dummyData = 0x2;

	EXPECT_CALL(mock, read(dummyAddress))
		.WillRepeatedly(Return(dummyData));

	EXPECT_THROW(driver.write(dummyAddress, dummyData), WriteFailException);
}

TEST(DeviceDriverTest, normalWrite) {
	MockDevice mock;
	DeviceDriver driver(&mock);

	const long dummyAddress = 0x1;
	const unsigned char emptyData = 0xFF;
	const unsigned char dummyData = 0x2;

	EXPECT_CALL(mock, read(dummyAddress))
		.WillOnce(Return(emptyData))
		.WillRepeatedly(Return(dummyData));

	driver.write(dummyAddress, dummyData);
	EXPECT_THAT(dummyData, Eq(driver.read(dummyAddress)));
}

TEST(AppTest, readMultipleAddr) {
	MockDevice mock;
	DeviceDriver driver(&mock);
	App app(&driver);

	const long addressOffset = 5;
	const int readTimesinDriver = 5;
	const long dummyAddress = 0x1;
	const long dummyData = 0x2;

	EXPECT_CALL(mock, read(_))
		.Times(addressOffset * readTimesinDriver)
		.WillRepeatedly(Return(dummyData));

	app.ReadAndPrint(dummyAddress, dummyAddress + addressOffset - 1);
}

TEST(AppTest, writeMultipleAddr) {
	MockDevice mock;
	DeviceDriver driver(&mock);
	App app(&driver);

	const long addressOffset = 5;
	const long targetAddress = 0x0;
	const unsigned char emptyData = 0xFF;
	const unsigned char dummyData = 0x2;

	EXPECT_CALL(mock, read(_))
		.WillOnce(Return(emptyData))
		.WillOnce(Return(emptyData))
		.WillOnce(Return(emptyData))
		.WillOnce(Return(emptyData))
		.WillOnce(Return(emptyData))
		.WillRepeatedly(Return(dummyData));

	for (long address = targetAddress; address < targetAddress + addressOffset; address++) {
		EXPECT_CALL(mock, write(address, dummyData))
			.Times(1);
	}

	app.WriteAll(dummyData);

	for (long address = targetAddress; address < targetAddress + addressOffset; address++) {
		EXPECT_THAT(dummyData, Eq(driver.read(address)));
	}
}

