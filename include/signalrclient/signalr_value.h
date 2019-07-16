// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include <string>

namespace signalr
{
    class signalr_value_impl;

    class signalr_value
    {
    public:
        static signalr_value array();
        static signalr_value value();

        signalr_value()
            : m_impl(nullptr)
        { }

        signalr_value(signalr_value&& rhs)
            : m_impl(std::move(rhs.m_impl))
        { }

        signalr_value& operator=(signalr_value&& rhs)
        {
            m_impl = rhs.m_impl;
            rhs.m_impl = nullptr;

            return *this;
        }

        signalr_value operator[](const std::string& name) const;

        std::string getString() const;

        bool hasMember(const std::string& name) const;
        bool isArray() const;
        bool isString() const;
    private:
        friend signalr_value_impl;
        signalr_value_impl* m_impl;

        signalr_value(signalr_value_impl* impl)
            : m_impl(impl)
        { }
    };
}