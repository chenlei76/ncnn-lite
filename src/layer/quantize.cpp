// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2018 THL A29 Limited, a Tencent company. All rights reserved.
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

#include "quantize.h"

#include <math.h>

void *Quantize_ctor(void *_self, va_list *args)
{
    Layer *self = (Layer *)_self;

    self->one_blob_only = true;
    self->support_inplace = false;

    return _self;
}

int Quantize_load_param(void *_self, const ParamDict& pd)
{
    Quantize *self = (Quantize *)_self;

    self->scale = pd.get(0, 1.f);

    return 0;
}

static inline signed char float2int8(float v)
{
    int int32 = static_cast<int>(round(v));
    if (int32 > 127) return 127;
    if (int32 < -127) return -127;
    return (signed char)int32;
}

int Quantize_forward(void *_self, const Mat& bottom_blob, Mat& top_blob, const Option& opt)
{
    Quantize *self = (Quantize *)_self;

    int dims = bottom_blob.dims;

    if (dims == 1)
    {
        int w = bottom_blob.w;

        top_blob.create(w, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        const float* ptr = bottom_blob;
        signed char* outptr = top_blob;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int i=0; i<w; i++)
        {
            outptr[i] = float2int8(ptr[i] * self->scale);
        }
    }

    if (dims == 2)
    {
        int w = bottom_blob.w;
        int h = bottom_blob.h;
        int size = w * h;

        top_blob.create(w, h, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        const float* ptr = bottom_blob;
        signed char* outptr = top_blob;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int i=0; i<size; i++)
        {
            outptr[i] = float2int8(ptr[i] * self->scale);
        }
    }

    if (dims == 3)
    {
        int w = bottom_blob.w;
        int h = bottom_blob.h;
        int channels = bottom_blob.c;
        int size = w * h;

        top_blob.create(w, h, channels, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int q=0; q<channels; q++)
        {
            const float* ptr = bottom_blob.channel(q);
            signed char* outptr = top_blob.channel(q);

            for (int i=0; i<size; i++)
            {
                outptr[i] = float2int8(ptr[i] * self->scale);
            }
        }
    }

    return 0;
}
