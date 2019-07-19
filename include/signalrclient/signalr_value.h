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

        signalr_value();

        signalr_value(signalr_value&& rhs);

        signalr_value& operator=(signalr_value&& rhs) noexcept;

        signalr_value operator[](const std::string& name) const;
        signalr_value operator[](int index) const;

        std::string getString() const;
        int getInt() const;
        float getFloat() const;

        bool hasMember(const std::string& name) const;
        bool isArray() const;
        bool isString() const;
        bool isNumber() const;
    private:
        friend signalr_value_impl;
        signalr_value_impl* m_impl;

        signalr_value(signalr_value_impl* impl)
            : m_impl(impl)
        { }
    };
}