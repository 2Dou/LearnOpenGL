//  Created by SelFree on 2019/2/21.
//  Copyright © 2018年 SelFree. All rights reserved.

#include "src/util/util.h"

namespace util {

std::string GetResourceFilename(const std::string &name) {
    return std::string("./Resources/") + name;
}

}  // namespace util
