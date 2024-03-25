#ifndef COMMON_H
#define COMMON_H

#include <memory>

#define SZ_1K    (1024)
#define SZ_1M    (SZ_1K*SZ_1K)
#define SZ_1G    (SZ_1M*SZ_1M)

#define TM_1S    (1)
#define TM_1M    (TM_1S*60)
#define TM_1H    (TM_1M*60)

template <typename T>
using sp = std::shared_ptr<T>;

#endif // COMMON_H
