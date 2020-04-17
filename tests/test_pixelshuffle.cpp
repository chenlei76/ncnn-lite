// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "testutil.h"

#include "layer/pixelshuffle.h"

static int test_pixelshuffle(const ncnn::Mat& a, int upscale_factor)
{
    ncnn::ParamDict pd;
    pd.set(0, upscale_factor);

    std::vector<ncnn::Mat> weights(0);

    ncnn::Option opt;
    opt.num_threads = 1;
    opt.use_vulkan_compute = true;
    opt.use_fp16_packed = false;
    opt.use_fp16_storage = false;
    opt.use_fp16_arithmetic = false;
    opt.use_int8_storage = false;
    opt.use_int8_arithmetic = false;

    int ret = test_layer<ncnn::PixelShuffle>("PixelShuffle", pd, weights, opt, a);
    if (ret != 0)
    {
        fprintf(stderr, "test_pixelshuffle failed a.dims=%d a=(%d %d %d) upscale_factor=%d\n", a.dims, a.w, a.h, a.c, upscale_factor);
    }

    return ret;
}

static int test_pixelshuffle_0()
{
    return 0
        || test_pixelshuffle(RandomMat(3, 7, 1), 1)
        || test_pixelshuffle(RandomMat(2, 3, 4), 2)
        || test_pixelshuffle(RandomMat(3, 4, 12), 2)
        || test_pixelshuffle(RandomMat(2, 2, 64), 4)
        || test_pixelshuffle(RandomMat(4, 4, 32), 2)
        || test_pixelshuffle(RandomMat(5, 5, 48), 2)
        || test_pixelshuffle(RandomMat(3, 3, 90), 3)
        ;
}

int main()
{
    SRAND(7767517);

    return test_pixelshuffle_0();
}
