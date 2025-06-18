/*
 * ErrorFunction.cpp
 *
 *  Created on: 26/ago/2016
 *      Author: pc
 */

#include "StreamString.h"
#include "StreamMemoryReference.h"
#include "task.h"
#include "MemoryOperationsHelper.h"
#include "ErrorInformation.h"
#ifdef ERROR_ON_USB
#include "usbd_cdc_custom.h"
#endif
#include QUOTE(_HAL_H)

//#define ERROR_ON_USB

using namespace MARTe;

extern UART_HandleTypeDef *errorUartHandle;

#define ERROR_BUF_SIZE 512

static char8 buffer[ERROR_BUF_SIZE] = { 0 };
static char8 buffer2[ERROR_BUF_SIZE] = { 0 };

const struct {
    const char8 * const name;
    ErrorManagement::ErrorIntegerFormat errorBitSet;
} errorNames[] = {
        { "NoError", ErrorManagement::NoError },
        { "Debug", ErrorManagement::Debug },
        { "Information", ErrorManagement::Information },
        { "Warning", ErrorManagement::Warning },
        { "FatalError", ErrorManagement::FatalError },
        { "RecoverableError", ErrorManagement::RecoverableError },
        { "InitialisationError", ErrorManagement::InitialisationError },
        { "OSError", ErrorManagement::OSError },
        { "ParametersError", ErrorManagement::ParametersError },
        { "IllegalOperation", ErrorManagement::IllegalOperation },
        { "ErrorSharing", ErrorManagement::ErrorSharing },
        { "ErrorAccessDenied", ErrorManagement::ErrorAccessDenied },
        { "Exception", ErrorManagement::Exception },
        { "Timeout", ErrorManagement::Timeout },
        { "CommunicationError", ErrorManagement::CommunicationError },
        { "SyntaxError", ErrorManagement::SyntaxError },
        { "UnsupportedFeature", ErrorManagement::UnsupportedFeature },
        { "InternalSetupError", ErrorManagement::InternalSetupError },
        { "Completed", ErrorManagement::Completed },
        { "NotCompleted", ErrorManagement::NotCompleted },
        { NULL, ErrorManagement::NoError }, };


static const char8* ErrorCodeToStream(const ErrorManagement::ErrorType &errorCode) {
    //Skip the NoError
    uint32 i = 1u;
    bool firstErrorWritten = false;
    bool ok = true;
    while (ok && (errorNames[i].name != NULL)) {
        if (errorCode.Contains(errorNames[i].errorBitSet)) {
            return errorNames[i].name;
        }
        i++;
    }
    return "NoError";
}

#ifdef ERROR_ON_USB

void PrintStack(ThreadIdentifier &tid) {
    while (1) {
        MemoryOperationsHelper::Set(buffer2, 0, ERROR_BUF_SIZE);

        StreamMemoryReference buffer2_stream(buffer2, ERROR_BUF_SIZE);

        buffer2_stream.Seek(0);
        uint32 stack_remained = uxTaskGetStackHighWaterMark(tid);

//allErrors.Printf(" File %s Function %s Line %d", errorInfo.fileName, errorInfo.functionName, errorInfo.header.lineNumber);
        buffer2_stream.Printf("Stack remained %d\n\r", stack_remained);
        uint32_t size = buffer2_stream.Size() + 1;
        if (!USBInitialized()) {
            USBOpen();
        }
        USBWrite(buffer2_stream.Buffer(), &size, 0);
        Sleep::Sec(20.);
    }
}

void PrintF(const char * const message) {
//  StackType_t *stackTop=(StackType_t*) xTaskGetCurrentTaskHandle();
    MemoryOperationsHelper::Set(buffer, 0, ERROR_BUF_SIZE);
    StreamMemoryReference buffer_stream(buffer, ERROR_BUF_SIZE);

    buffer_stream.Seek(0);
    buffer_stream.Printf("%s\n\r", message);

    uint32_t size = buffer_stream.Size() + 1;
    if (!USBInitialized()) {
        USBOpen();
    }
    USBWrite(buffer, &size, 0);

}

void DebugErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                               const char * const errorDescription) {
//StackType_t *stackTop=(StackType_t*) xTaskGetCurrentTaskHandle();
    MemoryOperationsHelper::Set(buffer, 0, ERROR_BUF_SIZE);
    StreamMemoryReference buffer_stream(buffer, ERROR_BUF_SIZE);

    buffer_stream.Seek(0);
    buffer_stream.Printf("%s: [%s-%d] : ", ErrorCodeToStream(errorInfo.header.errorType), errorInfo.fileName, errorInfo.header.lineNumber);
    buffer_stream.Printf("%s\n\r", errorDescription);

    uint32_t size = buffer_stream.Size() + 1;
    if (!USBInitialized()) {
        USBOpen();
    }
    USBWrite(buffer, &size, 0);
}

#else

void PrintStack(ThreadIdentifier &tid) {
    while (1) {
        MemoryOperationsHelper::Set(buffer2, 0, ERROR_BUF_SIZE);

        StreamMemoryReference buffer2_stream(buffer2, ERROR_BUF_SIZE);

        buffer2_stream.Seek(0);
        uint32 stack_remained = uxTaskGetStackHighWaterMark(tid);

        //allErrors.Printf(" File %s Function %s Line %d", errorInfo.fileName, errorInfo.functionName, errorInfo.header.lineNumber);
        buffer2_stream.Printf("Stack remained %d\n\r", stack_remained);
        uint32_t size = buffer2_stream.Size() + 1;
        HAL_UART_Transmit(errorUartHandle, (uint8_t*) buffer, size, HAL_MAX_DELAY);

        Sleep::Sec(20.);
    }
}

void PrintF(const char *const message) {
//  StackType_t *stackTop=(StackType_t*) xTaskGetCurrentTaskHandle();
    MemoryOperationsHelper::Set(buffer, 0, ERROR_BUF_SIZE);
    StreamMemoryReference buffer_stream(buffer, ERROR_BUF_SIZE);
    buffer_stream.Seek(0);
    buffer_stream.Printf("%s\n\r", message);

    uint32_t size = buffer_stream.Size() + 1;
    HAL_UART_Transmit(errorUartHandle, (uint8_t*) buffer, size, HAL_MAX_DELAY);

}

void DebugErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char *const errorDescription) {
//	StackType_t *stackTop=(StackType_t*) xTaskGetCurrentTaskHandle();
    MemoryOperationsHelper::Set(buffer, 0, ERROR_BUF_SIZE);
    StreamMemoryReference buffer_stream(buffer, ERROR_BUF_SIZE);
    buffer_stream.Seek(0);
    buffer_stream.Printf("%s: [%s:%d] : ", ErrorCodeToStream(errorInfo.header.errorType), errorInfo.fileName, errorInfo.header.lineNumber);
    buffer_stream.Printf("%s\n\r", errorDescription);
    uint32_t size = buffer_stream.Size() + 1;
    HAL_UART_Transmit(errorUartHandle, (uint8_t*) buffer, size, HAL_MAX_DELAY);

}
#endif

