// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "ngraph/op/hswish.hpp"

#include <string>
#include <vector>

#include "engines_util/execute_tools.hpp"
#include "gtest/gtest.h"
#include "ngraph/runtime/host_tensor.hpp"
#include "ngraph/validation_util.hpp"
#include "util/test_tools.hpp"

using namespace std;
using namespace ngraph;
OPENVINO_SUPPRESS_DEPRECATED_START

TEST(op_eval, hswish) {
    auto p = make_shared<op::Parameter>(element::f32, Shape{3});
    auto swish = make_shared<op::v4::HSwish>(p);
    auto fun = make_shared<Function>(OutputVector{swish}, ParameterVector{p});

    std::vector<float> inputs{-0.5f, 0.0f, 0.5f};
    std::vector<float> expected_result{-0.208333f, 0.0f, 0.29166667f};

    auto result = make_shared<HostTensor>();
    ASSERT_TRUE(fun->evaluate({result}, {make_host_tensor<element::Type_t::f32>(Shape{3}, inputs)}));
    EXPECT_EQ(result->get_element_type(), element::f32);
    EXPECT_EQ(result->get_shape(), Shape{3});
    auto result_data = read_vector<float>(result);
    for (size_t i = 0; i < inputs.size(); i++)
        EXPECT_NEAR(result_data[i], expected_result[i], 0.000001);
}
