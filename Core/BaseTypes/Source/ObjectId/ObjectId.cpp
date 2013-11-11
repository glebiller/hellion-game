/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <boost/functional/hash/hash.hpp>  // for hash_combine
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/random/uniform_int.hpp>  // for uniform_int
#include <limits>                       // for numeric_limits

#include "Assert.h"                     // for ASSERT
#include "Date_t.h"                     // for Date_t
#include "Hex.h"                        // for fromHex
#include "ObjectId.h"

//BOOST_STATIC_ASSERT( sizeof(ObjectId) == ObjectId::kOIdSize );
//BOOST_STATIC_ASSERT( sizeof(ObjectId) == 12 );

void ObjectId::hash_combine(size_t &seed) const {
    boost::hash_combine(seed, x);
    boost::hash_combine(seed, y);
    boost::hash_combine(seed, z);
}

// machine # before folding in the process id
ObjectId::MachineAndPid ObjectId::ourMachine;
boost::mt19937_64 ObjectId::randomGenerator(std::time(0));

unsigned ObjectId::ourPid() {
    return boost::interprocess::ipcdetail::get_current_process_id();
}

void ObjectId::foldInPid(ObjectId::MachineAndPid& x) {
    unsigned p = ourPid();
    x._pid ^= (unsigned short) p;
    // when the pid is greater than 16 bits, let the high bits modulate the machine id field.
    unsigned short& rest = (unsigned short &) x._machineNumber[1];
    rest ^= p >> 16;
}

ObjectId::MachineAndPid ObjectId::genMachineAndPid() {
    //BOOST_STATIC_ASSERT( sizeof(ObjectId::MachineAndPid) == 5 );

    // we only call this once per process
    boost::uniform_int<u64> distrubution(0, std::numeric_limits<u64>::max());
    u64 n = distrubution(randomGenerator);
    ObjectId::MachineAndPid x = ourMachine = reinterpret_cast<ObjectId::MachineAndPid&>(n);
    foldInPid(x);
    return x;
}

// after folding in the process id
ObjectId::MachineAndPid ObjectId::ourMachineAndPid = ObjectId::genMachineAndPid();

void ObjectId::regenMachineId() {
    ourMachineAndPid = genMachineAndPid();
}

inline bool ObjectId::MachineAndPid::operator!=(const ObjectId::MachineAndPid& rhs) const {
    return _pid != rhs._pid || _machineNumber != rhs._machineNumber;
}

unsigned ObjectId::getMachineId() {
    unsigned char x[4];
    x[0] = ourMachineAndPid._machineNumber[0];
    x[1] = ourMachineAndPid._machineNumber[1];
    x[2] = ourMachineAndPid._machineNumber[2];
    x[3] = 0;
    return (unsigned&) x[0];
}

void ObjectId::justForked() {
    MachineAndPid x = ourMachine;
    // we let the random # for machine go into all 5 bytes of MachineAndPid, and then
    // xor in the pid into _pid.  this reduces the probability of collisions.
    foldInPid(x);
    ourMachineAndPid = genMachineAndPid();
    ASSERT( x != ourMachineAndPid );
    ourMachineAndPid = x;
}

void ObjectId::init() {
    boost::uniform_int<u64> distrubution(0, std::numeric_limits<u64>::max());
    u64 inc = distrubution(randomGenerator);

    {
        unsigned t = (unsigned) time(0);
        unsigned char *T = (unsigned char *) &t;
        _time[0] = T[3]; // big endian order because we use memcmp() to compare OID's
        _time[1] = T[2];
        _time[2] = T[1];
        _time[3] = T[0];
    }

    _machineAndPid = ourMachineAndPid;

    {
        u64 new_inc = inc++;
        unsigned char *T = (unsigned char *) &new_inc;
        _inc[0] = T[2];
        _inc[1] = T[1];
        _inc[2] = T[0];
    }
}

/*static AtomicUInt64 _initSequential_sequence;
void ObjectId::initSequential() {

    {
        unsigned t = (unsigned) time(0);
        unsigned char *T = (unsigned char *) &t;
        _time[0] = T[3]; // big endian order because we use memcmp() to compare OID's
        _time[1] = T[2];
        _time[2] = T[1];
        _time[3] = T[0];
    }
        
    {
        unsigned long long nextNumber = _initSequential_sequence.fetchAndAdd(1);
        unsigned char* numberData = reinterpret_cast<unsigned char*>(&nextNumber);
        for ( int i=0; i<8; i++ ) {
            data[4+i] = numberData[7-i];
        }
    }
}*/

void ObjectId::init( const std::string& s ) {
    ASSERT( s.size() == 24 );
    const char *p = s.c_str();
    for( size_t i = 0; i < kOIdSize; i++ ) {
        data[i] = fromHex(p);
        p += 2;
    }
}

void ObjectId::init(Date_t date, bool max) {
    int time = (int) (date / 1000);
    char* T = (char *) &time;
    data[0] = T[3];
    data[1] = T[2];
    data[2] = T[1];
    data[3] = T[0];

    if (max)
        *(long long*)(data + 4) = 0xFFFFFFFFFFFFFFFFll;
    else
        *(long long*)(data + 4) = 0x0000000000000000ll;
}

time_t ObjectId::asTimeT() {
    int time;
    char* T = (char *) &time;
    T[0] = data[3];
    T[1] = data[2];
    T[2] = data[1];
    T[3] = data[0];
    return time;
}

std::ostream& operator<<( std::ostream &s, const ObjectId &o ) {
    s << o.str();
    return s;
}
