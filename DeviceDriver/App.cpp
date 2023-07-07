#include <iostream>
#include "../DeviceDriver/DeviceDriver.h"

using namespace std;

class App {
public:
	explicit App(DeviceDriver* driver)
		: driver(driver) {
	}

	void ReadAndPrint(long startAddr, long endAddr) {
		for (long address = startAddr; address <= endAddr; address++) {
			cout << driver->read(address) << " ";
		}
		cout << "\n";
	}
	void WriteAll(unsigned char value) {
		for (long address = minAddr; address <= maxAddr; address++) {
			driver->write(address, value);
		}
	}
private:
	DeviceDriver* driver;
	const long minAddr = 0x0;
	const long maxAddr = 0x4;
};