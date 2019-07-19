// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "stdafx.h"
#include "callback_manager.h"
#include "signalr_value_impl.h"

using namespace signalr;

TEST(callback_manager_register_callback, register_returns_unique_callback_ids)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };
    auto callback_id1 = callback_mgr.register_callback([](const signalr_value&){});
    auto callback_id2 = callback_mgr.register_callback([](const signalr_value&){});

    ASSERT_NE(callback_id1, callback_id2);
}

TEST(callback_manager_invoke_callback, invoke_callback_invokes_and_removes_callback_if_remove_callback_true)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };

    std::string callback_argument{ "" };

    auto callback_id = callback_mgr.register_callback(
        [&callback_argument](const signalr_value& argument)
        {
            auto writer = Json::FastWriter();
            writer.omitEndingLineFeed();
            callback_argument = writer.write(signalr_value_impl::getJson(argument));
        });

    auto callback_found = callback_mgr.invoke_callback(callback_id, signalr_value_impl::create(Json::Value(42)), true);

    ASSERT_TRUE(callback_found);
    ASSERT_EQ("42", callback_argument);
    ASSERT_FALSE(callback_mgr.remove_callback(callback_id));
}

TEST(callback_manager_invoke_callback, invoke_callback_invokes_and_does_not_remove_callback_if_remove_callback_false)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };

    std::string callback_argument{ "" };

    auto callback_id = callback_mgr.register_callback(
        [&callback_argument](const signalr_value& argument)
    {
        auto writer = Json::FastWriter();
        writer.omitEndingLineFeed();
        callback_argument = writer.write(signalr_value_impl::getJson(argument));
    });

    auto callback_found = callback_mgr.invoke_callback(callback_id, signalr_value_impl::create(Json::Value(42)), false);

    ASSERT_TRUE(callback_found);
    ASSERT_EQ("42", callback_argument);
    ASSERT_TRUE(callback_mgr.remove_callback(callback_id));
}

TEST(callback_manager_ivoke_callback, invoke_callback_returns_false_for_invalid_callback_id)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };
    auto callback_found = callback_mgr.invoke_callback("42", signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)), true);

    ASSERT_FALSE(callback_found);
}

TEST(callback_manager_remove, remove_removes_callback_and_returns_true_for_valid_callback_id)
{
    auto callback_called = false;

    {
        callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };

        auto callback_id = callback_mgr.register_callback(
            [&callback_called](const signalr_value&)
        {
            callback_called = true;
        });

        ASSERT_TRUE(callback_mgr.remove_callback(callback_id));
    }

    ASSERT_FALSE(callback_called);
}

TEST(callback_manager_remove, remove_returns_false_for_invalid_callback_id)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };
    ASSERT_FALSE(callback_mgr.remove_callback("42"));
}

TEST(callback_manager_clear, clear_invokes_all_callbacks)
{
    callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(Json::ValueType::objectValue)) };
    auto invocation_count = 0;

    for (auto i = 0; i < 10; i++)
    {
        callback_mgr.register_callback(
            [&invocation_count](const signalr_value& argument)
        {
            invocation_count++;
            auto writer = Json::FastWriter();
            writer.omitEndingLineFeed();
            ASSERT_EQ("42", writer.write(signalr_value_impl::getJson(argument)));
        });
    }

    callback_mgr.clear(signalr_value_impl::create(Json::Value(42)));

    ASSERT_EQ(10, invocation_count);
}

TEST(callback_manager_dtor, clear_invokes_all_callbacks)
{
    auto invocation_count = 0;
    bool parameter_correct = true;

    {
        callback_manager callback_mgr{ signalr_value_impl::create(Json::Value(42)) };
        for (auto i = 0; i < 10; i++)
        {
            callback_mgr.register_callback(
                [&invocation_count, &parameter_correct](const signalr_value& argument)
            {
                invocation_count++;
                auto writer = Json::FastWriter();
                writer.omitEndingLineFeed();
                parameter_correct &= writer.write(signalr_value_impl::getJson(argument)) == "42";
            });
        }
    }

    ASSERT_EQ(10, invocation_count);
    ASSERT_TRUE(parameter_correct);
}
