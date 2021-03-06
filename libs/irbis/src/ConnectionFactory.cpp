// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

/// \brief Получение нового подключения к серверу ИРБИС64.
/// \return Созданное подключение.
Connection* ConnectionFactory::GetConnection()
{
    return new Connection();
}

}
