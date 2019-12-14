# PlusIrbis

ManagedClient ported to plain C++14. Currently supported:

* 32-bit and 64-bit Windows;
* MSVS 2017/2019 Community Edition;
* 32-bit and 64-bit Linux;
* MacOS X 10.14;
* GCC 7 and higher;
* Clang

### Example

```c++
#include "irbis.h"
#include <iostream>

#pragma comment (lib, "irbis.lib")

int main()
{
    setlocale(LC_ALL, ""); // for cyrillic output

    irbis::Connection connection;
    connection.host = L"192.168.1.35";
    connection.port = 6666;
    connection.username = L"librarian";
    connection.password = L"secret";

    const auto isConnected = connection.connect();
    std::wcout << L"connected: " << isConnected << std::endl;
    if (!isConnected) {
        return -1;
    }

    const auto version = connection.getServerVersion();
    std::wcout << L"Version: " << version.version << std::endl;

    const auto maxMfn = connection.getMaxMfn(L"IBIS");
    std::wcout << L"max MFN: " << maxMfn << std::endl;

    const auto formatted = connection.formatRecord(L"@brief", 1);
    std::wcout << L"FORMAT: " << formatted << std::endl;

    const auto record = connection.readRawRecord(1);
    std::wcout << L"READ RAW:" << std::endl << record << std::endl;

    irbis::MfnList found = connection.search(L"K=бетон");
    std::wcout << L"SEARCH:";
    for (auto mfn : found) {
        std::wcout << L" " << std::to_wstring(mfn);
    }
    std::wcout << std::endl;

    connection.disconnect();
    std::wcout << L"disconnected" << std::endl;

    return 0;
}
```

### Links

- [IRBIS64 home page](http://www.elnit.org/index.php?option=com_content&view=article&id=35&Itemid=108) (russian);
- [IRBIS64 online documentation](http://sntnarciss.ru/irbis.html) (russian);
- [IRBIS64 wiki](http://wiki.elnit.org/index.php/%D0%92%D0%B8%D0%BA%D0%B8-%D0%B4%D0%BE%D0%BA%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D0%B0%D1%86%D0%B8%D1%8F_%D0%BF%D0%BE_%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%B5_%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA_%D0%98%D0%A0%D0%91%D0%98%D0%A1) (russian);
- [IRBIS64 support forum](http://irbis.gpntb.ru) (russian);

### Build status

[![Issues](https://img.shields.io/github/issues/amironov73/PlusIrbis.svg)](https://github.com/amironov73/PlusIrbis/issues)
[![Build status](https://img.shields.io/appveyor/ci/AlexeyMironov/plusirbis.svg)](https://ci.appveyor.com/project/AlexeyMironov/plusirbis/)
[![Build status](https://api.travis-ci.org/amironov73/PlusIrbis.svg)](https://travis-ci.org/amironov73/PlusIrbis/)