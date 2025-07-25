/*
 * Copyright (c) 2021-2022, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/OwnPtr.h>
#include <LibGC/Root.h>
#include <LibJS/Export.h>
#include <LibJS/Runtime/Completion.h>
#include <LibJS/Runtime/FunctionObject.h>
#include <LibJS/Runtime/VM.h>

namespace JS {

// 9.5.1 JobCallback Records, https://tc39.es/ecma262/#sec-jobcallback-records
class JS_API JobCallback : public JS::Cell {
    GC_CELL(JobCallback, JS::Cell);
    GC_DECLARE_ALLOCATOR(JobCallback);

public:
    struct CustomData {
        virtual ~CustomData() = default;
    };

    [[nodiscard]] static GC::Ref<JobCallback> create(JS::VM& vm, FunctionObject& callback, OwnPtr<CustomData> custom_data);

    JobCallback(FunctionObject& callback, OwnPtr<CustomData> custom_data)
        : m_callback(callback)
        , m_custom_data(move(custom_data))
    {
    }

    void visit_edges(Visitor& visitor) override;

    FunctionObject& callback() { return m_callback; }
    CustomData* custom_data() { return m_custom_data; }

private:
    GC::Ref<FunctionObject> m_callback;
    OwnPtr<CustomData> m_custom_data { nullptr };
};

GC::Ref<JobCallback> make_job_callback(FunctionObject& callback);
ThrowCompletionOr<Value> call_job_callback(VM&, GC::Ref<JobCallback>, Value this_value, ReadonlySpan<Value> arguments_list);

}
