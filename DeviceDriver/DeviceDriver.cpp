#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
	int refValue = m_hardware->read(address);

	for (int tryRead = 0; tryRead < 4; tryRead++){
		int curValue = m_hardware->read(address);

		if (refValue != curValue) {
			throw(ReadFailException());
		}
	}
	return (int)(refValue);
}

void DeviceDriver::write(long address, int data)
{
	// read 한 값이 0xFF 가 아니면 exception
	int curValue = m_hardware->read(address);
	if (curValue != 0xFF) throw(WriteFailException());
	m_hardware->write(address, (unsigned char)data);
}