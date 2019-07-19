// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "signalr_value_impl.h"
#include "signalrclient/signalr_value.h"

namespace signalr
{
    signalr_value::signalr_value()
        : m_impl(nullptr)
    { }

    signalr_value::signalr_value(signalr_value&& rhs)
        : m_impl(std::move(rhs.m_impl))
    { }

    signalr_value& signalr_value::operator=(signalr_value&& rhs) noexcept
    {
        m_impl = rhs.m_impl;
        rhs.m_impl = nullptr;

        return *this;
    }

    signalr_value signalr_value::array()
    {
        return signalr_value_impl::create(Json::Value(Json::ValueType::arrayValue));
    }

    signalr_value signalr_value::value()
    {
        return signalr_value_impl::create(Json::Value());
    }

    std::string signalr_value::getString() const
    {
        return m_impl->getString();
    }

    signalr_value signalr_value::operator[](const std::string& name) const
    {
        return (*m_impl)[name];
    }

    bool signalr_value::hasMember(const std::string& name) const
    {
        return m_impl->hasMember(name);
    }

    bool signalr_value::isArray() const
    {
        return m_impl->isArray();
    }

    bool signalr_value::isString() const
    {
        return m_impl->isString();
    }
}