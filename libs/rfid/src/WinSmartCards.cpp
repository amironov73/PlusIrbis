// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "rfid.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "hicpp-signed-bitwise"

namespace irbis {

/// \brief Деструктор.
WinSmartCards::~WinSmartCards()
{
    this->disconnect();
    this->close();
}

/// \brief Подключение к менеджеру ресурсов.
/// \return `true` если успешно.
bool WinSmartCards::open()
{
#ifdef IRBIS_WINDOWS

    if (this->context) {
        return true;
    }

    LONG code = SCardEstablishContext
        (
            SCARD_SCOPE_USER,
            nullptr,
            nullptr,
            &this->context
        );
    return (code == SCARD_S_SUCCESS);

#else

    return false;

#endif
}

/// \brief Получение массива считывателей.
/// \return Массив считывателей.
std::vector<std::string> WinSmartCards::listReaders()
{
    std::vector<std::string> result;

#ifdef IRBIS_WINDOWS

    LPSTR pmszReaders = nullptr;
    LPSTR pReader;
    auto cch = SCARD_AUTOALLOCATE;

    LONG code = SCardListReadersA
        (
            this->context,
            nullptr,
            (LPSTR)&pmszReaders,
            &cch
        );
    if (code != SCARD_S_SUCCESS) {
        return result;
    }

    pReader = pmszReaders;
    while (*pReader != '\0') {
        result.emplace_back (pReader);
        pReader += (strlen (pReader) + 1);
    }

    SCardFreeMemory (this->context, pmszReaders);

#endif

    return result;
}

/// \brief Подключение к указанному считывателю.
/// \param name Имя считывателя, например `"OMNIKEY CardMan 5x21-CL 0"`.
/// \return Признак успешности.
bool WinSmartCards::connect (const std::string &name)
{
#ifdef IRBIS_WINDOWS

    if (this->reader) {
        this->disconnect();
    }

    LONG code = SCardConnectA
        (
           this->context,
           name.c_str(),
           SCARD_SHARE_SHARED,
           SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
           &this->reader,
           &this->protocol
        );
    return (code == SCARD_S_SUCCESS);

#else

    return false;

#endif
}

/// \brief Считывание идентификатора смарт-карты.
/// \return Идентификатор. Пусто при неудаче.
std::string WinSmartCards::readCard()
{
#ifdef IRBIS_WINDOWS

    // Get Data: CLA = 0xFF, INS = 0xCA, P1 = 0x00, P2 = 0x00, Le = 0x00
    BYTE baCmdApduGetData[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00 };
    BYTE baResponseApdu[300];
    DWORD lResponseApduLen = 300;
    LPCSCARD_IO_REQUEST  ioRequest;
    LONG lRetValue;

    switch (this->protocol)
    {
        case SCARD_PROTOCOL_T0:
            ioRequest = SCARD_PCI_T0;
            break;

        case SCARD_PROTOCOL_T1:
            ioRequest = SCARD_PCI_T1;
            break;

        default:
            ioRequest = SCARD_PCI_RAW;
            break;
    }

    std::string result;
    lRetValue = SCardTransmit
        (
            this->reader, // card handle
            ioRequest,  // pointer to the send protocol header
            baCmdApduGetData, // send buffer
            sizeof(baCmdApduGetData), // send buffer length
            nullptr, // pointer to the rec. protocol header
            baResponseApdu,	// receive buffer
            &lResponseApduLen // receive buffer length
        );
    if (lRetValue != SCARD_S_SUCCESS) {
        return result;
    }

    if( baResponseApdu[lResponseApduLen - 2] == 0x90 &&
        baResponseApdu[lResponseApduLen - 1] == 0x00) {
        const auto length = static_cast<int>(lResponseApduLen - 2);
        //result.reserve(2 * length);
        for (auto i = length - 1; i >= 0; i--) {
        //for (auto i = 0; i < length; i++) {
            char buf[4];
            std::sprintf (buf, "%02X", baResponseApdu[i]);
            result.append (buf);
        }
    }

    return result;

#else

    return std::string();

#endif
}

/// \brief Отключение от считывателя.
void WinSmartCards::disconnect ()
{
#ifdef IRBIS_WINDOWS

    if (this->reader) {
        SCardDisconnect (this->reader, SCARD_LEAVE_CARD);
        this->reader = 0;
    }

#endif
}

void WinSmartCards::close()
{
#ifdef IRBIS_WINDOWS

    if (this->context) {
        SCardReleaseContext (this->context);
        this->context = 0u;
    }

#endif
}

}
