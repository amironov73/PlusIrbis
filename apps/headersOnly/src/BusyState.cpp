// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#include <cctype>
#include <limits>
#include <iterator>

/// \brief Индикатор состояния занятости.
struct BusyState final
{
    /// \brief Занято?
    /// \return true если да.
    bool busy() const noexcept { return this->_state; }

    /// \brief Отпускаем.
    void release() noexcept { this->_waitHandle.unlock(); this->_state = false; }

    /// \brief Запускаем некое длинное действие.
    /// \tparam Result
    /// \tparam F
    /// \tparam Args
    /// \param func
    /// \param args
    /// \returns
    template <class Result, class F, class ... Args>
    Result run (F&& func, Args&&... args)
    {
        try
        {
            this->wait();
            const auto result = F (args...);
            return result;
        }
        catch (const std::exception &ex)
        {
            this->release();
        }
    }

    /// \brief Запускаем некое длинное действие.
    /// \tparam F
    /// \tparam Args
    /// \param func
    /// \param args
    /// \returns
    template <class F, class ... Args>
    void run (F&& func, Args&&... args)
    {
        try
        {
            this->wait();
            F (args...);
        }
        catch (const std::exception &ex)
        {
            this->release();
        }
    }

    /// \brief Ожидаем и захватываем.
    void wait() { this->_waitHandle.lock(); this->_state = true; }

private:

    std::recursive_mutex _waitHandle {};
    bool volatile _state { false };
};
