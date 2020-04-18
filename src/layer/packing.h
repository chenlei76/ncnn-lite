// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
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

#ifndef LAYER_PACKING_H
#define LAYER_PACKING_H

#include "layer.h"

struct Packing
{
    // layer base
    Layer layer;

    // proprietary data
    int out_elempack;
    int use_padding;
};

void *Packing_ctor(void *_self, va_list *args);

int Packing_load_param(void *_self, const ParamDict& pd);

int Packing_forward(void *_self, const Mat& bottom_blob, Mat& top_blob, const Option& opt);

// default operators
#define Packing_dtor                     Layer_dtor
#define Packing_load_model               Layer_load_model
#define Packing_create_pipeline          Layer_create_pipeline
#define Packing_destroy_pipeline         Layer_destroy_pipeline
#define Packing_forward_multi            Layer_forward_multi
#define Packing_forward_inplace_multi    Layer_forward_inplace_multi
#define Packing_forward_inplace          Layer_forward_inplace

#endif // LAYER_PACKING_H
