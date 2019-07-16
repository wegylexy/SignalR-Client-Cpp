// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include "signalrclient/signalr_value.h"
#include "json/json.h"

namespace signalr
{
    class signalr_value_impl
    {
    public:
        static signalr_value create(const Json::Value& value)
        {
            return signalr_value(new signalr_value_impl(value));
        }

        static Json::Value getJson(const signalr_value& value)
        {
            return value.m_impl->json;
        }

        signalr_value operator[](const std::string& name) const
        {
            return create(json[name]);
        }

        std::string getString() const
        {
            return json.asString();
        }

        bool hasMember(const std::string& name) const
        {
            return json[name] != nullptr;
        }

        bool isArray() const
        {
            return json.isArray();
        }

        bool isString() const
        {
            return json.isString();
        }
    private:
        signalr_value_impl() {}
        signalr_value_impl(const Json::Value& value)
            : json(value)
        { }

        Json::Value json;
    };
}