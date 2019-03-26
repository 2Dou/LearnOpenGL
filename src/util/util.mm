//  Created by SelFree on 2019/2/21.
//  Copyright © 2018年 SelFree. All rights reserved.

#include "src/util/util.h"
#include <string>

#import <Foundation/Foundation.h>
#import <Foundation/NSString.h>

namespace util {

std::string GetResourceFilename(const std::string &name) {
    NSString *res = @"Resources/";
    res = [res stringByAppendingString: [NSString stringWithUTF8String: name.c_str()]] ;
    auto s = [[NSBundle mainBundle]
              pathForResource: res
              ofType: nil];
    if (s != nil) {
        return [s UTF8String];
    }

    return "";
}

}  // namespace util
