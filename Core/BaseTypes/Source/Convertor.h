#pragma once

#include <sstream>

#include "DataTypes.h"

class Convertor {
    public:

        //
        // Use template maybe ?
        //

        inline static i32 StringToInt(std::string s) {
            std::istringstream iss(s);
            i32 x;

            if (!(iss >> x)) {
                return 0;
            }

            return x;
        }

        inline static f32 StringToFloat(std::string s) {
            std::istringstream iss(s);
            f32 x;

            if (!(iss >> x)) {
                return 0;
            }

            return x;
        };
};
