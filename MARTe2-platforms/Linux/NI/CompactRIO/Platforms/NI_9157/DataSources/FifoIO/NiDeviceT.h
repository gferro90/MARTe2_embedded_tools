/*
 * NiDeviceT.h
 *
 *  Created on: Feb 20, 2018
 *      Author: codac-dev
 */

#ifndef NIDEVICET_H_
#define NIDEVICET_H_

#include "NiDevice.h"
#include "ReferenceT.h"

class NiDeviceTGen {
public:
	NiDeviceTGen();
	virtual ~NiDeviceTGen();
	virtual NiFpga_Status NiRead(uint32 indicator, void *value)=0;

	virtual NiFpga_Status NiWrite(uint32 control, void *value)=0;

	virtual NiFpga_Status NiReadFifo(uint32 fifo, void* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining)=0;

	virtual NiFpga_Status NiWriteFifo(uint32 fifo, const void* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining)=0;

	virtual ReferenceT<NiDevice> GetNiDevice()=0;

	virtual uint32 GetByteSize()=0;

	virtual int32 Compare(uint8* a, uint8* b)=0;

	virtual void Copy(uint8* dest, uint8* source)=0;
};

//TODO template version of NiDevice
template<typename T>
class NiDeviceT: public NiDeviceTGen {
public:
	NiDeviceT(ReferenceT<NiDevice> niDev);
	virtual ~NiDeviceT();

	virtual NiFpga_Status NiRead(uint32 indicator, void *value);

	virtual NiFpga_Status NiWrite(uint32 control, void *value);

	virtual NiFpga_Status NiReadFifo(uint32 fifo, void* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	virtual NiFpga_Status NiWriteFifo(uint32 fifo, const void* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	virtual ReferenceT<NiDevice> GetNiDevice();

	virtual uint32 GetByteSize();

	virtual int32 Compare(uint8* a, uint8* b);

	virtual void Copy(uint8* dest, uint8* source);
private:
	ReferenceT<NiDevice> niDevice;
};

template<typename T>
NiDeviceT<T>::NiDeviceT(ReferenceT<NiDevice> niDev) {
	niDevice = niDev;
}

template<typename T>
NiDeviceT<T>::~NiDeviceT() {

}

template<typename T>
NiFpga_Status NiDeviceT<T>::NiRead(uint32 indicator, void *value) {
	return niDevice->NiRead(indicator, *(T*) (value));
}

template<typename T>
NiFpga_Status NiDeviceT<T>::NiWrite(uint32 indicator, void *value) {
	return niDevice->NiWrite(indicator, *(T*) value);
}

template<typename T>
NiFpga_Status NiDeviceT<T>::NiReadFifo(uint32 fifo, void* data,
		uint32 numberOfElements, uint32 timeout, uint32 &elementsRemaining) {
	return niDevice->NiReadFifo(fifo, (T*) data, numberOfElements, timeout,
			elementsRemaining);
}

template<typename T>
NiFpga_Status NiDeviceT<T>::NiWriteFifo(uint32 fifo, const void* data,
		uint32 numberOfElements, uint32 timeout,
		uint32 &emptyElementsRemaining) {
	return niDevice->NiWriteFifo(fifo, (const T*) data, numberOfElements,
			timeout, emptyElementsRemaining);
}

template<typename T>
ReferenceT<NiDevice> NiDeviceT<T>::GetNiDevice() {
	return niDevice;
}


template<typename T>
uint32 NiDeviceT<T>::GetByteSize(){
	return sizeof(T);
}


template<typename T>
int32 NiDeviceT<T>::Compare(uint8* a, uint8* b){
	T x=*(T*)a;
	T y=*(T*)b;
	if(x==y){
		return 0;
	}
	if(x>y){
		return 1;
	}
	if(x<y){
		return -1;
	}
	return -2;

}

template<typename T>
void NiDeviceT<T>::Copy(uint8* dest, uint8* source){
	*(T*)dest=*(T*)source;
}



class CreateNiDevice {
public:
	CreateNiDevice();
	virtual ~CreateNiDevice();
	virtual NiDeviceTGen *Create(ReferenceT<NiDevice> niDev)=0;
	virtual const char8 *GetIdentifier()=0;
};

template<typename T>
class CreateNiDeviceT: public CreateNiDevice {
public:
	CreateNiDeviceT(const char8 *id);
	virtual ~CreateNiDeviceT();
	virtual NiDeviceTGen *Create(ReferenceT<NiDevice> niDev);
	virtual const char8 *GetIdentifier();
private:
	const char8 *identifier;
};

template<typename T>
CreateNiDeviceT<T>::CreateNiDeviceT(const char8 *id) {
	identifier = id;
}

template<typename T>
CreateNiDeviceT<T>::~CreateNiDeviceT() {
}

template<typename T>
NiDeviceTGen* CreateNiDeviceT<T>::Create(ReferenceT<NiDevice> niDev) {
	NiDeviceTGen *newDev = new NiDeviceT<T>(niDev);
	return newDev;
}

template<typename T>
const char8 * CreateNiDeviceT<T>::GetIdentifier(){
	return identifier;
}


CreateNiDevice *GetNiDevice(const char8 *niDevId);



#endif /* NIDEVICET_H_ */
