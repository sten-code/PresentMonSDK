/******************************************************************************
** Copyright (c) 2013-2022 Intel Corporation All Rights Reserved
**
** Licensed under the Apache LICENSE, Version 2.0 (the "LICENSE"); you may not
** use this file except in compliance with the LICENSE.
**
** You may obtain a copy of the LICENSE at
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the LICENSE is distributed on an "AS IS" BASIS, WITHOUT
** WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**
** See the LICENSE for the specific language governing permissions and
** limitations under the LICENSE.
**
******************************************************************************/

#pragma once

#include "Ver.h"

#define STR(arg) #arg
#define STR2(arg) STR(##arg)
#define EXPAND_TOSTR(arg) STR(arg)

//
// TODO:  Modify to set version during build process via automation
//
#define IPF_VER_MAJOR        VER_MAJOR
#define IPF_VER_MINOR        VER_MINOR
#define IPF_VER_HOTFIX       VER_HOTFIX
#define IPF_VER_BUILD        VER_BUILD

#define IPF_APP_VERSION EXPAND_TOSTR(IPF_VER_MAJOR.IPF_VER_MINOR.IPF_VER_HOTFIX.IPF_VER_BUILD)
