#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
	int prevValue = m_hardware->read(address);
	int nextValue;
	for (int tryRead = 0; tryRead < 4; tryRead++){
		nextValue = m_hardware->read(address);
		if (prevValue != nextValue) throw(ReadFailException());
	}
	return (int)(prevValue);
}

void DeviceDriver::write(long address, int data)
{
	// TODO: implement this method
	m_hardware->write(address, (unsigned char)data);
}