/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:*
* Copyright 2023 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "lpa_hal.h"


int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress)
{
  /*TODO: Implement Me!*/
  (void)ActivationCodeStr;
  (void)download_progress;
  return (int)0;
}

int cellular_esim_download_profile_from_smds(char* smds)
{
  /*TODO: Implement Me!*/
  (void)smds;
  return (int)0;
}

int cellular_esim_download_profile_from_defaultsmdp(char* smdp)
{
  /*TODO: Implement Me!*/
  (void)smdp;
  return (int)0;
}

int cellular_esim_get_profile_info(eSIMProfileStruct** profile_list, int* nb_profiles)
{
  /*TODO: Implement Me!*/
  (void)profile_list;
  (void)nb_profiles;
  return (int)0;
}

int cellular_esim_enable_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_disable_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_delete_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_lpa_init(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_lpa_exit(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_get_eid(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_get_euicc(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

