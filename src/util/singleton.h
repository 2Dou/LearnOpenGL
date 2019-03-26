//  Created by SelFree on 2019/2/20.
//  Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_UTIL_SINGLETON_H_
#define SRC_UTIL_SINGLETON_H_

template <typename T>
class Singleton {
 public:
    static T* Instance() {
        static T instance;
        return &instance;
    }

    void operator = (Singleton<T> const&) = delete;
    Singleton<T>(Singleton<T> const&) = delete;

    virtual ~Singleton<T>() {}

 protected:
    Singleton<T>() {}
};

#endif  // SRC_UTIL_SINGLETON_H_
