/**
 * @file FastMathA.h
 * @brief Header file for class FastMathA
 * @date 05/ago/2015
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class FastMathA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATHA_H_
#define FASTMATHA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
//#include <math.h>
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace FastMath {

//128 elements
#define SIN_RESOLUTION 128
static float32 sinLookupTable[] = { 0.0, 0.012271538285719925, 0.024541228522912288, 0.03680722294135883, 0.049067674327418015, 0.06132073630220858, 0.07356456359966743, 0.0857973123444399, 0.0980171403295606, 0.11022220729388306, 0.1224106751992162, 0.13458070850712617, 0.14673047445536175,
        0.15885814333386145, 0.17096188876030124, 0.18303988795514098, 0.1950903220161283, 0.20711137619221862, 0.21910124015686988, 0.2310581082806712, 0.24298017990326398, 0.2548656596045147, 0.26671275747489853, 0.2785196893850532, 0.2902846772544625, 0.30200594931922825, 0.31368174039889163,
        0.3253102921622631, 0.3368898533922203, 0.3484186802494348, 0.3598950365349884, 0.37131719395183777, 0.38268343236509, 0.3939920400610484, 0.40524131400499014, 0.41642956009763743, 0.42755509343028236, 0.43861623853852794, 0.44961132965460693, 0.46053871095824034, 0.471396736825998,
        0.4821837720791231, 0.4928981922297843, 0.5035383837257178, 0.514102744193222, 0.5245896826784691, 0.5349976198870974, 0.5453249884220466, 0.5555702330196023, 0.5657318107836132, 0.5758081914178453, 0.5857978574564389, 0.5956993044924332, 0.6055110414043254, 0.6152315905806267,
        0.6248594881423862, 0.6343932841636453, 0.6438315428897913, 0.6531728429537765, 0.6624157775901715, 0.6715589548470181, 0.6806009977954527, 0.6895405447370666, 0.6983762494089725, 0.7071067811865471, 0.7157308252838183, 0.7242470829514664, 0.7326542716724124, 0.7409511253549586,
        0.7491363945234588, 0.757208846506484, 0.7651672656224584, 0.7730104533627363, 0.7807372285720938, 0.7883464276266057, 0.7958369046088829, 0.8032075314806443, 0.8104571982525941, 0.817584813151583, 0.8245893027850246, 0.8314696123025446, 0.8382247055548373, 0.8448535652497065,
        0.8513551931052644, 0.8577286100002713, 0.863972856121586, 0.8700869911087107, 0.8760700941954059, 0.8819212643483544, 0.8876396204028533, 0.8932243011955147, 0.8986744656939532, 0.9039892931234427, 0.9091679830905217, 0.9142097557035299, 0.919113851690057, 0.9238795325112861,
        0.9285060804732149, 0.9329927988347382, 0.9373390119125743, 0.9415440651830201, 0.9456073253805206, 0.949528180593036, 0.9533060403541932, 0.9569403357322083, 0.9604305194155652, 0.9637760657954393, 0.9669764710448515, 0.9700312531945434, 0.9729399522055596, 0.975702130038528,
        0.9783173707196271, 0.98078528040323, 0.9831054874312158, 0.9852776423889408, 0.987301418157858, 0.9891765099647806, 0.9909026354277797, 0.9924795345987096, 0.9939069700023557, 0.9951847266721966, 0.9963126121827778, 0.99729045667869, 0.9981181129001491, 0.9987954562051723,
        0.9993223845883494, 0.9996988186962041, 0.9999247018391445 };

inline float32 Sin(float32 angle) {
    const float32 PI = 3.14159265358979323846;
    uint32 k = (uint32)(angle / (2 * PI));
    float32 rem = angle - (k * (2 * PI));
    uint32 index;
    float32 ret = 0.0;
    if (rem < (0.5 * PI)) {
        index = (uint32)((rem / (0.5 * PI)) * SIN_RESOLUTION);
        ret = sinLookupTable[index];
    } else if ((rem >= (0.5 * PI)) && (rem < PI)) {
        float32 adj = PI - rem;
        index = (uint32)((adj / (0.5 * PI)) * SIN_RESOLUTION);
        ret = sinLookupTable[index];
    } else if ((rem >= PI) && (rem < (1.5 * PI))) {
        float32 adj = rem - PI;
        index = (uint32)((adj / (PI / 2)) * SIN_RESOLUTION);
        ret = -sinLookupTable[index];
    } else if ((rem >= (1.5 * PI)) && (rem < (2 * PI))) {
        float32 adj = (2 * PI) - rem;
        index = (uint32)((adj / (PI / 2)) * SIN_RESOLUTION);
        ret = -sinLookupTable[index];
    }
    return ret;
}

inline float32 Cos(float32 angle) {
    const float32 PI = 3.14159265358979323846;
    return sin(angle + (PI / 2));
}

inline int32 FloatToInt(float32 input) {
    return static_cast<int32>(input);
}

template<typename T1, typename T2> T1 UMulT(T1 x1, T1 x2, T1 &high) {
    const uint8 nBits = sizeof(T1) * 8;
    const T2 mask = static_cast<T2>(~((T1) 0));
    T2 ret = (T2) x1 * (T2) x2;
    high = (T1) (ret >> nBits);
    return (T1) (ret & mask);
}

inline uint8 CompleteMultiply(uint8 x1, uint8 x2, uint8 &high) {
    return UMulT<uint8, uint16>(x1, x2, high);
}

inline uint16 CompleteMultiply(uint16 x1, uint16 x2, uint16 &high) {
    return UMulT<uint16, uint32>(x1, x2, high);
}

inline uint32 CompleteMultiply(uint32 x1, uint32 x2, uint32 &high) {
    return UMulT<uint32, uint64>(x1, x2, high);
}

inline uint64 CompleteMultiply(uint64 x1, uint64 x2, uint64 &high) {
    return UMulT<uint64, uint64>(x1, x2, high);
}

inline int8 CompleteMultiply(int8 x1, int8 x2, int8 &high) {
    return UMulT<int8, int16>(x1, x2, high);
}

inline int16 CompleteMultiply(int16 x1, int16 x2, int16 &high) {
    return UMulT<int16, int32>(x1, x2, high);
}

inline int32 CompleteMultiply(int32 x1, int32 x2, int32 &high) {
    return UMulT<int32, int64>(x1, x2, high);
}

inline int64 CompleteMultiply(int64 x1, int64 x2, int64 &high) {
    return UMulT<int64, int64>(x1, x2, high);
}
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTMATHA_H_ */

